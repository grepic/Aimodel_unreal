# AI Digital Human - Unreal Engine 5

A production-ready framework for creating a fully synthetic AI digital human capable of real-time 4K/60FPS streaming with natural conversation, live chat interaction, and monetization support.

## Features

- **Hyper-Realistic Rendering** - MetaHuman-based character with Lumen GI, Nanite, and DLSS/TSR
- **Natural Conversation** - LLM-powered dialogue with personality consistency and long-term memory
- **Neural Voice Synthesis** - Real-time TTS with emotion-aware prosody and lip sync
- **Live Chat Integration** - WebSocket-based chat with priority queue (paid > subscriber > free)
- **Monetization Reactions** - Automatic reactions to tips, subscriptions, and paid messages
- **Cinematic Camera** - Dynamic shot selection with smooth transitions and DOF
- **4K/60FPS Streaming** - Hardware-accelerated encoding with RTMP output
- **Safety Controls** - Emergency stop, content filtering, and comprehensive logging

## Requirements

- Unreal Engine 5.4+
- Visual Studio 2022 / Rider
- Windows 10/11 (64-bit)
- NVIDIA RTX 3070+ or AMD RX 6800+ (recommended)
- 32GB RAM minimum

## Quick Start

```bash
# Clone the repository
git clone <repo-url>
cd Aimodel_unreal

# Generate project files (Windows)
"C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\GenerateProjectFiles.bat" AIDigitalHuman.uproject

# Open in Unreal Editor
start AIDigitalHuman.uproject
```

## Project Structure

```
AIDigitalHuman/
├── Source/AIDigitalHuman/
│   ├── Public/
│   │   ├── Core/           # Types, Subsystem
│   │   ├── AI/             # LLM Conversation
│   │   ├── Voice/          # TTS & Lip Sync
│   │   ├── Chat/           # Live Chat Integration
│   │   ├── Monetization/   # Tip/Sub Handling
│   │   ├── Camera/         # Cinematic Camera
│   │   ├── Streaming/      # RTMP Output
│   │   ├── Safety/         # Moderation & Logging
│   │   ├── Character/      # MetaHuman Character
│   │   └── Animation/      # Animation Instance
│   └── Private/            # Implementation
├── Config/                 # Engine/Game settings
├── Content/               # Assets (create in editor)
└── Documentation/         # Architecture docs
```

## Core Systems

### AIDigitalHumanSubsystem
Central coordinator accessible via `GetGameInstance()->GetSubsystem<UAIDigitalHumanSubsystem>()`

```cpp
// Quick access methods
Subsystem->Speak("Hello!", EDigitalHumanEmotion::Happy);
Subsystem->SetEmotion(EDigitalHumanEmotion::Excited, 0.8f);
Subsystem->TriggerAnimation("Wave");
Subsystem->EmergencyStop();
```

### Managers
- **AIConversationManager** - LLM integration, memory, personality
- **VoiceSynthesisManager** - TTS, lip sync, emotion modulation
- **ChatIntegrationManager** - WebSocket, commands, priority queue
- **MonetizationManager** - Tips, subscriptions, reactions
- **CinematicCameraManager** - Shots, transitions, auto-switching
- **StreamingManager** - Capture, encode, RTMP output
- **SafetyManager** - Filters, logging, emergency controls

## Configuration

### API Endpoints
```cpp
// LLM (OpenAI-compatible)
ConversationManager->SetAPIEndpoint("https://api.openai.com/v1/chat/completions", "api-key");

// TTS (ElevenLabs-compatible)
VoiceManager->SetTTSEndpoint("https://api.elevenlabs.io/v1/text-to-speech/voice", "api-key");

// Chat (WebSocket)
ChatManager->Connect({
    .WebSocketURL = "wss://chat.platform.com/ws",
    .APIKey = "api-key"
});
```

### Streaming
```cpp
StreamingManager->SetConfiguration({
    .Quality = EStreamingQuality::Quality_4K,
    .TargetFPS = 60,
    .RTMPUrl = "rtmp://live.platform.com/app",
    .StreamKey = "your-key"
});
```

## Character Verification

All characters include built-in verification flags:
```cpp
FCharacterVerificationFlags:
  bIsFullySynthetic = true      // No real person likeness
  bAgeVerified18Plus = true     // Adult character
  bNoFaceCloning = true         // No face cloning used
  bNoRealLikeness = true        // Procedurally generated
```

## Performance Targets

| Resolution | Target FPS | DLSS/TSR | GPU Requirement |
|------------|------------|----------|-----------------|
| 4K (3840×2160) | 60 | Quality | RTX 4080+ |
| 1440p (2560×1440) | 60 | Balanced | RTX 3070+ |
| 1080p (1920×1080) | 60 | Off | RTX 3060 |

## Documentation

- [Architecture Overview](Documentation/ARCHITECTURE.md)
- [4K/60FPS Performance Guide](Documentation/PERFORMANCE_4K60.md)
- [API Integration](Documentation/API_INTEGRATION.md)
- [Quick Start Guide](Documentation/QUICK_START.md)

## Key Controls

| Input | Action |
|-------|--------|
| Shift+Ctrl+Esc | Emergency Stop |
| 1-4 | Camera Shots |
| F1-F4 | Test Emotions |

## Safety Features

- **Emergency Stop** - Immediately halts all activity
- **Content Filtering** - Blacklist, patterns, caps detection
- **Rate Limiting** - Per-user message limits
- **Action Logging** - Complete audit trail
- **AI Disclosure** - Configurable disclosure text

## License

Copyright 2024 AI Digital Human Project. All Rights Reserved.

## Disclaimer

This system creates fully synthetic digital humans that do not represent any real person. All characters are procedurally generated with no face cloning or real likeness used. The system includes mandatory AI disclosure and is designed for adult platforms with appropriate content controls.
