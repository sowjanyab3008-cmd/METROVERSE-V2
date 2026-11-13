#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetroverseRPTypes.h"
#include "MetroverseFactionHQ.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

/** Physical greybox headquarters marker for one roleplay faction. */
UCLASS(BlueprintType)
class METROVERSE_API AMetroverseFactionHQ : public AActor
{
    GENERATED_BODY()

public:
    AMetroverseFactionHQ();

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Factions")
    void InitializeHQ(EMetroverseFaction InFaction, const FString& InDisplayName, const FString& InSubtitle);

    UFUNCTION(BlueprintPure, Category="METROVERSE|Factions")
    EMetroverseFaction GetFaction() const { return Faction; }

    UFUNCTION(BlueprintPure, Category="METROVERSE|Factions")
    FString GetHQDisplayName() const { return HQDisplayName; }

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="METROVERSE|Factions")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="METROVERSE|Factions")
    TObjectPtr<UStaticMeshComponent> BuildingMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="METROVERSE|Factions")
    TObjectPtr<UTextRenderComponent> SignText;

    UPROPERTY(ReplicatedUsing=OnRep_HQData, VisibleAnywhere, BlueprintReadOnly, Category="METROVERSE|Factions")
    EMetroverseFaction Faction = EMetroverseFaction::None;

    UPROPERTY(ReplicatedUsing=OnRep_HQData, VisibleAnywhere, BlueprintReadOnly, Category="METROVERSE|Factions")
    FString HQDisplayName;

    UPROPERTY(ReplicatedUsing=OnRep_HQData, VisibleAnywhere, BlueprintReadOnly, Category="METROVERSE|Factions")
    FString HQSubtitle;

    UFUNCTION()
    void OnRep_HQData();

    void UpdateSign();
};
