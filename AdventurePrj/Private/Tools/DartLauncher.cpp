// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/DartLauncher.h"
#include "AdventureCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

#include "Projectile/FirstPersionProjectile.h"



void ADartLauncher::UseMe()
{


    UWorld* const world = GetWorld();

    if (world != nullptr && ProjectileClass != nullptr) {
        FVector targetPos = OwningCharacter->GetCameraTargetLocation();

        FVector socketPos = ToolMeshComp->GetSocketLocation("Muzzle");

        FRotator spawnRot  = UKismetMathLibrary::FindLookAtRotation(socketPos, targetPos);

        FVector spawnPos = socketPos + UKismetMathLibrary::GetForwardVector(spawnRot) * 10.f;

        FActorSpawnParameters projectileSpawnParams;
        projectileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        world->SpawnActor<AFirstPersionProjectile>(ProjectileClass, spawnPos, spawnRot, projectileSpawnParams);

    }

}

void ADartLauncher::BindInputAction(const UInputAction *ActionToBind)
{
    if( APlayerController* p = Cast<APlayerController>(OwningCharacter->GetController())){
        if(  UEnhancedInputComponent* eic = Cast<UEnhancedInputComponent>(p->InputComponent) ){
            eic->BindAction(ActionToBind, ETriggerEvent::Triggered , this, &ADartLauncher::UseMe);
      }

   }
}
