#include "MetroverseCityGenerator.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AMetroverseCityGenerator::AMetroverseCityGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    RoadInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RoadInstances"));
    RoadInstances->SetupAttachment(SceneRoot);
    RoadInstances->SetCollisionProfileName(TEXT("BlockAll"));

    SidewalkInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("SidewalkInstances"));
    SidewalkInstances->SetupAttachment(SceneRoot);
    SidewalkInstances->SetCollisionProfileName(TEXT("BlockAll"));

    BuildingInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BuildingInstances"));
    BuildingInstances->SetupAttachment(SceneRoot);
    BuildingInstances->SetCollisionProfileName(TEXT("BlockAll"));

    WindowBandInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WindowBandInstances"));
    WindowBandInstances->SetupAttachment(SceneRoot);
    WindowBandInstances->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    StreetFurnitureInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StreetFurnitureInstances"));
    StreetFurnitureInstances->SetupAttachment(SceneRoot);
    StreetFurnitureInstances->SetCollisionProfileName(TEXT("BlockAll"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded())
    {
        CubeMesh = CubeAsset.Object;
        RoadInstances->SetStaticMesh(CubeMesh);
        SidewalkInstances->SetStaticMesh(CubeMesh);
        BuildingInstances->SetStaticMesh(CubeMesh);
        WindowBandInstances->SetStaticMesh(CubeMesh);
        StreetFurnitureInstances->SetStaticMesh(CubeMesh);
    }
}


FString AMetroverseCityGenerator::GetCityDisplayName() const
{
    switch (CityToGenerate)
    {
        case EMetroverseCity::Central: return TEXT("Metro City");
        case EMetroverseCity::North: return TEXT("Northpoint");
        case EMetroverseCity::South: return TEXT("Southbay");
        case EMetroverseCity::East: return TEXT("Eastford");
        case EMetroverseCity::West: return TEXT("Westhaven");
        case EMetroverseCity::Coastal: return TEXT("Port Azure");
        case EMetroverseCity::Industrial: return TEXT("Ironworks");
        case EMetroverseCity::Highland: return TEXT("Hillcrest");
        default: return TEXT("Metro City");
    }
}

FString AMetroverseCityGenerator::GetCityLandmark() const
{
    switch (CityToGenerate)
    {
        case EMetroverseCity::Central: return TEXT("Civic Plaza and Central Tower");
        case EMetroverseCity::North: return TEXT("Northpoint University and Observatory");
        case EMetroverseCity::South: return TEXT("Southbay Stadium and Waterfront Park");
        case EMetroverseCity::East: return TEXT("Eastford Technology Campus");
        case EMetroverseCity::West: return TEXT("Westhaven Grand Market");
        case EMetroverseCity::Coastal: return TEXT("Port Azure Harbour and Marina");
        case EMetroverseCity::Industrial: return TEXT("Ironworks Freight Terminal");
        case EMetroverseCity::Highland: return TEXT("Hillcrest Ridge and Old Town");
        default: return TEXT("Civic Plaza and Central Tower");
    }
}

FString AMetroverseCityGenerator::GetCityTheme() const
{
    switch (CityToGenerate)
    {
        case EMetroverseCity::Central: return TEXT("Dense modern downtown skyline");
        case EMetroverseCity::North: return TEXT("Leafy university and residential district");
        case EMetroverseCity::South: return TEXT("Warm coastal-suburban city");
        case EMetroverseCity::East: return TEXT("Modern technology and business district");
        case EMetroverseCity::West: return TEXT("Historic market and mixed-use neighbourhoods");
        case EMetroverseCity::Coastal: return TEXT("Harbour, marina and low-rise waterfront");
        case EMetroverseCity::Industrial: return TEXT("Warehouses, logistics and industrial blocks");
        case EMetroverseCity::Highland: return TEXT("Hillside town and terraced neighbourhoods");
        default: return TEXT("Dense modern downtown skyline");
    }
}

