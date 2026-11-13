#include "MetroversePlayerController.h"
#include "MetroversePlayerState.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h"
#include "MetroverseTransitNetwork.h"
#include "MetroverseVehiclePawn.h"
#include "Engine/Engine.h"

void AMetroversePlayerController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());
    if (IsLocalController() && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Cyan, TEXT("METROVERSE RP | WASD Move | Mouse Look | Shift Sprint | Space Jump | V Camera | E Interact"));
        GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Green, TEXT("Starter delivery: press E near the pickup marker, reach the destination, then press E again."));
    }
}

void AMetroversePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (InputComponent) InputComponent->BindAction("Interact", IE_Pressed, this, &AMetroversePlayerController::InteractWithJob);
}

void AMetroversePlayerController::InteractWithJob()
{
    if (!GetPawn() || !GetWorld()) return;

    if (IsValid(ActiveJobContract))
    {
        RequestCompleteJob();
        return;
    }

    AMetroverseJobContract* ClosestContract = nullptr;
    float ClosestDistanceSquared = TNumericLimits<float>::Max();
    for (TActorIterator<AMetroverseJobContract> It(GetWorld()); It; ++It)
    {
        AMetroverseJobContract* Candidate = *It;
        if (!Candidate || !Candidate->IsPlayerAtPickup(GetPawn())) continue;
        const float DistanceSquared = FVector::DistSquared(GetPawn()->GetActorLocation(), Candidate->GetPickupWorldLocation());
        if (DistanceSquared < ClosestDistanceSquared)
        {
            ClosestDistanceSquared = DistanceSquared;
            ClosestContract = Candidate;
        }
    }

    if (ClosestContract) RequestStartJob(ClosestContract);
    else if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Yellow, TEXT("Move close to the glowing job pickup and press E."));
}

void AMetroversePlayerController::SendChatMessage(EMetroverseChatChannel Channel, const FString& Message)
{
    ServerSendChatMessage(Channel, Message);
}

void AMetroversePlayerController::ServerSendChatMessage_Implementation(EMetroverseChatChannel Channel, const FString& Message)
{
    AMetroversePlayerState* SenderState = GetPlayerState<AMetroversePlayerState>();
    if (!SenderState || !SenderState->CanSendToChatChannel(Channel)) return;

    const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;
    if (Now - LastChatSentAt < 0.8) return;
    LastChatSentAt = Now;

    FString CleanMessage;
    CleanMessage.Reserve(FMath::Min(Message.Len(), MaxMessageLength));
    for (TCHAR Character : Message)
    {
        if (!FChar::IsControl(Character) || Character == TEXT('\t'))
        {
            CleanMessage.AppendChar(Character);
            if (CleanMessage.Len() >= MaxMessageLength) break;
        }
    }
    CleanMessage = CleanMessage.TrimStartAndEnd();
    if (CleanMessage.IsEmpty()) return;

    FMetroverseChatMessage ChatMessage;
    ChatMessage.SenderName = SenderState->RPDisplayName.IsEmpty() ? SenderState->GetPlayerName() : SenderState->RPDisplayName;
    ChatMessage.Message = CleanMessage;
    ChatMessage.Timestamp = FDateTime::UtcNow();
    ChatMessage.Channel = Channel;

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AMetroversePlayerController* RecipientController = Cast<AMetroversePlayerController>(It->Get());
        if (!RecipientController) continue;
        const AMetroversePlayerState* RecipientState = RecipientController->GetPlayerState<AMetroversePlayerState>();
        if (RecipientState && RecipientState->CanReceiveFromChatChannel(Channel, SenderState))
        {
            RecipientController->ClientReceiveChatMessage(ChatMessage);
        }
    }
}


void AMetroversePlayerController::RequestCityTravel(EMetroverseCity Destination)
{
    ServerRequestCityTravel(Destination);
}

