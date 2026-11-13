#include "MetroverseVehiclePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "ChaosWheeledVehicleMovementComponent.h"

AMetroverseVehiclePawn::AMetroverseVehiclePawn()
{
    bReplicates = true;
    SetReplicateMovement(true);
    NetUpdateFrequency = 60.0f;
    MinNetUpdateFrequency = 30.0f;

    VehicleCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleCameraBoom"));
    VehicleCameraBoom->SetupAttachment(GetMesh());
    VehicleCameraBoom->TargetArmLength = 520.0f;
    VehicleCameraBoom->bUsePawnControlRotation = true;
    VehicleCameraBoom->bEnableCameraLag = true;
    VehicleCameraBoom->CameraLagSpeed = 8.0f;

    VehicleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCamera"));
    VehicleCamera->SetupAttachment(VehicleCameraBoom, USpringArmComponent::SocketName);
    VehicleCamera->bUsePawnControlRotation = false;
}

bool AMetroverseVehiclePawn::CanAcceptDriver() const
{
    return DriverController == nullptr;
}

void AMetroverseVehiclePawn::SetDriverInputs(float Throttle, float Steering, float Brake, bool bHandbrake)
{
    if (!IsValid(DriverController) || GetController() != DriverController) return;

    if (UChaosWheeledVehicleMovementComponent* Movement = GetVehicleMovementComponent())
    {
        Movement->SetThrottleInput(FMath::Clamp(Throttle, 0.0f, 1.0f));
        Movement->SetSteeringInput(FMath::Clamp(Steering, -1.0f, 1.0f));
        Movement->SetBrakeInput(FMath::Clamp(Brake, 0.0f, 1.0f));
        Movement->SetHandbrakeInput(bHandbrake);
    }
}

void AMetroverseVehiclePawn::ClearDriverInputs()
{
    if (UChaosWheeledVehicleMovementComponent* Movement = GetVehicleMovementComponent())
    {
        Movement->SetThrottleInput(0.0f);
        Movement->SetSteeringInput(0.0f);
        Movement->SetBrakeInput(1.0f);
        Movement->SetHandbrakeInput(true);
    }
}

void AMetroverseVehiclePawn::SetDriverController(APlayerController* NewDriver)
{
    if (!HasAuthority()) return;
    DriverController = NewDriver;
    if (!NewDriver) ClearDriverInputs();
    ForceNetUpdate();
}

void AMetroverseVehiclePawn::OnRep_DriverController()
{
    if (!DriverController) ClearDriverInputs();
}

void AMetroverseVehiclePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMetroverseVehiclePawn, DriverController);
}
