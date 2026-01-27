# External API Integration Guide

## Overview

The AI Digital Human system integrates with several external services:
- **LLM API** (OpenAI GPT-4, Claude, etc.) - For conversation
- **TTS API** (ElevenLabs, Azure, etc.) - For voice synthesis
- **Chat Platform API** - For live chat integration
- **Streaming Platform** - For video output

## LLM Integration

### Configuration

```cpp
// In your GameMode or startup logic
UAIDigitalHumanSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UAIDigitalHumanSubsystem>();
UAIConversationManager* ConversationMgr = Subsystem->GetConversationManager();

// Configure OpenAI
ConversationMgr->SetAPIEndpoint(
    TEXT("https://api.openai.com/v1/chat/completions"),
    TEXT("sk-your-api-key-here")
);

// Or configure Claude
ConversationMgr->SetAPIEndpoint(
    TEXT("https://api.anthropic.com/v1/messages"),
    TEXT("your-anthropic-key")
);
```

### Request Format

The system sends requests in OpenAI-compatible format:

```json
{
    "model": "gpt-4",
    "messages": [
        {
            "role": "system",
            "content": "You are Nova, a friendly AI digital human..."
        },
        {
            "role": "user",
            "content": "CONTEXT:\nUser: JohnDoe\nTier: Premium\n..."
        }
    ],
    "max_tokens": 256,
    "temperature": 0.8
}
```

### Response Parsing

The AI response is parsed for:
- **Emotion markers**: `[happy]`, `[excited]`, `[thinking]`
- **Action cues**: `{waves}`, `{laughs}`, `{leans closer}`
- **Camera suggestions**: Inferred from emotion intensity

### Custom Personality

```cpp
FCharacterPersonality Personality;
Personality.Name = TEXT("Nova");
Personality.SystemPrompt = TEXT(
    "You are Nova, a warm and engaging AI companion. "
    "You express emotions naturally, remember past conversations, "
    "and maintain a consistent, friendly personality."
);
Personality.PersonalityTraits = {
    TEXT("Warm and approachable"),
    TEXT("Intellectually curious"),
    TEXT("Playful sense of humor")
};
ConversationMgr->SetPersonality(Personality);
```

## TTS Integration

### Configuration

```cpp
UVoiceSynthesisManager* VoiceMgr = Subsystem->GetVoiceManager();

// Configure ElevenLabs
VoiceMgr->SetTTSEndpoint(
    TEXT("https://api.elevenlabs.io/v1/text-to-speech/voice-id"),
    TEXT("your-elevenlabs-key")
);

// Voice configuration
FVoiceConfiguration VoiceConfig;
VoiceConfig.VoiceID = TEXT("EXAVITQu4vr4xnSDxMaL"); // Example voice ID
VoiceConfig.SpeakingRate = 1.0f;
VoiceConfig.Pitch = 1.0f;
VoiceConfig.bEmotionModulation = true;
VoiceMgr->SetVoiceConfiguration(VoiceConfig);
```

### Emotion-Based Voice Modulation

The system automatically adjusts voice parameters based on emotion:

| Emotion | Pitch | Rate | Volume |
|---------|-------|------|--------|
| Happy | +5% | +5% | Normal |
| Excited | +10% | +15% | +10% |
| Sad | -5% | -10% | Normal |
| Thinking | Normal | -15% | Normal |
| Surprised | +15% | +10% | Normal |

### Lip Sync

Lip sync is generated automatically from audio:
1. Audio is analyzed for amplitude/frequency
2. Phonemes are detected
3. ARKit-compatible blendshapes are generated
4. Applied to MetaHuman face at 60 FPS

## Chat Platform Integration

### WebSocket Connection

```cpp
UChatIntegrationManager* ChatMgr = Subsystem->GetChatManager();

FChatPlatformConfig Config;
Config.PlatformName = TEXT("MyPlatform");
Config.WebSocketURL = TEXT("wss://chat.platform.com/ws");
Config.APIKey = TEXT("your-api-key");
Config.ChannelID = TEXT("channel-123");
Config.bAutoReconnect = true;
Config.MaxReconnectAttempts = 10;

ChatMgr->Connect(Config);
```

