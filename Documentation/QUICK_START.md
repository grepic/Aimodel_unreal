# Quick Start Guide

## Prerequisites

- Unreal Engine 5.4+
- Visual Studio 2022 or Rider
- NVIDIA GPU with RTX (recommended) or AMD RX 6000+
- MetaHuman plugin enabled
- 32GB+ RAM

## Project Setup

### 1. Generate Project Files

```bash
# Windows
cd /path/to/AIDigitalHuman
"C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\GenerateProjectFiles.bat" AIDigitalHuman.uproject

# Or use UnrealBuildTool
UnrealBuildTool -projectfiles -project="AIDigitalHuman.uproject" -game -engine
```

### 2. Build the Project

```bash
# Development build
UnrealBuildTool AIDigitalHuman Development Win64 -project="AIDigitalHuman.uproject"

# Or open AIDigitalHuman.sln in Visual Studio and build
```

### 3. Open in Editor

Double-click `AIDigitalHuman.uproject` to open in Unreal Editor.

## Creating Your Digital Human

### Step 1: Import MetaHuman

1. Open Quixel Bridge (Window → Quixel Bridge)
2. Create or select a MetaHuman
3. Export to project

### Step 2: Create Character Blueprint

1. Create Blueprint: `Content/Blueprints/Character/BP_AIDigitalHuman`
2. Parent Class: `AIDigitalHumanCharacter`
3. Assign MetaHuman skeletal meshes to components:
   - Body → Skeletal Mesh Component
   - Face → Face Mesh Component
   - Hair → Hair Groom Component

### Step 3: Setup Animation Blueprint

1. Create AnimBP: `Content/Blueprints/Character/ABP_AIDigitalHuman`
2. Parent Class: `AIDigitalHumanAnimInstance`
3. Assign to character's Skeletal Mesh

### Step 4: Configure Streaming Stage

1. Create new level: `Content/Maps/StreamingStage`
2. Add lighting (Lumen-compatible):
   - Rect Lights for soft fill
   - Directional Light for key
   - Sky Light for ambient
3. Add your character
4. Add Cine Camera for streaming output

## Configuring the System

### API Configuration (GameMode or Level Blueprint)

```cpp
// Get the subsystem
UAIDigitalHumanSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UAIDigitalHumanSubsystem>();

// Configure LLM
Subsystem->GetConversationManager()->SetAPIEndpoint(
    "https://api.openai.com/v1/chat/completions",
    "your-api-key"
);

// Configure TTS
Subsystem->GetVoiceManager()->SetTTSEndpoint(
    "https://api.elevenlabs.io/v1/text-to-speech/voice-id",
    "your-api-key"
);

// Configure Chat
FChatPlatformConfig ChatConfig;
ChatConfig.WebSocketURL = "wss://your-chat-server.com/ws";
ChatConfig.APIKey = "your-key";
Subsystem->GetChatManager()->Connect(ChatConfig);

// Configure Streaming
FStreamingConfiguration StreamConfig;
StreamConfig.RTMPUrl = "rtmp://live.platform.com/app";
StreamConfig.StreamKey = "your-stream-key";
StreamConfig.Quality = EStreamingQuality::Quality_4K;
Subsystem->GetStreamingManager()->SetConfiguration(StreamConfig);
```

### Blueprint Configuration

1. Open your GameMode Blueprint
2. On Begin Play:
   - Get AI Digital Human Subsystem
   - Call configuration functions
   - Start streaming when ready

## Testing Without APIs

### Simulate Chat Messages

```cpp
// In editor or via console
UChatIntegrationManager* ChatMgr = Subsystem->GetChatManager();
ChatMgr->SimulateMessage("TestUser", "Hello Nova!", EUserTier::Premium);
```

### Simulate Monetization

```cpp
UMonetizationManager* MonMgr = Subsystem->GetMonetizationManager();
MonMgr->OnTip(50.0f, "GenerousUser", "Great stream!");
```

### Test Emotions

```cpp
Subsystem->SetEmotion(EDigitalHumanEmotion::Happy, 0.8f);
```

### Test Speech (without TTS API)

The system generates placeholder lip sync when no TTS endpoint is configured.

## Key Controls (Default)

| Key | Action |
|-----|--------|
| Shift+Ctrl+Esc | Emergency Stop |
| 1 | Camera: Close Up |
| 2 | Camera: Medium |
| 3 | Camera: Wide |
| 4 | Camera: Auto Mode |
| F1-F4 | Test Emotions |

## Monitoring

### Console Commands

```
stat fps          # Frame rate
stat unit         # Frame time breakdown
stat gpu          # GPU timing
profilegpu        # Detailed GPU profile
showflag.lumen 1  # Toggle Lumen visibility
```

### Blueprint Debugging

1. Open AI Digital Human Subsystem in debugger
2. Watch delegates fire in real-time
3. Monitor queue sizes and processing states

## Common Issues

### Low Frame Rate

1. Check GPU temperature and throttling
2. Enable DLSS/TSR
3. Reduce screen percentage
4. Switch to 1440p output

### Lip Sync Not Working

1. Verify TTS endpoint is configured
2. Check audio component is playing
3. Verify face mesh has morph targets

### Chat Not Connecting

1. Verify WebSocket URL is correct
2. Check API key validity
3. Look for connection errors in output log

### Character Not Animating

1. Verify AnimBP is assigned
2. Check skeletal mesh is valid
3. Verify character is spawned in level

## Next Steps

1. Customize personality in `FCharacterPersonality`
2. Add custom animations to `AnimationMontages` map
3. Configure tip reactions in `MonetizationManager`
4. Create custom chat commands
5. Setup your streaming environment
6. Configure external moderation integration

## Support

For issues, check:
1. Output Log (Window → Developer Tools → Output Log)
2. Message Log for Blueprint errors
3. Insights for performance profiling
