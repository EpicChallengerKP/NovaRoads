// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LandscapeBlueprintBrushBase.h"
#include "RoadLandscapeBlueprintBrush.generated.h"

UCLASS(Blueprintable)
class NOVAROADS_API ARoadSplineLandscapeBrush : public ALandscapeBlueprintBrushBase
{
    GENERATED_BODY()

public:
    void Initialize(const FTransform& InLandscapeTransform, const FIntPoint& InLandscapeSize, const FIntPoint& InRenderTargetSize);

    UTextureRenderTarget2D* Render(bool bIsHeightmap, UTextureRenderTarget2D* CombinedResult, const FName& WeightmapLayerName);

    bool IsValidBrush() const { return true; }

    TArray<UObject*> GetBlueprintRenderDependencies() { return {}; }

protected:
    UPROPERTY(EditAnywhere, Category = "Road Brush")
    float DeformStrength = 512.0f;

    UPROPERTY(EditAnywhere, Category = "Road Brush")
    float Radius = 256.0f;

    FTransform LandscapeTransform;
    FIntPoint LandscapeSize;
    FIntPoint RenderTargetSize;
};
