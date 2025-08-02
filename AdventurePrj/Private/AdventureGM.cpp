// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGM.h"



#include "Engine/Engine.h"





void AAdventureGM::StartPlay()
{

    Super::StartPlay();


    check(GEngine != nullptr);



    if( GEngine ){
        GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Cyan, FString::Printf(TEXT("GM 666666 %hs....") , __func__) );
    }

}



