#pragma once

#include "CoreMinimal.h"
#include "MetroverseRPTypes.generated.h"

UENUM(BlueprintType)
enum class EMetroverseFaction : uint8
{
    None,
    Government,
    Police,
    Hospital,
    NewsNetwork,
    FIB,
    Administration
};

UENUM(BlueprintType)
enum class EMetroverseCity : uint8
{
    Central UMETA(DisplayName="Metro City"),
    North UMETA(DisplayName="Northpoint"),
    South UMETA(DisplayName="Southbay"),
    East UMETA(DisplayName="Eastford"),
    West UMETA(DisplayName="Westhaven"),
    Coastal UMETA(DisplayName="Port Azure"),
    Industrial UMETA(DisplayName="Ironworks"),
    Highland UMETA(DisplayName="Hillcrest")
};

UENUM(BlueprintType)
enum class EMetroverseApplicationStatus : uint8
{
    None,
    Pending,
    Approved,
    Rejected
};

UENUM(BlueprintType)
enum class EMetroverseJobType : uint8
{
    None,
    DeliveryCourier,
    BusOperator,
    TaxiDriver,
    TruckHauler,
    AirportGroundCrew
};

enum class EMetroverseChatChannel : uint8
{
    RP,
    NRP,
    Faction,
    SeniorStaff,
    Department,
    Event,
    Action,
    Interview
};

USTRUCT(BlueprintType)
struct FMetroverseOwnedVehicle
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString VehicleID;
    UPROPERTY(BlueprintReadOnly) FString DisplayName;
    UPROPERTY(BlueprintReadOnly) FString Plate;
    UPROPERTY(BlueprintReadOnly) int64 PurchasePriceMC = 0;
    UPROPERTY(BlueprintReadOnly) FDateTime PurchasedAt;
    UPROPERTY(BlueprintReadOnly) FName GarageID = NAME_None;
    UPROPERTY(BlueprintReadOnly) bool bStoredInGarage = false;
};

USTRUCT(BlueprintType)
struct FMetroverseFactionApplication
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) EMetroverseFaction RequestedFaction = EMetroverseFaction::None;
    UPROPERTY(BlueprintReadOnly) FString Statement;
    UPROPERTY(BlueprintReadOnly) EMetroverseApplicationStatus Status = EMetroverseApplicationStatus::None;
    UPROPERTY(BlueprintReadOnly) FDateTime SubmittedAt;
    UPROPERTY(BlueprintReadOnly) FString ReviewerName;
    UPROPERTY(BlueprintReadOnly) FString ReviewNote;
};

USTRUCT(BlueprintType)
struct FMetroversePromotionRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) int32 RequestedRank = 0;
    UPROPERTY(BlueprintReadOnly) EMetroverseApplicationStatus Status = EMetroverseApplicationStatus::None;
    UPROPERTY(BlueprintReadOnly) FDateTime SubmittedAt;
    UPROPERTY(BlueprintReadOnly) FString ReviewerName;
    UPROPERTY(BlueprintReadOnly) FString ReviewNote;
};

USTRUCT(BlueprintType)
struct FMetroverseReprimandRecord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString IssuerName;

    UPROPERTY(BlueprintReadOnly)
    FString Action;

    UPROPERTY(BlueprintReadOnly)
    FString Reason;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString RelatedRecordSummary;
};

USTRUCT(BlueprintType)
struct FMetroverseChatMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SenderName;

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    EMetroverseChatChannel Channel = EMetroverseChatChannel::RP;
};
