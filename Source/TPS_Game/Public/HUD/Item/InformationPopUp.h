// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HLSLTypeAliases.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InformationPopUp.generated.h"

class AShooterCharacter;
class UImage;
class UTextBlock;
class ABaseItem;

UCLASS()
class TPS_GAME_API UInformationPopUp : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
	// Getters and Setters
	void SetRightBoxBackgroundColors(FLinearColor& BrightColor, FLinearColor& DarkColor);
	void SetItemNameText(FString& ItemName);
	void SetAmmoAmountText(int8 Amount);
	void SetStarsImagesVisibility(int32 ActiveStarAmount);
	void SetItemLabelText();
	
private:

	TObjectPtr<AShooterCharacter> ShooterRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UImage> RightTopBackground;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UImage> RightBottomBackground;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UTextBlock> AmmoAmountText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UTextBlock> IconLabelText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UImage> Star1Image;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UImage> Star2Image;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UImage> Star3Image;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UImage> Star4Image;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"), meta=(BindWidget))
	TObjectPtr<UImage> Star5Image;

	TArray<TObjectPtr<UImage>> StarImageArray;
};
