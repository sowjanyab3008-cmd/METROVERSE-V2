#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "MetroverseVehiclePawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class APlayerController;

/**
 * Chaos Vehicles-based drivable vehicle foundation.
 * Create a Blueprint child, assign a skeletal mesh with a valid vehicle physics asset,
 * configure wheel setups in the VehicleMovement component, then add input mappings.
 */
UCLASS()
class METROVERSE_API AMetroverseVehiclePawn : public AWheeledVehiclePawn
{
    GENERATED_BODY()

public:
    AMetroverseVehiclePawn();

    UFUNCTION(BlueprintPure, Category="METROVERSE|Vehicle")
    bool CanAcceptDriver() const;

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Vehicle")
    void SetDriverInputs(float Throttle, float Steering, float Brake, bool bHandbrake);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Vehicle")
    void ClearDriverInputs();

    void SetDriverController(APlayerController* NewDriver);
    APlayerController* GetDriverController() const { return DriverController.Get(); }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="METROVERSE|Vehicle", meta=(ClampMin="100.0"))
    float MaximumEntryDistance = 450.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="METROVERSE|Vehicle")
    FName DriverEntrySocket = TEXT("DriverEntry");

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="METROVERSE|Vehicle")
    TObjectPtr<USpringArmComponent> VehicleCameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="METROVERSE|Vehicle")
    TObjectPtr<UCameraComponent> VehicleCamera;

private:
    UPROPERTY(ReplicatedUsing=OnRep_DriverController)
    TObjectPtr<APlayerController> DriverController;

    UFUNCTION()
    void OnRep_DriverController();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
