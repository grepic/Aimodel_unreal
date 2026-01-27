# MetaHuman Setup Guide

## Overview

This guide covers creating and configuring a fully synthetic MetaHuman for the AI Digital Human system. The character must be procedurally generated - NO real person likeness.

---

## Part 1: Creating the MetaHuman

### Option A: MetaHuman Creator (Recommended)

1. **Access MetaHuman Creator**
   ```
   https://metahuman.unrealengine.com/
   ```

2. **Create New MetaHuman**
   - Click "Create MetaHuman"
   - Choose "Start from Preset" or "Blend"
   - **IMPORTANT**: Do NOT upload photos or scan real faces

3. **Customize Procedurally**

   Use the following randomization approach:
   ```
   Face Structure:
   ├── Blend 3-4 different presets (33% each)
   ├── Adjust bone structure: ±15% random
   ├── Adjust proportions: ±10% random
   └── Asymmetry: Add 5-8% subtle asymmetry

   Features:
   ├── Eyes: Random from preset pool
   ├── Nose: Random + adjustments
   ├── Lips: Random + adjustments
   ├── Ears: Random from pool
   └── Jaw/Chin: Blend + adjust
   ```

4. **Appearance Settings**

   ```
   Age: 21-30 (adult verified)

   Skin:
   ├── Tone: Choose from diverse palette
   ├── Texture: Add unique freckles/marks
   ├── Subsurface: Keep realistic
   └── Makeup: Optional, natural or glam

   Eyes:
   ├── Color: Any natural or enhanced
   ├── Iris detail: Maximum
   └── Sclera: Slight veins for realism

   Hair:
   ├── Style: Choose from library
   ├── Color: Natural or fantasy
   ├── Strand quality: Maximum
   └── Physics: Enable

   Body:
   ├── Type: Choose proportions
   ├── Height: Define
   └── Build: Athletic/Slim/Curvy
   ```

5. **Export to Project**
   - Click "Download"
   - Select project: AIDigitalHuman
   - Download all LODs
   - Include animation data

### Option B: Quixel Bridge Import

1. Open Quixel Bridge in Unreal
2. Navigate to MetaHumans section
3. Select desired MetaHuman preset
4. Click "Add to Project"
5. Wait for download and processing

---

## Part 2: Project Integration

### 2.1 Folder Structure After Import

```
Content/MetaHumans/
├── [CharacterName]/
│   ├── BP_[CharacterName].uasset      ← Base blueprint
│   ├── Face/
│   │   ├── SK_Face.uasset             ← Face skeletal mesh
│   │   ├── M_Face_Inst.uasset         ← Face material instance
│   │   └── Textures/
│   │       ├── T_Face_D.uasset        ← Diffuse
│   │       ├── T_Face_N.uasset        ← Normal
│   │       └── T_Face_Makeup.uasset   ← Makeup mask
│   ├── Body/
│   │   ├── SK_Body.uasset             ← Body skeletal mesh
│   │   ├── M_Body_Inst.uasset
│   │   └── Textures/
│   ├── Hair/
│   │   ├── GR_Hair.uasset             ← Groom asset
│   │   ├── BP_Hair.uasset             ← Hair blueprint
│   │   └── M_Hair_Inst.uasset
│   ├── Eyebrows/
│   │   └── GR_Eyebrows.uasset
│   ├── Eyelashes/
│   │   └── GR_Eyelashes.uasset
│   └── Animations/
│       └── (animation assets)
└── Common/
    ├── Animations/
    ├── Materials/
    └── Rigs/
```

### 2.2 Reparent to Custom Character

1. **Open MetaHuman Blueprint**
   ```
   Content/MetaHumans/[Name]/BP_[Name]
   ```

2. **Change Parent Class**
   ```
   Class Settings → Parent Class → AIDigitalHumanCharacter
   ```

3. **Or Create New Blueprint**
   ```
   Create BP_AIDigitalHuman (parent: AIDigitalHumanCharacter)
   Copy components from MetaHuman BP
   ```

### 2.3 Component Assignment

In BP_AIDigitalHuman:

```
Mesh (Skeletal Mesh Component):
├── Skeletal Mesh: SK_Body
├── Animation Blueprint: ABP_AIDigitalHuman
└── Materials: M_Body_Inst

FaceMesh (attached to head socket):
├── Skeletal Mesh: SK_Face
├── Parent Socket: head
└── Materials: M_Face_Inst

HairGroom (attached to Mesh):
├── Groom Asset: GR_Hair
├── Binding Asset: (auto-generated)
└── Enable Physics: true

EyebrowGroom (attached to FaceMesh):
├── Groom Asset: GR_Eyebrows
└── Parent Socket: head

EyelashGroom (attached to FaceMesh):
├── Groom Asset: GR_Eyelashes
└── Parent Socket: head
```

---

## Part 3: Material Customization

### 3.1 Skin Material Parameters

```cpp
// In Material Instance M_Face_Inst

Skin Color:
├── Base Color Tint: (R=1.0, G=0.85, B=0.75)  // Adjust for skin tone
├── Subsurface Color: (R=0.8, G=0.2, B=0.15)  // Blood color
└── Subsurface Intensity: 0.5

Detail:
├── Micro Normal Intensity: 1.0
├── Pore Intensity: 0.8
├── Wrinkle Intensity: 0.3 (age dependent)
└── Roughness: 0.4

Makeup (optional):
├── Lip Color: (color)
├── Lip Glossiness: 0.7
├── Eye Shadow: (color, intensity)
├── Blush: (color, intensity)
└── Foundation Coverage: 0.5
```

