# Asset Import Guide - Complete File List

## Overview

This document lists ALL files you need to create or obtain, their specifications, and exact locations where they should be placed in the project.

---

## 📁 REQUIRED FILE STRUCTURE

```
Content/
├── Characters/
│   ├── Nova/                          ← Your character folder
│   │   ├── Meshes/
│   │   ├── Textures/
│   │   │   ├── Body/
│   │   │   ├── Face/
│   │   │   ├── Eyes/
│   │   │   └── Hair/
│   │   ├── Materials/
│   │   └── Animations/
│   └── Common/
│       ├── Animations/
│       └── Materials/
│
├── Environments/
│   ├── Bedroom/
│   │   ├── Meshes/
│   │   ├── Textures/
│   │   └── Materials/
│   ├── Bathroom/
│   ├── LivingRoom/
│   └── ... (all 12 environments)
│
├── Audio/
│   ├── Ambient/
│   ├── Music/
│   └── SFX/
│
└── UI/
    └── Textures/
```

---

## 🎭 CHARACTER TEXTURES (Body Parts)

### FACE TEXTURES

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Face_Diffuse.png` | 4096×4096 | PNG 24-bit | Base skin color |
| `T_Face_Normal.png` | 4096×4096 | PNG 24-bit | Surface detail normals |
| `T_Face_Roughness.png` | 2048×2048 | PNG 8-bit Gray | Skin roughness map |
| `T_Face_Specular.png` | 2048×2048 | PNG 8-bit Gray | Specular intensity |
| `T_Face_Subsurface.png` | 2048×2048 | PNG 24-bit | Subsurface scattering color |
| `T_Face_Cavity.png` | 2048×2048 | PNG 8-bit Gray | Ambient occlusion |
| `T_Face_MicroNormal.png` | 1024×1024 | PNG 24-bit | Skin pore detail (tiling) |
| `T_Face_Makeup_Mask.png` | 2048×2048 | PNG 32-bit RGBA | Makeup regions mask |

**Location:** `Content/Characters/Nova/Textures/Face/`

**How to Create:**
- Use MetaHuman Creator export (automatic)
- Or create in Substance Painter / Mari
- Or use AI generation (Midjourney + cleanup)

---

### BODY TEXTURES

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Body_Diffuse.png` | 4096×4096 | PNG 24-bit | Skin color |
| `T_Body_Normal.png` | 4096×4096 | PNG 24-bit | Body surface normals |
| `T_Body_Roughness.png` | 2048×2048 | PNG 8-bit Gray | Roughness variation |
| `T_Body_Specular.png` | 2048×2048 | PNG 8-bit Gray | Specular map |
| `T_Body_Subsurface.png` | 2048×2048 | PNG 24-bit | SSS color |
| `T_Body_Cavity.png` | 2048×2048 | PNG 8-bit Gray | AO/cavity |

**Location:** `Content/Characters/Nova/Textures/Body/`

---

### EYE TEXTURES

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Eye_Iris_D.png` | 1024×1024 | PNG 24-bit | Iris color/pattern |
| `T_Eye_Iris_N.png` | 1024×1024 | PNG 24-bit | Iris normal |
| `T_Eye_Sclera_D.png` | 512×512 | PNG 24-bit | Sclera (white) with veins |
| `T_Eye_Sclera_N.png` | 512×512 | PNG 24-bit | Sclera normal |
| `T_Eye_Cornea_N.png` | 256×256 | PNG 24-bit | Cornea refraction normal |

**Location:** `Content/Characters/Nova/Textures/Eyes/`

**Notes:**
- Iris should be tileable for pupil dilation effect
- Include subtle color variation in iris
- Sclera needs realistic vein patterns

---

### HAIR TEXTURES

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Hair_Diffuse.png` | 2048×2048 | PNG 24-bit | Hair color gradient |
| `T_Hair_ID.png` | 2048×2048 | PNG 8-bit Gray | Strand ID for variation |
| `T_Hair_Root.png` | 2048×2048 | PNG 8-bit Gray | Root-to-tip gradient |
| `T_Hair_Depth.png` | 2048×2048 | PNG 8-bit Gray | Depth/AO |
| `T_Hair_Alpha.png` | 2048×2048 | PNG 8-bit Gray | Transparency (card-based) |

**Location:** `Content/Characters/Nova/Textures/Hair/`

**Notes:**
- For Groom (strand-based), most texturing is procedural
- Card-based hair needs alpha maps

---

