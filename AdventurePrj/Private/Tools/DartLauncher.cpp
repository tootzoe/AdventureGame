// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/DartLauncher.h"
#include "AdventureCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"




void ADartLauncher::UseMe()
{

    UE_LOG(LogTemp, Warning, TEXT("msg....%hs") , __func__);
}

void ADartLauncher::BindInputAction(const UInputAction *ActionToBind)
{
    if( APlayerController* p = Cast<APlayerController>(OwningCharacter->GetController())){
        if(  UEnhancedInputComponent* eic = Cast<UEnhancedInputComponent>(p->InputComponent) ){
            eic->BindAction(ActionToBind, ETriggerEvent::Triggered , this, &ADartLauncher::UseMe);
      }

   }
}
