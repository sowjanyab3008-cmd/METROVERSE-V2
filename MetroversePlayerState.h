#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MetroverseRPTypes.h"
#include "MetroversePlayerState.generated.h"

UCLASS()
class METROVERSE_API AMetroversePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AMetroversePlayerState();

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Profile")
    FString RPDisplayName;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Organisation")
    EMetroverseFaction Faction = EMetroverseFaction::None;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Organisation")
    int32 FactionRank = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Organisation")
    EMetroverseCity City = EMetroverseCity::Central;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Sessions")
    FString EventSessionId;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Sessions")
    FString InterviewSessionId;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Sessions")
    bool bInterviewReadOnlyObserver = false;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Progression")
    int32 PlayerLevel = 1;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Progression")
    int32 ExperiencePoints = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Progression")
    int32 CompletedLifeActivities = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Factions")
    int32 FactionServiceMinutes = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Activities")
    TMap<FName, FDateTime> LifeActivityLastCompletedAt;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Jobs")
    EMetroverseJobType CurrentJob = EMetroverseJobType::None;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Economy")
    int64 SoftCurrency = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Moderation")
    TArray<FMetroverseReprimandRecord> ActiveReprimands;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Moderation")
    TArray<FMetroverseReprimandRecord> ReprimandAuditLog;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Vehicles")
    TArray<FMetroverseOwnedVehicle> OwnedVehicles;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Factions")
    FMetroverseFactionApplication FactionApplication;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="METROVERSE|Factions")
    FMetroversePromotionRequest PromotionRequest;

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Permissions")
    bool CanConductSeniorInterview(int32 TargetRank) const;

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Permissions")
    bool CanManageReprimandsFor(const AMetroversePlayerState* Target) const;

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Chat")
    bool CanSendToChatChannel(EMetroverseChatChannel Channel) const;

    UFUNCTION(BlueprintCallable, Category="METROVERSE|Chat")
    bool CanReceiveFromChatChannel(EMetroverseChatChannel Channel, const AMetroversePlayerState* Sender) const;

    // These mutations are intentionally server-only. Call them from trusted server gameplay code.
    bool ServerSetOrganisation(EMetroverseFaction NewFaction, int32 NewRank);
    bool ServerSetCity(EMetroverseCity NewCity);
    bool ServerSetEventSession(const FString& SessionId);
    bool ServerSetInterviewSession(const FString& SessionId, bool bReadOnlyObserver);
    bool ServerAddCurrency(int64 Amount);
    bool ServerSetCurrentJob(EMetroverseJobType NewJob);
    bool ServerCompleteJob(int64 CurrencyReward, int32 ExperienceReward);
    bool ServerCompleteLifeActivity(FName ActivityID, int64 CurrencyReward, int32 ExperienceReward, int32 CooldownHours, bool bRequiresFaction);
    bool ServerRecordFactionServiceMinutes(int32 Minutes);
    bool ServerIssueReprimand(AMetroversePlayerState* Target, const FString& Reason);
    bool ServerRemoveLatestReprimand(AMetroversePlayerState* Target, const FString& Reason);

    // Vehicle ownership and faction career mutations must be called by trusted server code.
    bool ServerPurchaseVehicle(const FString& VehicleID, const FString& DisplayName, const FString& Plate, int64 PriceMC);
    bool ServerSellVehicle(const FString& VehicleID, int64 ResaleValueMC);
    bool ServerStoreVehicleInGarage(const FString& VehicleID, FName GarageID, int32 GarageCapacity = 10);
    bool ServerRetrieveVehicleFromGarage(const FString& VehicleID);
    bool ServerSubmitFactionApplication(EMetroverseFaction RequestedFaction, const FString& Statement);
    bool ServerReviewFactionApplication(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote);
    bool ServerRequestPromotion(int32 RequestedRank, float RequiredServiceHours, int32 RequiredCompletedActivities, int32 MaximumActiveReprimands);
    bool ServerReviewPromotion(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote);

    static FString GetRankTitle(EMetroverseFaction InFaction, int32 Rank);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