### ADDITIONAL BODY DETAIL TEXTURES (Optional)

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Nails_D.png` | 512×512 | PNG 24-bit | Fingernail/toenail color |
| `T_Nails_N.png` | 512×512 | PNG 24-bit | Nail normal |
| `T_Lips_Detail.png` | 1024×1024 | PNG 24-bit | Lip texture detail |
| `T_Teeth_D.png` | 1024×1024 | PNG 24-bit | Teeth color |
| `T_Tongue_D.png` | 512×512 | PNG 24-bit | Tongue color |
| `T_Gums_D.png` | 512×512 | PNG 24-bit | Gums color |

**Location:** `Content/Characters/Nova/Textures/Details/`

---

## 🏠 ENVIRONMENT TEXTURES

### For Each Environment (Example: Bedroom)

#### FLOOR

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Floor_Wood_D.png` | 2048×2048 | PNG 24-bit | Wood floor color |
| `T_Floor_Wood_N.png` | 2048×2048 | PNG 24-bit | Wood floor normal |
| `T_Floor_Wood_R.png` | 2048×2048 | PNG 8-bit | Roughness |
| `T_Floor_Wood_AO.png` | 2048×2048 | PNG 8-bit | Ambient occlusion |

**Location:** `Content/Environments/Bedroom/Textures/`

#### WALLS

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Wall_Plaster_D.png` | 2048×2048 | PNG 24-bit | Wall paint/plaster |
| `T_Wall_Plaster_N.png` | 2048×2048 | PNG 24-bit | Subtle texture |
| `T_Wall_Accent_D.png` | 2048×2048 | PNG 24-bit | Accent wall (velvet, etc.) |
| `T_Wall_Accent_N.png` | 2048×2048 | PNG 24-bit | Accent wall normal |

#### FABRIC/BEDDING

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Silk_D.png` | 2048×2048 | PNG 24-bit | Silk sheet color |
| `T_Silk_N.png` | 2048×2048 | PNG 24-bit | Silk weave normal |
| `T_Velvet_D.png` | 2048×2048 | PNG 24-bit | Velvet color |
| `T_Velvet_N.png` | 2048×2048 | PNG 24-bit | Velvet pile normal |
| `T_Leather_D.png` | 2048×2048 | PNG 24-bit | Leather color |
| `T_Leather_N.png` | 2048×2048 | PNG 24-bit | Leather grain |

