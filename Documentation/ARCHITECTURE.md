# AI Digital Human - System Architecture

## Overview

This document describes the complete architecture for a real-time AI Digital Human system capable of:
- 4K (3840x2160) rendering at 60 FPS
- Natural conversation with LLM integration
- Live chat interaction with monetization support
- Seamless streaming to external platforms

## System Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           EXTERNAL SERVICES                                  │
├─────────────────────────────────────────────────────────────────────────────┤
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │   LLM API    │  │   TTS API    │  │ Chat Platform│  │  RTMP Server │     │
│  │  (GPT-4/etc) │  │(ElevenLabs)  │  │  (WebSocket) │  │  (Streaming) │     │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘  └──────▲───────┘     │
│         │                 │                 │                 │             │
└─────────┼─────────────────┼─────────────────┼─────────────────┼─────────────┘
          │                 │                 │                 │
          ▼                 ▼                 ▼                 │
┌─────────────────────────────────────────────────────────────────────────────┐
│                         UNREAL ENGINE 5 APPLICATION                          │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  ┌────────────────────────────────────────────────────────────────────┐     │
│  │                    AIDigitalHumanSubsystem                          │     │
│  │  (Central coordinator for all managers - GameInstance Subsystem)    │     │
│  └────────────────────────────────────────────────────────────────────┘     │
│         │                                                                    │
│         ├──────────────────┬──────────────────┬──────────────────┐          │
│         ▼                  ▼                  ▼                  ▼          │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │ Conversation │  │    Voice     │  │     Chat     │  │ Monetization │     │
│  │   Manager    │  │   Manager    │  │   Manager    │  │   Manager    │     │
│  │              │  │              │  │              │  │              │     │
│  │ • LLM API    │  │ • TTS API    │  │ • WebSocket  │  │ • Tip Events │     │
│  │ • Memory     │  │ • Lip Sync   │  │ • Commands   │  │ • Reactions  │     │
│  │ • Personality│  │ • Emotion    │  │ • Priority Q │  │ • User Tiers │     │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘     │
│         │                 │                 │                 │             │
│         └────────────────►├◄────────────────┴─────────────────┘             │
│                           ▼                                                  │
│  ┌────────────────────────────────────────────────────────────────────┐     │
│  │                    AIDigitalHumanCharacter                          │     │
│  │                                                                     │     │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐             │     │
│  │  │   Face Mesh  │  │  Body Mesh   │  │  Groom (Hair)│             │     │
│  │  │  (MetaHuman) │  │  (MetaHuman) │  │  Strand-based│             │     │
│  │  └──────────────┘  └──────────────┘  └──────────────┘             │     │
│  │                                                                     │     │
│  │  ┌──────────────────────────────────────────────────┐             │     │
│  │  │           AIDigitalHumanAnimInstance             │             │     │
│  │  │  • Emotion Blending  • Breathing  • Look-At     │             │     │
│  │  │  • Gesture Layers    • Idle Variations          │             │     │
│  │  └──────────────────────────────────────────────────┘             │     │
│  └────────────────────────────────────────────────────────────────────┘     │
│         │                                                                    │
│         ▼                                                                    │
│  ┌────────────────────────────────────────────────────────────────────┐     │
│  │                         RENDERING PIPELINE                          │     │
│  │                                                                     │     │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐             │     │
│  │  │    Lumen     │  │    Nanite    │  │  DLSS / TSR  │             │     │
│  │  │      GI      │  │   Geometry   │  │  Upscaling   │             │     │
│  │  └──────────────┘  └──────────────┘  └──────────────┘             │     │
│  │                                                                     │     │
│  │  ┌──────────────────────────────────────────────────┐             │     │
│  │  │           CinematicCameraManager                 │             │     │
│  │  │  • Shot Types  • Auto Switching  • DOF Control  │             │     │
│  │  └──────────────────────────────────────────────────┘             │     │
│  └────────────────────────────────────────────────────────────────────┘     │
│         │                                                                    │
│         ▼                                                                    │
│  ┌────────────────────────────────────────────────────────────────────┐     │
│  │                       StreamingManager                              │     │
│  │  • Scene Capture  • Hardware Encoding  • RTMP Output               │     │
│  │  • Adaptive Bitrate  • Low Latency Mode  • Audio Sync              │     │
│  └────────────────────────────────────────────────────────────────────┘     │
│                                                                              │
│  ┌────────────────────────────────────────────────────────────────────┐     │
│  │                        SafetyManager                                │     │
│  │  • Emergency Stop  • Content Filter  • Logging  • AI Disclosure    │     │
│  └────────────────────────────────────────────────────────────────────┘     │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

