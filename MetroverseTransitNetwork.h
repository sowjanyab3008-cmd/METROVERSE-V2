#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetroverseRPTypes.h"
#include "MetroverseTransitNetwork.generated.h"

/**
 * Server-authoritative inter-city transit network.
 * City coordinates are configurable anchors for a shared-world blockout.
 * Replace this with streamed levels/World Partition travel for production.
 */
UCLASS(BlueprintType)
class METROVERSE_API AMetroverseTransitNetwork : public AActor
{
    GENERATED_BODY()

public:
    AMetroverseTransitNetwork();

    UFUNCTION(BlueprintPure, Category="METROVERSE|Transit")
    bool GetCityStationWorldLocation(EMetroverseCity City, FVector& OutLocation) const;

    UFUNCTION(BlueprintPure, Category="METROVERSE|Transit")
    bool IsRouteAvailable(EMetroverseCity From, EMetroverseCity To) const;

    UFUNCTION(BlueprintPure, Category="METROVERSE|Transit")
    FString GetRouteDescription(EMetroverseCity From, EMetroverseCity To) const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Transit", meta=(ClampMin="100.0"))
    float StationArrivalRadius = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Transit")
    TMap<EMetroverseCity, FVector> CityStationOffsets;
};