#### FURNITURE

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Wood_Walnut_D.png` | 2048×2048 | PNG 24-bit | Walnut wood grain |
| `T_Wood_Walnut_N.png` | 2048×2048 | PNG 24-bit | Wood normal |
| `T_Marble_D.png` | 2048×2048 | PNG 24-bit | Marble pattern |
| `T_Marble_N.png` | 2048×2048 | PNG 24-bit | Marble surface |
| `T_Metal_Brass_D.png` | 1024×1024 | PNG 24-bit | Brushed brass |
| `T_Metal_Gold_D.png` | 1024×1024 | PNG 24-bit | Gold accents |

#### WINDOW/BACKDROP

| File Name | Resolution | Format | Description |
|-----------|------------|--------|-------------|
| `T_Skyline_Night.hdr` | 8192×4096 | HDR/EXR | City night HDRI |
| `T_Skyline_Day.hdr` | 8192×4096 | HDR/EXR | City day HDRI |
| `T_Skyline_Sunset.hdr` | 8192×4096 | HDR/EXR | Sunset HDRI |
| `T_Curtain_Sheer.png` | 2048×2048 | PNG 32-bit | Sheer curtain with alpha |

---

## 🔊 AUDIO FILES

### Ambient Sounds

| File Name | Format | Duration | Description |
|-----------|--------|----------|-------------|
| `A_AMB_Bedroom_Night.wav` | WAV 48kHz | Loop | Night room tone, AC |
| `A_AMB_Bedroom_Day.wav` | WAV 48kHz | Loop | Day room, outside noise |
| `A_AMB_City_Night.wav` | WAV 48kHz | Loop | Distant traffic, city |
| `A_AMB_Forest_Day.wav` | WAV 48kHz | Loop | Birds, wind, nature |
| `A_AMB_Beach_Waves.wav` | WAV 48kHz | Loop | Ocean waves |
| `A_AMB_Rain_Window.wav` | WAV 48kHz | Loop | Rain on window |
| `A_AMB_Fireplace.wav` | WAV 48kHz | Loop | Fire crackling |

**Location:** `Content/Audio/Ambient/`

### Music Tracks

| File Name | Format | Duration | Description |
|-----------|--------|----------|-------------|
| `A_MUS_Chill_01.wav` | WAV 48kHz | 3-5 min | Lo-fi chill |
| `A_MUS_Chill_02.wav` | WAV 48kHz | 3-5 min | Ambient chill |
| `A_MUS_Romantic_01.wav` | WAV 48kHz | 3-5 min | Soft romantic |
| `A_MUS_Playful_01.wav` | WAV 48kHz | 3-5 min | Upbeat playful |
| `A_MUS_Sensual_01.wav` | WAV 48kHz | 3-5 min | Sultry beats |

**Location:** `Content/Audio/Music/`

### Sound Effects

| File Name | Format | Description |
|-----------|--------|-------------|
| `A_SFX_Kiss.wav` | WAV 48kHz | Kiss sound |
| `A_SFX_Giggle.wav` | WAV 48kHz | Cute giggle |
| `A_SFX_Sigh.wav` | WAV 48kHz | Content sigh |
| `A_SFX_Gasp.wav` | WAV 48kHz | Surprised gasp |
| `A_SFX_Moan_Soft.wav` | WAV 48kHz | Soft moan |
| `A_SFX_Breath_In.wav` | WAV 48kHz | Breath inhale |
| `A_SFX_Breath_Out.wav` | WAV 48kHz | Breath exhale |
| `A_SFX_Fabric_Rustle.wav` | WAV 48kHz | Clothing/sheets |
| `A_SFX_Tip_Ding.wav` | WAV 48kHz | Tip notification |
| `A_SFX_Sub_Chime.wav` | WAV 48kHz | Subscription sound |

**Location:** `Content/Audio/SFX/`

---

## 🎬 ANIMATION FILES

### Required Animation Montages

| File Name | Duration | Description |
|-----------|----------|-------------|
| `AM_Idle_01.uasset` | Loop | Default idle |
| `AM_Idle_02.uasset` | Loop | Idle variation |
| `AM_Idle_03.uasset` | Loop | Idle variation |
| `AM_Talk_Idle.uasset` | Loop | Subtle talking pose |
| `AM_Wave.uasset` | 2-3s | Friendly wave |
| `AM_Blow_Kiss.uasset` | 2s | Blow kiss gesture |
| `AM_Wink.uasset` | 1s | Flirty wink |
| `AM_Hair_Flip.uasset` | 2s | Hair flip |
| `AM_Giggle.uasset` | 2s | Giggling reaction |
| `AM_Shy.uasset` | 2s | Shy/coy reaction |
| `AM_Excited.uasset` | 3s | Excited jump/clap |
| `AM_Thank_You.uasset` | 2s | Thank you gesture |
| `AM_Tip_Reaction_Small.uasset` | 2s | Small tip reaction |
| `AM_Tip_Reaction_Big.uasset` | 4s | Big tip celebration |
| `AM_Dance_01.uasset` | Loop | Dance loop |
| `AM_Dance_02.uasset` | Loop | Dance variation |
| `AM_Pose_01.uasset` | Hold | Model pose |
| `AM_Pose_02.uasset` | Hold | Model pose |
| `AM_Pose_Sexy_01.uasset` | Hold | Alluring pose |
| `AM_Sit_Down.uasset` | 2s | Sit down transition |
| `AM_Stand_Up.uasset` | 2s | Stand up transition |
| `AM_Lie_Down.uasset` | 3s | Lie down on bed |
| `AM_Turn_Around.uasset` | 3s | Turn around (request) |

**Location:** `Content/Characters/Nova/Animations/`

### Facial Animation Curves

| File Name | Type | Description |
|-----------|------|-------------|
| `FC_Happy.uasset` | Pose Asset | Happy expression |
| `FC_Sad.uasset` | Pose Asset | Sad expression |
| `FC_Surprised.uasset` | Pose Asset | Surprised expression |
| `FC_Flirty.uasset` | Pose Asset | Flirty expression |
| `FC_Thinking.uasset` | Pose Asset | Thinking expression |
| `FC_Excited.uasset` | Pose Asset | Excited expression |
| `FC_Pouty.uasset` | Pose Asset | Pouty/begging expression |

**Location:** `Content/Characters/Nova/Animations/Facial/`

---

## 🛋️ 3D MODELS (Meshes)

### Furniture (per environment)

| File Name | Format | Polycount | Description |
|-----------|--------|-----------|-------------|
| `SM_Bed_King.fbx` | FBX | <100k | King bed frame |
| `SM_Mattress.fbx` | FBX | <20k | Mattress |
| `SM_Headboard.fbx` | FBX | <30k | Tufted headboard |
| `SM_Nightstand.fbx` | FBX | <10k | Bedside table |
| `SM_Lamp_Table.fbx` | FBX | <5k | Table lamp |
| `SM_Vanity.fbx` | FBX | <30k | Vanity with mirror |
| `SM_Chair_Accent.fbx` | FBX | <15k | Accent chair |
| `SM_Rug_Area.fbx` | FBX | <5k | Area rug |
| `SM_Curtain.fbx` | FBX | <10k | Curtain (with cloth sim) |
| `SM_Window_Frame.fbx` | FBX | <5k | Window frame |

**Location:** `Content/Environments/Bedroom/Meshes/`

**Note:** Use Nanite for high-poly meshes (auto-LOD)

### Sources for 3D Models:
- **Quixel Megascans** (included with UE5)
- **Sketchfab** (purchase or free)
- **TurboSquid**
- **CGTrader**
- **KitBash3D**

---

## 📋 COMPLETE CHECKLIST

### Character Assets
```
□ Face diffuse texture (4K)
□ Face normal map (4K)
□ Face roughness map (2K)
□ Face subsurface map (2K)
□ Body diffuse texture (4K)
□ Body normal map (4K)
□ Eye iris texture
□ Eye sclera texture
□ Hair groom or cards
□ Eyebrow groom
□ Eyelash groom
□ 23+ animation montages
□ 7 facial expression poses
□ Idle variations (3+)
□ Dance animations (2+)
□ Reaction animations (10+)
```

### Environment Assets (per environment)
```
□ Floor textures (D, N, R)
□ Wall textures (D, N)
□ Furniture models (10-20 pieces)
□ Fabric textures
□ HDRI backdrop
□ Ambient audio loop
□ Music tracks (3-5)
□ Props (decorative items)
□ Lighting setup
□ Post-process preset
```

### Audio Assets
```
□ 7+ ambient loops
□ 5+ music tracks
□ 15+ SFX sounds
□ Breathing sounds
□ Reaction sounds
```

### UI Assets
```
□ Chat tier icons
□ Tip notification graphics
□ Loading screen
□ Logo/branding
```

---

## 🔧 IMPORT SETTINGS

### Texture Import Settings

```
Compression: BC7 (high quality) or BC1 (performance)
sRGB: ON for Diffuse, OFF for Normal/Roughness/Metallic
Mip Gen: From Texture Group
Max Size: 4096 or 2048
LOD Bias: 0
Virtual Texture: Enable for 4K+ textures
```

### Mesh Import Settings

```
Skeletal Mesh:
├── Import Mesh: Yes
├── Skeleton: Use existing (metahuman_base_skel)
├── Import Morph Targets: Yes
├── Import Animations: Separate
└── Nanite: Enable

