// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGameMode.h"





void AAdventureGameMode::StartPlay()
{
    Super::StartPlay();

    UE_LOG(LogTemp, Warning, TEXT("msg....%hs") , __func__);

}
