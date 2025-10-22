// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquippableToolBase.h"
#include "DartLauncher.generated.h"

class AFirstPersonProjectile;


UCLASS(BlueprintType, Blueprintable)
class ADVENTUREGAME_API ADartLauncher : public AEquippableToolBase
{
      GENERATED_BODY()

public:

      virtual void Use() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void BindInputAction(const UInputAction* InputToBind) override;


    // Projectile class to spawn
        UPROPERTY(EditAnywhere, Category = Projectile)
        TSubclassOf<AFirstPersonProjectile> ProjectileClass;

};
