// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemDefinition.h"



UItemDefinition *UItemDefinition::CreateItemCopy() const
{

    UItemDefinition *itCp = NewObject<UItemDefinition>( StaticClass( ) );

    itCp->ID = this->ID;
    itCp->ItemType = this->ItemType;
    itCp->ItemText = this->ItemText;
    itCp->WorldMesh = this->WorldMesh;

   return itCp;
}
