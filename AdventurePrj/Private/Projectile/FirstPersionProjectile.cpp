// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FirstPersionProjectile.h"


#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"



// Sets default values
AFirstPersionProjectile::AFirstPersionProjectile()
{

    RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
    check(RootSphere != nullptr);
    SetRootComponent(RootSphere);

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    check(ProjectileMesh != nullptr);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    check(ProjectileMovement != nullptr);

    ProjectileMesh->SetupAttachment(RootSphere);



 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootSphere->InitSphereRadius(5.f);
    RootSphere->BodyInstance.SetCollisionProfileName(FName("Projectile"));
    RootSphere->OnComponentHit. AddDynamic(this, &AFirstPersionProjectile::OnSphereHit);

    //
    ProjectileMovement->UpdatedComponent = RootSphere;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    //
    InitialLifeSpan = ProjectileLifespan;

}

// Called when the game starts or when spawned
void AFirstPersionProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFirstPersionProjectile::OnSphereHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
    if(OtherActor == nullptr || OtherActor == this || OtherComponent == nullptr || !OtherComponent->IsSimulatingPhysics() )
    {
        return;
    }

    OtherComponent->AddImpulseAtLocation(GetVelocity() * PhyisicsForce , GetActorLocation() );
    Destroy();

}

// Called every frame
void AFirstPersionProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