Static Mesh:
├── Generate Lightmap UVs: Yes
├── Nanite: Enable for >10k polys
├── Collision: Auto generate
└── LODs: Use Nanite or generate 4 LODs
```

### Audio Import Settings

```
Format: PCM or ADPCM
Sample Rate: 48000
Channels: Stereo (music), Mono (SFX)
Compression: Quality 80-100
Streaming: Enable for music >30s
```

---

## 📍 QUICK REFERENCE - WHERE TO PUT FILES

| Asset Type | Location |
|------------|----------|
| Character textures | `Content/Characters/[Name]/Textures/` |
| Character meshes | `Content/Characters/[Name]/Meshes/` |
| Animations | `Content/Characters/[Name]/Animations/` |
| Environment textures | `Content/Environments/[Env]/Textures/` |
| Environment meshes | `Content/Environments/[Env]/Meshes/` |
| Ambient audio | `Content/Audio/Ambient/` |
| Music | `Content/Audio/Music/` |
| SFX | `Content/Audio/SFX/` |
| HDRIs | `Content/Environments/HDRI/` |
| UI textures | `Content/UI/Textures/` |
| Blueprints | `Content/Blueprints/` |
| Data assets | `Content/Data/` |

---

## 🎨 AI-GENERATED TEXTURE WORKFLOW

### Using AI (Midjourney/Stable Diffusion) for Textures

1. **Generate Base Image**
   ```
   Prompt example for skin:
   "human skin texture, photorealistic, 8k, seamless tileable,
   pores visible, subsurface scattering reference, medical quality"
   ```

2. **Process in Photoshop/GIMP**
   - Make seamless (Offset filter + clone stamp)
   - Color correct
   - Generate normal map (Filter > 3D > Generate Normal Map)
   - Create roughness from desaturated version
   - Export as PNG

3. **Validate in Substance**
   - Check tiling
   - Adjust levels
   - Export final maps

### Recommended AI Prompts

**Skin Texture:**
```
"photorealistic human skin close-up, pores, fine detail,
subsurface scattering visible, 8k texture, seamless"
```

**Fabric (Silk):**
```
"silk fabric texture, subtle sheen, soft folds,
photorealistic, seamless tileable, 4k"
```

**Wood Grain:**
```
"walnut wood grain texture, polished, photorealistic,
seamless tileable, furniture quality, 4k"
```

**Marble:**
```
"calacatta marble texture, white with grey veins,
polished, photorealistic, seamless, 4k"
```
