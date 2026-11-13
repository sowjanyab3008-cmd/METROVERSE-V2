#include "MetroversePlayerState.h"
#include "Net/UnrealNetwork.h"

AMetroversePlayerState::AMetroversePlayerState()
{
    bReplicates = true;
}

bool AMetroversePlayerState::CanConductSeniorInterview(int32 TargetRank) const
{
    return (Faction == EMetroverseFaction::Government ||
            Faction == EMetroverseFaction::Police ||
            Faction == EMetroverseFaction::Hospital ||
            Faction == EMetroverseFaction::NewsNetwork ||
            Faction == EMetroverseFaction::FIB) &&
           FactionRank >= 9 && (TargetRank == 9 || TargetRank == 10);
}

bool AMetroversePlayerState::CanManageReprimandsFor(const AMetroversePlayerState* Target) const
{
    return HasAuthority() && Target && Target != this &&
           Faction != EMetroverseFaction::None && Faction != EMetroverseFaction::Administration &&
           Faction == Target->Faction && FactionRank >= 7 && FactionRank <= 10;
}

bool AMetroversePlayerState::CanSendToChatChannel(EMetroverseChatChannel Channel) const
{
    switch (Channel)
    {
        case EMetroverseChatChannel::RP:
        case EMetroverseChatChannel::NRP:
            return true;
        case EMetroverseChatChannel::Faction:
            return Faction != EMetroverseFaction::None && FactionRank > 0;
        case EMetroverseChatChannel::SeniorStaff:
            return (Faction != EMetroverseFaction::None && FactionRank >= 7) || Faction == EMetroverseFaction::Administration;
        case EMetroverseChatChannel::Department:
            return Faction == EMetroverseFaction::Police;
        case EMetroverseChatChannel::Event:
            return !EventSessionId.IsEmpty();
        case EMetroverseChatChannel::Action:
            return GetPawn() != nullptr;
        case EMetroverseChatChannel::Interview:
            return !bInterviewReadOnlyObserver && !InterviewSessionId.IsEmpty();
        default:
            return false;
    }
}

bool AMetroversePlayerState::CanReceiveFromChatChannel(EMetroverseChatChannel Channel, const AMetroversePlayerState* Sender) const
{
    if (!Sender) return false;
    switch (Channel)
    {
        case EMetroverseChatChannel::RP:
        case EMetroverseChatChannel::NRP:
            return true;
        case EMetroverseChatChannel::Faction:
            return Faction != EMetroverseFaction::None && Faction == Sender->Faction && City == Sender->City;
        case EMetroverseChatChannel::SeniorStaff:
            return ((Faction != EMetroverseFaction::None && FactionRank >= 7) || Faction == EMetroverseFaction::Administration) &&
                   ((Sender->Faction != EMetroverseFaction::None && Sender->FactionRank >= 7) || Sender->Faction == EMetroverseFaction::Administration);
        case EMetroverseChatChannel::Department:
            return Faction == EMetroverseFaction::Police && Sender->Faction == EMetroverseFaction::Police;
        case EMetroverseChatChannel::Event:
            return !EventSessionId.IsEmpty() && EventSessionId == Sender->EventSessionId;
        case EMetroverseChatChannel::Action:
            return GetPawn() && Sender->GetPawn() && FVector::DistSquared(GetPawn()->GetActorLocation(), Sender->GetPawn()->GetActorLocation()) <= FMath::Square(2500.0f);
        case EMetroverseChatChannel::Interview:
            return !InterviewSessionId.IsEmpty() && InterviewSessionId == Sender->InterviewSessionId;
        default:
            return false;
    }
}