## Module Structure

### Source/AIDigitalHuman/
```
├── AIDigitalHuman.Build.cs        # Module build configuration
├── Public/
│   ├── AIDigitalHumanModule.h     # Module interface
│   ├── Core/
│   │   ├── AIDigitalHumanTypes.h  # Shared types, enums, structs
│   │   └── AIDigitalHumanSubsystem.h
│   ├── AI/
│   │   └── AIConversationManager.h
│   ├── Voice/
│   │   └── VoiceSynthesisManager.h
│   ├── Chat/
│   │   └── ChatIntegrationManager.h
│   ├── Monetization/
│   │   └── MonetizationManager.h
│   ├── Camera/
│   │   └── CinematicCameraManager.h
│   ├── Streaming/
│   │   └── StreamingManager.h
│   ├── Safety/
│   │   └── SafetyManager.h
│   ├── Character/
│   │   └── AIDigitalHumanCharacter.h
│   └── Animation/
│       └── AIDigitalHumanAnimInstance.h
└── Private/
    └── [Implementation files]
```

## Blueprint vs C++ Split

| Component | C++ | Blueprint | Rationale |
|-----------|-----|-----------|-----------|
| Subsystem | ✓ | - | Core systems need performance |
| Conversation Manager | ✓ | - | HTTP/async operations |
| Voice Manager | ✓ | - | Audio processing |
| Chat Manager | ✓ | - | WebSocket handling |
| Monetization Manager | ✓ | Events exposed | Business logic in C++, reactions in BP |
| Camera Manager | ✓ | Shot configs | Core logic in C++, tuning in BP |
| Streaming Manager | ✓ | - | Low-level media handling |
| Safety Manager | ✓ | - | Security-critical code |
| Character | ✓ | Customization | Base in C++, MetaHuman setup in BP |
| Animation Instance | ✓ | State machine | Core in C++, AnimBP for blending |
| UI/HUD | - | ✓ | Designer-friendly |
| Level/Environment | - | ✓ | Art content |

## Data Flow

### Chat Message Processing
```
1. WebSocket receives message
2. ChatIntegrationManager parses JSON
3. Priority assigned based on user tier
4. Message added to priority queue
5. SafetyManager validates content
6. ConversationManager receives message
7. LLM API called with context
8. Response parsed for emotion/animation
9. VoiceSynthesisManager generates speech
10. Character speaks with lip sync
```

### Monetization Event Flow
```
1. External platform sends event (tip/subscription)
2. ChatIntegrationManager receives via WebSocket
3. MonetizationManager processes event
4. User tier updated if applicable
5. Appropriate reaction triggered:
   - Animation plays
   - Emotion changes
   - Voice line spoken
   - Camera cuts to close-up
```

## Key Design Decisions

### 1. GameInstance Subsystem
Using `UGameInstanceSubsystem` for the main coordinator ensures:
- Persistence across level changes
- Automatic lifecycle management
- Easy access from anywhere via `GetGameInstance()`

### 2. Event-Driven Architecture
All managers communicate via delegates:
- Loose coupling between systems
- Easy Blueprint binding
- Testable components

### 3. Priority Queue for Chat
Messages processed in order:
1. Critical (whale tips)
2. Urgent (VIP messages)
3. High (premium/paid)
4. Normal (subscribers)
5. Low (free users)

### 4. Verification Flags
Character includes built-in verification:
```cpp
FCharacterVerificationFlags
├── bIsFullySynthetic = true
├── bAgeVerified18Plus = true
├── bNoFaceCloning = true
└── bNoRealLikeness = true
```

## Thread Safety

| Manager | Threading Model |
|---------|-----------------|
| Conversation | Async HTTP, results on game thread |
| Voice | Async TTS, playback on game thread |
| Chat | WebSocket on background thread, events on game thread |
| Streaming | Encoding on background thread |
| Others | Game thread only |

## Error Handling

All external API calls include:
- Retry logic with exponential backoff
- Fallback responses for offline operation
- Error events for monitoring
- Graceful degradation

## Extensibility Points

1. **Custom LLM**: Implement `SetAPIEndpoint()` for any OpenAI-compatible API
2. **Custom TTS**: Implement `SetTTSEndpoint()` for any TTS service
3. **Custom Platforms**: Extend `ChatIntegrationManager` for new platforms
4. **Custom Reactions**: Add `FTipReaction` configurations
5. **Custom Animations**: Add entries to `AnimationMontages` map
