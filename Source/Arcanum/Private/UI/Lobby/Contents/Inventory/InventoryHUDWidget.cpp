#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/SubLayout/InventoryItemSlotWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Core/ARGameInstance.h"
#include "core/ARPlayerAccountService.h"
#include "Components/WrapBox.h"
#include "Components/Border.h"

void UInventoryHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedSlotIndex = INDEX_NONE;
	CachedViewSlots.Reset();

	if (AllCategoryBtn)
	{
		AllCategoryBtn->OnClicked.RemoveDynamic(this, &UInventoryHUDWidget::HandleAllCategoryClicked);
		AllCategoryBtn->OnClicked.AddDynamic(this, &UInventoryHUDWidget::HandleAllCategoryClicked);
	}

	if (EquipmentCategoryBtn)
	{
		EquipmentCategoryBtn->OnClicked.RemoveDynamic(this, &UInventoryHUDWidget::HandleEquipmentCategoryClicked);
		EquipmentCategoryBtn->OnClicked.AddDynamic(this, &UInventoryHUDWidget::HandleEquipmentCategoryClicked);
	}

	if (ConsumableCategoryBtn)
	{
		ConsumableCategoryBtn->OnClicked.RemoveDynamic(this, &UInventoryHUDWidget::HandleConsumableCategoryClicked);
		ConsumableCategoryBtn->OnClicked.AddDynamic(this, &UInventoryHUDWidget::HandleConsumableCategoryClicked);
	}

	if (InventorySortBtn)
	{
		InventorySortBtn->OnClicked.RemoveDynamic(this, &UInventoryHUDWidget::ClickInventorySortBtn);
		InventorySortBtn->OnClicked.AddDynamic(this, &UInventoryHUDWidget::ClickInventorySortBtn);
	}

	RefreshCategoryButtonState();
}

void UInventoryHUDWidget::RefreshInventoryUI()
{
	if (!ParentLobby) return;

	const int32 slotCount = FMath::Max(1, ParentLobby->GetCachedPlayerData().InventoryCapacity);

	TArray<FInventoryViewSlot> viewSlots;
	viewSlots.Reserve(slotCount);

	BuildInventoryViewSlots(viewSlots, slotCount);
	ApplyInventorySlots(viewSlots);

	// 판매/갱신 후 선택했던 슬롯 초기화
	int32 foundIndex = INDEX_NONE;
	if (SelectedInventoryItemGuid.IsValid())
	{
		for (int32 i = 0; i < CachedViewSlots.Num(); i++)
		{
			const FInventoryViewSlot& slot = CachedViewSlots[i];
			if (slot.Type == EInventoryViewSlotType::Equipment &&
				slot.ItemGuid == SelectedInventoryItemGuid)
			{
				foundIndex = i;
				break;
			}
		}
	}
	else if (SelectedStackItemTag.IsValid())
	{
		for (int32 i = 0; i < CachedViewSlots.Num(); i++)
		{
			const FInventoryViewSlot& slot = CachedViewSlots[i];
			if (slot.Type == EInventoryViewSlotType::StackItem &&
				slot.ItemTag.MatchesTagExact(SelectedStackItemTag))
			{
				foundIndex = i;
				break;
			}
		}
	}

	if (foundIndex != INDEX_NONE)
	{
		SelectedSlotIndex = foundIndex;
		RefreshSelection();
	}
	else
	{
		ClearSelection();
		OnInventorySlotSelected.Broadcast(FInventoryViewSlot());
	}
}

void UInventoryHUDWidget::BuildInventoryViewSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const
{
	OutSlots.Reset();

	const int32 slotCount = FMath::Max(1, InSlotLimit);
	OutSlots.Reserve(slotCount);

	if (bInventorySortedView)
	{
		AppendStackItemSlots(OutSlots, slotCount);
		AppendGuidSlotsSorted(OutSlots, slotCount);
	}
	else
	{
		AppendGuidSlots(OutSlots, slotCount);
		AppendStackItemSlots(OutSlots, slotCount);
	}
}

