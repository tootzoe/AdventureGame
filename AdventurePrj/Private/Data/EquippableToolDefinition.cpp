// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/EquippableToolDefinition.h"






UEquippableToolDefinition *UEquippableToolDefinition::CreateItemCopy() const
{
    UEquippableToolDefinition *itCp = NewObject<UEquippableToolDefinition>( StaticClass( ) );

    itCp->ID = this->ID;
    itCp->ItemType = this->ItemType;
    itCp->ItemText = this->ItemText;
    itCp->WorldMesh = this->WorldMesh;
    itCp->ToolAsset = this->ToolAsset;

   return itCp;

}
