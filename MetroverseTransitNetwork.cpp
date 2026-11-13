#include "MetroverseTransitNetwork.h"

AMetroverseTransitNetwork::AMetroverseTransitNetwork()
{
    PrimaryActorTick.bCanEverTick = false;
    SetActorHiddenInGame(true);
    SetCanBeDamaged(false);

    // Eight city hubs laid out as a shared-world test map. Place each city generator
    // at its matching anchor; production should use streamed maps or World Partition.
    CityStationOffsets.Add(EMetroverseCity::Central, FVector(0.0, 0.0, 120.0));
    CityStationOffsets.Add(EMetroverseCity::North, FVector(180000.0, 0.0, 120.0));
    CityStationOffsets.Add(EMetroverseCity::South, FVector(-180000.0, 0.0, 120.0));
    CityStationOffsets.Add(EMetroverseCity::East, FVector(0.0, 180000.0, 120.0));
    CityStationOffsets.Add(EMetroverseCity::West, FVector(0.0, -180000.0, 120.0));
    CityStationOffsets.Add(EMetroverseCity::Coastal, FVector(180000.0, 180000.0, 120.0));
    CityStationOffsets.Add(EMetroverseCity::Industrial, FVector(-180000.0, 180000.0, 120.0));
    CityStationOffsets.Add(EMetroverseCity::Highland, FVector(180000.0, -180000.0, 120.0));
}

bool AMetroverseTransitNetwork::GetCityStationWorldLocation(EMetroverseCity City, FVector& OutLocation) const
{
    const FVector* Offset = CityStationOffsets.Find(City);
    if (!Offset) return false;
    OutLocation = GetActorTransform().TransformPosition(*Offset);
    return true;
}

bool AMetroverseTransitNetwork::IsRouteAvailable(EMetroverseCity From, EMetroverseCity To) const
{
    if (From == To || !CityStationOffsets.Contains(From) || !CityStationOffsets.Contains(To)) return false;

    // Metro City is the central interchange. A ring route also links the outer cities.
    if (From == EMetroverseCity::Central || To == EMetroverseCity::Central) return true;

    const TPair<EMetroverseCity, EMetroverseCity> Ring[] = {
        { EMetroverseCity::North, EMetroverseCity::East },
        { EMetroverseCity::East, EMetroverseCity::Coastal },
        { EMetroverseCity::Coastal, EMetroverseCity::South },
        { EMetroverseCity::South, EMetroverseCity::West },
        { EMetroverseCity::West, EMetroverseCity::Industrial },
        { EMetroverseCity::Industrial, EMetroverseCity::Highland },
        { EMetroverseCity::Highland, EMetroverseCity::North }
    };
    for (const auto& Link : Ring)
    {
        if ((From == Link.Key && To == Link.Value) || (From == Link.Value && To == Link.Key)) return true;
    }
    return false;
}

FString AMetroverseTransitNetwork::GetRouteDescription(EMetroverseCity From, EMetroverseCity To) const
{
    if (!IsRouteAvailable(From, To)) return TEXT("Route unavailable");
    return FString::Printf(TEXT("%s Transit: %s to %s"),
        (From == EMetroverseCity::Central || To == EMetroverseCity::Central) ? TEXT("Metro Express") : TEXT("Intercity Link"),
        *UEnum::GetDisplayValueAsText(From).ToString(), *UEnum::GetDisplayValueAsText(To).ToString());
}
