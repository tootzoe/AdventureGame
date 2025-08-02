/**************************************************************************
**   Copyright @ 2025 TOOTzoe.com
**   Special keywords: thor 8/2/2025 2025
**   Environment variables:
**
**
**
**   E-mail : toot@tootzeo.com
**   Tel    : 13802205042
**   Website: http://www.tootzoe.com
**
**************************************************************************/


#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "ItemData.generated.h"




class UItemDefinition;


UENUM(BlueprintType)
enum class EItemType : uint8
{
  E_Tool UMETA(DisplayName="Tool"),
  E_Consumable UMETA(DisplayName="Consumable"),

  E_Max UMETA(Hidden)
  };


USTRUCT(BlueprintType)
struct FItemText
{
    GENERATED_USTRUCT_BODY()



    UPROPERTY(EditAnywhere, Category="TOOT")
     FText Name;
    UPROPERTY(EditAnywhere, Category="TOOT")
    FText Description;

};



USTRUCT( )
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    //         FItemData : public FTableRowBase() {
    //     Val1 = 0;
    // }

    UPROPERTY(EditAnywhere, Category="TOOT Item Data")
    FName ID;
    UPROPERTY(EditAnywhere, Category="TOOT Item Data")
    EItemType ItemType;
    UPROPERTY(EditAnywhere, Category="TOOT Item Data")
    FItemText ItemText;
    UPROPERTY(EditAnywhere, Category="TOOT Item Data")
    TObjectPtr<UItemDefinition> ItemBase;



};
