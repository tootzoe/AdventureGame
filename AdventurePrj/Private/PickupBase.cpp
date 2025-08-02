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

    PickupMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    check(PickupMeshComp != nullptr);


    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    check(SphereComp != nullptr);

   SphereComp ->SetupAttachment(PickupMeshComp);
   SphereComp ->SetSphereRadius(32.f);


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void APickupBase::initializePickup()
{
    if( GEngine ){
        GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Yellow, FString::Printf(TEXT("%hs....") , __func__) );
    }

    if(PickupDataTable && !PickupItemID.IsNone() )
    {
        const FItemData* itRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());

        RefItem = NewObject<UItemDefinition>(this, MakeUniqueObjectName(this, UItemDefinition::StaticClass()));
        RefItem->ID = itRow->ID;
        RefItem->ItemType = itRow->ItemType;
        RefItem-> ItemText = itRow->ItemText;
        RefItem->WorldMesh = itRow->ItemBase->WorldMesh;

        UItemDefinition* tmpIt = itRow->ItemBase.Get();

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
        GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Green, FString::Printf(TEXT("%hs....attempting a pickup collision....") , __func__) );
    }

    AAdventureCharacter* c = Cast<AAdventureCharacter>(OtherActor);
    if(c != nullptr)
    {
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