### Message Format (Expected)

```json
{
    "type": "chat",
    "username": "User123",
    "user_id": "abc-123",
    "text": "Hello Nova!",
    "tier": "subscriber",
    "tip_amount": 0.0
}
```

### Monetization Events

```json
{
    "type": "tip",
    "username": "Generous_User",
    "user_id": "xyz-789",
    "amount": 50.0,
    "message": "Love the stream!"
}
```

### Custom Commands

```cpp
// Register custom command
FChatCommand CustomCmd;
CustomCmd.Trigger = TEXT("!spin");
CustomCmd.Description = TEXT("Do a spin animation");
CustomCmd.AnimationName = TEXT("Spin360");
CustomCmd.RequiredTier = EUserTier::Subscriber;
CustomCmd.CooldownSeconds = 30.0f;
ChatMgr->RegisterCommand(CustomCmd);

// Listen for command execution
ChatMgr->OnCommandExecuted.AddDynamic(this, &AMyActor::HandleCommand);
```

## Streaming Output

### RTMP Configuration

```cpp
UStreamingManager* StreamMgr = Subsystem->GetStreamingManager();

FStreamingConfiguration StreamConfig;
StreamConfig.Quality = EStreamingQuality::Quality_4K;
StreamConfig.TargetFPS = 60;
StreamConfig.VideoBitrate = 20000; // kbps
StreamConfig.AudioBitrate = 320;   // kbps
StreamConfig.RTMPUrl = TEXT("rtmp://live.platform.com/app");
StreamConfig.StreamKey = TEXT("your-stream-key");
StreamConfig.bUseHardwareEncoder = true;
StreamConfig.bLowLatencyMode = true;

StreamMgr->SetConfiguration(StreamConfig);
StreamMgr->StartStreaming();
```

### Scene Capture Setup

```cpp
// In your level Blueprint or C++
USceneCaptureComponent2D* Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("StreamCapture"));
Capture->bCaptureEveryFrame = true;
Capture->bCaptureOnMovement = false;
Capture->TextureTarget = CreateRenderTarget(3840, 2160);
Capture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

StreamMgr->SetCaptureSource(Capture);
```

## Error Handling

### LLM Fallback

```cpp
// If LLM fails, fallback response is used
ConversationMgr->OnError.AddDynamic(this, &AMyActor::HandleLLMError);

void AMyActor::HandleLLMError(const FString& Error)
{
    // Log error
    UE_LOG(LogAIDigitalHuman, Error, TEXT("LLM Error: %s"), *Error);

    // Use canned response
    Subsystem->Speak(TEXT("I'm having trouble thinking right now, give me a moment!"),
        EDigitalHumanEmotion::Confused);
}
```

### Connection Recovery

```cpp
// Chat reconnection is automatic, but you can monitor
ChatMgr->OnDisconnected.AddDynamic(this, &AMyActor::HandleDisconnect);
ChatMgr->OnConnected.AddDynamic(this, &AMyActor::HandleReconnect);
```

## Rate Limiting

### Default Limits

| API | Rate Limit | Handling |
|-----|------------|----------|
| LLM | Queue-based | Priority queue |
| TTS | Sequential | Queue with overlap |
| Chat | 10 msg/sec | Drop lowest priority |

### Custom Rate Limits

```cpp
// Adjust chat processing rate
ChatMgr->MaxMessagesPerSecond = 15;

// Conversation queue settings
ConversationMgr->MaxQueueSize = 100;
ConversationMgr->QueueTimeoutSeconds = 30.0f;
```

## Security Considerations

1. **Never expose API keys in shipping builds**
2. **Use environment variables or secure storage**
3. **Implement server-side API proxies for production**
4. **Validate all incoming WebSocket messages**
5. **Use HTTPS/WSS for all connections**

```cpp
// Example: Load keys from environment
FString APIKey = FPlatformMisc::GetEnvironmentVariable(TEXT("LLM_API_KEY"));
if (APIKey.IsEmpty())
{
    UE_LOG(LogAIDigitalHuman, Error, TEXT("API key not configured!"));
}
```