void UInventoryHUDWidget::AppendStackItemSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const
{
	if (!ParentLobby) return; 
	if (InSlotLimit <= 0) return;
	if (OutSlots.Num() >= InSlotLimit) return;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();

	TArray<FGameplayTag> itemTags;
	playerData.StackCounts.GetKeys(itemTags);

	itemTags.Sort([](const FGameplayTag& InA, const FGameplayTag& InB)
		{
			return InA.GetTagName().LexicalLess(InB.GetTagName());
		});

	for (const FGameplayTag& itemTag : itemTags)
	{
		if (OutSlots.Num() >= InSlotLimit) break;

		const int32 totalCount = playerData.StackCounts.FindRef(itemTag);
		if (totalCount <= 0) continue;

		const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, itemTag);
		if (!catalogRow) continue;

		// 카테고리 필터
		if (CurrentFilter == EInventoryCategoryFilter::Equipment) continue;
		if (CurrentFilter == EInventoryCategoryFilter::Consumable &&
			!catalogRow->ItemTypeTag.MatchesTagExact(Arcanum::Items::Type::Potion))	continue;

		const int32 maxStack = FMath::Max(1, catalogRow->MaxStack);

		for (int32 remaining = totalCount; remaining > 0 && OutSlots.Num() < InSlotLimit; remaining -= maxStack)
		{
			FInventoryViewSlot slot;
			slot.Type = EInventoryViewSlotType::StackItem;
			slot.ItemTag = itemTag;

			const int32 stack = FMath::Min(maxStack, remaining);
			slot.StackCount = stack;

			slot.Icon = catalogRow->Icon;
			slot.UpgradeLevel = 0;

			OutSlots.Add(MoveTemp(slot));
		}
	}
}

void UInventoryHUDWidget::AppendGuidSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const
{
	if (InSlotLimit <= 0) return;
	if (OutSlots.Num() >= InSlotLimit) return;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();

	for (int32 i = 0; i < playerData.Inventory.Num(); i++)
	{
		if (OutSlots.Num() >= InSlotLimit) break;

		const FEquipmentInfo& equip = playerData.Inventory[i];
		if (!equip.ItemGuid.IsValid()) continue;

		const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, equip.ItemTag);
		if (!catalogRow) continue;
		if (!IsMatchedEquipSlotFilter(equip.ItemTag)) continue;

		// 카테고리 필터
		if (CurrentFilter == EInventoryCategoryFilter::Consumable) continue;
		if (CurrentFilter == EInventoryCategoryFilter::Equipment &&
			!catalogRow->ItemTypeTag.MatchesTagExact(Arcanum::Items::Type::Equipment)) continue;

		FInventoryViewSlot slot;
		slot.Type = EInventoryViewSlotType::Equipment;
		slot.ItemGuid = equip.ItemGuid;
		slot.ItemTag = equip.ItemTag;
		slot.StackCount = 0;
		slot.UpgradeLevel = equip.CurrUpgradeLevel;
		slot.Icon = catalogRow->Icon;

		OutSlots.Add(MoveTemp(slot));
	}
}


void UInventoryHUDWidget::AppendGuidSlotsSorted(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const
{
	if (!ParentLobby) return;
	if (InSlotLimit <= 0) return;
	if (OutSlots.Num() >= InSlotLimit) return;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();

	// 정렬에 필요한 정보 (장비 + DT로우 + 슬롯 우선순위)
	struct FGuidSortItem
	{
		const FEquipmentInfo* Equip = nullptr;
		const FDTItemCatalogRow* CatalogRow = nullptr;
		int32 SortOrder = 0;
	};

	// 캐시된 인벤토리에서 정렬 가능한 장비만 추출
	TArray<FGuidSortItem> items;
	items.Reserve(playerData.Inventory.Num());

	for (int32 i = 0; i < playerData.Inventory.Num(); i++)
	{
		const FEquipmentInfo& equip = playerData.Inventory[i];
		if (!equip.ItemGuid.IsValid()) continue;

		const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, equip.ItemTag);
		if (!catalogRow) continue;

		if (!IsMatchedEquipSlotFilter(equip.ItemTag)) continue;

		// 카테고리 필터
		if (CurrentFilter == EInventoryCategoryFilter::Consumable) continue;
		if (CurrentFilter == EInventoryCategoryFilter::Equipment &&
			!catalogRow->ItemTypeTag.MatchesTagExact(Arcanum::Items::Type::Equipment)) continue;
		
		FGuidSortItem item;
		item.Equip = &equip;
		item.CatalogRow = catalogRow;
		item.SortOrder = catalogRow->SortOrder;

		items.Add(item);
	}

	// 정렬 기준: (1)슬롯 우선순위 -> (2)강화 내림차순 -> (3)태그명 사전순
	items.Sort([](const FGuidSortItem& InA, const FGuidSortItem& InB)
		{
			if (InA.SortOrder != InB.SortOrder)
			{
				return InA.SortOrder < InB.SortOrder;
			}

			const int32 aUp = InA.Equip ? InA.Equip->CurrUpgradeLevel : 0;
			const int32 bUp = InB.Equip ? InB.Equip->CurrUpgradeLevel : 0;

			if (aUp != bUp)
			{
				// 같은 슬롯이면 강화 높은 게 먼저
				return aUp > bUp;
			}

			const FGameplayTag aTag = InA.Equip ? InA.Equip->ItemTag : FGameplayTag();
			const FGameplayTag bTag = InB.Equip ? InB.Equip->ItemTag : FGameplayTag();

			return aTag.GetTagName().LexicalLess(bTag.GetTagName());
		});

	// 정렬된 결과를 UI 슬롯 구조체로 변환해서 OutSlots에 채움
	for (int32 i = 0; i < items.Num(); i++)
	{
		if (OutSlots.Num() >= InSlotLimit) break;

		const FEquipmentInfo* equip = items[i].Equip;
		if (!equip) continue;

		FInventoryViewSlot slot;
		slot.Type = EInventoryViewSlotType::Equipment;

		slot.ItemGuid = equip->ItemGuid;
		slot.ItemTag = equip->ItemTag;
		slot.StackCount = 0;
		slot.UpgradeLevel = equip->CurrUpgradeLevel;

		if (items[i].CatalogRow)
		{
			slot.Icon = items[i].CatalogRow->Icon;
		}

		OutSlots.Add(MoveTemp(slot));
	}
}

