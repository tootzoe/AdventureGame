// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureCharacter.h"

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/World.h"

#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimBlueprint.h"
//
#include "Data/EquippableToolDefinition.h"
#include "InventoryComp.h"
#include "EquippableToolBase.h"
#include "Engine/HitResult.h"
#include "TimerManager.h"




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
    //
    MainInventoryComp = CreateDefaultSubobject<UInventoryComp>(TEXT("InventoryComp"));
    check(MainInventoryComp != nullptr);


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

bool AAdventureCharacter::IsToolAlreadyOwned(UEquippableToolDefinition *ToolDefinition)
{
    for (auto it : MainInventoryComp->ToolInventory) {

        if(it->ID == ToolDefinition->ID)
        {
            return true;
        }

    }

    return false;
}

void AAdventureCharacter::AttachTool(UEquippableToolDefinition *ToolDefinition)
{

    if(not IsToolAlreadyOwned(ToolDefinition)){

        AEquippableToolBase* toolToEquip = GetWorld()->SpawnActor<AEquippableToolBase>(ToolDefinition->ToolAsset , GetActorTransform());
        FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget ,  true);

        toolToEquip->AttachToActor(this, attachmentRules);
        toolToEquip->AttachToComponent(FirstPersionSkelMeshComp, attachmentRules, FName(TEXT("HandGrip_R")));
        toolToEquip->AttachToComponent(GetMesh(), attachmentRules, FName(TEXT("HandGrip_R")));
     //   FirstPersionSkelMeshComp->SetAnimInstanceClass(toolToEquip->FirstPersionToolAnim->GeneratedClass);  //tootzoe
     //   GetMesh()->SetAnimInstanceClass(toolToEquip->ThirdPersionToolAnim->GeneratedClass);

        MainInventoryComp->ToolInventory.Add(ToolDefinition);
        toolToEquip->OwningCharacter  = this;
        EquippableTool = toolToEquip;


        if(APlayerController* p = Cast<APlayerController>(Controller)){
            if(UEnhancedInputLocalPlayerSubsystem* locSubSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(p->GetLocalPlayer())){
                locSubSys->AddMappingContext(toolToEquip->ToolInputMappingCtx , 1);
              }
            toolToEquip->BindInputAction(UseToolAction);
        }

    }
}

void AAdventureCharacter::GiveItem(UItemDefinition *ItemDefinition)
{
    switch (ItemDefinition->ItemType) {
    case EItemType::E_Tool:{
        UEquippableToolDefinition* toolEqui = Cast<UEquippableToolDefinition>(ItemDefinition);
        if(toolEqui){
            AttachTool(toolEqui);
        }else{
            UE_LOG(LogTemp, Warning, TEXT("Try to equip invalid tool....%hs") , __func__);
        }

    }
        break;
    case EItemType::E_Consumable:

        break;
    default:
        break;
    }
}

FVector AAdventureCharacter::GetCameraTargetLocation()
{

    FVector targetPos;

    UWorld* const world = GetWorld();

    if(world ){
        FHitResult hit;
        const FVector traceStart = FirstPersionCameraComp->GetComponentLocation();
        const FVector traceEnd = traceStart + FirstPersionCameraComp->GetForwardVector() * 10000.f;

        world->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility );

        targetPos = hit.bBlockingHit ? hit.ImpactPoint : hit.TraceEnd;
    }


    return targetPos;
}



// Called when the game starts or when spawned
void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(GEngine != nullptr);


    FirstPersionSkelMeshComp->SetOnlyOwnerSee(true);
    //
    if(ensureAlways(FirstPersionDefaultAnim != nullptr)){  // here, can't setup FirstPersionDefaultAnim properly on release build,
     // FirstPersionSkelMeshComp->SetAnimInstanceClass(FirstPersionDefaultAnim->GeneratedClass); // we disable animation temploraly
    }
    // else{

    //     GetWorld()->GetTimerManager().SetTimerForNextTick([&](){
    //         if(ensureAlways(FirstPersionDefaultAnim != nullptr)){
    //           FirstPersionSkelMeshComp->SetAnimInstanceClass(FirstPersionDefaultAnim->GeneratedClass);
    //         }
    //     });
    // }

	
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
