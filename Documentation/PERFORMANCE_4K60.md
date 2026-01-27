# 4K / 60 FPS Performance Strategy

## Target Specifications

| Metric | Target | Fallback |
|--------|--------|----------|
| Resolution | 3840×2160 (4K) | 2560×1440 (1440p) |
| Frame Rate | 60 FPS stable | 60 FPS with drops allowed |
| Frame Time | 16.67ms | <20ms acceptable |
| GPU Memory | <10GB | <8GB |
| System RAM | <16GB | <12GB |

## Recommended Hardware

### Minimum (1440p/60)
- GPU: NVIDIA RTX 3070 / AMD RX 6800
- CPU: Intel i7-10700 / AMD Ryzen 7 5800X
- RAM: 32GB DDR4
- VRAM: 8GB
- Storage: NVMe SSD

### Recommended (4K/60)
- GPU: NVIDIA RTX 4080 / AMD RX 7900 XT
- CPU: Intel i9-13900K / AMD Ryzen 9 7950X
- RAM: 64GB DDR5
- VRAM: 16GB
- Storage: NVMe Gen4 SSD

### Optimal (4K/60 with headroom)
- GPU: NVIDIA RTX 4090
- CPU: Intel i9-14900K / AMD Ryzen 9 7950X3D
- RAM: 128GB DDR5
- VRAM: 24GB
- Storage: NVMe Gen4 SSD RAID

## Rendering Strategy

### Lumen Configuration
```ini
; Enable hardware ray tracing for better quality
r.Lumen.HardwareRayTracing=1
r.Lumen.HardwareRayTracing.LightingMode=2

; Trace quality settings
r.Lumen.TraceMeshSDFs=1
r.Lumen.ScreenProbeGather.RadianceCache.NumProbes=96

; For 4K, reduce some settings slightly
r.Lumen.Reflections.MaxRayIntensity=20
r.Lumen.ScreenProbeGather.ScreenSpaceBentNormal=0
```

### Nanite Configuration
```ini
r.Nanite=1
r.Nanite.MaxPixelsPerEdge=1

; For streaming, slightly more aggressive LOD
r.Nanite.ViewMipBias=0.5
```

### Upscaling Strategy

#### DLSS (NVIDIA GPUs)
```ini
; Quality mode for 4K output
r.NGX.DLSS.Quality=1
r.NGX.DLSS.Enable=1

; Render at 1440p, upscale to 4K
r.ScreenPercentage=66.67
```

#### TSR (AMD/Intel GPUs)
```ini
r.TemporalAA.Upsampling=1
r.TSR.History.ScreenPercentage=100

; Render at 1440p, upscale to 4K
r.ScreenPercentage=66.67
```

### Quality vs Performance Trade-offs

| Feature | 4K Quality | 4K Performance | 1440p Fallback |
|---------|------------|----------------|----------------|
| Lumen GI | HW Ray Traced | SW Tracing | SW Tracing Low |
| Reflections | Lumen RT | Lumen SW | SSR Only |
| Shadows | VSM High | VSM Medium | CSM |
| Hair | Full Strands | Strands Optimized | Cards |
| DLSS/TSR | Quality | Balanced | Performance |
| Screen % | 67% (upscaled) | 50% (upscaled) | 100% (native) |

## MetaHuman Optimization

### Face Rendering
```
- LOD0 for streaming (highest quality)
- Subsurface scattering: Multi-layer enabled
- Eye caustics: Full quality
- Micro-normal details: Enabled
```

### Hair (Groom)
```ini
; Strand count optimization
r.HairStrands.Strands.MaxDensity=1.0

; Physics simulation
r.HairStrands.Strands.PhysicsSimulation=1
r.HairStrands.Strands.PhysicsIterations=8

; Rendering
r.HairStrands.Strands.RenderOIT=1
r.HairStrands.Strands.AOMinDist=0.5
```

