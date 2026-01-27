# Blueprint Templates Guide

## Overview

This document provides step-by-step instructions for creating all required Blueprint assets for the AI Digital Human system.

---

## 1. Character Blueprint (BP_AIDigitalHuman)

### Creation Steps

```
Content Browser → Right-click → Blueprint Class
Parent Class: AIDigitalHumanCharacter
Name: BP_AIDigitalHuman
Location: Content/Blueprints/Character/
```

### Component Setup

```
BP_AIDigitalHuman
├── CapsuleComponent (inherited)
├── ArrowComponent (inherited)
├── Mesh (SkeletalMeshComponent) ← Body skeletal mesh
│   ├── FaceMesh (SkeletalMeshComponent) ← Face skeletal mesh
│   │   ├── EyebrowGroom (GroomComponent)
│   │   └── EyelashGroom (GroomComponent)
│   └── HairGroom (GroomComponent)
├── VoiceAudio (AudioComponent)
└── CharacterMovement (inherited)
```

### Variables to Add

```cpp
// Character Identity
CharacterName: String = "Nova"
PersonalityPreset: DataAsset (CharacterPersonality)

// Visual Customization
SkinMaterialInstance: MaterialInstanceDynamic
EyeMaterialInstance: MaterialInstanceDynamic
HairColor: LinearColor
EyeColor: LinearColor

// State
bIsInteracting: Boolean = false
CurrentInteractionTarget: Actor Reference
```

### Event Graph Setup

```
Event BeginPlay
├── Get Game Instance → Get Subsystem (AIDigitalHumanSubsystem)
│   └── Store as variable: Subsystem
├── Subsystem → OnEmotionChanged → Bind to SetEmotion
├── Subsystem → OnSpeechStarted → Bind to OnSpeechStart
├── Subsystem → OnSpeechEnded → Bind to OnSpeechEnd
└── Initialize Eye Tracking (find player camera)

Event Tick
├── Update Gaze Target (smooth follow)
├── Update Breathing Animation
└── Update Idle Variation Timer
```

### Key Functions to Implement

```
Function: InitializeFromPreset
Input: PersonalityPreset (DataAsset)
├── Set CharacterName
├── Apply Skin Material Parameters
├── Apply Hair Settings
└── Configure Voice Settings

Function: ReactToTip
Input: Amount (float), Username (string)
├── Branch on Amount
│   ├── >= 100: Play "BigTipReaction" montage
│   ├── >= 50: Play "MediumTipReaction" montage
│   ├── >= 10: Play "SmallTipReaction" montage
│   └── Default: Play "ThankYou" montage
├── Set Emotion (Excited, Amount/100 clamped)
└── Trigger Voice Response

Function: LookAtCamera
├── Get Player Camera Location
├── Set Gaze Target (Location)
└── Enable Smooth Follow
```

---

## 2. Animation Blueprint (ABP_AIDigitalHuman)

### Creation Steps

```
Content Browser → Right-click → Animation → Animation Blueprint
Skeleton: metahuman_base_skel (or your MetaHuman skeleton)
Parent Class: AIDigitalHumanAnimInstance
Name: ABP_AIDigitalHuman
Location: Content/Blueprints/Character/
```

### AnimGraph Structure

```
AnimGraph
├── Slot 'DefaultSlot' (for montages)
│   └── Layered Blend Per Bone
│       ├── Base Pose
│       │   └── State Machine: LocomotionSM
│       ├── Additive Layer (Breathing)
│       │   └── Breathing Animation (looping)
│       ├── Facial Layer
│       │   └── Pose driven by morph targets
│       └── Look At Layer
│           └── Control Rig: HeadLookAt
```

### State Machine: LocomotionSM

```
States:
├── Idle
│   ├── Entry: Default state
│   ├── Animation: Idle_Variations (blend by IdleVariationIndex)
│   └── Transitions:
│       ├── → Talking (IsTalking == true)
│       ├── → Gesturing (IsGesturing == true)
│       └── → Interacting (IsInteracting == true)
│
├── Talking
│   ├── Animation: Talk_Idle (subtle body movement)
│   ├── Blend: GestureWeight controls hand movements
│   └── Transitions:
│       └── → Idle (IsTalking == false, 0.3s blend)
│
├── Gesturing
│   ├── Animation: Current Gesture Animation
│   └── Transitions:
│       └── → Idle (gesture complete)
│
├── Interacting
│   ├── Animation: Interaction_Base
│   └── Transitions:
│       └── → Idle (IsInteracting == false)
│
└── Special
    ├── Entry: From any state via montage
    └── Exit: Return to previous state
```

### Event Graph

```
Event Blueprint Initialize Animation
├── Get Owning Actor → Cast to BP_AIDigitalHuman
│   └── Store as OwnerCharacter
└── Initialize Cached Variables

Event Blueprint Update Animation
├── Update Emotion Blend Weights
├── Update Breathing Phase
├── Update Look At Target
└── Apply Lip Sync Weights (from VoiceSynthesisManager)
```

### Key Variables

