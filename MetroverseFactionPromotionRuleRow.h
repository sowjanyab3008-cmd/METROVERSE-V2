#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MetroverseFactionPromotionRuleRow.generated.h"

USTRUCT(BlueprintType)
struct METROVERSE_API FMetroverseFactionPromotionRuleRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Faction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Rank = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float MinimumServiceHours = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumCompletedActivities = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaximumActiveReprimands = 3;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bRequiresSeniorApproval = true;
};
