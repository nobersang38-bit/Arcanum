#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/SubLayout/InventoryItemSlotWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Core/ARGameInstance.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Components/WrapBox.h"

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

		if (InventorySortBtn)
		{
			InventorySortBtn->OnClicked.RemoveDynamic(this, &UInventoryHUDWidget::ClickInventorySortBtn);
			InventorySortBtn->OnClicked.AddDynamic(this, &UInventoryHUDWidget::ClickInventorySortBtn);
		}
	}
}

void UInventoryHUDWidget::RefreshInventoryUI()
{
	if (!ParentLobby) return;

	const int32 slotCount = FMath::Max(1, ParentLobby->GetCachedPlayerData().InventoryCapacity);

	TArray<FInventoryViewSlot> viewSlots;
	viewSlots.Reserve(slotCount);

	BuildInventoryViewSlots(viewSlots, slotCount);
	ApplyInventorySlots(viewSlots);
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

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

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

		const FDTItemCatalogRow* catalogRow = dataSubsystem->FindItemCatalogRowByTag(itemTag);
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

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();

	for (int32 i = 0; i < playerData.Inventory.Num(); i++)
	{
		if (OutSlots.Num() >= InSlotLimit) break;

		const FEquipmentInfo& equip = playerData.Inventory[i];
		if (!equip.ItemGuid.IsValid()) continue;

		const FDTItemCatalogRow* catalogRow = dataSubsystem->FindItemCatalogRowByTag(equip.ItemTag);
		if (!catalogRow) continue;

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

		const FDTItemCatalogRow* catalogRow = dataSubsystem->FindItemCatalogRowByTag(equip.ItemTag);
		if (!catalogRow) continue;

		// 카테고리 필터
		if (CurrentFilter == EInventoryCategoryFilter::Consumable) continue;
		if (CurrentFilter == EInventoryCategoryFilter::Equipment &&
			!catalogRow->ItemTypeTag.MatchesTagExact(Arcanum::Items::Type::Equipment)) continue;
		
		FGuidSortItem item;
		item.Equip = &equip;

		item.CatalogRow = dataSubsystem->FindItemCatalogRowByTag(equip.ItemTag);
		item.SortOrder = item.CatalogRow ? item.CatalogRow->SortOrder : 0;

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
	OnCategoryChanged.Broadcast(CurrentFilter);
	RefreshInventoryUI();
}

void UInventoryHUDWidget::HandleEquipmentCategoryClicked()
{
	CurrentFilter = EInventoryCategoryFilter::Equipment;
	OnCategoryChanged.Broadcast(CurrentFilter);
	RefreshInventoryUI();
}

void UInventoryHUDWidget::HandleConsumableCategoryClicked()
{
	CurrentFilter = EInventoryCategoryFilter::Consumable;
	OnCategoryChanged.Broadcast(CurrentFilter);
	RefreshInventoryUI();
}