void UInventoryHUDWidget::RefreshEquipmentInventory()
{
	if (!ParentLobby) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();
	const int32 slotCount = FMath::Max(1, playerData.InventoryCapacity);

	InitInventorySlots(slotCount);

	TArray<FInventoryViewSlot> viewSlots;
	viewSlots.Reserve(slotCount);

	AppendGuidSlots(viewSlots, slotCount);

	ApplyInventorySlots(viewSlots);

	if (SelectedInventoryItemGuid.IsValid())
	{
		int32 foundIndex = INDEX_NONE;

		for (int32 i = 0; i < CachedViewSlots.Num(); i++)
		{
			const FInventoryViewSlot& slot = CachedViewSlots[i];

			if (slot.Type == EInventoryViewSlotType::Equipment &&
				slot.ItemGuid == SelectedInventoryItemGuid)
			{
				foundIndex = i;
				break;
			}
		}

		if (foundIndex == INDEX_NONE)
		{
			SelectedInventoryItemGuid.Invalidate();
			SelectedStackItemTag = FGameplayTag();
			SelectedStackItemCount = 0;
			SelectedSlotIndex = INDEX_NONE;

			RefreshSelection();
			OnInventorySlotSelected.Broadcast(FInventoryViewSlot());
		}
	}
}

void UInventoryHUDWidget::RefreshEquipmentInventoryBySlot(EInventoryEquipSlotFilter InFilter)
{
	CurrentFilter = EInventoryCategoryFilter::Equipment;
	CurrentEquipSlotFilter = InFilter;

	if (!ParentLobby) return;

	const int32 slotCount = FMath::Max(1, ParentLobby->GetCachedPlayerData().InventoryCapacity);

	TArray<FInventoryViewSlot> viewSlots;
	viewSlots.Reserve(slotCount);

	BuildInventoryViewSlots(viewSlots, slotCount);
	ApplyInventorySlots(viewSlots);

	int32 foundIndex = INDEX_NONE;
	if (SelectedInventoryItemGuid.IsValid())
	{
		for (int32 i = 0; i < CachedViewSlots.Num(); i++)
		{
			const FInventoryViewSlot& slot = CachedViewSlots[i];
			if (slot.Type == EInventoryViewSlotType::Equipment &&
				slot.ItemGuid == SelectedInventoryItemGuid)
			{
				foundIndex = i;
				break;
			}
		}
	}

	if (foundIndex != INDEX_NONE)
	{
		SelectedSlotIndex = foundIndex;
		RefreshSelection();
	}
	else
	{
		ClearSelection();
		OnInventorySlotSelected.Broadcast(FInventoryViewSlot());
	}
}

