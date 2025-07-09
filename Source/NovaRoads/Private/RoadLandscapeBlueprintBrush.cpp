// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadLandscapeBlueprintBrush.h"
#include "CanvasTypes.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"

void ARoadSplineLandscapeBrush::Initialize(const FTransform& InLandscapeTransform, const FIntPoint& InLandscapeSize, const FIntPoint& InRenderTargetSize)
{
    LandscapeTransform = InLandscapeTransform;
    LandscapeSize = InLandscapeSize;
    RenderTargetSize = InRenderTargetSize;

    MarkPackageDirty();
}

UTextureRenderTarget2D* ARoadSplineLandscapeBrush::Render(bool bIsHeightmap, UTextureRenderTarget2D* CombinedResult, const FName& InWeightmapLayerName)
{
    if (!bIsHeightmap || !CombinedResult)
        return CombinedResult;

    FTextureRenderTargetResource* RTResource = CombinedResult->GameThread_GetRenderTargetResource();
    FCanvas Canvas(RTResource, nullptr, 0, 0, 0, GMaxRHIFeatureLevel);

    Canvas.Clear(FLinearColor::Black);

    float RadiusInPixels = Radius / LandscapeTransform.GetScale3D().X;

    FVector2D Center(RenderTargetSize.X * 0.5f, RenderTargetSize.Y * 0.5f);
    FCanvasTileItem Circle(Center - FVector2D(RadiusInPixels), GWhiteTexture, FVector2D(RadiusInPixels * 2), FLinearColor::White * (DeformStrength / 512.0f));
    Circle.BlendMode = SE_BLEND_Additive;
    Canvas.DrawItem(Circle);

    Canvas.Flush_GameThread();
    return CombinedResult;
}
