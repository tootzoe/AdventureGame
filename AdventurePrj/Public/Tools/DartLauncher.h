// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquippableToolBase.h"
#include "DartLauncher.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ADVENTUREPRJ_API ADartLauncher : public AEquippableToolBase
{
	GENERATED_BODY()
	
public:
       virtual void UseMe() override;
      virtual void BindInputAction(const UInputAction *ActionToBind) override;
};
