# Adult Animation System - Complete Specification

## ⚠️ Content Notice

This document specifies animations for an **adult entertainment platform** featuring a **fully synthetic AI character**. All content is for characters verified as:
- 100% AI-generated (no real person likeness)
- Adult (18+) in appearance
- Clearly disclosed as AI

---

## Animation Categories Overview

| Category | Count | Tier Required |
|----------|-------|---------------|
| Basic (SFW) | 63 | Free |
| Flirty/Teasing | 25 | Subscriber |
| Lingerie/Swimwear | 20 | Premium |
| Artistic Nude | 30 | VIP |
| Sensual/Intimate | 40 | VIP+ |
| Interactive | 25 | Premium+ |

**Total: ~203 animations**

---

## 1️⃣ CLOTHING TRANSITION ANIMATIONS

### Undressing Sequences (Top)

```
AM_Undress_Shirt_Buttons
├── Duration: 8 seconds
├── Description: Slowly unbutton shirt, one button at a time
├── Emotion: Playful/teasing
├── Camera: Medium → Close-up on hands
└── Tier: Subscriber

AM_Undress_Shirt_Pull
├── Duration: 4 seconds
├── Description: Pull shirt over head
├── Body physics: Active on reveal
└── Tier: Subscriber

AM_Undress_Shirt_SlideOff
├── Duration: 6 seconds
├── Description: Slide shirt off shoulders slowly
├── Emotion: Sensual
└── Tier: Premium

AM_Undress_Bra_Back
├── Duration: 5 seconds
├── Description: Reach back, unclasp bra
├── Turn: Partial back view
└── Tier: VIP

AM_Undress_Bra_Front
├── Duration: 5 seconds
├── Description: Front unclasp bra
├── Eye contact: Maintained
└── Tier: VIP

AM_Undress_Bra_SlideOff
├── Duration: 4 seconds
├── Description: Let straps fall, remove
├── Covering: Optional hand cover
└── Tier: VIP
```

### Undressing Sequences (Bottom)

```
AM_Undress_Pants_Unbutton
├── Duration: 3 seconds
├── Description: Unbutton/unzip pants
└── Tier: Subscriber

AM_Undress_Pants_SlideDown
├── Duration: 6 seconds
├── Description: Slide pants down legs
├── Bend: Forward bend or squat
└── Tier: Premium

AM_Undress_Skirt_Unzip
├── Duration: 4 seconds
├── Description: Side zip, let fall
└── Tier: Subscriber

AM_Undress_Skirt_SlideUp
├── Duration: 5 seconds
├── Description: Slowly raise skirt
├── Tease: Incremental reveal
└── Tier: Premium

AM_Undress_Panties_Side
├── Duration: 6 seconds
├── Description: Hook thumbs, slide down sides
└── Tier: VIP

AM_Undress_Panties_Back
├── Duration: 5 seconds
├── Description: Back view removal
└── Tier: VIP
```

### Dressing Sequences

```
AM_Dress_Bra_Put
├── Duration: 6 seconds
├── Description: Put on bra (reverse of removal)

AM_Dress_Panties_Put
├── Duration: 5 seconds
├── Description: Step into, pull up

AM_Dress_Shirt_Put
├── Duration: 5 seconds

AM_Dress_Pants_Put
├── Duration: 6 seconds

AM_Dress_Robe_Wrap
├── Duration: 4 seconds
├── Description: Wrap robe around body
```

---

## 2️⃣ POSE ANIMATIONS (Model Style)

### Standing Poses - Clothed

```
AM_Pose_Model_Front
├── Duration: Hold
├── Description: Classic model pose, one hip out
├── Hands: On hip / relaxed

AM_Pose_Model_ThreeQuarter
├── Duration: Hold
├── Description: 3/4 angle, flattering

AM_Pose_Model_Profile
├── Duration: Hold
├── Description: Side profile, chin up

AM_Pose_Model_Back
├── Duration: Hold
├── Description: Looking over shoulder

AM_Pose_Confident
├── Duration: Hold
├── Description: Power pose, both hands on hips

AM_Pose_Cute
├── Duration: Hold
├── Description: Playful, peace sign or similar
```

### Standing Poses - Lingerie/Swimwear

```
AM_Pose_Lingerie_Front
├── Duration: Hold
├── Description: Confident lingerie pose
├── Tier: Premium

AM_Pose_Lingerie_Side
├── Duration: Hold
├── Description: Side angle, curves emphasized
├── Tier: Premium

AM_Pose_Lingerie_Back
├── Duration: Hold
├── Description: Back view, looking over shoulder
├── Tier: Premium

AM_Pose_Bikini_Beach
├── Duration: Hold
├── Description: Relaxed beach pose

AM_Pose_Bikini_Playful
├── Duration: Hold
├── Description: Playful swimwear pose
```

