// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureCharacter.h"
#include "InventoryComponent.h"
#include "EquippableToolBase.h"
#include "Data/EquippableToolDefinition.h"

// Sets default values
AAdventureCharacter::AAdventureCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create a first-person camera component
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    check(FirstPersonCameraComponent != nullptr);

    // Create a first-person mesh component for the owning player
    FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    check(FirstPersonMeshComponent != nullptr);

    // Attach the first-person mesh to the skeletal mesh
    FirstPersonMeshComponent->SetupAttachment(GetMesh());

    // The first-person mesh is included in First Person rendering (use FirstPersonFieldofView and FirstPersonScale on this mesh)
    FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;

    // Only the owning player sees the first-person mesh
    FirstPersonMeshComponent->SetOnlyOwnerSee(true);

    // The owning player doesn't see the regular (third-person) body mesh, but it casts a shadow
    GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

    // Set the first-person mesh to not collide with other objects
    FirstPersonMeshComponent->SetCollisionProfileName(FName("NoCollision"));

    FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, FName("head"));

    // Position the camera slightly above the eyes and rotate it to behind the player's head
    FirstPersonCameraComponent->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(0.0f, 90.0f, -90.0f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Enable first-person rendering on the camera and set default FOV and scale values
    FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
    FirstPersonCameraComponent->bEnableFirstPersonScale = true;
    FirstPersonCameraComponent->FirstPersonFieldOfView = FirstPersonFieldOfView;
    FirstPersonCameraComponent->FirstPersonScale = FirstPersonScale;

    //

    // Create an inventory component for the owning player
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

}

// Called when the game starts or when spawned
void AAdventureCharacter::BeginPlay()
{

    Super::BeginPlay();

    check(GEngine != nullptr);

    // Only the owning player sees the first person mesh
    FirstPersonMeshComponent->SetOnlyOwnerSee(true);

    // Set the animations on the first person mesh.
    FirstPersonMeshComponent->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);

    // The owning player doesn't see the regular (third-person) body mesh
    GetMesh()->SetOwnerNoSee(true);

    // Position the camera slightly above the eyes.
    FirstPersonCameraComponent->SetRelativeLocation(FVector(2.8f, 5.9f, 0.0f));


    // Get the player controller for this character
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        // Get the enhanced input local player subsystem and add a new input mapping context to it
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(FirstPersonContext, 0);
        }
    }
    // Display a debug message for five seconds.
    // The -1 "Key" value argument prevents the message from being updated or refreshed.
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using AdventureCharacter."));
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


    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind Movement Actions
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Move);

        // Bind Look Actions
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Look);

        // Bind Jump Actions
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    }

}

void AAdventureCharacter::Move(const FInputActionValue& Value)
{
    // 2D Vector of movement values returned from the input action
    const FVector2D MovementValue = Value.Get<FVector2D>();



    // Check if the controller posessing this Actor is valid
    if (Controller)
    {
        // Add left and right movement
        const FVector Right = GetActorRightVector();
        AddMovementInput(Right, MovementValue.X);

        // Add forward and back movement
        const FVector Forward = GetActorForwardVector();
        AddMovementInput(Forward, MovementValue.Y);
    }
}

void AAdventureCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisValue = Value.Get<FVector2D>();

    if (Controller)
    {
        AddControllerYawInput(LookAxisValue.X);
        AddControllerPitchInput(LookAxisValue.Y);
    }
}

bool AAdventureCharacter::IsToolAlreadyOwned(UEquippableToolDefinition* ToolDefinition)
{
    // Check that the character does not yet have this particular tool
    for (UEquippableToolDefinition* InventoryItem : InventoryComponent->ToolInventory)
    {
        if (ToolDefinition->ID == InventoryItem->ID)
        {
            return true;
        }
    }

   return false;
}



void AAdventureCharacter::AttachTool(UEquippableToolDefinition* ToolDefinition)
{

    // Only equip this tool if it isn't already owned
    if (not IsToolAlreadyOwned(ToolDefinition))
    {
        // Spawn a new instance of the tool to equip
        AEquippableToolBase* ToolToEquip = GetWorld()->SpawnActor<AEquippableToolBase>(ToolDefinition->ToolAsset, this->GetActorTransform());

        // Attach the tool to the First Person Character
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);


        // Attach the tool to this character, and then the right hand of their first person mesh
        ToolToEquip->AttachToActor(this, AttachmentRules);

        ToolToEquip->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("HandGrip_R")));
       ToolToEquip->AttachToComponent(FirstPersonMeshComponent, AttachmentRules, FName(TEXT("HandGrip_R")));

        ToolToEquip->OwningCharacter = this;

        // Add the tool to this character's inventory
        InventoryComponent->ToolInventory.Add(ToolDefinition);

        // Set the animations on the first person mesh.
         FirstPersonMeshComponent->SetAnimInstanceClass(ToolToEquip->FirstPersonToolAnim->GeneratedClass);
         GetMesh()->SetAnimInstanceClass(ToolToEquip->ThirdPersonToolAnim->GeneratedClass);

        EquippedTool = ToolToEquip;

        // Get the player controller for this character
        if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
            {
                Subsystem->AddMappingContext(ToolToEquip->ToolMappingContext, 1);
            }

            ToolToEquip->BindInputAction(UseAction);
        }
    }
}



void AAdventureCharacter::GiveItem(UItemDefinition* ItemDefinition)
{
    // Case based on the type of the item
    switch (ItemDefinition->ItemType)
    {

    case EItemType::Tool:
    {
        // If the item is a tool, attempt to cast and attach it to the character

        UEquippableToolDefinition* ToolDefinition = Cast<UEquippableToolDefinition>(ItemDefinition);

        if (ToolDefinition != nullptr)
        {
            AttachTool(ToolDefinition);
        }
        else {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Cast to tool failed!"));
        }
        break;
    }

    case EItemType::Consumable:
    {
        // Not implemented
        break;
    }
    default:
        break;

    }
}



FVector AAdventureCharacter::GetCameraTargetLocation()
{
    // The target position to return
    FVector TargetPosition;

    UWorld* const World = GetWorld();
    if (World != nullptr)
    {
        // The result of the line trace
        FHitResult Hit;

        // Simulate a line trace from the character along the vector they're looking down
        const FVector TraceStart = FirstPersonCameraComponent->GetComponentLocation();
        const FVector TraceEnd = TraceStart + FirstPersonCameraComponent->GetForwardVector() * 10000.0;
        World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

        // Set the target position to the impact point of the hit or the end of the trace depending on whether it hit an object
        TargetPosition = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;

    }
    return TargetPosition;
}
