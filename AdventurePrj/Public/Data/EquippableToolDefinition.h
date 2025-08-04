// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"




#include "Data/ItemDefinition.h"


#include "EquippableToolDefinition.generated.h"


class UInputMappingContext;
class AEquippableToolBase;


/**
 * 
 */
UCLASS()
class ADVENTUREPRJ_API UEquippableToolDefinition : public UItemDefinition
{
	GENERATED_BODY()

	

public:
    UPROPERTY(EditDefaultsOnly,  Category="TOOT")
    TSubclassOf<AEquippableToolBase> ToolAsset;

    UFUNCTION()
    virtual UEquippableToolDefinition *CreateItemCopy() const override;
};