### Skin Material
```
- SSS Profile: MetaHuman SSS
- Detail Normal: 2048×2048
- Roughness Map: Dynamic
- Pore detail: Micro-normal enabled
```

## Frame Budget Allocation

### Target: 16.67ms per frame

| Stage | Budget | Notes |
|-------|--------|-------|
| Game Thread | 3ms | Character logic, AI response |
| Animation | 2ms | Blend, IK, facial |
| Visibility | 1ms | Culling, LOD |
| Base Pass | 4ms | Materials, lighting |
| Lumen | 3ms | GI, reflections |
| Post Process | 2ms | DOF, bloom, color |
| Hair | 1ms | Strand rendering |
| Misc/Buffer | 0.67ms | Headroom |

## Streaming Optimization

### Video Encoding
```
Encoder: NVENC (preferred) / AMF
Preset: P4 (balanced quality/speed)
Rate Control: CBR for streaming
Bitrate: 20-35 Mbps for 4K/60
B-frames: 2 (for quality)
Lookahead: 16 frames
```

### Audio Encoding
```
Codec: AAC
Bitrate: 320 kbps
Sample Rate: 48kHz
Channels: Stereo
```

### Latency Targets
```
Total glass-to-glass: <100ms
- Render: ~17ms
- Encode: ~8ms
- Network: ~50ms
- Decode: ~17ms
- Display: ~8ms
```

## Adaptive Quality System

### Automatic Quality Scaling
```cpp
// Implemented in StreamingManager
void HandleAdaptiveBitrate()
{
    if (Stats.FrameDropPercentage > 5.0f)
    {
        // Reduce quality one step
        switch (Configuration.Quality)
        {
        case Quality_4K: AdjustQuality(Quality_1440p); break;
        case Quality_1440p: AdjustQuality(Quality_1080p); break;
        case Quality_1080p: AdjustQuality(Quality_720p); break;
        }
    }
    else if (Stats.FrameDropPercentage < 1.0f && AverageFrameTime < TargetFrameTime * 0.8f)
    {
        // Performance headroom, try upgrading
        switch (Configuration.Quality)
        {
        case Quality_720p: AdjustQuality(Quality_1080p); break;
        case Quality_1080p: AdjustQuality(Quality_1440p); break;
        case Quality_1440p: AdjustQuality(Quality_4K); break;
        }
    }
}
```

### Performance Monitoring
```cpp
// Console variables for monitoring
stat fps
stat unit
stat gpu
stat rhi
stat streaming
profilegpu
```

## Memory Management

### Texture Streaming
```ini
r.Streaming.PoolSize=4096
r.Streaming.MipBias=-1
r.Streaming.MaxTempMemoryAllowed=256
```

### Asset Loading
- Use async loading for non-critical assets
- Preload all character assets before streaming
- Use memory pools for frequently allocated objects

## Best Practices

1. **Profile Regularly**: Use Unreal Insights for detailed profiling
2. **Monitor GPU Time**: Keep GPU time under 14ms for safety margin
3. **Avoid Hitches**: Preload all assets, avoid runtime allocations
4. **Test on Target Hardware**: Profile on minimum spec hardware
5. **Use Hardware Encoding**: NVENC/AMF for minimal CPU impact
6. **Limit Draw Calls**: Use instancing, merge static meshes
7. **Optimize Blueprints**: Move critical logic to C++
8. **Batch State Changes**: Minimize material switches
9. **Use LODs**: Even for streaming, LODs help with GPU budget
10. **Monitor Memory**: Watch for leaks during long sessions

## Fallback Cascade

If performance drops below target:

1. **First**: Reduce screen percentage (67% → 50%)
2. **Second**: Switch from Lumen RT to Software
3. **Third**: Reduce hair strand density
4. **Fourth**: Disable SSS subsurface detail
5. **Fifth**: Switch to 1440p output
6. **Sixth**: Switch to 1080p output
7. **Last Resort**: Reduce to 30 FPS
