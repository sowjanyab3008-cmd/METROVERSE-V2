#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetroverseRPTypes.h"
#include "MetroverseCityGenerator.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;

/**
 * Runtime city blockout generator for the first METROVERSE district.
 * This creates a lightweight greybox layout from engine primitive meshes;
 * it is a level-design foundation, not finished realistic city art.
 */
UCLASS(BlueprintType)
class METROVERSE_API AMetroverseCityGenerator : public AActor
{
    GENERATED_BODY()

public:
    AMetroverseCityGenerator();

    // Each generator actor represents one independently buildable city district/map.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|City")
    EMetroverseCity CityToGenerate = EMetroverseCity::Central;

    UFUNCTION(BlueprintPure, Category="METROVERSE|City")
    FString GetCityDisplayName() const;

    UFUNCTION(BlueprintPure, Category="METROVERSE|City")
    FString GetCityLandmark() const;

    UFUNCTION(BlueprintPure, Category="METROVERSE|City")
    FString GetCityTheme() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|City", meta=(ClampMin="2", ClampMax="12"))
    int32 BlocksPerSide = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|City", meta=(ClampMin="800.0"))
    float BlockSpacing = 2400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|City", meta=(ClampMin="300.0"))
    float RoadWidth = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|City", meta=(ClampMin="500.0"))
    float MinBuildingHeight = 650.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|City", meta=(ClampMin="700.0"))
    float MaxBuildingHeight = 2600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|City")
    int32 RandomSeed = 2026;

    UFUNCTION(CallInEditor, BlueprintCallable, Category="METROVERSE|City")
    void RebuildDistrict();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    UPROPERTY(VisibleAnywhere, Category="METROVERSE|City")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category="METROVERSE|City")
    TObjectPtr<UInstancedStaticMeshComponent> RoadInstances;

    UPROPERTY(VisibleAnywhere, Category="METROVERSE|City")
    TObjectPtr<UInstancedStaticMeshComponent> SidewalkInstances;

    UPROPERTY(VisibleAnywhere, Category="METROVERSE|City")
    TObjectPtr<UInstancedStaticMeshComponent> BuildingInstances;

    UPROPERTY(VisibleAnywhere, Category="METROVERSE|City")
    TObjectPtr<UInstancedStaticMeshComponent> WindowBandInstances;

    UPROPERTY(VisibleAnywhere, Category="METROVERSE|City")
    TObjectPtr<UInstancedStaticMeshComponent> StreetFurnitureInstances;

    UPROPERTY()
    TObjectPtr<UStaticMesh> CubeMesh;

    void AddBox(UInstancedStaticMeshComponent* Component, const FVector& LocalLocation, const FVector& Scale) const;
    void GetCityPreset(int32& OutBlocks, float& OutSpacing, float& OutRoadWidth,
                       float& OutMinHeight, float& OutMaxHeight, int32& OutSeed) const;
};
