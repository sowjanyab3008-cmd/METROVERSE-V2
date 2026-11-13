#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetroverseRPTypes.h"
#include "MetroverseJobContract.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UStaticMesh;

/** Repeatable pickup-to-destination RP job contract placed in a city level. */
UCLASS(BlueprintType)
class METROVERSE_API AMetroverseJobContract : public AActor
{
    GENERATED_BODY()

public:
    AMetroverseJobContract();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Job")
    EMetroverseJobType JobType = EMetroverseJobType::DeliveryCourier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Job", meta=(ClampMin="1"))
    int64 CurrencyReward = 180;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Job", meta=(ClampMin="1"))
    int32 ExperienceReward = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Job", meta=(ClampMin="1.0"))
    float MinimumDurationSeconds = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Job", meta=(ClampMin="10.0"))
    float MaximumDurationSeconds = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Job")
    FVector PickupOffset = FVector(-500.0f, 0.0f, 50.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Job")
    FVector DropoffOffset = FVector(500.0f, 0.0f, 50.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="METROVERSE|Job", meta=(ClampMin="100.0"))
    float InteractionRadius = 350.0f;

    UFUNCTION(BlueprintPure, Category="METROVERSE|Job")
    FVector GetPickupWorldLocation() const;

    UFUNCTION(BlueprintPure, Category="METROVERSE|Job")
    FVector GetDropoffWorldLocation() const;

    UFUNCTION(BlueprintPure, Category="METROVERSE|Job")
    FString GetJobDisplayName() const;

    UFUNCTION(BlueprintPure, Category="METROVERSE|Job")
    bool IsPlayerAtPickup(const AActor* PlayerActor) const;

    UFUNCTION(BlueprintPure, Category="METROVERSE|Job")
    bool IsPlayerAtDropoff(const AActor* PlayerActor) const;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    UPROPERTY(VisibleAnywhere, Category="METROVERSE|Job")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category="METROVERSE|Job")
    TObjectPtr<UStaticMeshComponent> PickupMarker;

    UPROPERTY(VisibleAnywhere, Category="METROVERSE|Job")
    TObjectPtr<UStaticMeshComponent> DropoffMarker;

    UPROPERTY()
    TObjectPtr<UStaticMesh> MarkerMesh;

    void UpdateMarkers();
};