### Standing Poses - Artistic Nude

```
AM_Pose_Artistic_Venus
├── Duration: Hold
├── Description: Classical Venus pose
├── Covering: Strategic arm/hand placement
├── Tier: VIP

AM_Pose_Artistic_Contrapposto
├── Duration: Hold
├── Description: Classical weight shift pose
├── Tier: VIP

AM_Pose_Artistic_Profile
├── Duration: Hold
├── Description: Elegant side profile
├── Tier: VIP

AM_Pose_Artistic_Back
├── Duration: Hold
├── Description: Back view, artistic
├── Tier: VIP

AM_Pose_Nude_Confident
├── Duration: Hold
├── Description: Confident full pose
├── Tier: VIP+

AM_Pose_Nude_Playful
├── Duration: Hold
├── Description: Playful/flirty nude
├── Tier: VIP+
```

### Sitting Poses

```
AM_Pose_Sit_Elegant
├── Duration: Hold
├── Surface: Chair/couch
├── Legs: Crossed

AM_Pose_Sit_Casual
├── Duration: Hold
├── Description: Relaxed sitting

AM_Pose_Sit_Flirty
├── Duration: Hold
├── Description: Legs to side, leaning

AM_Pose_Sit_Edge
├── Duration: Hold
├── Surface: Bed edge
├── Tier: Premium

AM_Pose_Sit_Spread
├── Duration: Hold
├── Description: Legs apart, confident
├── Tier: VIP
```

### Lying Poses

```
AM_Pose_Lie_Side
├── Duration: Hold
├── Surface: Bed
├── Description: Side lying, head propped

AM_Pose_Lie_Back
├── Duration: Hold
├── Description: On back, relaxed

AM_Pose_Lie_Stomach
├── Duration: Hold
├── Description: On stomach, looking back

AM_Pose_Lie_Sensual_Side
├── Duration: Hold
├── Description: Sensual side pose
├── Tier: Premium

AM_Pose_Lie_Sensual_Back
├── Duration: Hold
├── Description: Sensual back pose
├── Tier: VIP

AM_Pose_Lie_Artistic
├── Duration: Hold
├── Description: Artistic nude lying
├── Tier: VIP
```

### Kneeling Poses

```
AM_Pose_Kneel_Elegant
├── Duration: Hold
├── Description: Elegant kneeling

AM_Pose_Kneel_Playful
├── Duration: Hold
├── Description: Playful on knees

AM_Pose_Kneel_AllFours
├── Duration: Hold
├── Description: Hands and knees
├── Tier: Premium

AM_Pose_Kneel_Back
├── Duration: Hold
├── Description: Kneeling back view
├── Tier: VIP
```

---

## 3️⃣ MOVEMENT ANIMATIONS

### Walking

```
AM_Walk_Normal
├── Duration: Loop
├── Description: Natural feminine walk

AM_Walk_Runway
├── Duration: Loop
├── Description: Model runway walk

AM_Walk_Sexy
├── Duration: Loop
├── Description: Exaggerated hip sway
├── Tier: Premium

AM_Walk_Heels
├── Duration: Loop
├── Description: Walking in high heels
```

### Dancing

```
AM_Dance_Sway
├── Duration: Loop
├── Description: Gentle swaying to music

AM_Dance_Club
├── Duration: Loop
├── Description: Club/party dancing

AM_Dance_Sensual
├── Duration: Loop
├── Description: Slow sensual movement
├── Tier: Premium

AM_Dance_Twerk
├── Duration: Loop
├── Description: Twerking motion
├── Body physics: Enhanced butt physics
├── Tier: Premium

AM_Dance_Lapdance
├── Duration: 30 seconds
├── Description: Seated lapdance movements
├── Tier: VIP

AM_Dance_Pole_Basic
├── Duration: Loop
├── Description: Basic pole movements
├── Requires: Pole prop
├── Tier: VIP

AM_Dance_Strip_Tease
├── Duration: 60 seconds
├── Description: Full striptease routine
├── Tier: VIP
```

---

## 4️⃣ INTERACTIVE ANIMATIONS

### Touch Reactions (Body Part Specific)

