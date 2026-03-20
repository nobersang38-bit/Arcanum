#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "GameOptionSettings.generated.h"

/// 260318 추가

UCLASS()
class ARCANUM_API UGameOptionSettings : public UGameUserSettings
{
	GENERATED_BODY()
		
public:
	UGameOptionSettings();

	static UGameOptionSettings* Get();

#pragma region 오디오
public:
	UFUNCTION() float GetOverallVolume() const { return OverallVolume; }
	UFUNCTION() void SetOverallVolume(float InVolume);

	UFUNCTION() float GetMusicVolume() const { return MusicVolume; }
	UFUNCTION() void SetMusicVolume(float InVolume);

	UFUNCTION() float GetSoundFXVolume() const { return SoundFXVolume; }
	UFUNCTION() void SetSoundFXVolume(float InVolume);

	UFUNCTION() bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }
	UFUNCTION() void SetAllowBackgroundAudio(bool bIsAllowed);

	UFUNCTION() bool GetUseHDRAudioMode() const { return bUseHDRAudioMode; }
	UFUNCTION() void SetUseHDRAudioMode(bool bIsAllowed);
#pragma endregion

#pragma region 비디오
public:
	UFUNCTION() float GetCurrentDisplayGamma() const;
	UFUNCTION() void SetCurrentDisplayGamma(float InNewGamma);
	UFUNCTION() float GetFrameRateLimitValue() const { return GetFrameRateLimit(); }
	UFUNCTION() void SetFrameRateLimitValue(float InLimit) { SetFrameRateLimit(InLimit); }

	void SetOverallScalabilityLevel(int32 Value) { SetOverallScalabilityLevel(Value); }
	void SetGlobalIlluminationQuality(int32 Value) { ScalabilityQuality.GlobalIlluminationQuality = Value; }
	void SetShadowQuality(int32 Value) { ScalabilityQuality.ShadowQuality = Value; }
	void SetAntiAliasingQuality(int32 Value) { ScalabilityQuality.AntiAliasingQuality = Value; }
	void SetViewDistanceQuality(int32 Value) { ScalabilityQuality.ViewDistanceQuality = Value; }
	void SetTextureQuality(int32 Value) { ScalabilityQuality.TextureQuality = Value; }
	void SetVisualEffectQuality(int32 Value) { ScalabilityQuality.EffectsQuality = Value; }
	void SetReflectionQuality(int32 Value) { ScalabilityQuality.ReflectionQuality = Value; }
	void SetPostProcessingQuality(int32 Value) { ScalabilityQuality.PostProcessQuality = Value; }
#pragma endregion

private:
	UPROPERTY(Config) float OverallVolume;
	UPROPERTY(Config) float MusicVolume;
	UPROPERTY(Config) float SoundFXVolume;
	UPROPERTY(Config) bool bAllowBackgroundAudio;
	UPROPERTY(Config) bool bUseHDRAudioMode;
};