#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MetroverseVehicleCatalogRow.generated.h"

USTRUCT(BlueprintType)
struct METROVERSE_API FMetroverseVehicleCatalogRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName VehicleID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Category;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName VehicleClass;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName BodyStyle;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Seats = 4;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float TopSpeedKmh = 120.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float Acceleration0To100Sec = 10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float Handling = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName FuelType;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName DriveType;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 BasePriceMC = 10000;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Description;
};
