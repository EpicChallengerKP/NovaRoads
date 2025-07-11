[![License](https://img.shields.io/badge/license-Beta%20Non--Commercial-yellow)](https://github.com/EpicChallengerKP/NovaRoads?tab=License-1-ov-file) 
[![Built for UE5](https://img.shields.io/badge/built%20for-UE5.6-5e5eff)](https://www.unrealengine.com/) <br>

# NovaRoads

**NovaRoads** is an Unreal Engine 5 plugin focused on procedural road generation using spline components. It integrates closely with the engine's landscape system to allow flexible, editor-friendly road creation workflows with real-time mesh updates and support for terrain deformation via custom landscape brushes.

---

## Features

- Procedural road mesh generation along a spline
- Customizable width, UV tiling, and curvature-based subdivision
- Automatically updates road mesh during spline edits (OnConstruction)
- Supports physics and trace collisions using ProceduralMeshComponent
- Clean, modular C++ codebase designed for extendability
- Experimental landscape deformation system using `ALandscapeBlueprintBrush`

---

## Completed Work

- `ARoadGenerator` actor for spline-based mesh generation
- Mesh generation with UVs that scale uniformly along the spline
- Collision setup compatible with both tracing and physics
- Tangent smoothing and normal consistency
- Plugin structure with proper module setup and editor support

---

## In Progress

- `ARoadSplineLandscapeBrush` to deform terrain under road splines
- Integration with Landscape Blueprint Brushes and render targets
- Parameterized falloff radius and deform strength per spline
- Smooth deformation blending with existing terrain

---

## Planned Features

- Runtime support for road placement and terrain carving
- Automatic landscape painting (material layers under road)
- Foliage masking/removal in road area
- Bridge and overpass generation tools
- Decal and marking support on road surface
- Road intersection tools and branching support
- Static mesh export and Nanite-ready options

---

## Development Status

This plugin is currently in **active development** and may not be feature-complete. While the core road mesh generation is functional, the terrain deformation system is still experimental.

Contributions, suggestions, and feedback are welcome.
