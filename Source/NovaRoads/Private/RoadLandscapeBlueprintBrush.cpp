// Fill out your copyright notice in the Description page of Project Settings.

#include "RoadLandscapeBlueprintBrush.h"
#include "LandscapeEdit.h" // Required for FLandscapeEditDataInterface
#include "LandscapeDataAccess.h" // Required for LandscapeDataAccess::GetLocalHeight, LandscapeDataAccess::GetTexHeight
#include "LandscapeInfo.h" // Required for ULandscapeInfo, GetXYOfLocation, RequestLandscapeUpdate
#include "Components/SplineComponent.h" // Required for USplineComponent
#include "Kismet/KismetMathLibrary.h" // Required for FMath::Lerp, FMath::SmoothStep

ARoadLandscapeBlueprintBrush::ARoadLandscapeBlueprintBrush()
{
	// Set default values for properties
	RoadWidth = 1000.0f; // 10 meters (100 units = 1 meter in UE)
	FalloffDistance = 500.0f; // 5 meters falloff
	LowerDepth = 200.0f; // Lower terrain by 2 meters
	RaiseHeight = 0.0f; // Don't raise by default
	RoadLayerName = FName(TEXT("Road")); // Default landscape layer name for the road texture
	SplineSampleDensity = 100.0f; // Sample every 100 units (1 meter) along the spline for precision
}

FVector ARoadLandscapeBlueprintBrush::GetBrushExtent_Implementation() const
{
	// If a RoadGeneratorActor is linked and has a valid spline:
	if (RoadGeneratorActor && RoadGeneratorActor->Spline) // Use RoadGeneratorActor->Spline
	{
		USplineComponent* Spline = RoadGeneratorActor->Spline;
		const float SplineLength = Spline->GetSplineLength();

		// Initialize a bounding box to encompass all spline points.
		FBox BoundingBox(ForceInit);
		for (int32 i = 0; i < Spline->GetNumberOfSplinePoints(); ++i)
		{
			BoundingBox += Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		}

		// Expand the bounding box by the maximum influence radius (RoadWidth + FalloffDistance)
		// to ensure the entire affected area is covered. This is crucial for landscape streaming/update.
		BoundingBox = BoundingBox.ExpandBy(RoadWidth + FalloffDistance + 100.0f); // Add a little extra buffer

		// Return the size of the calculated bounding box.
		return BoundingBox.GetExtent();
	}
	// If no RoadGeneratorActor or spline is set, return a large default extent.
	// This ensures the brush still covers some area, but it's less efficient.
	return FVector(100000.0f, 100000.0f, 100000.0f);
}

float ARoadLandscapeBlueprintBrush::CalculateFalloffAlpha(float DistanceFromSplineCenter) const
{
	// If FalloffDistance is very small or zero, treat it as a hard edge.
	if (FalloffDistance <= KINDA_SMALL_NUMBER)
	{
		return (DistanceFromSplineCenter <= RoadWidth * 0.5f) ? 1.0f : 0.0f;
	}

	// Calculate the inner and outer radius of the deformation
	const float InnerRadius = RoadWidth * 0.5f;
	const float OuterRadius = InnerRadius + FalloffDistance;

	// If outside the outer radius, no influence
	if (DistanceFromSplineCenter >= OuterRadius)
	{
		return 0.0f;
	}
	// If inside the inner radius, full influence
	if (DistanceFromSplineCenter <= InnerRadius)
	{
		return 1.0f;
	}

	// For distances within the falloff zone, interpolate smoothly
	float Alpha = 1.0f - ((DistanceFromSplineCenter - InnerRadius) / FalloffDistance);
	return FMath::SmoothStep(0.0f, 1.0f, Alpha); // Use SmoothStep for a nicer blend
}