### 3.2 Eye Material Parameters

```cpp
// In Material Instance M_Eyes_Inst

Iris:
├── Iris Color: (your chosen color)
├── Iris Pattern Intensity: 1.0
├── Limbal Ring Intensity: 0.8
└── Pupil Size Base: 0.4

Sclera:
├── Sclera Color: (off-white)
├── Vein Intensity: 0.3
└── Wetness: 0.8

Cornea:
├── Refraction: 1.38
├── Specular: 0.5
└── Roughness: 0.05
```

### 3.3 Hair Material Parameters

```cpp
// In Groom Material M_Hair_Inst

Color:
├── Root Color: (darker shade)
├── Tip Color: (lighter shade)
├── Melanin: 0.5 (0=blonde, 1=black)
├── Melanin Redness: 0.5 (red tint)
└── Random Color Variation: 0.1

Physical:
├── Roughness: 0.4
├── Specular: 0.5
├── Backlit: 0.3
└── Scatter: 0.6
```

---

## Part 4: Animation Retargeting

### 4.1 Create IK Rig

```
Content Browser → Right-click → Animation → IK Rig
Skeletal Mesh: metahuman_base_skel
Name: IKR_MetaHuman
```

Define chains:
```
├── Spine (pelvis → head)
├── LeftArm (clavicle_l → hand_l)
├── RightArm (clavicle_r → hand_r)
├── LeftLeg (thigh_l → foot_l)
├── RightLeg (thigh_r → foot_r)
├── LeftHand (hand_l → fingers)
├── RightHand (hand_r → fingers)
├── Neck (neck_01 → head)
└── Head (head → jaw)
```

### 4.2 Create Retargeter

```
Content Browser → Right-click → Animation → IK Retargeter
Source IK Rig: IKR_Mannequin (or source)
Target IK Rig: IKR_MetaHuman
Name: RTG_ToMetaHuman
```

### 4.3 Retarget Animations

```
Select animation(s) → Right-click → Retarget Animations
Retargeter: RTG_ToMetaHuman
Export to: Content/Characters/Animations/
```

---

## Part 5: Face Animation Setup

### 5.1 Morph Targets (Blend Shapes)

MetaHuman includes 200+ morph targets. Key ones for lip sync:

```
Jaw:
├── jawOpen
├── jawForward
├── jawLeft / jawRight

Lips:
├── mouthClose
├── mouthFunnel
├── mouthPucker
├── mouthLeft / mouthRight
├── mouthSmileLeft / mouthSmileRight
├── mouthFrownLeft / mouthFrownRight
├── mouthDimpleLeft / mouthDimpleRight
├── mouthStretchLeft / mouthStretchRight
├── mouthRollLower / mouthRollUpper
├── mouthShrugLower / mouthShrugUpper
├── mouthPressLeft / mouthPressRight
├── mouthLowerDownLeft / mouthLowerDownRight
└── mouthUpperUpLeft / mouthUpperUpRight

Eyes:
├── eyeBlinkLeft / eyeBlinkRight
├── eyeLookDownLeft / eyeLookDownRight
├── eyeLookInLeft / eyeLookInRight
├── eyeLookOutLeft / eyeLookOutRight
├── eyeLookUpLeft / eyeLookUpRight
├── eyeSquintLeft / eyeSquintRight
└── eyeWideLeft / eyeWideRight

Brows:
├── browDownLeft / browDownRight
├── browInnerUp
├── browOuterUpLeft / browOuterUpRight

Cheeks:
├── cheekPuff
├── cheekSquintLeft / cheekSquintRight

Nose:
├── noseSneerLeft / noseSneerRight
```

### 5.2 Face Control Rig

The MetaHuman includes a Control Rig for face. Enable it:

```
In Animation Blueprint:
├── Add Control Rig Node
├── Control Rig: face_controlrig
└── Alpha: 1.0
```

### 5.3 Live Link (Optional)

For live facial capture:

```
1. Install Live Link Face app (iOS)
2. Enable Live Link plugin in UE5
3. Add Live Link Pose node in AnimBP
4. Map ARKit blendshapes to MetaHuman
```

---

## Part 6: Verification Checklist

### Synthetic Character Verification

```
□ Character created from presets (NO photo upload)
□ Multiple presets blended (3+ sources)
□ Face structure modified from any single preset
□ No real person likeness identifiable
□ Unique features added (freckles, marks, etc.)
□ Documentation of creation process saved
```

### Technical Verification

```
□ All meshes loading correctly
□ Materials rendering properly
□ Hair/Groom physics working
□ All morph targets functional
□ Animation blueprint connected
□ Lip sync morph targets responding
□ Eye tracking working
□ Blinking automatic
```

### Performance Verification

```
□ Character renders at 60 FPS (4K)
□ Hair strand count optimized
□ LODs generating correctly
□ Memory usage acceptable (<2GB for character)
□ No shadow artifacts
□ Subsurface scattering smooth
```

---

## Common Issues & Solutions

| Issue | Solution |
|-------|----------|
| Hair not rendering | Check Groom binding asset, rebind |
| Face seams visible | Adjust material blend at neck |
| Morph targets not working | Verify target names match exactly |
| Animation jittery | Check skeleton compatibility, retarget |
| Eyes look dead | Enable saccades, adjust wetness |
| Skin looks plastic | Increase subsurface, adjust roughness |
| Performance low | Reduce hair strands, enable DLSS |
