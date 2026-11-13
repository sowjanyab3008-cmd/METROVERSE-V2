#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MetroverseRPTypes.h"
#include "MetroverseJobContract.h"

class AMetroversePlayerState;
class AMetroverseVehiclePawn;
#include "MetroversePlayerController.generated.h"

UCLASS()
class METROVERSE_API AMetroversePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="METROVERSE|Chat")
    void SendChatMessage(EMetroverseChatChannel Channel, const FString& Message);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Transit")
    void RequestCityTravel(EMetroverseCity Destination);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Jobs")
    void RequestStartJob(AMetroverseJobContract* Contract);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Jobs")
    void RequestCompleteJob();

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Factions")
    void RequestFactionApplication(EMetroverseFaction RequestedFaction, const FString& Statement);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Factions")
    void RequestPromotion(int32 RequestedRank);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Factions")
    void ReviewFactionApplication(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Factions")
    void ReviewPromotion(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Vehicle")
    void RequestEnterVehicle(AMetroverseVehiclePawn* Vehicle);

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Vehicle")
    void RequestExitVehicle();
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintImplementableEvent, Category="METROVERSE|Jobs")
    void OnJobContractResult(bool bSuccess, const FString& Message);

    UPROPERTY(BlueprintReadOnly, Category="METROVERSE|Chat")
    TArray<FMetroverseChatMessage> RecentChatMessages;

protected:
    UFUNCTION(Server, Reliable)
    void ServerSendChatMessage(EMetroverseChatChannel Channel, const FString& Message);

    UFUNCTION(Server, Reliable)
    void ServerRequestCityTravel(EMetroverseCity Destination);

    UFUNCTION(Client, Reliable)
    void ClientReceiveChatMessage(const FMetroverseChatMessage& Message);

    UFUNCTION(Server, Reliable)
    void ServerStartJob(AMetroverseJobContract* Contract);

    UFUNCTION(Server, Reliable)
    void ServerCompleteJob();

    UFUNCTION(Server, Reliable)
    void ServerApplyToFaction(EMetroverseFaction RequestedFaction, const FString& Statement);

    UFUNCTION(Server, Reliable)
    void ServerRequestFactionPromotion(int32 RequestedRank);

    UFUNCTION(Server, Reliable)
    void ServerReviewFactionApplication(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote);

    UFUNCTION(Server, Reliable)
    void ServerReviewFactionPromotion(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote);

    UFUNCTION(Server, Reliable)
    void ServerRequestEnterVehicle(AMetroverseVehiclePawn* Vehicle);

    UFUNCTION(Server, Reliable)
    void ServerRequestExitVehicle();

    UFUNCTION(Client, Reliable)
    void ClientJobResult(bool bSuccess, const FString& Message);

    UFUNCTION(Client, Reliable)
    void ClientFactionResult(bool bSuccess, const FString& Message);

    void InteractWithJob();

private:
    double LastChatSentAt = -1000.0;
    static constexpr int32 MaxMessageLength = 240;
    static constexpr int32 MaxStoredMessages = 100;

    UPROPERTY()
    TObjectPtr<AMetroverseJobContract> ActiveJobContract;

    double ActiveJobStartedAt = 0.0;

    UPROPERTY()
    TObjectPtr<APawn> PreviousPawnBeforeVehicle;

    UPROPERTY()
    TObjectPtr<AMetroverseVehiclePawn> CurrentDrivenVehicle;
};
