#include "Components/CombatComponent.h"
#include "Character/ShooterCharacter.h"
#include "Components/AnimatorComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/TracerComponent.h"
#include "Item/Weapon.h"
#include "Components/MotionComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/ShooterCameraShake.h"

UCombatComponent::UCombatComponent() :
	// Automatic Gun Fire Factors
	CombatState(ECombatState::ECS_Unoccupied),
	bAiming(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerRef == nullptr)
	{
		OwnerRef = Cast<AShooterCharacter>(GetOwner());
	}
}
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::ShootingStart()
{
	if (OwnerRef && OwnerRef->GetTracerComponent() && OwnerRef->GetEquippedWeapon())
	{
		FVector BeamEndPoint = OwnerRef->GetTracerComponent()->GetBeamEndPoint();
		FTransform BarrelSocketTransform = OwnerRef->GetEquippedWeapon()->GetBarrelSocketTransform();
		OwnerRef->GetAnimatorComponent()->PlayGunFireMontage();
		OwnerRef->GetEquippedWeapon()->PlayHitParticle(BeamEndPoint);
		OwnerRef->GetEquippedWeapon()->PlayBeamParticle(BarrelSocketTransform, BeamEndPoint);
		OwnerRef->GetEquippedWeapon()->PlayFireSoundCue();
		OwnerRef->GetEquippedWeapon()->PlayBarrelMuzzleFlash();
		OwnerRef->GetEquippedWeapon()->DecremenetAmmo();
		ShakeCamera();
		StartFireTimer();

		if (OwnerRef->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_Pistol)
		{
			OwnerRef->GetEquippedWeapon()->StartSlideTimer();
		}
	}
}

void UCombatComponent::StartAim()
{
	if (OwnerRef)
	{
		SetIsScopeOpen(true);
		SetIsAiming(true);
		OwnerRef->GetCharacterMovement()->MaxWalkSpeed = OwnerRef->GetMotionComponent()->GetCrouchingSpeed();
	}
}

void UCombatComponent::StopAim()
{
	if (OwnerRef)
	{
		SetIsAiming(false);
		SetIsScopeOpen(false);
		OwnerRef->GetCharacterMovement()->MaxWalkSpeed = OwnerRef->GetMotionComponent()->GetRunningSpeed();
	}
}

void UCombatComponent::StartFireTimer()
{
	if (OwnerRef && OwnerRef->GetEquippedWeapon())
	{
		CombatState = ECombatState::ECS_FireTimerInProgress;
		OwnerRef->GetWorldTimerManager().SetTimer(AutomaticFireHandle, this, &UCombatComponent::AutomaticFireReset, OwnerRef->GetEquippedWeapon()->GetAutoFireRate());
	}
}

void UCombatComponent::AutomaticFireReset()
{
	CombatState = ECombatState::ECS_Unoccupied;
	if (OwnerRef && OwnerRef->GetEquippedWeapon())
	{
		if (OwnerRef->GetEquippedWeapon()->GetCurrentAmmo() > 0)
		{
			if (OwnerRef->GetEquippedWeapon()->IsAutomatic() && OwnerRef->GetIsFireButtonPressed())
			{
				ShootingStart();
			}	
		}

		else
		{
			ReloadWeapon();
		}	
	}
}

void UCombatComponent::ReloadWeapon()
{
	if (CombatState == ECombatState::ECS_Unoccupied &&
		OwnerRef && OwnerRef->GetInventoryComponent() &&
		OwnerRef->GetInventoryComponent()->CarryingAmmo() &&
		!OwnerRef->GetEquippedWeapon()->IsMagazineFull())
	{
		if (GetIsAiming())
		{
			StopAim();
		}
		CombatState = ECombatState::ECS_Reloading;
		OwnerRef->GetAnimatorComponent()->PlayReloadWeaponMontage();
	}
}


void UCombatComponent::FinishReloading()
{
	CombatState = ECombatState::ECS_Unoccupied;

	if (bIsScopeOpen)
	{
		StartAim();
	}

	if (OwnerRef && OwnerRef->GetInventoryComponent() && OwnerRef->GetEquippedWeapon())
	{
		const EAmmoType AmmoType = OwnerRef->GetEquippedWeapon()->GetAmmoType();

		if (OwnerRef->GetInventoryComponent()->GetAmmoMap().Contains(AmmoType))
		{
			// Returns to us useable ammo amount of that AmmoType
			int32 CarriedAmmo = OwnerRef->GetInventoryComponent()->GetAmmoMap()[AmmoType];
			// Space left in the magazine of EquippedWeapon
			const int32 MagazineEmptySpace = OwnerRef->GetEquippedWeapon()->GetMagazineCapacity() - OwnerRef->GetEquippedWeapon()->GetCurrentAmmo();

			if (MagazineEmptySpace > CarriedAmmo)
			{
				// Reload the weapon with carried ammo
				OwnerRef->GetEquippedWeapon()->ReloadAmmo(CarriedAmmo);
				CarriedAmmo = 0;
			}

			else
			{
				// Fill the weapon
				OwnerRef->GetEquippedWeapon()->ReloadAmmo(MagazineEmptySpace);
				CarriedAmmo -= MagazineEmptySpace;
			}
			
			OwnerRef->GetInventoryComponent()->UpdateAmmoMap(AmmoType, CarriedAmmo);
		}	
	}
}

void UCombatComponent::ShakeCamera()
{
	if (OwnerRef)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(
		GetWorld(),
		CameraShakeClass,
		OwnerRef->GetActorLocation(),
		0.f,
		1000.f,
		1.f	);	
	}
}
