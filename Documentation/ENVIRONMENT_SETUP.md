# Environment Setup Guide

## Creating Environments Step-by-Step

This guide walks through creating production-ready environments for the AI Digital Human.

---

## Part 1: Base Level Setup

### 1.1 Create Streaming Level

```
Content Browser → Right-click → Level → Empty Level
Name: L_ENV_Bedroom (or appropriate name)
```

### 1.2 Enable Required Features

In Level Settings:
```
World Settings:
├── Enable Lumen Global Illumination: ✓
├── Enable Lumen Reflections: ✓
├── Enable Nanite: ✓
└── Enable Virtual Shadow Maps: ✓
```

### 1.3 Post Process Volume (Full Scene)

Add: Place Actors → Visual Effects → Post Process Volume

Settings for cinematic quality:
```
Infinite Extent: ✓

Bloom:
├── Intensity: 0.5
├── Threshold: 1.0
└── Convolution: ✓

Exposure:
├── Metering Mode: Auto Exposure Histogram
├── Min Brightness: 0.5
├── Max Brightness: 2.0
└── Speed Up/Down: 3.0

Color Grading:
├── Temperature: (adjust per scene)
├── Saturation: 1.05
├── Contrast: 1.05
└── Toe: 0.5

Depth of Field:
├── Method: Circle DoF
├── Focal Distance: (auto from camera)
├── Aperture: 2.8
└── Max Bokeh Size: 32

Motion Blur:
├── Amount: 0.3
└── Max: 3.0

Film:
├── Slope: 0.88
├── Toe: 0.55
├── Shoulder: 0.26
└── Black Clip: 0.0

Lumen:
├── Final Gather Quality: 1.0
├── Scene Lighting Quality: 1.0
└── Max Trace Distance: 20000
```

---

## Part 2: Bedroom Environment (Detailed)

### 2.1 Room Geometry

Create with BSP or import Nanite mesh:

```
Dimensions: 6m × 8m × 3m (W × L × H)

Walls:
├── Material: M_Wall_Plaster (light grey)
├── Accent Wall: M_Wall_Velvet (behind bed)
└── Baseboards: M_Wood_Oak

Floor:
├── Material: M_Floor_Hardwood
└── Reflectivity: 0.3

Ceiling:
├── Material: M_Ceiling_White
├── Crown Molding: SM_Molding_Classic
└── Recessed light cutouts: 4x
```

### 2.2 Window Setup

```
Asset: SM_Window_FloorToCeiling (3m × 2.5m)
Position: Back wall, centered

Components:
├── Glass Material: M_Glass_Tinted (night reflection)
├── Frame: M_Metal_Brass_Brushed
└── Curtain Track: Hidden

Curtains (2 layers):
├── Sheer: M_Fabric_Sheer_White (always visible)
└── Blackout: M_Fabric_Velvet_Navy (optional)

Backdrop:
├── Night: T_Cityscape_Night_HDRI
├── Day: T_Cityscape_Day_HDRI
└── Use: Parallax material for depth
```

### 2.3 Bed Setup

```
Bed Frame: SM_Bed_King_Modern
├── Material: M_Wood_Walnut
├── Headboard: Tufted velvet (M_Velvet_Navy)
└── Size: 2.0m × 2.2m

Mattress & Bedding:
├── Base: SM_Mattress_King
├── Fitted Sheet: M_Fabric_Silk_Ivory
├── Top Sheet: M_Fabric_Egyptian_Cotton
├── Duvet: SM_Duvet_Folded (M_Fabric_Velvet_Burgundy)
├── Pillows: 6x SM_Pillow_Euro + 4x SM_Pillow_Standard
└── Throw: SM_Blanket_Cashmere (draped)

Cloth Simulation:
├── Enable on duvet, throw
├── Damping: 0.4
└── Stiffness: 0.6
```

### 2.4 Furniture Placement

