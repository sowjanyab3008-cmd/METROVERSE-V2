#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MetroverseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;

UCLASS()
class METROVERSE_API AMetroverseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMetroverseCharacter();
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UStaticMeshComponent> BodyVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UStaticMeshComponent> HeadVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UStaticMeshComponent> LeftArmVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UStaticMeshComponent> RightArmVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UStaticMeshComponent> LeftLegVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Appearance", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UStaticMeshComponent> RightLegVisual;

    UPROPERTY(EditDefaultsOnly, Category="Movement")
    float WalkSpeed = 360.0f;

    UPROPERTY(EditDefaultsOnly, Category="Movement")
    float SprintSpeed = 620.0f;

    UPROPERTY(EditDefaultsOnly, Category="Camera")
    float ThirdPersonArmLength = 320.0f;

    bool bFirstPerson = false;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void StartSprint();
    void StopSprint();
    void ToggleCamera();
};