bool UInventoryHUDWidget::IsMatchedEquipSlotFilter(const FGameplayTag& InItemTag) const
{
	if (!InItemTag.IsValid()) return false;

	if (CurrentEquipSlotFilter == EInventoryEquipSlotFilter::None) return true;
	const FDTEquipmentInfoRow* equiprow = FPlayerAccountService::FindEquipmentInfoRowByTag(this, InItemTag);
	if (!equiprow) return false;

	switch (CurrentEquipSlotFilter)
	{
	case EInventoryEquipSlotFilter::Weapon:
		return equiprow->SlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot1) ||
			equiprow->SlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot2);
	case EInventoryEquipSlotFilter::Legendary:
		return equiprow->SlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Legendary);
	case EInventoryEquipSlotFilter::Helmet:
		return equiprow->SlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Armor::Helmet);
	case EInventoryEquipSlotFilter::Chest:
		return equiprow->SlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Armor::Chest);
	case EInventoryEquipSlotFilter::Glove:
		return equiprow->SlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Armor::Glove);
	case EInventoryEquipSlotFilter::Boots:
		return equiprow->SlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Armor::Boot);

	default:
		break;
	}

	return false;
}

void UInventoryHUDWidget::RefreshStackInventory()
{
	if (!ParentLobby) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();
	const int32 slotCount = FMath::Max(1, playerData.InventoryCapacity);

	InitInventorySlots(slotCount);

	TArray<FInventoryViewSlot> viewSlots;
	viewSlots.Reserve(slotCount);

	AppendStackItemSlots(viewSlots, slotCount);

	ApplyInventorySlots(viewSlots);

	if (SelectedStackItemTag.IsValid())
	{
		int32 foundIndex = INDEX_NONE;

		for (int32 i = 0; i < CachedViewSlots.Num(); i++)
		{
			const FInventoryViewSlot& slot = CachedViewSlots[i];

			if (slot.Type == EInventoryViewSlotType::StackItem &&
				slot.ItemTag.MatchesTagExact(SelectedStackItemTag))
			{
				foundIndex = i;
				break;
			}
		}

		if (foundIndex == INDEX_NONE)
		{
			SelectedInventoryItemGuid.Invalidate();
			SelectedStackItemTag = FGameplayTag();
			SelectedStackItemCount = 0;
			SelectedSlotIndex = INDEX_NONE;

			RefreshSelection();
			OnInventorySlotSelected.Broadcast(FInventoryViewSlot());
		}
	}
}

void UInventoryHUDWidget::SetCurrentFilter(EInventoryCategoryFilter InFilter)
{
	CurrentFilter = InFilter;

	RefreshCategoryButtonState();
}

void UInventoryHUDWidget::ApplyInventorySlots(const TArray<FInventoryViewSlot>& InSlots)
{
	const int32 slotCount = Slots.Num();

	CachedViewSlots.Reset();
	CachedViewSlots.SetNum(slotCount);

	for (int32 slotIndex = 0; slotIndex < slotCount; slotIndex++)
	{
		if (Slots.IsValidIndex(slotIndex))
		{
			if (UInventoryItemSlotWidget* slot = Slots[slotIndex])
			{
				if (InSlots.IsValidIndex(slotIndex))
				{
					CachedViewSlots[slotIndex] = InSlots[slotIndex];
					slot->SetSlotData(CachedViewSlots[slotIndex], slotIndex);

					const bool bIsSelected = (SelectedSlotIndex == slotIndex);
					slot->SetSelected(bIsSelected);
				}
				// 입력 데이터가 없는 경우 (빈 슬롯 처리)
				else
				{
					CachedViewSlots[slotIndex] = FInventoryViewSlot();
					slot->ClearSlot(slotIndex);
					slot->SetSelected(false);
				}
			}
		}
	}
}

void UInventoryHUDWidget::ClearSelection()
{
	SelectedSlotIndex = INDEX_NONE;
	SelectedInventoryItemGuid.IsValid();
	SelectedStackItemTag = FGameplayTag();
	SelectedStackItemCount = 0;

	RefreshSelection();
}

void UInventoryHUDWidget::InitInventorySlots(int32 InSlotCount)
{
	const int32 slotCount = FMath::Max(1, InSlotCount);

	CreateInventorySlots(slotCount);
	BindSlotEvents();

	SelectedSlotIndex = INDEX_NONE;
	RefreshSelection();
}

