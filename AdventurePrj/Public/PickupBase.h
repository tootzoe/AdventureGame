// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

#include "AdventureCharacter.h"



#include "PickupBase.generated.h"





class UItemDefinition;



UCLASS(BlueprintType, Blueprintable)
class ADVENTUREPRJ_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();



    void initializePickup();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


#if WITH_EDITOR


    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;

#endif


    UPROPERTY(EditInstanceOnly, Category="TOOT Pickup | Item Table")
    FName PickupItemID;
    UPROPERTY(EditInstanceOnly, Category="TOOT Pickup | Item Table")
    TSoftObjectPtr<UDataTable> PickupDataTable;
    //
    UPROPERTY(VisibleAnywhere, Category="TOOT Pickup | Reference Item")
    TObjectPtr<UItemDefinition> RefItem;
    //
    UPROPERTY(VisibleDefaultsOnly, Category="TOOT Pickup | Mesh")
    TObjectPtr<UStaticMeshComponent> PickupMeshComp;
    //
    UPROPERTY(EditAnywhere, BlueprintReadOnly,  Category="TOOT Pickup | Components")
    TObjectPtr<USphereComponent> SphereComp;
    //
    UPROPERTY(EditAnywhere, BlueprintReadOnly,  Category="TOOT Pickup | Respawn")
    bool bShouldRespawn;
    UPROPERTY(EditAnywhere, BlueprintReadOnly,  Category="TOOT Pickup | Respawn")
   float RespawnTime = 4.f;

    FTimerHandle RespawnTimerHandle;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

  };
