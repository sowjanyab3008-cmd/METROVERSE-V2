#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MetroverseLifeActivityRow.generated.h"

USTRUCT(BlueprintType)
struct METROVERSE_API FMetroverseLifeActivityRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName ActivityID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Category;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RewardMC = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RewardXP = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 CooldownHours = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bRequiresFaction = false;
};
