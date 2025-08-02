// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AdventureCharacter.generated.h"


 class UInputAction;
 class UAnimBlueprint;




UCLASS()
class ADVENTUREPRJ_API AAdventureCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAdventureCharacter();

    UPROPERTY(VisibleAnywhere, Category="TOOT")
    TObjectPtr< class UCameraComponent> FirstPersionCameraComp;
    UPROPERTY(VisibleAnywhere, Category="TOOT")
    TObjectPtr<USkeletalMeshComponent> FirstPersionSkelMeshComp;


    UPROPERTY(EditAnywhere, Category="TOOT")
    FVector FirstPersionCameraOffset = FVector(2.8f , 5.9f, .0f);

    UPROPERTY(EditAnywhere, Category="TOOT Camera")
    float FirstPersionFOV = 70.f;
    UPROPERTY(EditAnywhere, Category="TOOT Camera")
    float FirstPersionViewScale = 0.6f;
    //
    UPROPERTY(EditAnywhere, Category="TOOT Animation")
    TObjectPtr<UAnimBlueprint> FirstPersionDefaultAnim;



  UFUNCTION(  )
  void FirstPersionMove(const FInputActionValue& Value);
  UFUNCTION(  )
  void FirstPersionLook(const FInputActionValue& Value);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


    UPROPERTY(EditAnywhere, BlueprintReadOnly , Category="TOOT")
        TObjectPtr<class UInputMappingContext> FirstPersionIMC;
    UPROPERTY(EditAnywhere, BlueprintReadOnly , Category="TOOT")
    TObjectPtr<class UInputMappingContext> FirstPersionMouseIMC;
    UPROPERTY(EditAnywhere, BlueprintReadOnly , Category="TOOT")
    TObjectPtr<  UInputAction> MoveAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly , Category="TOOT")
    TObjectPtr<  UInputAction> JumpAction;
    //
    UPROPERTY(EditAnywhere, BlueprintReadOnly , Category="TOOT")
    TObjectPtr<  UInputAction> LookAction;


    //



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	

    // APawn interface
    public:
     virtual void PossessedBy(AController *NewController) override;
    };
