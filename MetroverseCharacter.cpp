#include "MetroverseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

AMetroverseCharacter::AMetroverseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = ThirdPersonArmLength;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 10.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Temporary engine-primitive avatar: makes the C++ starter character visible
    // before a licensed/custom skeletal character and animation blueprint are added.
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    auto MakeVisual = [this](const TCHAR* Name, UStaticMesh* Mesh, const FVector& Location, const FVector& Scale)
    {
        UStaticMeshComponent* Visual = CreateDefaultSubobject<UStaticMeshComponent>(Name);
        Visual->SetupAttachment(RootComponent);
        Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Visual->SetGenerateOverlapEvents(false);
        Visual->SetRelativeLocation(Location);
        Visual->SetRelativeScale3D(Scale);
        if (Mesh) Visual->SetStaticMesh(Mesh);
        return Visual;
    };
    BodyVisual = MakeVisual(TEXT("BodyVisual"), CylinderAsset.Succeeded() ? CylinderAsset.Object : nullptr, FVector(0.0f, 0.0f, 0.0f), FVector(0.42f, 0.42f, 0.82f));
    HeadVisual = MakeVisual(TEXT("HeadVisual"), SphereAsset.Succeeded() ? SphereAsset.Object : nullptr, FVector(0.0f, 0.0f, 78.0f), FVector(0.32f, 0.32f, 0.32f));
    LeftArmVisual = MakeVisual(TEXT("LeftArmVisual"), CylinderAsset.Succeeded() ? CylinderAsset.Object : nullptr, FVector(0.0f, -48.0f, 8.0f), FVector(0.13f, 0.13f, 0.48f));
    RightArmVisual = MakeVisual(TEXT("RightArmVisual"), CylinderAsset.Succeeded() ? CylinderAsset.Object : nullptr, FVector(0.0f, 48.0f, 8.0f), FVector(0.13f, 0.13f, 0.48f));
    LeftLegVisual = MakeVisual(TEXT("LeftLegVisual"), CylinderAsset.Succeeded() ? CylinderAsset.Object : nullptr, FVector(0.0f, -18.0f, -58.0f), FVector(0.16f, 0.16f, 0.48f));
    RightLegVisual = MakeVisual(TEXT("RightLegVisual"), CylinderAsset.Succeeded() ? CylinderAsset.Object : nullptr, FVector(0.0f, 18.0f, -58.0f), FVector(0.16f, 0.16f, 0.48f));

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 150.0f;
    GetCharacterMovement()->JumpZVelocity = 480.0f;
    GetCharacterMovement()->AirControl = 0.30f;
}

void AMetroverseCharacter::BeginPlay()
{
    Super::BeginPlay();
    CameraBoom->TargetArmLength = ThirdPersonArmLength;
}

void AMetroverseCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMetroverseCharacter::MoveForward(float Value)
{
    if (Controller && FMath::Abs(Value) > KINDA_SMALL_NUMBER)
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
    }
}

void AMetroverseCharacter::MoveRight(float Value)
{
    if (Controller && FMath::Abs(Value) > KINDA_SMALL_NUMBER)
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
    }
}

void AMetroverseCharacter::Turn(float Value) { AddControllerYawInput(Value); }
void AMetroverseCharacter::LookUp(float Value) { AddControllerPitchInput(Value); }
void AMetroverseCharacter::StartSprint() { GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; }
void AMetroverseCharacter::StopSprint() { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; }

void AMetroverseCharacter::ToggleCamera()
{
    bFirstPerson = !bFirstPerson;
    CameraBoom->TargetArmLength = bFirstPerson ? 0.0f : ThirdPersonArmLength;
    CameraBoom->SocketOffset = bFirstPerson ? FVector(0.0f, 0.0f, 62.0f) : FVector(0.0f, 55.0f, 70.0f);
    GetMesh()->SetOwnerNoSee(bFirstPerson);
    BodyVisual->SetVisibility(!bFirstPerson, true);
    HeadVisual->SetVisibility(!bFirstPerson, true);
    LeftArmVisual->SetVisibility(!bFirstPerson, true);
    RightArmVisual->SetVisibility(!bFirstPerson, true);
    LeftLegVisual->SetVisibility(!bFirstPerson, true);
    RightLegVisual->SetVisibility(!bFirstPerson, true);
}

void AMetroverseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &AMetroverseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMetroverseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AMetroverseCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &AMetroverseCharacter::LookUp);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMetroverseCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMetroverseCharacter::StopSprint);
    PlayerInputComponent->BindAction("ToggleCamera", IE_Pressed, this, &AMetroverseCharacter::ToggleCamera);
}

void AMetroverseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
