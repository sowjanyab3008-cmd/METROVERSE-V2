#include "MetroverseFactionHQ.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

AMetroverseFactionHQ::AMetroverseFactionHQ()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HQBuilding"));
    BuildingMesh->SetupAttachment(SceneRoot);
    BuildingMesh->SetCollisionProfileName(TEXT("BlockAll"));
    BuildingMesh->SetMobility(EComponentMobility::Static);
    BuildingMesh->SetRelativeScale3D(FVector(9.0f, 9.0f, 7.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded()) BuildingMesh->SetStaticMesh(CubeAsset.Object);

    SignText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HQSign"));
    SignText->SetupAttachment(SceneRoot);
    SignText->SetRelativeLocation(FVector(0.0f, -460.0f, 420.0f));
    SignText->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    SignText->SetWorldSize(90.0f);
    SignText->SetTextRenderColor(FColor::White);
    SignText->SetText(FText::FromString(TEXT("METROVERSE HQ")));
    SignText->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMetroverseFactionHQ::InitializeHQ(EMetroverseFaction InFaction, const FString& InDisplayName, const FString& InSubtitle)
{
    Faction = InFaction;
    HQDisplayName = InDisplayName;
    HQSubtitle = InSubtitle;
    UpdateSign();
    ForceNetUpdate();
}

void AMetroverseFactionHQ::OnRep_HQData()
{
    UpdateSign();
}

void AMetroverseFactionHQ::UpdateSign()
{
    if (SignText)
    {
        SignText->SetText(FText::FromString(HQDisplayName + TEXT("\n") + HQSubtitle));
    }
}

void AMetroverseFactionHQ::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMetroverseFactionHQ, Faction);
    DOREPLIFETIME(AMetroverseFactionHQ, HQDisplayName);
    DOREPLIFETIME(AMetroverseFactionHQ, HQSubtitle);
}