```
Nightstands (2x):
├── Asset: SM_Nightstand_Modern
├── Position: Both sides of bed, 10cm gap
├── Materials: M_Marble_Calacatta (top), M_Wood_Walnut (base)
└── Props: Lamp, book, phone charger

Vanity:
├── Asset: SM_Vanity_Hollywood
├── Position: Adjacent wall, 2m from bed
├── Mirror: 120 LED bulbs (emissive material)
└── Chair: SM_Chair_Velvet (M_Velvet_Blush)

Bench:
├── Asset: SM_Bench_EndOfBed
├── Position: Foot of bed, 30cm gap
├── Material: M_Leather_Cognac
└── Dimensions: 1.5m × 0.5m

Accent Chair:
├── Asset: SM_Chair_Accent_Modern
├── Position: Corner, angled toward bed
├── Material: M_Velvet_Emerald
└── Side Table: SM_Table_Round_Brass
```

### 2.5 Lighting Setup

```
Key Light (Window simulation):
├── Type: Rect Light
├── Size: 200cm × 100cm
├── Position: Behind camera, window-side
├── Temperature: 5500K (day) / 3200K (golden hour)
├── Intensity: 10 lux
└── Cast Shadows: ✓

Fill Light:
├── Type: Rect Light
├── Size: 100cm × 100cm
├── Position: Opposite wall, bounced
├── Temperature: 4500K
├── Intensity: 3 lux (30% of key)
└── Cast Shadows: ✗

Rim Light:
├── Type: Spot Light
├── Position: Behind headboard, high
├── Temperature: 5500K
├── Intensity: 5 lux
├── Cone Angle: 45°
└── Purpose: Hair/shoulder separation

Practical - Table Lamps (2x):
├── Type: Point Light
├── Position: Inside lamp shades
├── Temperature: 2700K
├── Intensity: 2 lux
├── Attenuation: 150cm
└── Shadows: ✓ (soft)

Practical - Vanity Mirror:
├── Type: 120x Point Lights (or emissive)
├── Temperature: 4000K
├── Intensity: 0.5 lux each
└── Arrangement: Border pattern

Accent - Under Bed:
├── Type: Rect Light
├── Position: Under bed frame
├── Color: Warm white or RGB
├── Intensity: 0.2 lux
└── Hidden source
```

### 2.6 Character Positions

Define transforms for character placement:

```cpp
// Position 1: Lying on bed (center)
FTransform(
    FRotator(0, 180, 0),
    FVector(0, 0, 85),      // Bed surface height
    FVector(1, 1, 1)
);

// Position 2: Lying on bed (side)
FTransform(
    FRotator(0, 90, 0),
    FVector(60, 0, 85),
    FVector(1, 1, 1)
);

// Position 3: Sitting on bed edge
FTransform(
    FRotator(0, 0, 0),
    FVector(0, -100, 60),
    FVector(1, 1, 1)
);

// Position 4: At vanity
FTransform(
    FRotator(0, 45, 0),
    FVector(200, 150, 0),   // Vanity position
    FVector(1, 1, 1)
);

// Position 5: Standing by window
FTransform(
    FRotator(0, 180, 0),
    FVector(0, 300, 0),
    FVector(1, 1, 1)
);

// Position 6: In accent chair
FTransform(
    FRotator(0, -30, 0),
    FVector(-180, -100, 45),
    FVector(1, 1, 1)
);
```

### 2.7 Camera Presets

```cpp
// Camera 1: Wide shot
FCameraPreset WideShot;
WideShot.Name = "Wide";
WideShot.Transform = FTransform(
    FRotator(-5, 180, 0),
    FVector(0, -400, 150)
);
WideShot.FocalLength = 24.0f;
WideShot.Aperture = 2.8f;

// Camera 2: Medium shot
FCameraPreset MediumShot;
MediumShot.Name = "Medium";
MediumShot.Transform = FTransform(
    FRotator(-3, 180, 0),
    FVector(0, -250, 120)
);
MediumShot.FocalLength = 50.0f;
MediumShot.Aperture = 2.0f;

// Camera 3: Close-up
FCameraPreset CloseUp;
CloseUp.Name = "CloseUp";
CloseUp.Transform = FTransform(
    FRotator(0, 160, 0),
    FVector(80, -150, 100)
);
CloseUp.FocalLength = 85.0f;
CloseUp.Aperture = 1.8f;

// Camera 4: Vanity mirror (reflection)
FCameraPreset VanityShot;
VanityShot.Name = "Vanity";
VanityShot.Transform = FTransform(
    FRotator(0, 45, 0),
    FVector(250, 200, 100)
);
VanityShot.FocalLength = 35.0f;
VanityShot.Aperture = 2.2f;

// Camera 5: Window silhouette
FCameraPreset WindowShot;
WindowShot.Name = "Window";
WindowShot.Transform = FTransform(
    FRotator(5, 0, 0),
    FVector(0, -100, 80)
);
WindowShot.FocalLength = 35.0f;
WindowShot.Aperture = 4.0f;
```

