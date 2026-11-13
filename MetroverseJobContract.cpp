#include "MetroverseJobContract.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AMetroverseJobContract::AMetroverseJobContract()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    SetReplicateMovement(false);

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    PickupMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMarker"));
    PickupMarker->SetupAttachment(SceneRoot);
    PickupMarker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PickupMarker->SetRelativeScale3D(FVector(0.65f, 0.65f, 0.65f));

    DropoffMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropoffMarker"));
    DropoffMarker->SetupAttachment(SceneRoot);
    DropoffMarker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DropoffMarker->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded())
    {
        MarkerMesh = CubeAsset.Object;
        PickupMarker->SetStaticMesh(MarkerMesh);
        DropoffMarker->SetStaticMesh(MarkerMesh);
    }
    UpdateMarkers();
}

void AMetroverseJobContract::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    UpdateMarkers();
}

void AMetroverseJobContract::UpdateMarkers()
{
    if (PickupMarker) PickupMarker->SetRelativeLocation(PickupOffset);
    if (DropoffMarker) DropoffMarker->SetRelativeLocation(DropoffOffset);
}

FVector AMetroverseJobContract::GetPickupWorldLocation() const
{
    return GetActorTransform().TransformPosition(PickupOffset);
}

FVector AMetroverseJobContract::GetDropoffWorldLocation() const
{
    return GetActorTransform().TransformPosition(DropoffOffset);
}

FString AMetroverseJobContract::GetJobDisplayName() const
{
    switch (JobType)
    {
        case EMetroverseJobType::DeliveryCourier: return TEXT("Delivery Courier");
        case EMetroverseJobType::BusOperator: return TEXT("Bus Operator");
        case EMetroverseJobType::TaxiDriver: return TEXT("Taxi Driver");
        case EMetroverseJobType::TruckHauler: return TEXT("Truck Hauler");
        case EMetroverseJobType::AirportGroundCrew: return TEXT("Airport Ground Crew");
        default: return TEXT("City Worker");
    }
}

bool AMetroverseJobContract::IsPlayerAtPickup(const AActor* PlayerActor) const
{
    return PlayerActor && FVector::DistSquared(PlayerActor->GetActorLocation(), GetPickupWorldLocation()) <= FMath::Square(InteractionRadius);
}

bool AMetroverseJobContract::IsPlayerAtDropoff(const AActor* PlayerActor) const
{
    return PlayerActor && FVector::DistSquared(PlayerActor->GetActorLocation(), GetDropoffWorldLocation()) <= FMath::Square(InteractionRadius);
}