```cpp
// From Parent Class (inherited)
CurrentEmotion: EDigitalHumanEmotion
EmotionIntensity: float
IsTalking: bool
SpeechIntensity: float

// Custom Variables
IdleVariationIndex: int (0-5)
IdleVariationTimer: float
GestureWeight: float (0-1)
IsGesturing: bool
CurrentGestureName: string
BreathingMultiplier: float (1.0 default)

// Blend Weights
EmotionBlendAlpha: float
TalkingBlendAlpha: float
LookAtBlendAlpha: float
```

---

## 3. Game Mode Blueprint (GM_AIDigitalHuman)

### Creation Steps

```
Content Browser → Right-click → Blueprint Class
Parent Class: GameModeBase
Name: GM_AIDigitalHuman
Location: Content/Blueprints/
```

### Default Classes

```
Default Pawn Class: None (spectator mode)
HUD Class: WBP_StreamingHUD
Player Controller Class: PC_Streaming
Game State Class: Default
```

### Event Graph

```
Event BeginPlay
├── Get Subsystem (AIDigitalHumanSubsystem)
│   └── Store as Subsystem
├── Configure API Endpoints
│   ├── Subsystem → GetConversationManager → SetAPIEndpoint
│   └── Subsystem → GetVoiceManager → SetTTSEndpoint
├── Configure Chat Connection
│   └── Subsystem → GetChatManager → Connect
├── Configure Streaming
│   └── Subsystem → GetStreamingManager → SetConfiguration
├── Spawn AI Character
│   └── Spawn Actor: BP_AIDigitalHuman at PlayerStart
├── Load Default Environment
│   └── Subsystem → GetEnvironmentManager → LoadEnvironmentByType
└── Setup Camera
    └── Subsystem → GetCameraManager → SetTarget(SpawnedCharacter)

Function: ConfigureFromSettings
├── Load Settings from SaveGame or Config
├── Apply API Keys
├── Apply Stream Settings
└── Apply Quality Settings
```

---

## 4. Streaming HUD Widget (WBP_StreamingHUD)

### Creation Steps

```
Content Browser → Right-click → User Interface → Widget Blueprint
Name: WBP_StreamingHUD
Location: Content/Blueprints/UI/
```

### Widget Hierarchy

```
Canvas Panel
├── [Top Bar] Horizontal Box
│   ├── Text: "LIVE" (red, blinking when streaming)
│   ├── Text: Viewer Count
│   ├── Spacer
│   └── Text: Stream Duration
│
├── [Bottom Left] Vertical Box
│   ├── Text: Current Emotion
│   ├── Progress Bar: Emotion Intensity
│   └── Text: Speaking Status
│
├── [Bottom Center] Chat Display
│   ├── Scroll Box (last 10 messages)
│   └── Text Block per message (styled by tier)
│
├── [Bottom Right] Vertical Box
│   ├── Text: FPS Counter
│   ├── Text: Resolution
│   └── Text: Bitrate
│
├── [Center] AI Disclosure (semi-transparent)
│   └── Text: "AI-Generated Digital Human"
│
└── [Hidden Controls - Admin Only]
    ├── Button: Emergency Stop
    ├── Dropdown: Environment Select
    ├── Dropdown: Mood Select
    └── Slider: Lighting Intensity
```

### Event Graph

```
Event Construct
├── Get Subsystem → Bind Events
│   ├── OnChatMessageReceived → AddChatMessage
│   ├── OnEmotionChanged → UpdateEmotionDisplay
│   ├── OnPerformanceWarning → ShowWarning
│   └── OnStreamingStatsUpdated → UpdateStats
└── Start Update Timer (0.5s interval)

Event Tick (Custom, 0.5s)
├── Update FPS Display
├── Update Stream Duration
└── Update Viewer Count (from platform API)

Function: AddChatMessage
Input: FChatMessage
├── Create Chat Entry Widget
├── Style by UserTier
│   ├── VIP: Gold text
│   ├── Premium: Purple text
│   ├── Subscriber: Blue text
│   └── Free: White text
├── Add to Scroll Box
└── Remove oldest if > 10 messages

Function: OnEmergencyStopClicked
├── Subsystem → EmergencyStop
├── Set Button Text: "STOPPED"
└── Change Button Color: Red
```

---

## 5. Player Controller (PC_Streaming)

### Creation Steps

```
Content Browser → Right-click → Blueprint Class
Parent Class: PlayerController
Name: PC_Streaming
Location: Content/Blueprints/
```

### Settings

```
Auto Manage Active Camera Player: false
Show Mouse Cursor: true (for admin UI)
Enable Click Events: true
```

### Input Actions (Enhanced Input)

```
IA_EmergencyStop
├── Trigger: Shift + Ctrl + Escape
└── Action: Call Subsystem → EmergencyStop

IA_CameraSwitch
├── Trigger: 1, 2, 3, 4 keys
└── Action: Call CameraManager → SwitchToPreset(index)

IA_ToggleHUD
├── Trigger: H key
└── Action: Toggle HUD visibility

IA_TestEmotion
├── Trigger: F1-F4 keys
└── Action: Call Subsystem → SetEmotion(mapped emotion)
```