### 2.8 Mood Lighting Presets

```cpp
// Romantic mood
FEnvironmentLighting RomanticLighting;
RomanticLighting.KeyLightIntensity = 3.0f;
RomanticLighting.KeyLightTemperature = 2700.0f;
RomanticLighting.bEnablePracticalLights = true;
// Enable candles, dim main lights

// Cozy mood
FEnvironmentLighting CozyLighting;
CozyLighting.KeyLightIntensity = 5.0f;
CozyLighting.KeyLightTemperature = 3200.0f;
CozyLighting.bEnablePracticalLights = true;
// All lamps on, warm feeling

// Mysterious mood
FEnvironmentLighting MysteriousLighting;
MysteriousLighting.KeyLightIntensity = 2.0f;
MysteriousLighting.KeyLightTemperature = 4500.0f;
MysteriousLighting.bEnableVolumetricFog = true;
MysteriousLighting.FogDensity = 0.03f;
// Single rim light dominant, fog
```

---

## Part 3: Creating Data Asset

### 3.1 Create Environment Preset

```
Content Browser → Right-click → Miscellaneous → Data Asset
Class: EnvironmentPreset
Name: DA_ENV_Bedroom
```

### 3.2 Configure Preset

```
Environment Name: "Luxury Bedroom"
Type: Bedroom
Default Mood: Cozy
Description: "Elegant modern bedroom with city views"

Level Asset: L_ENV_Bedroom

Lighting:
├── Time of Day: Night
├── Key Light Temperature: 3200
├── Key Light Intensity: 8.0
├── Fill Light Ratio: 0.3
├── Rim Light Intensity: 5.0
├── Enable Practicals: ✓
└── Ambient Color: (0.02, 0.02, 0.03)

Character Spawn Transform:
└── Position 3 (sitting on bed edge)

Character Positions: [All 6 transforms]

Camera Presets: [All 5 presets]

Interactive Props:
├── Bed (is_surface, 4 positions)
├── Vanity Chair (is_surface, 1 position)
├── Accent Chair (is_surface, 1 position)
└── Window Curtains (animation: open/close)

Ambient Sound: A_AMB_Bedroom_Night
Music Playlist: [Chill lofi tracks]
```

---

## Part 4: Outdoor Environment (Forest)

### 4.1 Landscape Setup

```
Landscape:
├── Size: 50m × 50m (small clearing)
├── Section Size: 63×63
├── Scale: 100
└── Material: M_Landscape_Forest

Layers:
├── Grass (dominant)
├── Dirt (paths)
├── Moss (shaded areas)
└── Flowers (scattered)
```

### 4.2 Foliage Painting

```
Trees (Procedural):
├── Oak: 8-12 around clearing
├── Birch: 4-6 mixed
├── Density: Sparse at edges
└── LOD: Nanite enabled

Undergrowth:
├── Ferns: Medium density
├── Grass: High density
├── Wildflowers: Clusters
└── Mushrooms: Occasional

Ground Cover:
├── Fallen leaves
├── Twigs
├── Small rocks
└── Moss patches
```

### 4.3 Lighting (Golden Hour)

```
Directional Light (Sun):
├── Rotation: (0, -30, 0) - low angle
├── Temperature: 3200K
├── Intensity: 8.0 lux
├── Light Shaft Bloom: ✓
└── Cast Volumetric Shadows: ✓

Sky Light:
├── Source: HDRI Cubemap
├── HDRI: T_Sky_Forest_GoldenHour
├── Intensity: 1.5
└── Real Time Capture: ✓

Exponential Height Fog:
├── Density: 0.01
├── Height Falloff: 0.5
├── Volumetric Fog: ✓
├── Scattering: 0.8
└── Color: Warm amber

God Rays:
├── Spot Light through canopy gaps
├── Volumetric Scattering
└── Subtle bloom
```

