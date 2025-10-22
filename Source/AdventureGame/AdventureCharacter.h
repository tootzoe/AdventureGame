// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "AdventureCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UAnimBlueprint;
class UItemDefinition;
class UInventoryComponent;
class AEquippableToolBase;
class UEquippableToolDefinition;

UCLASS()
class ADVENTUREGAME_API AAdventureCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAdventureCharacter();

    // Returns whether or not the player already owns this tool
    UFUNCTION()
    bool IsToolAlreadyOwned(UEquippableToolDefinition* ToolDefinition);

    // Attaches and equips a tool to the player
    UFUNCTION()
    void AttachTool(UEquippableToolDefinition* ToolDefinition);

    // Public function that other classes can call to attempt to give an item to the player
    UFUNCTION()
    void GiveItem(UItemDefinition* ItemDefinition);


    // First Person animations
     UPROPERTY(EditAnywhere, Category = Animation)
     UAnimBlueprint* FirstPersonDefaultAnim;

     // Inventory Component
     UPROPERTY(VisibleAnywhere, Category = Inventory)
     TObjectPtr<UInventoryComponent> InventoryComponent;

     // Returns the location in the world the character is looking at
     UFUNCTION()
     FVector GetCameraTargetLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputMappingContext> FirstPersonContext;

    // Move Input Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> MoveAction;

    // Jump Input Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> JumpAction;

    // Look Input Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* LookAction;

    // Use Input Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> UseAction;

    // The currently-equipped tool
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tools)
    TObjectPtr<AEquippableToolBase> EquippedTool;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Handles 2D Movement Input
    UFUNCTION()
    void Move(const FInputActionValue& Value);

    // Handles Look Input
    UFUNCTION()
    void Look(const FInputActionValue& Value);

    // First Person camera
    UPROPERTY(VisibleAnywhere, Category = Camera)
    UCameraComponent* FirstPersonCameraComponent;

    // Offset for the first-person camera
    UPROPERTY(EditAnywhere, Category = Camera)
    FVector FirstPersonCameraOffset = FVector(2.8f, 5.9f, 0.0f);

    // First-person camera field of view
    UPROPERTY(EditAnywhere, Category = Camera)
    float FirstPersonFieldOfView = 70.0f;

    // First-person camera view scale
    UPROPERTY(EditAnywhere, Category = Camera)
    float FirstPersonScale = 0.6f;

    // First-person mesh, visible only to the owning player
    UPROPERTY(VisibleAnywhere, Category = Mesh)
    USkeletalMeshComponent* FirstPersonMeshComponent;

};