void AMetroverseCityGenerator::GetCityPreset(int32& OutBlocks, float& OutSpacing, float& OutRoadWidth,
                                              float& OutMinHeight, float& OutMaxHeight, int32& OutSeed) const
{
    // Presets intentionally produce distinct greybox footprints. Final city art and landmarks
    // will be authored as original modular assets and level content.
    OutBlocks = BlocksPerSide;
    OutSpacing = BlockSpacing;
    OutRoadWidth = RoadWidth;
    OutMinHeight = MinBuildingHeight;
    OutMaxHeight = MaxBuildingHeight;
    OutSeed = RandomSeed;

    switch (CityToGenerate)
    {
        case EMetroverseCity::Central: OutBlocks=6; OutSpacing=2300.f; OutRoadWidth=950.f; OutMinHeight=900.f; OutMaxHeight=3600.f; OutSeed+=11; break;
        case EMetroverseCity::North: OutBlocks=5; OutSpacing=2550.f; OutRoadWidth=850.f; OutMinHeight=500.f; OutMaxHeight=2100.f; OutSeed+=23; break;
        case EMetroverseCity::South: OutBlocks=5; OutSpacing=2250.f; OutRoadWidth=1000.f; OutMinHeight=450.f; OutMaxHeight=1750.f; OutSeed+=37; break;
        case EMetroverseCity::East: OutBlocks=6; OutSpacing=2200.f; OutRoadWidth=900.f; OutMinHeight=650.f; OutMaxHeight=2900.f; OutSeed+=41; break;
        case EMetroverseCity::West: OutBlocks=5; OutSpacing=2700.f; OutRoadWidth=900.f; OutMinHeight=700.f; OutMaxHeight=3000.f; OutSeed+=53; break;
        case EMetroverseCity::Coastal: OutBlocks=6; OutSpacing=2450.f; OutRoadWidth=1050.f; OutMinHeight=450.f; OutMaxHeight=2200.f; OutSeed+=67; break;
        case EMetroverseCity::Industrial: OutBlocks=5; OutSpacing=2500.f; OutRoadWidth=1150.f; OutMinHeight=500.f; OutMaxHeight=1900.f; OutSeed+=79; break;
        case EMetroverseCity::Highland: OutBlocks=5; OutSpacing=2800.f; OutRoadWidth=850.f; OutMinHeight=600.f; OutMaxHeight=2400.f; OutSeed+=97; break;
        default: break;
    }
}

void AMetroverseCityGenerator::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    RebuildDistrict();
}

void AMetroverseCityGenerator::AddBox(UInstancedStaticMeshComponent* Component, const FVector& LocalLocation, const FVector& Scale) const
{
    if (!Component || !CubeMesh) return;
    Component->AddInstance(FTransform(FRotator::ZeroRotator, LocalLocation, Scale));
}