void AMetroversePlayerController::ServerRequestCityTravel_Implementation(EMetroverseCity Destination)
{
    if (!HasAuthority() || !GetWorld() || !GetPawn()) return;

    AMetroversePlayerState* PlayerState = GetPlayerState<AMetroversePlayerState>();
    if (!PlayerState || PlayerState->City == Destination) return;

    AMetroverseTransitNetwork* Transit = nullptr;
    for (TActorIterator<AMetroverseTransitNetwork> It(GetWorld()); It; ++It)
    {
        Transit = *It;
        break;
    }
    if (!Transit || !Transit->IsRouteAvailable(PlayerState->City, Destination)) return;

    FVector CurrentStation;
    FVector DestinationStation;
    if (!Transit->GetCityStationWorldLocation(PlayerState->City, CurrentStation) ||
        !Transit->GetCityStationWorldLocation(Destination, DestinationStation)) return;

    // The server checks station proximity and route availability; clients cannot
    // simply submit a city name to teleport across the world.
    const float AllowedDistance = Transit->StationArrivalRadius;
    if (FVector::DistSquared(GetPawn()->GetActorLocation(), CurrentStation) > FMath::Square(AllowedDistance)) return;

    if (PlayerState->ServerSetCity(Destination))
    {
        GetPawn()->SetActorLocation(DestinationStation + FVector(0.0, 0.0, 120.0), false, nullptr, ETeleportType::TeleportPhysics);
        GetPawn()->ForceNetUpdate();
    }
}

void AMetroversePlayerController::ClientReceiveChatMessage_Implementation(const FMetroverseChatMessage& Message)
{
    RecentChatMessages.Add(Message);
    if (RecentChatMessages.Num() > MaxStoredMessages)
    {
        RecentChatMessages.RemoveAt(0, RecentChatMessages.Num() - MaxStoredMessages, EAllowShrinking::No);
    }
}


void AMetroversePlayerController::RequestStartJob(AMetroverseJobContract* Contract)
{
    ServerStartJob(Contract);
}

void AMetroversePlayerController::RequestCompleteJob()
{
    ServerCompleteJob();
}

void AMetroversePlayerController::ServerStartJob_Implementation(AMetroverseJobContract* Contract)
{
    AMetroversePlayerState* State = GetPlayerState<AMetroversePlayerState>();
    APawn* ControlledPawn = GetPawn();
    if (!HasAuthority() || !GetWorld() || !State || !ControlledPawn || !IsValid(Contract))
    {
        ClientJobResult(false, TEXT("Job could not be started."));
        return;
    }
    if (ActiveJobContract || State->CurrentJob != EMetroverseJobType::None)
    {
        ClientJobResult(false, TEXT("Finish or cancel your current job first."));
        return;
    }
    if (!Contract->HasAuthority() || !Contract->IsPlayerAtPickup(ControlledPawn))
    {
        ClientJobResult(false, TEXT("Move closer to the job pickup marker."));
        return;
    }
    if (Contract->JobType == EMetroverseJobType::None || Contract->CurrencyReward <= 0 || Contract->ExperienceReward <= 0 ||
        Contract->MaximumDurationSeconds < Contract->MinimumDurationSeconds)
    {
        ClientJobResult(false, TEXT("This job contract is not configured correctly."));
        return;
    }
    if (!State->ServerSetCurrentJob(Contract->JobType))
    {
        ClientJobResult(false, TEXT("Unable to accept this job."));
        return;
    }

    ActiveJobContract = Contract;
    ActiveJobStartedAt = GetWorld()->GetTimeSeconds();
    ClientJobResult(true, FString::Printf(TEXT("Job accepted: %s. Reach the destination marker."), *Contract->GetJobDisplayName()));
}

