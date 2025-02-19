// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TracerComponent.generated.h"

class ABaseItem;
class AShooterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_GAME_API UTracerComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UTracerComponent();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess="true"))
	TObjectPtr<ABaseItem> TraceHitItem;
	/** Item Holder to control the widget visibility */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess="true"))
	TObjectPtr<ABaseItem> HeldItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Owner, meta=(AllowPrivateAccess="true"))
	TObjectPtr<AShooterCharacter> MainCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Crosshair, meta=(AllowPrivateAccess="true"))
	float FireBulletDuration;

	bool bShouldTraceForItems;
	bool bFiring;

	FTimerHandle FireBulletHandle;
	FVector BeamEnd;
	/** Trace Control Values */
	int8 OverlappedItemCount;

	bool CrouchingHit;
public:
	FORCEINLINE void SetShouldTraceItem(bool bIsTrace) { bShouldTraceForItems = bIsTrace; }
	FORCEINLINE bool GetIsFiring() const { return bFiring; }
	FORCEINLINE bool IsCrouchingHitValid() const { return CrouchingHit; }
	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }

	bool IsConvertedScreenToWorld(FVector& CrosshairWorldPosition, FVector& CrosshairWorldDirection);
	void LineTraceFromTheScreen(const FVector& CrosshairWorldPosition, const FVector& CrosshairWorldDirection, FVector& BeamEndPoint);
	void LineTraceFromTheGunBarrel(const FVector& GunSocketLocation, FVector& BeamEndPoint);
	void LineTraceForInformationPopUp();
	bool LineTraceForCrouching();
	void CrosshairStartFireBullet();
	void CrosshairFinishFireBullet();
	void InterpolateItem();
	void IncrementOverlappedItemCount(int8 Amount);
	FVector GetBeamEndPoint();

	UFUNCTION(BlueprintCallable)
	EPhysicalSurface GetSurfaceType();

};
