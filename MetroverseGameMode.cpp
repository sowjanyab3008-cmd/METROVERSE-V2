#include "MetroverseGameMode.h"
#include "MetroverseCharacter.h"
#include "MetroversePlayerState.h"
#include "MetroversePlayerController.h"
#include "MetroverseCityGenerator.h"
#include "MetroverseJobContract.h"
#include "MetroverseFactionHQ.h"
#include "MetroverseTransitNetwork.h"
#include "MetroverseRPTypes.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/World.h"

AMetroverseGameMode::AMetroverseGameMode()
{
    DefaultPawnClass = AMetroverseCharacter::StaticClass();
    PlayerControllerClass = AMetroversePlayerController::StaticClass();
    PlayerStateClass = AMetroversePlayerState::StaticClass();
}

void AMetroverseGameMode::BeginPlay()
{
    Super::BeginPlay();
    if (!HasAuthority() || !bCreateStarterDistrict || !GetWorld()) return;

    // A generated PlayerStart means the project can be tested in a fresh empty map.
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), FVector(-500.0f, 0.0f, 120.0f), FRotator::ZeroRotator, SpawnParams);

    // Add a broad primitive ground plane so a blank level is immediately walkable.
    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh)
    {
        AStaticMeshActor* Ground = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(0.0f, 0.0f, -60.0f), FRotator::ZeroRotator, SpawnParams);
        if (Ground)
        {
            Ground->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
            Ground->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("BlockAll"));
            Ground->SetActorScale3D(FVector(6000.0f, 6000.0f, 1.0f));
            Ground->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
        }
    }

    // Build all eight city districts on the shared-world test map and add the
    // central-hub / outer-ring transit network. Each city remains a greybox.
    AMetroverseTransitNetwork* Transit = GetWorld()->SpawnActor<AMetroverseTransitNetwork>(
        AMetroverseTransitNetwork::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    const EMetroverseCity Cities[] = {
        EMetroverseCity::Central, EMetroverseCity::North, EMetroverseCity::South, EMetroverseCity::East,
        EMetroverseCity::West, EMetroverseCity::Coastal, EMetroverseCity::Industrial, EMetroverseCity::Highland
    };
    for (EMetroverseCity City : Cities)
    {
        FVector StationLocation = FVector::ZeroVector;
        if (Transit && Transit->GetCityStationWorldLocation(City, StationLocation))
        {
            const FVector DistrictLocation(StationLocation.X, StationLocation.Y, 0.0f);
            AMetroverseCityGenerator* District = GetWorld()->SpawnActor<AMetroverseCityGenerator>(
                AMetroverseCityGenerator::StaticClass(), DistrictLocation, FRotator::ZeroRotator, SpawnParams);
            if (District)
            {
                District->CityToGenerate = City;
                District->RebuildDistrict();
            }
        }
    }

    // The starter delivery contract is placed at Metro City's central station.
    GetWorld()->SpawnActor<AMetroverseJobContract>(AMetroverseJobContract::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    // Six visible faction headquarters surround the first city blockout. These are
    // greybox buildings with readable signs; final interiors and art come later.
    auto SpawnHQ = [this, &SpawnParams](EMetroverseFaction Faction, const TCHAR* Name, const TCHAR* Subtitle, const FVector& Location)
    {
        AMetroverseFactionHQ* HQ = GetWorld()->SpawnActor<AMetroverseFactionHQ>(
            AMetroverseFactionHQ::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
        if (HQ) HQ->InitializeHQ(Faction, Name, Subtitle);
    };

    SpawnHQ(EMetroverseFaction::Government, TEXT("GOVERNMENT"), TEXT("CIVIC CENTRE"), FVector(-10500.0f, -7000.0f, 350.0f));
    SpawnHQ(EMetroverseFaction::Police, TEXT("METRO POLICE"), TEXT("CENTRAL STATION"), FVector(-10500.0f, 0.0f, 350.0f));
    SpawnHQ(EMetroverseFaction::Hospital, TEXT("MEDICAL SERVICES"), TEXT("GENERAL HOSPITAL"), FVector(-10500.0f, 7000.0f, 350.0f));
    SpawnHQ(EMetroverseFaction::NewsNetwork, TEXT("METRO NEWS"), TEXT("BROADCAST HOUSE"), FVector(10500.0f, -7000.0f, 350.0f));
    SpawnHQ(EMetroverseFaction::FIB, TEXT("FEDERAL BUREAU"), TEXT("FIB HEADQUARTERS"), FVector(10500.0f, 0.0f, 350.0f));
    SpawnHQ(EMetroverseFaction::Administration, TEXT("ADMINISTRATION"), TEXT("COMMUNITY CENTRE"), FVector(10500.0f, 7000.0f, 350.0f));
}
