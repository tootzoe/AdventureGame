// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"

#include "Engine/DataTable.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UnrealType.h"
#include "Engine/StaticMesh.h"

#include "Data/ItemDefinition.h"






// Sets default values
APickupBase::APickupBase()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    check(SphereComp != nullptr);

    SetRootComponent(SphereComp);
     SphereComp ->SetSphereRadius(32.f);

    PickupMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    check(PickupMeshComp != nullptr);


    PickupMeshComp  ->SetupAttachment(SphereComp);


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void APickupBase::initializePickup()
{

    if( ! ensureAlways( ! PickupDataTable.IsNull()  ) )
    {
        GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Yellow, FString::Printf(TEXT("%hs....PickupDataTable not set....") , __func__) );
        return;
    }

    if( !PickupDataTable.IsValid() ){
         PickupDataTable.LoadSynchronous();
    }

    if( ensureAlways( !PickupItemID.IsNone() ) )
    {
        const FItemData* itRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());

        UItemDefinition* tmpIt = itRow->ItemBase.Get();

        RefItem = tmpIt->CreateItemCopy();


        if (tmpIt->WorldMesh.IsValid()) {
            PickupMeshComp->SetStaticMesh(tmpIt->WorldMesh.Get());

        }else{
            UStaticMesh* mesh = tmpIt->WorldMesh.LoadSynchronous();
             PickupMeshComp->SetStaticMesh(mesh);
        }

        PickupMeshComp->SetVisibility(true);
        SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SphereComp->OnComponentBeginOverlap. AddDynamic(this, &APickupBase::OnSphereBeginOverlap);

    }
}


// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();


    initializePickup();
	
}

void APickupBase::OnSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

    if( GEngine ){
        GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Green, FString::Printf(TEXT("%hs....Attempting a pickup collision....") , __func__) );
    }

    AAdventureCharacter* c = Cast<AAdventureCharacter>(OtherActor);
    if(c  )
    {
        c->GiveItem(RefItem);

        SphereComp->OnComponentBeginOverlap.RemoveAll(this);
        PickupMeshComp->SetVisibility(false);
        PickupMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


        if(bShouldRespawn)
        {
          //  GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APickupBase::initializePickup, RespawnTime, false , 0);
            GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APickupBase::initializePickup,  RespawnTime );
        }
    }
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


#if WITH_EDITOR
void APickupBase::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);


    const FName chgedPropName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    if(chgedPropName == GET_MEMBER_NAME_CHECKED(APickupBase, PickupItemID) && PickupDataTable){

        if (const FItemData* itRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString())) {
            UItemDefinition* itDef = itRow->ItemBase;

            UStaticMesh* tmpMesh = itDef->WorldMesh.Get();

            PickupMeshComp->SetStaticMesh(tmpMesh);
            SphereComp->SetSphereRadius( tmpMesh->GetExtendedBounds().BoxExtent.Length()  );
        }
    }

}
#endif