### 4.4 Special Effects

```
Particle Systems (Niagara):

1. Dust Motes:
├── Spawn: In light shafts only
├── Size: 1-3mm
├── Velocity: Slow float
└── Color: Golden

2. Falling Leaves:
├── Spawn: Occasional (0.2/sec)
├── Path: Tumble physics
├── Variety: 4 leaf types
└── Ground collision

3. Fireflies (Night):
├── Spawn: After blue hour
├── Pattern: Random wander
├── Glow: Yellow-green emissive
└── Blink: Random interval

4. Pollen/Seeds:
├── Spawn: Floating
├── Drift: Wind-affected
└── Very subtle
```

### 4.5 Audio Setup

```
Ambient Sound Cue: A_AMB_Forest_Day

Layers:
├── Bird songs (varied, random)
├── Distant woodpecker
├── Rustling leaves (wind-reactive)
├── Nearby stream (if included)
└── Insect buzz (subtle)

Attenuation:
├── Inner Radius: 500cm
├── Falloff: 3000cm
└── Spatialization: ✓
```

---

## Part 5: Performance Optimization

### 5.1 LOD Configuration

```
For each major asset:

LOD 0: Full Nanite (0-500cm)
LOD 1: Reduced mesh (500-1500cm)
LOD 2: Simple mesh (1500-3000cm)
LOD 3: Billboard/Imposter (3000cm+)

Transition:
├── Dither fade: 0.1 seconds
└── Screen size threshold
```

### 5.2 Lighting Optimization

```
Shadow-casting lights: Maximum 4
├── 1x Directional (sun/moon)
├── 1x Key light (rect)
├── 1x Rim light (spot)
└── 1x Feature (fireplace/lamp)

Non-shadow lights: Unlimited
├── Practicals
├── Accent lights
└── Ambient fill
```

### 5.3 Material Optimization

```
Shared Material Instances:
├── All wood → MI_Wood_Base
├── All fabric → MI_Fabric_Base
├── All metal → MI_Metal_Base
└── Unique textures as parameters

Texture Streaming:
├── Max resolution: 4K for hero assets
├── Standard: 2K
├── Background: 1K
└── Mipmaps: Always enabled
```

### 5.4 Scalability Settings

```
r.Lumen.Quality = 1 (default) to 0.5 (performance)
r.Nanite.MaxPixelsPerEdge = 1 (quality) to 2 (performance)
r.Shadow.Virtual.SMRT.RayCountLocal = 4 (default)
r.ScreenPercentage = 100 (4K) to 66 (1440p internal)
r.TemporalAA.Upsampling = 1 (TSR) or DLSS
```

---

## Part 6: Quick Reference

### Environment Checklist

```
□ Level created and named correctly
□ Post process volume configured
□ Lighting setup (key, fill, rim, practicals)
□ All assets placed and materials assigned
□ Character positions defined (minimum 4)
□ Camera presets created (minimum 3)
□ Ambient audio assigned
□ Data Asset created and configured
□ Performance tested at 4K/60FPS
□ Mood presets configured
□ Interactive props marked
```

### Common Issues

| Issue | Solution |
|-------|----------|
| Dark shadows | Increase sky light intensity |
| Flickering | Check light overlap, reduce count |
| Low FPS | Enable DLSS/TSR, reduce shadow casters |
| Flat lighting | Add rim light, increase contrast |
| Noise in reflections | Increase Lumen quality, denoise |

### File Naming Convention

```
L_ENV_[Name]           - Level
DA_ENV_[Name]          - Data Asset
SM_[Category]_[Name]   - Static Mesh
M_[Material]_[Variant] - Material
MI_[Material]_[Use]    - Material Instance
T_[Type]_[Name]        - Texture
A_AMB_[Scene]_[Time]   - Ambient Sound
A_MUS_[Mood]_[Number]  - Music Track
```