void AMetroverseCityGenerator::RebuildDistrict()
{
    if (!CubeMesh) return;

    RoadInstances->ClearInstances();
    SidewalkInstances->ClearInstances();
    BuildingInstances->ClearInstances();
    WindowBandInstances->ClearInstances();
    StreetFurnitureInstances->ClearInstances();

    int32 PresetBlocks = BlocksPerSide;
    float PresetSpacing = BlockSpacing;
    float PresetRoadWidth = RoadWidth;
    float PresetMinHeight = MinBuildingHeight;
    float PresetMaxHeight = MaxBuildingHeight;
    int32 PresetSeed = RandomSeed;
    GetCityPreset(PresetBlocks, PresetSpacing, PresetRoadWidth, PresetMinHeight, PresetMaxHeight, PresetSeed);

    const int32 SafeBlocks = FMath::Clamp(PresetBlocks, 2, 12);
    const float SafeSpacing = FMath::Max(PresetSpacing, PresetRoadWidth + 500.0f);
    const float HalfRoad = FMath::Max(PresetRoadWidth, 300.0f) * 0.5f;
    const float DistrictHalf = (SafeBlocks - 1) * SafeSpacing * 0.5f;
    FRandomStream Random(PresetSeed);

    // Two perpendicular road strips at each street line create a connected grid.
    for (int32 Index = 0; Index <= SafeBlocks; ++Index)
    {
        const float Line = -DistrictHalf - SafeSpacing * 0.5f + Index * SafeSpacing;
        AddBox(RoadInstances, FVector(Line, 0.0f, -18.0f), FVector(HalfRoad / 50.0f, (DistrictHalf + SafeSpacing) / 50.0f, 0.18f));
        AddBox(RoadInstances, FVector(0.0f, Line, -18.0f), FVector((DistrictHalf + SafeSpacing) / 50.0f, HalfRoad / 50.0f, 0.18f));

        const float CurbOffset = HalfRoad + 55.0f;
        AddBox(SidewalkInstances, FVector(Line + CurbOffset, 0.0f, 8.0f), FVector(0.55f, (DistrictHalf + SafeSpacing) / 50.0f, 0.16f));
        AddBox(SidewalkInstances, FVector(Line - CurbOffset, 0.0f, 8.0f), FVector(0.55f, (DistrictHalf + SafeSpacing) / 50.0f, 0.16f));
        AddBox(SidewalkInstances, FVector(0.0f, Line + CurbOffset, 8.0f), FVector((DistrictHalf + SafeSpacing) / 50.0f, 0.55f, 0.16f));
        AddBox(SidewalkInstances, FVector(0.0f, Line - CurbOffset, 8.0f), FVector((DistrictHalf + SafeSpacing) / 50.0f, 0.55f, 0.16f));
    }

    // Odd-sized districts receive a clear central station/plaza intersection.
    // Even-sized presets already have their main crossing at the map origin.
    if (SafeBlocks % 2 == 1)
    {
        AddBox(RoadInstances, FVector(0.0f, 0.0f, -18.0f), FVector(HalfRoad / 50.0f, (DistrictHalf + SafeSpacing) / 50.0f, 0.18f));
        AddBox(RoadInstances, FVector(0.0f, 0.0f, -18.0f), FVector((DistrictHalf + SafeSpacing) / 50.0f, HalfRoad / 50.0f, 0.18f));
        const float CurbOffset = HalfRoad + 55.0f;
        AddBox(SidewalkInstances, FVector(CurbOffset, 0.0f, 8.0f), FVector(0.55f, (DistrictHalf + SafeSpacing) / 50.0f, 0.16f));
        AddBox(SidewalkInstances, FVector(-CurbOffset, 0.0f, 8.0f), FVector(0.55f, (DistrictHalf + SafeSpacing) / 50.0f, 0.16f));
        AddBox(SidewalkInstances, FVector(0.0f, CurbOffset, 8.0f), FVector((DistrictHalf + SafeSpacing) / 50.0f, 0.55f, 0.16f));
        AddBox(SidewalkInstances, FVector(0.0f, -CurbOffset, 8.0f), FVector((DistrictHalf + SafeSpacing) / 50.0f, 0.55f, 0.16f));
    }

    // Place varied building masses inside each block, leaving walkable street edges.
    for (int32 X = 0; X < SafeBlocks; ++X)
    {
        for (int32 Y = 0; Y < SafeBlocks; ++Y)
        {
            // Keep the middle block open in odd-sized districts for the transit plaza.
            if (SafeBlocks % 2 == 1 && X == SafeBlocks / 2 && Y == SafeBlocks / 2) continue;

            const float CenterX = (X - (SafeBlocks - 1) * 0.5f) * SafeSpacing;
            const float CenterY = (Y - (SafeBlocks - 1) * 0.5f) * SafeSpacing;
            const float FootprintX = Random.FRandRange(6.0f, 9.0f);
            const float FootprintY = Random.FRandRange(6.0f, 9.0f);
            const float Height = Random.FRandRange(PresetMinHeight, FMath::Max(PresetMaxHeight, PresetMinHeight + 100.0f));
            const float OffsetX = Random.FRandRange(-180.0f, 180.0f);
            const float OffsetY = Random.FRandRange(-180.0f, 180.0f);
            const FVector BuildingLocation(CenterX + OffsetX, CenterY + OffsetY, Height * 0.5f);

            AddBox(BuildingInstances, BuildingLocation, FVector(FootprintX, FootprintY, Height / 100.0f));

            // Simple facade bands provide visible floor rhythm for the greybox pass.
            const int32 Floors = FMath::Clamp(FMath::RoundToInt(Height / 330.0f), 2, 9);
            for (int32 Floor = 1; Floor < Floors; ++Floor)
            {
                const float Z = Floor * (Height / Floors);
                AddBox(WindowBandInstances, FVector(BuildingLocation.X, BuildingLocation.Y + FootprintY * 50.0f + 2.0f, Z), FVector(FootprintX * 0.82f, 0.035f, 0.055f));
                AddBox(WindowBandInstances, FVector(BuildingLocation.X + FootprintX * 50.0f + 2.0f, BuildingLocation.Y, Z), FVector(0.035f, FootprintY * 0.82f, 0.055f));
            }
        }
    }

    // Basic streetlight posts at alternating intersections; final art can replace these meshes.
    for (int32 X = 0; X <= SafeBlocks; X += 2)
    {
        for (int32 Y = 0; Y <= SafeBlocks; Y += 2)
        {
            const float PosX = -DistrictHalf - SafeSpacing * 0.5f + X * SafeSpacing + HalfRoad + 100.0f;
            const float PosY = -DistrictHalf - SafeSpacing * 0.5f + Y * SafeSpacing + HalfRoad + 100.0f;
            AddBox(StreetFurnitureInstances, FVector(PosX, PosY, 180.0f), FVector(0.07f, 0.07f, 3.6f));
            AddBox(StreetFurnitureInstances, FVector(PosX + 65.0f, PosY, 350.0f), FVector(1.2f, 0.07f, 0.07f));
        }
    }
}
