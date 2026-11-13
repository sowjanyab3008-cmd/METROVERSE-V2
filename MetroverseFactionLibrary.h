#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MetroverseRPTypes.h"
#include "MetroverseFactionLibrary.generated.h"

/** Shared faction directory for UI, world labels and Blueprint logic. */
UCLASS()
class METROVERSE_API UMetroverseFactionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category="METROVERSE|Factions")
    static FString GetFactionDisplayName(EMetroverseFaction Faction);

    UFUNCTION(BlueprintPure, Category="METROVERSE|Factions")
    static FString GetFactionDescription(EMetroverseFaction Faction);

    UFUNCTION(BlueprintPure, Category="METROVERSE|Factions")
    static FString GetFactionHeadquarters(EMetroverseFaction Faction);

    UFUNCTION(BlueprintPure, Category="METROVERSE|Factions")
    static int32 GetFactionMaximumRank(EMetroverseFaction Faction);
};
