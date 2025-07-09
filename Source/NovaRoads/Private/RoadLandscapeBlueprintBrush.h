// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LandscapeBlueprintBrush.h" // <--- IMPORTANT: Changed from "LandscapeBlueprintBrushBase.h"
#include "LandscapeDataAccess.h" // Required for LandscapeDataAccess helpers
#include "LandscapeInfo.h" // Required for ULandscapeInfo
#include "RoadGenerator.h" // To get the spline from your RoadGenerator actor
#include "RoadLandscapeBlueprintBrush.generated.h"

/**
 * Custom Landscape Brush for deforming terrain based on a RoadGenerator's spline.
 */
UCLASS()
class NOVAROADS_API ARoadLandscapeBlueprintBrush : public ALandscapeBlueprintBrush // <--- IMPORTANT: Changed from ALandscapeBlueprintBrushBase
{
	GENERATED_BODY()

public:
	ARoadLandscapeBlueprintBrush();

	// Reference to the RoadGenerator actor that contains the spline defining the road path.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Deformer")
	ARoadGenerator* RoadGeneratorActor;

	// Width of the road at its base (where deformation is strongest).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Deformer", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RoadWidth;

	// Distance over which the deformation blends out to the original landscape height.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Deformer", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FalloffDistance;

	// How much to lower the terrain relative to the spline's Z-height (e.g., for a trench or river bed).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Deformer")
	float LowerDepth;

	// How much to raise the terrain relative to the spline's Z-height (e.g., for a raised road or embankment).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Deformer")
	float RaiseHeight;

	// The name of the landscape layer to paint along the road (e.g., "Road", "Dirt").
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Deformer")
	FName RoadLayerName;

	// The density of samples along the spline for deformation calculation.
	// A lower value (e.g., 50.0f) means more frequent samples, leading to smoother deformation on curves.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Deformer", meta = (ClampMin = "10.0", ClampMax = "500.0", UIMin = "10.0", UIMax = "500.0"))
	float SplineSampleDensity;

protected:
	// Overrides from ALandscapeBlueprintBrush
	// This is the core function where the landscape modification logic is implemented.
	virtual void Render_Implementation(ULandscapeInfo* InLandscapeInfo, const FLandscapeBrushParameters& InParameters) override;

	// Defines the bounding box of the brush's influence in world space.
	// This helps the landscape system know which areas need to be updated.
	virtual FVector GetBrushExtent_Implementation() const override;

#if WITH_EDITOR
	// Called when a property of this brush is changed in the editor (e.g., RoadWidth, LowerDepth).
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	// Helper function to calculate the alpha (influence strength) based on distance from the spline center,
	// incorporating the road width and falloff distance.
	float CalculateFalloffAlpha(float DistanceFromSplineCenter) const;
};