---

## 6. Environment Preset Data Assets

### Creation Steps (for each environment)

```
Content Browser → Right-click → Miscellaneous → Data Asset
Class: EnvironmentPreset
Name: DA_ENV_[EnvironmentName]
Location: Content/Blueprints/Environments/
```

### Example: DA_ENV_LuxuryBedroom

```yaml
Environment Name: "Luxury Bedroom"
Type: Bedroom
Default Mood: Cozy
Description: "Elegant modern bedroom with city skyline views"

Level Asset: /Game/Maps/L_ENV_Bedroom

Lighting:
  Time Of Day: Night
  Key Light Temperature: 3200
  Key Light Intensity: 8.0
  Fill Light Ratio: 0.3
  Rim Light Intensity: 5.0
  Enable Volumetric Fog: false
  Enable Practical Lights: true
  Ambient Color: (R=0.02, G=0.02, B=0.03)

Character Spawn Transform:
  Location: (X=0, Y=-100, Z=60)
  Rotation: (Pitch=0, Yaw=0, Roll=0)

Character Positions:
  - "Bed Center": (0, 0, 85)
  - "Bed Edge": (0, -100, 60)
  - "Vanity": (200, 150, 0)
  - "Window": (0, 300, 0)
  - "Chair": (-180, -100, 45)

Camera Presets:
  - Name: "Wide"
    Location: (0, -400, 150)
    Focal Length: 24
    Aperture: 2.8

  - Name: "Medium"
    Location: (0, -250, 120)
    Focal Length: 50
    Aperture: 2.0

  - Name: "CloseUp"
    Location: (80, -150, 100)
    Focal Length: 85
    Aperture: 1.8

Interactive Props:
  - Name: "Bed"
    Is Surface: true
    Surface Positions: [4 transforms]

  - Name: "VanityChair"
    Is Surface: true
    Interaction Animation: "SitDown"

Ambient Sound: /Game/Audio/A_AMB_Bedroom_Night
Music Playlist:
  - /Game/Audio/Music/A_MUS_Chill_01
  - /Game/Audio/Music/A_MUS_Chill_02
```

---

## 7. Personality Data Asset

### Creation Steps

```
Content Browser → Right-click → Miscellaneous → Data Asset
Class: CharacterPersonality (create if not exists)
Name: DA_Personality_Nova
Location: Content/Blueprints/Character/
```

### Structure

```yaml
Name: "Nova"
Description: "A warm, intelligent, and engaging AI companion"

System Prompt: |
  You are Nova, a friendly and engaging AI digital human.
  You have a warm personality and enjoy meaningful conversations.
  You express emotions naturally and maintain consistent personality.
  Keep responses conversational and natural.

Personality Traits:
  - "Warm and approachable"
  - "Intellectually curious"
  - "Playful sense of humor"
  - "Empathetic listener"
  - "Confident but not arrogant"

Speaking Style:
  - "Uses natural conversational tone"
  - "Occasionally uses light humor"
  - "Asks follow-up questions"
  - "References previous conversations"

Warmth: 0.7
Playfulness: 0.6
Intelligence: 0.8
Confidence: 0.7

Voice Settings:
  Voice ID: "nova-female-1"
  Speaking Rate: 1.0
  Pitch: 1.0
  Enable Breathing: true
  Emotion Modulation: true
```

---

## Blueprint Folder Structure

```
Content/Blueprints/
├── Character/
│   ├── BP_AIDigitalHuman.uasset
│   ├── ABP_AIDigitalHuman.uasset
│   ├── DA_Personality_Nova.uasset
│   └── DA_Personality_Luna.uasset
│
├── Environments/
│   ├── DA_ENV_LuxuryBedroom.uasset
│   ├── DA_ENV_LivingRoom.uasset
│   ├── DA_ENV_SpaBathroom.uasset
│   ├── DA_ENV_RooftopTerrace.uasset
│   ├── DA_ENV_ForestClearing.uasset
│   ├── DA_ENV_BeachSunset.uasset
│   └── ... (12 total)
│
├── UI/
│   ├── WBP_StreamingHUD.uasset
│   ├── WBP_ChatEntry.uasset
│   ├── WBP_AdminPanel.uasset
│   └── WBP_LoadingScreen.uasset
│
├── Camera/
│   └── BP_CinematicCameraActor.uasset
│
├── GM_AIDigitalHuman.uasset
└── PC_Streaming.uasset
```

---

## Quick Setup Checklist

```
□ Create BP_AIDigitalHuman from AIDigitalHumanCharacter
□ Assign MetaHuman meshes to components
□ Create ABP_AIDigitalHuman with proper state machine
□ Assign animation blueprint to character
□ Create GM_AIDigitalHuman game mode
□ Create WBP_StreamingHUD widget
□ Create PC_Streaming player controller
□ Create all Environment Data Assets
□ Create Personality Data Asset(s)
□ Configure World Settings to use GM_AIDigitalHuman
□ Test in editor with PIE (Play In Editor)
```
