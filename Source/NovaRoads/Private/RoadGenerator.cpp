#include "RoadGenerator.h"

ARoadGenerator::ARoadGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
    RootComponent = Spline;

    RoadMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("RoadMesh"));
    RoadMesh->SetupAttachment(RootComponent);
    RoadMesh->bUseAsyncCooking = true;
}

void ARoadGenerator::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    GenerateRoadMesh();
}

void ARoadGenerator::GenerateRoadMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector2D> UVs;
    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;

    RoadMesh->ClearAllMeshSections();

    const int32 NumPoints = Spline->GetNumberOfSplinePoints();
    if (NumPoints < 2) return;

    float HalfWidth = RoadWidth * 0.5f;
    int32 VertexIndex = 0;

    for (int32 i = 0; i < NumPoints - 1; ++i)
    {
        FVector DirA = Spline->GetDirectionAtSplinePoint(i, ESplineCoordinateSpace::Local).GetSafeNormal();
        FVector DirB = Spline->GetDirectionAtSplinePoint(i + 1, ESplineCoordinateSpace::Local).GetSafeNormal();
        float Curvature = 1.0f - FVector::DotProduct(DirA, DirB);

        float StartDist = Spline->GetDistanceAlongSplineAtSplinePoint(i);
        float EndDist = Spline->GetDistanceAlongSplineAtSplinePoint(i + 1);
        float SegmentLength = EndDist - StartDist;

        int32 Subdivisions = FMath::Clamp(
            FMath::RoundToInt(SubdivisionsPerSegment * (1.0f + Curvature * CurvatureSubdivMultiplier)),
            1,
            MaxSubdivisions
        );

        for (int32 j = 0; j < Subdivisions; ++j)
        {
            float T0 = static_cast<float>(j) / Subdivisions;
            float T1 = static_cast<float>(j + 1) / Subdivisions;

            float D0 = FMath::Lerp(StartDist, EndDist, T0);
            float D1 = FMath::Lerp(StartDist, EndDist, T1);

            FVector Pos0 = Spline->GetLocationAtDistanceAlongSpline(D0, ESplineCoordinateSpace::Local);
            FVector Pos1 = Spline->GetLocationAtDistanceAlongSpline(D1, ESplineCoordinateSpace::Local);

            FVector Right0 = FVector::CrossProduct(FVector::UpVector, Spline->GetDirectionAtDistanceAlongSpline(D0, ESplineCoordinateSpace::Local)).GetSafeNormal();
            FVector Right1 = FVector::CrossProduct(FVector::UpVector, Spline->GetDirectionAtDistanceAlongSpline(D1, ESplineCoordinateSpace::Local)).GetSafeNormal();

            FVector A_Left = Pos0 - Right0 * HalfWidth;
            FVector A_Right = Pos0 + Right0 * HalfWidth;
            FVector B_Left = Pos1 - Right1 * HalfWidth;
            FVector B_Right = Pos1 + Right1 * HalfWidth;

            Vertices.Add(A_Left);
            Vertices.Add(A_Right);
            Vertices.Add(B_Left);
            Vertices.Add(B_Right);

            float V0 = D0 / UVScale;
            float V1 = D1 / UVScale;

            UVs.Add(FVector2D(U_Left, V0));
            UVs.Add(FVector2D(U_Right, V0));
            UVs.Add(FVector2D(U_Left, V1));
            UVs.Add(FVector2D(U_Right, V1));


            Normals.Add(FVector::UpVector);
            Normals.Add(FVector::UpVector);
            Normals.Add(FVector::UpVector);
            Normals.Add(FVector::UpVector);

            Tangents.Add(FProcMeshTangent(1, 0, 0));
            Tangents.Add(FProcMeshTangent(1, 0, 0));
            Tangents.Add(FProcMeshTangent(1, 0, 0));
            Tangents.Add(FProcMeshTangent(1, 0, 0));

            Triangles.Add(VertexIndex + 0);
            Triangles.Add(VertexIndex + 1);
            Triangles.Add(VertexIndex + 2);
            Triangles.Add(VertexIndex + 2);
            Triangles.Add(VertexIndex + 1);
            Triangles.Add(VertexIndex + 3);

            VertexIndex += 4;
        }
    }

    RoadMesh->CreateMeshSection_LinearColor(
        0,
        Vertices,
        Triangles,
        Normals,
        UVs,
        TArray<FLinearColor>(),
        Tangents,
        GenerateCollision
    );
#if WITH_EDITOR
    Spline->bShouldVisualizeScale = SplineDebug;
#endif
}