void ARoadLandscapeBlueprintBrush::Render_Implementation(ULandscapeInfo* InLandscapeInfo, const FLandscapeBrushParameters& InParameters)
{
	// Ensure we have a valid landscape info and a linked RoadGeneratorActor.
	if (!InLandscapeInfo || !RoadGeneratorActor)
	{
		return;
	}

	// Get the spline component from the RoadGeneratorActor.
	USplineComponent* RoadSpline = RoadGeneratorActor->Spline; // Access RoadGeneratorActor's Spline
	if (!RoadSpline || RoadSpline->GetNumberOfSplinePoints() < 2)
	{
		return; // Need at least two points for a spline
	}

	// Get the landscape edit data interface to read and write heightmap and weightmap data.
	FLandscapeEditDataInterface LandscapeEdit(InLandscapeInfo);

	const float SplineLength = RoadSpline->GetSplineLength();

	// Iterate along the spline, sampling at a defined density.
	// Using a fixed GridStep for iteration. This can be refined to be more dynamic.
	const float GridStep = 100.0f; // Assuming 1 meter increments (100 units)

	for (float DistanceAlongSpline = 0.0f; DistanceAlongSpline < SplineLength; DistanceAlongSpline += SplineSampleDensity) // Changed from GridStep for spline sampling
	{
		// Get the world location and right vector (perpendicular to spline direction in XY plane) at the current spline distance.
		const FVector SplineWorldLocation = RoadSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		const FVector SplineRightVector = RoadSpline->GetRightVectorAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

		// Calculate the total half-width, including the road width and falloff.
		const float TotalHalfWidth = (RoadWidth * 0.5f) + FalloffDistance;

		// Iterate perpendicular to the spline, covering the road's width and falloff.
		for (float OffsetPerpendicular = -TotalHalfWidth; OffsetPerpendicular <= TotalHalfWidth; OffsetPerpendicular += GridStep) // Using GridStep here
		{
			// Calculate the world location of the current sample point on the landscape grid.
			const FVector SampleWorldLocation = SplineWorldLocation + (SplineRightVector * OffsetPerpendicular);

			// Get the absolute distance from the spline center for falloff calculation.
			const float DistanceFromSplineCenter = FMath::Abs(OffsetPerpendicular);

			// Calculate the influence (alpha) of the brush at this specific point.
			const float FalloffAlpha = CalculateFalloffAlpha(DistanceFromSplineCenter);

			// Only apply deformation if there's significant influence.
			if (FalloffAlpha > KINDA_SMALL_NUMBER)
			{
				// Convert world location to landscape component (local) coordinates.
				int32 X, Y;
				InLandscapeInfo->GetXYOfLocation(SampleWorldLocation, X, Y); //

				// Get the current height at this landscape coordinate.
				uint16 CurrentHeight = LandscapeEdit.GetHeightData(X, Y); //
				float CurrentHeightFloat = LandscapeDataAccess::GetLocalHeight(CurrentHeight);

				// Calculate the target height based on the spline's Z-height and the LowerDepth/RaiseHeight properties.
				float TargetHeightFloat = SplineWorldLocation.Z + RaiseHeight - LowerDepth;

				// Interpolate between the current landscape height and the desired target height using the falloff alpha.
				float NewHeightFloat = FMath::Lerp(CurrentHeightFloat, TargetHeightFloat, FalloffAlpha);

				// Convert the new float height back to uint16 for landscape storage.
				uint16 NewHeight = LandscapeDataAccess::GetTexHeight(NewHeightFloat);

				// Apply the new height data to the landscape.
				LandscapeEdit.SetHeightData(X, Y, NewHeight);

				// Apply landscape layer weight data (if a layer name is specified).
				if (!RoadLayerName.IsNone())
				{
					// Get the current weight for the specified layer.
					uint8 CurrentWeight = LandscapeEdit.GetWeightData(X, Y, RoadLayerName); //

					// Calculate the new weight, blending towards 255 (full influence) based on the falloff alpha.
					uint8 NewWeight = FMath::Lerp(CurrentWeight, (uint8)255, FalloffAlpha);

					// Set the new weight data for the layer.
					LandscapeEdit.SetWeightData(X, Y, RoadLayerName, NewWeight); //
				}
			}
		}
	}

	// After modifying heightmap and/or weightmap data, request a landscape update to reflect changes in the editor.
	InLandscapeInfo->RequestLandscapeUpdate(true, true); //
}

#if WITH_EDITOR
void ARoadLandscapeBlueprintBrush::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// If any property of this brush is changed in the editor, invalidate the brush.
	// This forces the landscape system to re-render the brush's influence, applying the new settings.
	InvalidateBrush(); //

	// If the RoadGeneratorActor reference changed, also force the spline to update its properties in editor if it needs to.
	if (PropertyChangedEvent.Property != nullptr && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ARoadLandscapeBlueprintBrush, RoadGeneratorActor))
	{
		if (RoadGeneratorActor && RoadGeneratorActor->Spline)
		{
			RoadGeneratorActor->Spline->MarkRenderStateDirty(); // Rebuilds the spline visualization
		}
	}
}
#endif