```
AM_React_Touch_Hair
├── Duration: 2 seconds
├── Description: Pleased reaction to hair touch
├── Emotion: Happy

AM_React_Touch_Face
├── Duration: 2 seconds
├── Description: Gentle face touch reaction
├── Emotion: Tender

AM_React_Touch_Shoulder
├── Duration: 2 seconds
├── Description: Comfortable shoulder touch

AM_React_Touch_Back
├── Duration: 2 seconds
├── Description: Arching into back touch
├── Tier: Premium

AM_React_Touch_Hip
├── Duration: 2 seconds
├── Description: Hip touch reaction
├── Tier: Premium

AM_React_Touch_Thigh
├── Duration: 2 seconds
├── Description: Thigh touch reaction
├── Tier: VIP

AM_React_Touch_Breast
├── Duration: 2 seconds
├── Description: Breast touch reaction
├── Body physics: Responsive
├── Tier: VIP

AM_React_Touch_Butt
├── Duration: 2 seconds
├── Description: Butt touch/slap reaction
├── Body physics: Jiggle
├── Tier: VIP
```

### Self-Touch Animations

```
AM_SelfTouch_Hair
├── Duration: 3 seconds
├── Description: Playing with own hair

AM_SelfTouch_Neck
├── Duration: 3 seconds
├── Description: Touching own neck sensually
├── Tier: Premium

AM_SelfTouch_Body_Tease
├── Duration: 4 seconds
├── Description: Hands running down body
├── Tier: Premium

AM_SelfTouch_Chest
├── Duration: 3 seconds
├── Description: Hands on chest
├── Tier: VIP

AM_SelfTouch_Intimate
├── Duration: 5 seconds
├── Description: More intimate self-touch
├── Tier: VIP+
```

---

## 5️⃣ EMOTIONAL/EXPRESSIVE ANIMATIONS

### Flirty/Teasing

```
AM_Flirt_Wink
├── Duration: 1.5 seconds
├── Description: Flirty wink

AM_Flirt_BlowKiss
├── Duration: 2 seconds
├── Description: Blow kiss at camera

AM_Flirt_LipBite
├── Duration: 2 seconds
├── Description: Subtle lip bite
├── Tier: Subscriber

AM_Flirt_BeckonFinger
├── Duration: 2 seconds
├── Description: "Come here" finger gesture
├── Tier: Premium

AM_Flirt_Peek
├── Duration: 3 seconds
├── Description: Playful peek/hide

AM_Flirt_Tease_Strap
├── Duration: 4 seconds
├── Description: Play with bra/dress strap
├── Tier: Premium

AM_Flirt_Tease_Lift
├── Duration: 4 seconds
├── Description: Lift hem slightly, tease
├── Tier: Premium
```

### Sensual Expressions

```
AM_Express_Pleasure_Soft
├── Duration: 3 seconds
├── Description: Soft pleasure expression
├── Facial: Eyes close, slight moan
├── Tier: VIP

AM_Express_Pleasure_Medium
├── Duration: 3 seconds
├── Description: Medium intensity
├── Tier: VIP

AM_Express_Pleasure_Intense
├── Duration: 3 seconds
├── Description: Intense pleasure expression
├── Tier: VIP+

AM_Express_Moan_Soft
├── Duration: 2 seconds
├── Audio: Soft moan
├── Tier: VIP

AM_Express_Moan_Loud
├── Duration: 2 seconds
├── Audio: Louder moan
├── Tier: VIP+

AM_Express_Gasp
├── Duration: 1 second
├── Description: Surprised gasp
├── Tier: Premium

AM_Express_Sigh_Content
├── Duration: 2 seconds
├── Description: Satisfied sigh
```

---

## 6️⃣ SPECIAL SEQUENCES

### Shower/Bath Sequences

```
AM_Shower_Enter
├── Duration: 4 seconds
├── Environment: Bathroom

AM_Shower_WaterReact
├── Duration: Loop
├── Description: Reacting to water

AM_Shower_Wash_Hair
├── Duration: 8 seconds
├── Description: Washing hair

AM_Shower_Wash_Body
├── Duration: 10 seconds
├── Description: Washing body
├── Tier: Premium

AM_Bath_Enter
├── Duration: 5 seconds
├── Description: Getting into tub

AM_Bath_Relax
├── Duration: Loop
├── Description: Relaxing in bath

AM_Bath_Play
├── Duration: 6 seconds
├── Description: Playing with bubbles/water
```

### Bed Sequences

