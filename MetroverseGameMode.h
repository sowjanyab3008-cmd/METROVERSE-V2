#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MetroverseGameMode.generated.h"

UCLASS()
class METROVERSE_API AMetroverseGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    AMetroverseGameMode();
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category="METROVERSE|Starter World")
    bool bCreateStarterDistrict = true;
};
