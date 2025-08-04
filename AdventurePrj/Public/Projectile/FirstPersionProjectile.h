// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstPersionProjectile.generated.h"



class UProjectileMovementComponent;
class USphereComponent;



UCLASS(BlueprintType, Blueprintable)
class ADVENTUREPRJ_API AFirstPersionProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirstPersionProjectile();


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile | Mesh")
    TObjectPtr<UStaticMeshComponent> ProjectileMesh;



    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile | physics")
    float PhyisicsForce = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile | Lifespan ")
    float ProjectileLifespan = 5.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                     FVector NormalImpulse, const FHitResult& Hit);


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
    TObjectPtr<USphereComponent> RootSphere;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
