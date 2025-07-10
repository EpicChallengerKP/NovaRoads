[![License](https://img.shields.io/badge/license-Beta%20Non--Commercial-yellow)](https://github.com/EpicChallengerKP/NovaRoads?tab=License-1-ov-file) 
[![Built for UE5](https://img.shields.io/badge/built%20for-UE5-5e5eff)](https://www.unrealengine.com/) <br>

# NovaRoads 🛣️

**NovaRoads** is a plugin for Unreal Engine 5 designed to bring spline-based procedural road generation and dynamic landscape deformation to world-building workflows. It aims to be modular, performance-conscious, and editor-friendly, suitable for both runtime and design-time use.

---

## 🌟 Features

- ✅ Procedural road mesh generation using spline components  
- ✅ Customizable road width, UV scaling, and tangent smoothing  
- ✅ Auto-mesh updates in editor (OnConstruction)  
- ✅ Full collision support with `UProceduralMeshComponent`  
- ✅ UVs scale uniformly along the spline  
- 🚧 In-development: Landscape deformation brush integration (Landscape Blueprint Brush)  
- 🚧 Planned support for foliage masking and terrain painting  

---

## 📦 What’s Done

- ✅ Basic `ARoadGenerator` actor with procedural spline mesh generation  
- ✅ Collisions and proper normals for the generated mesh  
- ✅ UV mapping with uniform scale and direction  
- ✅ Road appearance adapts to curvature through segment subdivision  
- ✅ Road tool works entirely within UE5 plugin as a modular actor  
- ✅ Plugin compiles and functions cleanly in editor  
- ✅ Early work on `RoadLandscapeBlueprintBrush` for deforming terrain

---

## 🔧 In Progress

- 🛠️ `RoadSplineLandscapeBrush`: A custom `ALandscapeBlueprintBrush`-derived actor that reads spline data from `ARoadGenerator` and deforms the landscape accordingly  
- 🛠️ Runtime-friendly implementation using render targets  
- 🛠️ Optional falloff, deform strength, and blend settings  
- 🛠️ Clean API to link spline → terrain deformation → paint → final road

---

## 🔮 Planned Features

- 🚀 Runtime support for road placement and terrain carving  
- 🌿 Foliage masking/removal inside road spline bounds  
- 🧱 Material layer blending support for auto-painting terrain under roads  
- 🧭 Lane marking and mesh overlays (optional decal mesh components)  
- 🌉 Support for bridges and slope-aware road platforms  
- 🛣️ Road branching and intersection tools  
- 📤 In-editor export to static meshes or Nanite

---

## 💼 License & Usage

This plugin is released under a **custom license**. 

- ❌ **Not open for unrestricted commercial use**
- ✅ You may use NovaRoads freely in non-commercial projects during its beta phase
- 🛒 **Commercial use will require a paid license** once NovaRoads is out of beta
- 🧑‍🔧 Support is not guaranteed until after official release and license availability

Please contact the author if you'd like to discuss commercial licensing options during beta.

---

## 💬 Contact

Feel free to open an issue or reach out if you'd like to contribute or have feature suggestions!

---

© 2025 NovaRoads Plugin — Created with 💙 and sleepless nights.