bool AMetroversePlayerState::ServerSetCity(EMetroverseCity NewCity)
{
    if (!HasAuthority()) return false;
    City = NewCity;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerSetEventSession(const FString& SessionId)
{
    if (!HasAuthority() || SessionId.Len() > 64) return false;
    EventSessionId = SessionId;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerSetInterviewSession(const FString& SessionId, bool bReadOnlyObserver)
{
    if (!HasAuthority() || SessionId.Len() > 64) return false;
    InterviewSessionId = SessionId;
    bInterviewReadOnlyObserver = bReadOnlyObserver;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerSetOrganisation(EMetroverseFaction NewFaction, int32 NewRank)
{
    if (!HasAuthority() || NewRank < 0 || NewRank > 10) return false;
    if (NewFaction == EMetroverseFaction::None && NewRank != 0) return false;
    if (NewFaction != EMetroverseFaction::None && NewRank == 0) return false;
    Faction = NewFaction;
    FactionRank = NewRank;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerAddCurrency(int64 Amount)
{
    if (!HasAuthority() || Amount <= 0 || SoftCurrency > MAX_int64 - Amount) return false;
    SoftCurrency += Amount;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerSetCurrentJob(EMetroverseJobType NewJob)
{
    if (!HasAuthority() || (NewJob == EMetroverseJobType::None && CurrentJob == EMetroverseJobType::None)) return false;
    if (NewJob != EMetroverseJobType::None && CurrentJob != EMetroverseJobType::None) return false;
    CurrentJob = NewJob;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerCompleteJob(int64 CurrencyReward, int32 ExperienceReward)
{
    if (!HasAuthority() || CurrentJob == EMetroverseJobType::None || CurrencyReward <= 0 || ExperienceReward <= 0) return false;
    if (SoftCurrency > MAX_int64 - CurrencyReward || ExperiencePoints > MAX_int32 - ExperienceReward) return false;

    SoftCurrency += CurrencyReward;
    ExperiencePoints += ExperienceReward;
    // A new level is earned for every 500 lifetime XP. Keep XP as the lifetime total.
    PlayerLevel = FMath::Clamp(1 + ExperiencePoints / 500, 1, 100);
    CurrentJob = EMetroverseJobType::None;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerCompleteLifeActivity(FName ActivityID, int64 CurrencyReward, int32 ExperienceReward, int32 CooldownHours, bool bRequiresFaction)
{
    if (!HasAuthority() || ActivityID.IsNone() || CurrencyReward < 0 || ExperienceReward < 0 ||
        (CurrencyReward == 0 && ExperienceReward == 0) || CooldownHours < 0 || CooldownHours > 720 ||
        (bRequiresFaction && Faction == EMetroverseFaction::None) ||
        SoftCurrency > MAX_int64 - CurrencyReward || ExperiencePoints > MAX_int32 - ExperienceReward ||
        CompletedLifeActivities == MAX_int32)
        return false;

    const FDateTime Now = FDateTime::UtcNow();
    if (const FDateTime* LastCompleted = LifeActivityLastCompletedAt.Find(ActivityID))
    {
        const FTimespan Cooldown = FTimespan::FromHours(CooldownHours);
        if (CooldownHours > 0 && Now - *LastCompleted < Cooldown) return false;
    }

    SoftCurrency += CurrencyReward;
    ExperiencePoints += ExperienceReward;
    PlayerLevel = FMath::Clamp(1 + ExperiencePoints / 500, 1, 100);
    ++CompletedLifeActivities;
    LifeActivityLastCompletedAt.Add(ActivityID, Now);
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerRecordFactionServiceMinutes(int32 Minutes)
{
    // Trusted server systems should record elapsed, validated on-duty time in small increments.
    if (!HasAuthority() || Faction == EMetroverseFaction::None || Minutes <= 0 || Minutes > 60 ||
        FactionServiceMinutes > MAX_int32 - Minutes) return false;
    FactionServiceMinutes += Minutes;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerIssueReprimand(AMetroversePlayerState* Target, const FString& Reason)
{
    if (!CanManageReprimandsFor(Target) || Reason.TrimStartAndEnd().IsEmpty() || Target->ActiveReprimands.Num() >= 3)
        return false;

    FMetroverseReprimandRecord Record;
    Record.IssuerName = RPDisplayName.IsEmpty() ? GetPlayerName() : RPDisplayName;
    Record.Action = TEXT("Issued reprimand");
    Record.Reason = Reason.Left(300);
    Record.Timestamp = FDateTime::UtcNow();
    Target->ActiveReprimands.Add(Record);
    Target->ReprimandAuditLog.Add(Record);
    Target->ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerRemoveLatestReprimand(AMetroversePlayerState* Target, const FString& Reason)
{
    if (!CanManageReprimandsFor(Target) || Reason.TrimStartAndEnd().IsEmpty() || Target->ActiveReprimands.IsEmpty())
        return false;

    FMetroverseReprimandRecord Record;
    Record.IssuerName = RPDisplayName.IsEmpty() ? GetPlayerName() : RPDisplayName;
    Record.Action = TEXT("Removed latest reprimand");
    Record.Reason = Reason.Left(300);
    Record.Timestamp = FDateTime::UtcNow();
    const FMetroverseReprimandRecord RemovedRecord = Target->ActiveReprimands.Last();
    Record.RelatedRecordSummary = FString::Printf(TEXT("Removed: %s | Issued by: %s | Original reason: %s"),
        *RemovedRecord.Action, *RemovedRecord.IssuerName, *RemovedRecord.Reason);
    Target->ActiveReprimands.Pop();
    Target->ReprimandAuditLog.Add(Record);
    ForceNetUpdate();
    Target->ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerPurchaseVehicle(const FString& VehicleID, const FString& DisplayName, const FString& Plate, int64 PriceMC)
{
    if (!HasAuthority() || VehicleID.IsEmpty() || DisplayName.IsEmpty() || Plate.IsEmpty() || PriceMC <= 0 || SoftCurrency < PriceMC)
        return false;
    for (const FMetroverseOwnedVehicle& Vehicle : OwnedVehicles)
        if (Vehicle.VehicleID.Equals(VehicleID, ESearchCase::IgnoreCase) ||
            Vehicle.Plate.Equals(Plate, ESearchCase::IgnoreCase)) return false;

    FMetroverseOwnedVehicle Vehicle;
    Vehicle.VehicleID = VehicleID.Left(32);
    Vehicle.DisplayName = DisplayName.Left(64);
    Vehicle.Plate = Plate.Left(16);
    Vehicle.PurchasePriceMC = PriceMC;
    Vehicle.PurchasedAt = FDateTime::UtcNow();
    Vehicle.GarageID = NAME_None;
    Vehicle.bStoredInGarage = false;
    SoftCurrency -= PriceMC;
    OwnedVehicles.Add(Vehicle);
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerSellVehicle(const FString& VehicleID, int64 ResaleValueMC)
{
    if (!HasAuthority() || VehicleID.IsEmpty() || ResaleValueMC <= 0 || SoftCurrency > MAX_int64 - ResaleValueMC)
        return false;
    const int32 Index = OwnedVehicles.IndexOfByPredicate([&](const FMetroverseOwnedVehicle& V) {
        return V.VehicleID.Equals(VehicleID, ESearchCase::IgnoreCase);
    });
    if (Index == INDEX_NONE) return false;

    // A vehicle must be safely stored before sale; payout cannot exceed 70% of its recorded purchase price.
    const FMetroverseOwnedVehicle& VehicleToSell = OwnedVehicles[Index];
    if (!VehicleToSell.bStoredInGarage || VehicleToSell.PurchasePriceMC <= 0) return false;
    // Divide before multiplying to avoid overflowing int64 for unusually large trusted catalogue prices.
    const int64 MaximumResaleValue = (VehicleToSell.PurchasePriceMC / 10) * 7 +
        ((VehicleToSell.PurchasePriceMC % 10) * 7) / 10;
    if (ResaleValueMC > MaximumResaleValue) return false;

    OwnedVehicles.RemoveAt(Index);
    SoftCurrency += ResaleValueMC;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerStoreVehicleInGarage(const FString& VehicleID, FName GarageID, int32 GarageCapacity)
{
    if (!HasAuthority() || VehicleID.IsEmpty() || GarageID.IsNone() || GarageCapacity < 1 || GarageCapacity > 50)
        return false;

    static const TSet<FName> ValidGarages = {
        FName(TEXT("MetroCentral")), FName(TEXT("Northpoint")), FName(TEXT("Southbay")),
        FName(TEXT("Eastford")), FName(TEXT("Westhaven")), FName(TEXT("PortAzure")),
        FName(TEXT("Ironworks")), FName(TEXT("Hillcrest"))
    };
    if (!ValidGarages.Contains(GarageID)) return false;

    const int32 VehicleIndex = OwnedVehicles.IndexOfByPredicate(
        [&](const FMetroverseOwnedVehicle& V) { return V.VehicleID == VehicleID; });
    if (VehicleIndex == INDEX_NONE || OwnedVehicles[VehicleIndex].bStoredInGarage) return false;

    int32 StoredCount = 0;
    for (const FMetroverseOwnedVehicle& V : OwnedVehicles)
        if (V.bStoredInGarage && V.GarageID == GarageID) ++StoredCount;
    if (StoredCount >= GarageCapacity) return false;

    OwnedVehicles[VehicleIndex].GarageID = GarageID;
    OwnedVehicles[VehicleIndex].bStoredInGarage = true;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerRetrieveVehicleFromGarage(const FString& VehicleID)
{
    if (!HasAuthority() || VehicleID.IsEmpty()) return false;
    const int32 VehicleIndex = OwnedVehicles.IndexOfByPredicate(
        [&](const FMetroverseOwnedVehicle& V) { return V.VehicleID == VehicleID; });
    if (VehicleIndex == INDEX_NONE || !OwnedVehicles[VehicleIndex].bStoredInGarage) return false;

    OwnedVehicles[VehicleIndex].bStoredInGarage = false;
    OwnedVehicles[VehicleIndex].GarageID = NAME_None;
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerSubmitFactionApplication(EMetroverseFaction RequestedFaction, const FString& Statement)
{
    if (!HasAuthority() || RequestedFaction == EMetroverseFaction::None || RequestedFaction == EMetroverseFaction::Administration ||
        Faction != EMetroverseFaction::None || Statement.TrimStartAndEnd().Len() < 20 ||
        FactionApplication.Status == EMetroverseApplicationStatus::Pending)
        return false;
    FactionApplication = FMetroverseFactionApplication();
    FactionApplication.RequestedFaction = RequestedFaction;
    FactionApplication.Statement = Statement.Left(1000);
    FactionApplication.Status = EMetroverseApplicationStatus::Pending;
    FactionApplication.SubmittedAt = FDateTime::UtcNow();
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerReviewFactionApplication(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote)
{
    if (!HasAuthority() || !Applicant || Applicant == this || Applicant->FactionApplication.Status != EMetroverseApplicationStatus::Pending ||
        ReviewNote.TrimStartAndEnd().IsEmpty()) return false;
    const bool bCanReview = Faction == EMetroverseFaction::Administration ||
        (Faction == Applicant->FactionApplication.RequestedFaction && FactionRank >= 8);
    if (!bCanReview) return false;
    Applicant->FactionApplication.Status = bApprove ? EMetroverseApplicationStatus::Approved : EMetroverseApplicationStatus::Rejected;
    Applicant->FactionApplication.ReviewerName = RPDisplayName.IsEmpty() ? GetPlayerName() : RPDisplayName;
    Applicant->FactionApplication.ReviewNote = ReviewNote.Left(300);
    if (bApprove) Applicant->ServerSetOrganisation(Applicant->FactionApplication.RequestedFaction, 1);
    Applicant->ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerRequestPromotion(int32 RequestedRank, float RequiredServiceHours, int32 RequiredCompletedActivities, int32 MaximumActiveReprimands)
{
    if (!HasAuthority() || Faction == EMetroverseFaction::None || RequestedRank != FactionRank + 1 || RequestedRank > 10 ||
        !FMath::IsFinite(RequiredServiceHours) || RequiredServiceHours < 0.0f || RequiredCompletedActivities < 0 ||
        MaximumActiveReprimands < 0 || ActiveReprimands.Num() > MaximumActiveReprimands ||
        static_cast<float>(FactionServiceMinutes) < RequiredServiceHours * 60.0f ||
        CompletedLifeActivities < RequiredCompletedActivities || PromotionRequest.Status == EMetroverseApplicationStatus::Pending)
        return false;
    PromotionRequest = FMetroversePromotionRequest();
    PromotionRequest.RequestedRank = RequestedRank;
    PromotionRequest.Status = EMetroverseApplicationStatus::Pending;
    PromotionRequest.SubmittedAt = FDateTime::UtcNow();
    ForceNetUpdate();
    return true;
}

bool AMetroversePlayerState::ServerReviewPromotion(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote)
{
    if (!HasAuthority() || !Applicant || Applicant == this || Applicant->Faction != Faction || Faction == EMetroverseFaction::None ||
        Applicant->PromotionRequest.Status != EMetroverseApplicationStatus::Pending || ReviewNote.TrimStartAndEnd().IsEmpty() ||
        FactionRank < 7 || FactionRank <= Applicant->FactionRank) return false;
    const int32 RequestedRank = Applicant->PromotionRequest.RequestedRank;
    if (RequestedRank != Applicant->FactionRank + 1 || RequestedRank > 10) return false;
    Applicant->PromotionRequest.Status = bApprove ? EMetroverseApplicationStatus::Approved : EMetroverseApplicationStatus::Rejected;
    Applicant->PromotionRequest.ReviewerName = RPDisplayName.IsEmpty() ? GetPlayerName() : RPDisplayName;
    Applicant->PromotionRequest.ReviewNote = ReviewNote.Left(300);
    if (bApprove) Applicant->ServerSetOrganisation(Applicant->Faction, RequestedRank);
    Applicant->ForceNetUpdate();
    return true;
}

FString AMetroversePlayerState::GetRankTitle(EMetroverseFaction InFaction, int32 Rank)
{
    static const TArray<FString> Government = { TEXT("Government Intern"), TEXT("Junior Government Officer"), TEXT("Government Officer"), TEXT("Senior Government Officer"), TEXT("Department Director"), TEXT("Deputy Minister"), TEXT("Minister"), TEXT("Cabinet Secretary"), TEXT("Vice President / Deputy Governor"), TEXT("President / Governor") };
    static const TArray<FString> Police = { TEXT("Police Cadet"), TEXT("Senior Police Officer"), TEXT("Corporal"), TEXT("Sergeant"), TEXT("Lieutenant"), TEXT("Captain"), TEXT("Deputy Chief of Police"), TEXT("Chief of Police"), TEXT("Deputy Commissioner"), TEXT("Police Commissioner") };
    static const TArray<FString> Hospital = { TEXT("Medical Intern"), TEXT("Nurse"), TEXT("Medical Doctor"), TEXT("Specialist Doctor"), TEXT("Senior Surgeon"), TEXT("Head Doctor"), TEXT("Deputy Chief Physician"), TEXT("Chief Physician"), TEXT("Deputy Medical Director"), TEXT("Chief Medical Director") };
    static const TArray<FString> News = { TEXT("Trainee Reporter"), TEXT("Journalist"), TEXT("Senior Journalist"), TEXT("Editor"), TEXT("Senior Editor"), TEXT("News Producer"), TEXT("Deputy Chief Editor"), TEXT("Chief Editor"), TEXT("Deputy Network Director"), TEXT("Network Director") };
    static const TArray<FString> FIBRanks = { TEXT("Trainee Agent"), TEXT("Federal Agent"), TEXT("Senior Agent"), TEXT("Special Agent"), TEXT("Senior Special Agent"), TEXT("Supervisory Special Agent"), TEXT("Chief of Investigations"), TEXT("Assistant Director"), TEXT("Deputy Director"), TEXT("FIB Director") };
    static const TArray<FString> Admin = { TEXT("Trial Moderator"), TEXT("Moderator"), TEXT("Senior Moderator"), TEXT("Administrator"), TEXT("Senior Administrator"), TEXT("Head Administrator"), TEXT("Deputy Manager"), TEXT("Server Manager"), TEXT("Executive Manager"), TEXT("Founder") };

    if (Rank < 1 || Rank > 10) return TEXT("Unassigned");
    const int32 Index = Rank - 1;
    switch (InFaction)
    {
        case EMetroverseFaction::Government: return Government.IsValidIndex(Index) ? Government[Index] : TEXT("Unassigned");
        case EMetroverseFaction::Police: return Police.IsValidIndex(Index) ? Police[Index] : TEXT("Unassigned");
        case EMetroverseFaction::Hospital: return Hospital.IsValidIndex(Index) ? Hospital[Index] : TEXT("Unassigned");
        case EMetroverseFaction::NewsNetwork: return News.IsValidIndex(Index) ? News[Index] : TEXT("Unassigned");
        case EMetroverseFaction::FIB: return FIBRanks.IsValidIndex(Index) ? FIBRanks[Index] : TEXT("Unassigned");
        case EMetroverseFaction::Administration: return Admin.IsValidIndex(Index) ? Admin[Index] : TEXT("Unassigned");
        default: return TEXT("Unassigned");
    }
}

void AMetroversePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMetroversePlayerState, RPDisplayName);
    DOREPLIFETIME(AMetroversePlayerState, Faction);
    DOREPLIFETIME(AMetroversePlayerState, FactionRank);
    DOREPLIFETIME(AMetroversePlayerState, City);
    DOREPLIFETIME(AMetroversePlayerState, EventSessionId);
    DOREPLIFETIME(AMetroversePlayerState, InterviewSessionId);
    DOREPLIFETIME(AMetroversePlayerState, bInterviewReadOnlyObserver);
    DOREPLIFETIME(AMetroversePlayerState, PlayerLevel);
    DOREPLIFETIME(AMetroversePlayerState, ExperiencePoints);
    DOREPLIFETIME(AMetroversePlayerState, CompletedLifeActivities);
    DOREPLIFETIME(AMetroversePlayerState, FactionServiceMinutes);
    DOREPLIFETIME(AMetroversePlayerState, LifeActivityLastCompletedAt);
    DOREPLIFETIME(AMetroversePlayerState, CurrentJob);
    DOREPLIFETIME(AMetroversePlayerState, SoftCurrency);
    DOREPLIFETIME(AMetroversePlayerState, ActiveReprimands);
    DOREPLIFETIME(AMetroversePlayerState, ReprimandAuditLog);
    DOREPLIFETIME(AMetroversePlayerState, OwnedVehicles);
    DOREPLIFETIME(AMetroversePlayerState, FactionApplication);
    DOREPLIFETIME(AMetroversePlayerState, PromotionRequest);
}
