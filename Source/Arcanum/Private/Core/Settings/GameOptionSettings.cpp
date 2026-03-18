#include "Core/Settings/GameOptionSettings.h"

/// 260318 추가
UGameOptionSettings::UGameOptionSettings()
	: OverallVolume(1.f), MusicVolume(1.f), SoundFXVolume(1.f), bAllowBackgroundAudio(false), bUseHDRAudioMode(false)
{
}

UGameOptionSettings* UGameOptionSettings::Get()
{
	if (GEngine) return CastChecked<UGameOptionSettings>(GEngine->GetGameUserSettings());
	return nullptr;
}
void UGameOptionSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = FMath::Clamp(InVolume, 0.f, 1.f);
}
void UGameOptionSettings::SetMusicVolume(float InVolume) { 
	MusicVolume = FMath::Clamp(InVolume, 0.f, 1.f);
}
void UGameOptionSettings::SetSoundFXVolume(float InVolume) 
{ 
	SoundFXVolume = FMath::Clamp(InVolume, 0.f, 1.f);
}
void UGameOptionSettings::SetAllowBackgroundAudio(bool bIsAllowed) 
{ 
	bAllowBackgroundAudio = bIsAllowed;

	float VolumeMultiplier = bAllowBackgroundAudio ? 1.0f : 0.0f;
	FApp::SetUnfocusedVolumeMultiplier(VolumeMultiplier);
}
void UGameOptionSettings::SetUseHDRAudioMode(bool bIsAllowed) 
{ 
	bUseHDRAudioMode = bIsAllowed;
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.EnableHDRAudio"));
	if (CVar) CVar->Set(bUseHDRAudioMode ? 1 : 0);
}
float UGameOptionSettings::GetCurrentDisplayGamma() const
{
	if (GEngine) return GEngine->GetDisplayGamma();
	return 0.0f;
}
void UGameOptionSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine) GEngine->DisplayGamma = InNewGamma;
}