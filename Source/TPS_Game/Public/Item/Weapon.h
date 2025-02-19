// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "Weapon.generated.h"

enum class EAmmoType : uint8;
class UDataTable;
class UInformationPopUp;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SubmachineGun UMETA(DisplayName = "SubmachineGun"),
	EWT_AssaultRifle UMETA(DisplayName = "AssaultRifle"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),

	EWT_MAX UMETA(DisplayName = "Default"),
};

UCLASS()
class TPS_GAME_API AWeapon : public ABaseItem
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void Tick(float DeltaSeconds) override;
	void ThrowWeapon();
protected:
	virtual void BeginPlay() override;
	void StopFalling();

	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void FinishMovingSlide();
	void UpdateSlideDisplacement();
	
private: // Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInformationPopUp> InformationWidgetObject;
	/** Weapon Ammo values */
	UPROPERTY(EditAnywhere, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	int32 MagazineCapacity;
	UPROPERTY(EditAnywhere, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	int32 CurrentAmmoAmount;

	/** Throwing Control Values */
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	FName ReloadingMontageSection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	FName MagazineBoneName;

	bool bIsMovingMagazine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	int32 SlotIndex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponRarityDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Combat, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UParticleSystem> HitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Combat, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UParticleSystem> SmokeBeamParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Combat, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UParticleSystem> MuzzleFlash;
	/** Rarity Data Table Variables */
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	FLinearColor GlowColor;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	FLinearColor BrightColor;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	FLinearColor DarkColor;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	int32 ActiveStar;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	UTexture2D* SlotBackgroundImage;
	
	/** Weapon Data Table Varibles */
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	TObjectPtr<UTexture2D> AmmoIcon;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	TObjectPtr<UTexture2D> WeaponIcon;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	TObjectPtr<UTexture> CrosshairLeft;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	TObjectPtr<UTexture> CrosshairRight;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	TObjectPtr<UTexture> CrosshairBottom;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	TObjectPtr<UTexture> CrosshairTop;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	TObjectPtr<UTexture> CrosshairMiddle;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	int32 MaterialIndex;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	USoundBase* FireSoundCue;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	float AutoFireRate;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	FName BoneToHide;
	UPROPERTY(EditDefaultsOnly, Category="Table Property")
	bool bIsAutomatic;

	// Pistol Slider Values
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCurveFloat> SlideDisplacementCurve;
	/** Current Displacement Value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess="true"))
	float CurrentSlideDisplacement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess="true"))
	float CurrentRecoilRotation;
	FTimerHandle SlideTimer;
	/** How long it will slide */
	float SlideDisplacementTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess="true"))
	/** How far it will slide */
	float MaxSlideDisplacement;
	float MaxRecoilRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess="true"))
	bool bIsSliding;

public:

	void PlayFireSoundCue();
	void PlayBarrelMuzzleFlash();
	void PlayHitParticle(const FVector& HitLocation);
	void PlayBeamParticle(const FTransform& Start, const FVector& End);
	
	virtual void SetItemProperties(EItemState CurrentState);
	void DecremenetAmmo();
	bool IsMagazineFull();

	void SetWeaponRarityTableProperties();
	void SetWeaponTableProperties();

	void StartSlideTimer();
	
	// Setters
	void ReloadAmmo(int32 Ammo);
	FORCEINLINE void SetbIsMovingMagazine(bool bIsMoving) { bIsMovingMagazine = bIsMoving; }
	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index; }	
	// Getters
	const USkeletalMeshSocket* GetBarrelSocket() const;
	FTransform GetBarrelSocketTransform() const;
	FORCEINLINE float GetSlideDisplacement() const { return CurrentSlideDisplacement; }
	FORCEINLINE float GetRecoilRotation() const { return CurrentRecoilRotation; }
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmoAmount; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetReloadingWeaponSection() const { return ReloadingMontageSection; }
	FORCEINLINE FName GetMagazineBoneName() const { return MagazineBoneName; }
	FORCEINLINE bool IsMovingClip() const { return bIsMovingMagazine; }
	FORCEINLINE bool IsAutomatic() const { return bIsAutomatic; }
	FORCEINLINE TObjectPtr<UInformationPopUp> GetInformationWidgetObject() const { return InformationWidgetObject; };
	FORCEINLINE UTexture2D* GetSlotBackgroundImage() const { return SlotBackgroundImage; }
	FORCEINLINE TObjectPtr<UTexture> GetCrosshairLeftImage() const { return CrosshairLeft; }
	FORCEINLINE TObjectPtr<UTexture> GetCrosshairRightImage() const { return CrosshairRight; }
	FORCEINLINE TObjectPtr<UTexture> GetCrosshairBottomImage() const { return CrosshairBottom; }
	FORCEINLINE TObjectPtr<UTexture> GetCrosshairTopImage() const { return CrosshairTop; }
	FORCEINLINE TObjectPtr<UTexture> GetCrosshairMiddleImage() const { return CrosshairMiddle; }
	FORCEINLINE float GetAutoFireRate() const { return AutoFireRate; }
};