void AMetroversePlayerController::ServerCompleteJob_Implementation()
{
    AMetroversePlayerState* State = GetPlayerState<AMetroversePlayerState>();
    APawn* ControlledPawn = GetPawn();
    if (!HasAuthority() || !State || !ControlledPawn || !IsValid(ActiveJobContract))
    {
        ClientJobResult(false, TEXT("You do not have an active job."));
        return;
    }

    const double Elapsed = GetWorld()->GetTimeSeconds() - ActiveJobStartedAt;
    if (Elapsed > ActiveJobContract->MaximumDurationSeconds)
    {
        State->ServerSetCurrentJob(EMetroverseJobType::None);
        ActiveJobContract = nullptr;
        ActiveJobStartedAt = 0.0;
        ClientJobResult(false, TEXT("The job contract expired."));
        return;
    }
    if (Elapsed < ActiveJobContract->MinimumDurationSeconds)
    {
        ClientJobResult(false, TEXT("Complete the journey before claiming the reward."));
        return;
    }
    if (!ActiveJobContract->IsPlayerAtDropoff(ControlledPawn))
    {
        ClientJobResult(false, TEXT("You have not reached the destination marker."));
        return;
    }

    const int64 Reward = ActiveJobContract->CurrencyReward;
    const int32 XP = ActiveJobContract->ExperienceReward;
    const FString JobName = ActiveJobContract->GetJobDisplayName();
    if (!State->ServerCompleteJob(Reward, XP))
    {
        ClientJobResult(false, TEXT("The server could not process this job reward."));
        return;
    }

    ActiveJobContract = nullptr;
    ActiveJobStartedAt = 0.0;
    ClientJobResult(true, FString::Printf(TEXT("%s completed! +%lld credits, +%d XP."), *JobName, Reward, XP));
}

void AMetroversePlayerController::ClientJobResult_Implementation(bool bSuccess, const FString& Message)
{
    OnJobContractResult(bSuccess, Message);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.0f, bSuccess ? FColor::Green : FColor::Red, Message);
}



void AMetroversePlayerController::RequestFactionApplication(EMetroverseFaction RequestedFaction, const FString& Statement)
{
    ServerApplyToFaction(RequestedFaction, Statement);
}

void AMetroversePlayerController::ServerApplyToFaction_Implementation(EMetroverseFaction RequestedFaction, const FString& Statement)
{
    AMetroversePlayerState* State = GetPlayerState<AMetroversePlayerState>();
    if (!HasAuthority() || !State) return;
    const bool bSubmitted = State->ServerSubmitFactionApplication(RequestedFaction, Statement);
    ClientFactionResult(bSubmitted, bSubmitted ? TEXT("Faction application submitted for review.") : TEXT("Faction application rejected. Check your eligibility and statement."));
}

void AMetroversePlayerController::RequestPromotion(int32 RequestedRank)
{
    ServerRequestFactionPromotion(RequestedRank);
}

void AMetroversePlayerController::ServerRequestFactionPromotion_Implementation(int32 RequestedRank)
{
    AMetroversePlayerState* State = GetPlayerState<AMetroversePlayerState>();
    if (!HasAuthority() || !State) return;

    // Thresholds are selected on the server from FactionPromotionRules.csv.
    // Clients submit only the requested next rank and cannot lower their own requirements.
    static const float ServiceHoursByRank[11] = { 0.0f, 0.0f, 2.0f, 5.0f, 10.0f, 18.0f, 28.0f, 40.0f, 55.0f, 75.0f, 100.0f };
    static const int32 ActivitiesByRank[11] = { 0, 0, 3, 6, 10, 15, 22, 30, 40, 55, 75 };
    static const int32 ReprimandsByRank[11] = { 0, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0 };

    if (RequestedRank < 2 || RequestedRank > 10)
    {
        ClientFactionResult(false, TEXT("Choose a valid next rank between 2 and 10."));
        return;
    }

    const bool bSubmitted = State->ServerRequestPromotion(RequestedRank, ServiceHoursByRank[RequestedRank],
        ActivitiesByRank[RequestedRank], ReprimandsByRank[RequestedRank]);
    ClientFactionResult(bSubmitted,
        bSubmitted ? TEXT("Promotion request submitted for senior review.") : TEXT("Promotion requirements are not met, or a request is already pending."));
}