void UInventoryHUDWidget::CreateInventorySlots(int32 InSlotCount)
{
	if (!SlotContainer || !InventoryItemSlotWidgetClass) return;

	SlotContainer->ClearChildren();
	Slots.Reset();

	for (int32 slotIndex = 0; slotIndex < InSlotCount; slotIndex++)
	{
		if (UInventoryItemSlotWidget* slot = CreateWidget<UInventoryItemSlotWidget>(this, InventoryItemSlotWidgetClass))
		{
			slot->ClearSlot(slotIndex);
			SlotContainer->AddChildToWrapBox(slot);
			Slots.Add(slot);
		}
	}
}

void UInventoryHUDWidget::BindSlotEvents()
{
	for (int32 slotIndex = 0; slotIndex < Slots.Num(); slotIndex++)
	{
		if (Slots.IsValidIndex(slotIndex))
		{
			if (UInventoryItemSlotWidget* slot = Slots[slotIndex])
			{
				slot->OnInventorySlotClicked.RemoveDynamic(this, &UInventoryHUDWidget::HandleSlotClicked);
				slot->OnInventorySlotClicked.AddDynamic(this, &UInventoryHUDWidget::HandleSlotClicked);
			}
		}
	}
}

void UInventoryHUDWidget::HandleSlotClicked(int32 InSlotIndex)
{
	if (CachedViewSlots.IsValidIndex(InSlotIndex))
	{
		SelectedSlotIndex = InSlotIndex;

		// 선택값 초기화
		SelectedInventoryItemGuid.Invalidate();
		SelectedStackItemTag = FGameplayTag();
		SelectedStackItemCount = 0;

		// 선택 타입에 따라 저장
		const FInventoryViewSlot& selectedSlot = CachedViewSlots[InSlotIndex];
		if (selectedSlot.Type == EInventoryViewSlotType::Equipment)
		{
			SelectedInventoryItemGuid = selectedSlot.ItemGuid;
		}
		else if (selectedSlot.Type == EInventoryViewSlotType::StackItem)
		{
			SelectedStackItemTag = selectedSlot.ItemTag;
			SelectedStackItemCount = selectedSlot.StackCount;
		}

		OnInventorySlotSelected.Broadcast(CachedViewSlots[InSlotIndex]);

		RefreshSelection();
	}
}

void UInventoryHUDWidget::RefreshSelection()
{
	for (int32 slotIndex = 0; slotIndex < Slots.Num(); slotIndex++)
	{
		if (Slots.IsValidIndex(slotIndex))
		{
			if (UInventoryItemSlotWidget* slot = Slots[slotIndex])
			{
				const bool bIsSelected = (SelectedSlotIndex == slotIndex);

				slot->SetSelected(bIsSelected);
			}
		}
	}
}

void UInventoryHUDWidget::ClickInventorySortBtn()
{
	bInventorySortedView = !bInventorySortedView;
	RefreshInventoryUI();
}

void UInventoryHUDWidget::HandleAllCategoryClicked()
{
	CurrentFilter = EInventoryCategoryFilter::All;
	RefreshCategoryButtonState();
	OnCategoryChanged.Broadcast(CurrentFilter);
	RefreshInventoryUI();
}

void UInventoryHUDWidget::HandleEquipmentCategoryClicked()
{
	CurrentFilter = EInventoryCategoryFilter::Equipment;
	RefreshCategoryButtonState();
	OnCategoryChanged.Broadcast(CurrentFilter);
	RefreshInventoryUI();
}

void UInventoryHUDWidget::HandleConsumableCategoryClicked()
{
	CurrentFilter = EInventoryCategoryFilter::Consumable;
	RefreshCategoryButtonState();
	OnCategoryChanged.Broadcast(CurrentFilter);
	RefreshInventoryUI();
}

void UInventoryHUDWidget::RefreshCategoryButtonState()
{
	if (AllCategoryBorder)
	{
		AllCategoryBorder->SetBrushColor(
			CurrentFilter == EInventoryCategoryFilter::All
			? FLinearColor(1.f, 1.f, 1.f, 1.f)
			: FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	}

	if (EquipmentCategoryBorder)
	{
		EquipmentCategoryBorder->SetBrushColor(
			CurrentFilter == EInventoryCategoryFilter::Equipment
			? FLinearColor(1.f, 1.f, 1.f, 1.f)
			: FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	}

	if (ConsumableCategoryBorder)
	{
		ConsumableCategoryBorder->SetBrushColor(
			CurrentFilter == EInventoryCategoryFilter::Consumable
			? FLinearColor(1.f, 1.f, 1.f, 1.f)
			: FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	}
}

