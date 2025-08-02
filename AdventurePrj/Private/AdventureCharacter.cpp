// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureCharacter.h"

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimBlueprint.h"





// Sets default values
AAdventureCharacter::AAdventureCharacter()
{

    FirstPersionSkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSkelMesh"));
    check(FirstPersionSkelMeshComp);
    FirstPersionSkelMeshComp->SetupAttachment(GetMesh());
    FirstPersionSkelMeshComp->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
    FirstPersionSkelMeshComp->SetOnlyOwnerSee(true);


    GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

    FirstPersionSkelMeshComp->SetCollisionProfileName(FName("NoCollision"));

    FirstPersionCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersionCamera"));
    check(FirstPersionCameraComp);

    FirstPersionCameraComp->SetupAttachment(FirstPersionSkelMeshComp, FName("head"));
    FirstPersionCameraComp->SetRelativeLocationAndRotation(FirstPersionCameraOffset, FRotator(.0f, 90.f, -90.f));

    FirstPersionCameraComp->bUsePawnControlRotation = true;
    FirstPersionCameraComp->bEnableFirstPersonFieldOfView = true;
    FirstPersionCameraComp->bEnableFirstPersonScale = true;
    FirstPersionCameraComp->FirstPersonFieldOfView = FirstPersionFOV;
    FirstPersionCameraComp->FirstPersonScale = FirstPersionViewScale;


 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void AAdventureCharacter::FirstPersionMove(const FInputActionValue &Value)
{
    const FVector2D val = Value.Get<FInputActionValue::Axis2D>();

   // UE_LOG(LogTemp, Warning, TEXT("%02f....%hs") , __func__);

    if(Controller){

        const FVector f = GetActorForwardVector();
        const FVector r = GetActorRightVector();

        AddMovementInput(f, val.Y);
        AddMovementInput(r, val.X);


    }

}

void AAdventureCharacter::FirstPersionLook(const FInputActionValue &Value)
{
    const FVector2D val = Value.Get<FInputActionValue::Axis2D>();

    if(Controller){
        AddControllerYawInput(  val.X);
        AddControllerPitchInput(  val.Y);
    }
}



// Called when the game starts or when spawned
void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(GEngine != nullptr);


    FirstPersionSkelMeshComp->SetOnlyOwnerSee(true);
    //
    if(ensureAlways(FirstPersionDefaultAnim != nullptr)){
      FirstPersionSkelMeshComp->SetAnimInstanceClass(FirstPersionDefaultAnim->GeneratedClass);
    }

	
}

// Called every frame
void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdventureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


    if (UEnhancedInputComponent* eic = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
         if(ensureAlways(MoveAction)){
            eic->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::FirstPersionMove);
         }
         if(ensureAlways(LookAction)){
             eic->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::FirstPersionLook);
         }
        if(ensureAlways(JumpAction)){
            eic->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            eic->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }



    }

}



void AAdventureCharacter::PossessedBy(AController *NewController)
{

    if(  ! NewController->IsLocalController()   ) return;

    UE_LOG(LogTemp, Warning, TEXT("msg....%hs , is local: %d") , __func__  , NewController->IsLocalController());



    if( GEngine ){
        GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Cyan, FString::Printf(TEXT("%hs....controller: %s") , __func__ , *NewController->GetName()) );
    }

    if( APlayerController* p = Cast <APlayerController>(NewController)){

       UEnhancedInputLocalPlayerSubsystem* localSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(p->GetLocalPlayer());

       ensureAlways(FirstPersionIMC);
       ensureAlways(FirstPersionMouseIMC);
       localSys->AddMappingContext(FirstPersionIMC , 0);
       localSys->AddMappingContext(FirstPersionMouseIMC , 0);

    }



}