void AMetroversePlayerController::ReviewFactionApplication(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote)
{
    ServerReviewFactionApplication(Applicant, bApprove, ReviewNote);
}

void AMetroversePlayerController::ServerReviewFactionApplication_Implementation(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote)
{
    AMetroversePlayerState* Reviewer = GetPlayerState<AMetroversePlayerState>();
    if (!HasAuthority() || !Reviewer || !IsValid(Applicant)) return;
    const bool bReviewed = Reviewer->ServerReviewFactionApplication(Applicant, bApprove, ReviewNote);
    ClientFactionResult(bReviewed, bReviewed ? TEXT("Faction application review recorded.") : TEXT("Faction application review denied."));
}

void AMetroversePlayerController::ReviewPromotion(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote)
{
    ServerReviewFactionPromotion(Applicant, bApprove, ReviewNote);
}

void AMetroversePlayerController::ServerReviewFactionPromotion_Implementation(AMetroversePlayerState* Applicant, bool bApprove, const FString& ReviewNote)
{
    AMetroversePlayerState* Reviewer = GetPlayerState<AMetroversePlayerState>();
    if (!HasAuthority() || !Reviewer || !IsValid(Applicant)) return;
    const bool bReviewed = Reviewer->ServerReviewPromotion(Applicant, bApprove, ReviewNote);
    ClientFactionResult(bReviewed, bReviewed ? TEXT("Promotion review recorded.") : TEXT("Promotion review denied."));
}

void AMetroversePlayerController::ClientFactionResult_Implementation(bool bSuccess, const FString& Message)
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.0f, bSuccess ? FColor::Green : FColor::Red, Message);
}

void AMetroversePlayerController::RequestEnterVehicle(AMetroverseVehiclePawn* Vehicle)
{
    ServerRequestEnterVehicle(Vehicle);
}

void AMetroversePlayerController::RequestExitVehicle()
{
    ServerRequestExitVehicle();
}

void AMetroversePlayerController::ServerRequestEnterVehicle_Implementation(AMetroverseVehiclePawn* Vehicle)
{
    APawn* CurrentPawn = GetPawn();
    if (!HasAuthority() || !IsValid(Vehicle) || !IsValid(CurrentPawn) || CurrentDrivenVehicle ||
        Vehicle->GetWorld() != GetWorld() || !Vehicle->CanAcceptDriver()) return;

    const float AllowedDistance = FMath::Max(100.0f, Vehicle->MaximumEntryDistance);
    if (FVector::DistSquared(CurrentPawn->GetActorLocation(), Vehicle->GetActorLocation()) > FMath::Square(AllowedDistance)) return;

    PreviousPawnBeforeVehicle = CurrentPawn;
    CurrentDrivenVehicle = Vehicle;
    Vehicle->SetDriverController(this);
    Possess(Vehicle);
    if (GetPawn() != Vehicle)
    {
        Vehicle->SetDriverController(nullptr);
        CurrentDrivenVehicle = nullptr;
        PreviousPawnBeforeVehicle = nullptr;
    }
}

void AMetroversePlayerController::ServerRequestExitVehicle_Implementation()
{
    if (!HasAuthority() || !IsValid(CurrentDrivenVehicle) || !IsValid(PreviousPawnBeforeVehicle) ||
        GetPawn() != CurrentDrivenVehicle) return;

    AMetroverseVehiclePawn* Vehicle = CurrentDrivenVehicle;
    APawn* CharacterPawn = PreviousPawnBeforeVehicle;
    const FVector ExitLocation = Vehicle->GetActorLocation() + Vehicle->GetActorRightVector() * 220.0f + FVector(0.0f, 0.0f, 80.0f);

    Vehicle->SetDriverController(nullptr);
    UnPossess();
    CharacterPawn->SetActorLocation(ExitLocation, false, nullptr, ETeleportType::TeleportPhysics);
    Possess(CharacterPawn);

    CurrentDrivenVehicle = nullptr;
    PreviousPawnBeforeVehicle = nullptr;
}
