// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquippableToolBase.generated.h"



 class UAnimBlueprint;



UCLASS(BlueprintType, Blueprintable)
class ADVENTUREPRJ_API AEquippableToolBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquippableToolBase();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TOOT")
    TObjectPtr<UAnimBlueprint> FirstPersionToolAnim;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TOOT")
    TObjectPtr<UAnimBlueprint> ThirdPersionToolAnim;
    //
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TOOT")
    TObjectPtr<class USkeletalMeshComponent> ToolMeshComp;
    //
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TOOT")
    TObjectPtr<class AAdventureCharacter> OwningCharacter;
    //
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TOOT")
    TObjectPtr<class UInputMappingContext> ToolInputMappingCtx;

public:
        // Called every frame
        virtual void Tick(float DeltaTime) override;
    UFUNCTION()
    virtual void UseMe();

    UFUNCTION()
    virtual void BindInputAction(const class UInputAction* ActionToBind );




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
	
};
