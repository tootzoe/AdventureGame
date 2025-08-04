// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippableToolBase.h"


// Sets default values
AEquippableToolBase::AEquippableToolBase()
{

    ToolMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ToolMeshComp"));
    check(ToolMeshComp != nullptr);




 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEquippableToolBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEquippableToolBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquippableToolBase::UseMe()
{

}

void AEquippableToolBase::BindInputAction(const UInputAction *ActionToBind)
{

}