```
AM_Bed_GetIn
├── Duration: 4 seconds
├── Description: Getting into bed

AM_Bed_Stretch
├── Duration: 4 seconds
├── Description: Stretching in bed

AM_Bed_Roll
├── Duration: 3 seconds
├── Description: Rolling over

AM_Bed_Pillow_Hug
├── Duration: Hold
├── Description: Hugging pillow

AM_Bed_Sheets_Play
├── Duration: 5 seconds
├── Description: Playing with sheets
├── Tier: Premium

AM_Bed_Reveal
├── Duration: 4 seconds
├── Description: Pulling down sheet reveal
├── Tier: VIP
```

### Morning/Wake Up

```
AM_WakeUp_Stretch
├── Duration: 5 seconds
├── Description: Morning stretch in bed

AM_WakeUp_Yawn
├── Duration: 3 seconds
├── Description: Cute morning yawn

AM_WakeUp_Sit
├── Duration: 3 seconds
├── Description: Sitting up in bed
```

---

## 7️⃣ BREATHING & SUBTLE ANIMATIONS

### Breathing Overlays (Additive)

```
AM_Breath_Normal
├── Type: Additive Loop
├── Description: Normal breathing

AM_Breath_Heavy
├── Type: Additive Loop
├── Description: Heavy breathing
├── Chest: More expansion

AM_Breath_Excited
├── Type: Additive Loop
├── Description: Quick excited breaths

AM_Breath_Aroused
├── Type: Additive Loop
├── Description: Aroused breathing pattern
├── Tier: VIP
```

### Idle Overlays

```
AM_Idle_WeightShift
├── Type: Additive Loop
├── Description: Subtle weight shifting

AM_Idle_LookAround
├── Type: Additive
├── Description: Occasional looking around

AM_Idle_HairTouch
├── Type: Additive
├── Description: Occasional hair touch

AM_Idle_Fidget
├── Type: Additive
├── Description: Small fidgeting movements
```

---

## 📋 ANIMATION TECHNICAL SPECS

### Skeleton Requirements

```
Required Bones (beyond standard):
├── breast_l, breast_r (for physics)
├── butt_l, butt_r (for physics)
├── Additional spine twist bones
├── Detailed hand bones
└── Facial bones (ARKit compatible)

Morph Targets:
├── Full ARKit set (52 shapes)
├── Body morphs for breathing
├── Breast size variants
└── Muscle flex morphs
```

### Physics Bones Setup

```
Breast Physics:
├── breast_l: Child of spine_03
├── breast_r: Child of spine_03
├── Constraint: Soft, gravity-affected
├── Collision: Arm, chest

Butt Physics:
├── butt_l: Child of pelvis
├── butt_r: Child of pelvis
├── Constraint: Medium stiffness
├── Collision: Thighs

Belly Physics:
├── belly: Child of spine_02
├── Constraint: Subtle movement
```

### Layer Blending

```
Layer 0: Base Locomotion
Layer 1: Full Body Poses
Layer 2: Upper Body Override
Layer 3: Arm/Hand Layer
Layer 4: Head/Neck Layer
Layer 5: Facial Layer
Layer 6: Additive Breathing
Layer 7: Physics Response
```

---

## 🔒 TIER ACCESS MATRIX

| Animation Type | Free | Sub | Premium | VIP | VIP+ |
|----------------|------|-----|---------|-----|------|
| Basic idle/talk | ✓ | ✓ | ✓ | ✓ | ✓ |
| Clothed poses | ✓ | ✓ | ✓ | ✓ | ✓ |
| Flirty gestures | - | ✓ | ✓ | ✓ | ✓ |
| Undress (top) | - | ✓ | ✓ | ✓ | ✓ |
| Lingerie poses | - | - | ✓ | ✓ | ✓ |
| Undress (full) | - | - | ✓ | ✓ | ✓ |
| Artistic nude | - | - | - | ✓ | ✓ |
| Sensual dance | - | - | - | ✓ | ✓ |
| Touch reactions | - | - | - | ✓ | ✓ |
| Intimate content | - | - | - | - | ✓ |

---

## 📁 FILE ORGANIZATION

```
Content/Characters/Nova/Animations/
├── Basic/
│   ├── Idle/
│   ├── Talk/
│   └── React/
├── Movement/
│   ├── Walk/
│   └── Dance/
├── Poses/
│   ├── Standing/
│   ├── Sitting/
│   ├── Lying/
│   └── Kneeling/
├── Clothing/
│   ├── Undress/
│   └── Dress/
├── Flirty/
├── Sensual/
│   ├── Expressions/
│   ├── SelfTouch/
│   └── Reactions/
├── Sequences/
│   ├── Shower/
│   ├── Bed/
│   └── Special/
└── Additive/
    ├── Breathing/
    └── Overlays/
```
