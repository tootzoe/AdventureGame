// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "EquippableToolDefinition.generated.h"


class AEquippableToolBase;
class UInputMappingContext;
//
UCLASS(BlueprintType, Blueprintable)
class ADVENTUREGAME_API UEquippableToolDefinition : public UItemDefinition
{
    GENERATED_BODY()

public:

    // The tool asset associated with this item
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AEquippableToolBase> ToolAsset;

    // Create and return a copy of this item
    UFUNCTION()
    virtual UEquippableToolDefinition* CreateItemCopy() const override;
};
