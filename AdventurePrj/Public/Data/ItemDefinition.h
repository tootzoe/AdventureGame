// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ItemData.h"


#include "ItemDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ADVENTUREPRJ_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()

 public:

   virtual UItemDefinition* CreateItemCopy() const;

	
    UPROPERTY(EditAnywhere, Category="TOOT Item Data")
    FName ID;
    UPROPERTY(EditAnywhere, Category="TOOT Item Data")
    EItemType ItemType;
    UPROPERTY(EditAnywhere, Category="TOOT Item Data")
    FItemText ItemText;
    //
    UPROPERTY(EditAnywhere, Category="TOOT Item Data")
    TSoftObjectPtr<UStaticMesh> WorldMesh;

	
};
