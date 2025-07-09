#pragma once

#include "landscape.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "RoadGenerator.generated.h"

UCLASS(Blueprintable)
class NOVAROADS_API ARoadGenerator : public AActor
{
    GENERATED_BODY()

public:
    ARoadGenerator();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
    USplineComponent* Spline;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
    UProceduralMeshComponent* RoadMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|Mesh")
    float RoadWidth = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|Mesh")
    int32 SubdivisionsPerSegment = 8;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|Mesh")
    float CurvatureSubdivMultiplier = 16.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|Mesh")
    int32 MaxSubdivisions = 64;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|Mesh")
    bool GenerateCollision = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|Terrain")
    bool AffectLandscape = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|Spline")
    bool SplineDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|UVs")
	float UVScale = 0.01f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|UVs")
    float U_Right = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Road Settings|UVs")
    float U_Left = 0.0f;

    void GenerateRoadMesh();
};
