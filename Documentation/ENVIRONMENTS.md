# Environment Design Specifications

## Overview

This document details 12 photorealistic environments for the AI Digital Human, optimized for 4K/60FPS streaming with Lumen GI and Nanite geometry.

---

## 1. LUXURY BEDROOM (Primary Scene)

### Description
Elegant modern bedroom with king-size bed, soft ambient lighting, floor-to-ceiling windows with city skyline view.

### Layout (6m x 8m)
```
┌─────────────────────────────────────┐
│ ░░░░░░░░░░ WINDOW (City View) ░░░░░░│
│                                      │
│  ┌─────────────────────┐    ┌────┐  │
│  │                     │    │Vanity│ │
│  │    KING BED         │    │Mirror│ │
│  │   (Velvet/Silk)     │    └────┘  │
│  │                     │            │
│  └─────────────────────┘    ○ Chair │
│                                      │
│  ○ Bench      ║ Curtains ║   ⬡ Lamp │
│               ║ (Sheer)  ║          │
│  ┌────┐                    ┌────┐   │
│  │Armch│                   │Night│  │
│  └────┘                    │stand│  │
└─────────────────────────────────────┘
```

### Key Assets (Megascans/Quixel)
| Asset | Material | Notes |
|-------|----------|-------|
| Bed Frame | Walnut wood, brass accents | Nanite mesh |
| Mattress/Bedding | Silk sheets, velvet throw | Cloth simulation |
| Headboard | Tufted velvet (burgundy/navy) | Subsurface |
| Curtains | Sheer white + blackout | Two-layer system |
| Nightstands | Marble top, wood base | PBR materials |
| Vanity Mirror | Hollywood-style LED frame | Emissive border |
| Area Rug | Persian/Modern geometric | Displacement |
| Accent Chair | Velvet, gold legs | Position #1 |
| Bench | End-of-bed, leather | Position #2 |

### Lighting Setup
```
Key Light:      Rect Light 120x60cm, 3200K (warm), window-side
Fill Light:     Rect Light 60x60cm, 4500K, opposite wall
Rim Light:      Spot, 5500K, behind bed (hair separation)
Practicals:     2x Table lamps (2700K), LED mirror (4000K)
Ambient:        Sky Light + HDRI (night city)
```

### Camera Positions
1. **Wide** - Corner, showing full bed + window (24mm, f/2.8)
2. **Medium** - Foot of bed, character centered (50mm, f/2.0)
3. **Close-up** - Side angle, intimate (85mm, f/1.8)
4. **Vanity** - Mirror reflection shot (35mm, f/2.2)
5. **Window** - Silhouette against city lights (35mm, f/4.0)

### Character Positions
- Lying on bed (center, side, diagonal)
- Sitting on bed edge
- At vanity mirror
- Standing by window
- Seated in armchair

### Mood Presets
| Mood | Key Changes |
|------|-------------|
| Romantic | Candles ON, dim practicals, warm 2700K dominant |
| Cozy | All practicals ON, medium intensity |
| Mysterious | Single rim light, fog 0.03, blue accent |
| Playful | Brighter overall, pink/purple accent RGB |

---

## 2. MODERN LIVING ROOM

### Description
Open-plan luxury apartment with sectional sofa, fireplace, and panoramic windows.

### Layout (8m x 10m)
```
┌────────────────────────────────────────┐
│ ░░░░░░░░ PANORAMIC WINDOWS ░░░░░░░░░░░ │
│                                         │
│   ┌─────────────┐         ┌─────────┐  │
│   │  SECTIONAL  │         │Fireplace│  │
│   │    SOFA     │    ○    │  (Gas)  │  │
│   │             │  Table  │ TV Above│  │
│   └─────────────┘         └─────────┘  │
│                                         │
│  ○ Ottoman    ┌──────┐      ○ Floor   │
│               │ Bar  │        Lamp    │
│  ○ Accent     │ Cart │                │
│    Chair      └──────┘      ⬡ Plants  │
│                                         │
└────────────────────────────────────────┘
```

### Key Assets
- Sectional sofa (modular, bouclé fabric)
- Coffee table (marble + brass)
- Gas fireplace (modern linear)
- Bar cart (gold, glass shelves)
- Floor lamp (arc design)
- Large area rug (neutral tones)
- Decorative plants (fiddle leaf, monstera)
- Art pieces (abstract, large scale)

### Lighting
```
Key:        Large softbox through windows, 5500K daylight
Fill:       Bounce from white ceiling
Accent:     Fireplace (animated emissive), 1800K
Practicals: Arc lamp, under-cabinet LEDs
Evening:    Switch to interior lights, city glow through windows
```

### Character Positions
- Lounging on sectional (multiple poses)
- By fireplace (standing/seated)
- At bar cart (mixing drinks)
- Dancing in open space

---

## 3. SPA BATHROOM

### Description
Luxurious master bathroom with freestanding tub, rain shower, and ambient lighting.

### Layout (5m x 6m)
```
┌────────────────────────────────┐
│  ┌──────────────┐   ░ Window ░ │
│  │    RAIN      │   (frosted)  │
│  │   SHOWER     │              │
│  │  (Glass)     │  ┌────────┐  │
│  └──────────────┘  │ Double │  │
│                    │ Vanity │  │
│    ┌─────────┐     │(Marble)│  │
│    │FREESTAND│     └────────┘  │
│    │  TUB    │                 │
│    │(Copper) │    ○ Stool      │
│    └─────────┘                 │
│                                │
│  ⬡ Plants    ♨ Towel Warmer   │
│              ◇ Candles         │
└────────────────────────────────┘
```

### Key Assets
- Freestanding copper/brass tub
- Rain shower (glass enclosure)
- Double vanity (white marble)
- Large mirror (backlit LED)
- Towel warmer (heated rack)
- Wooden bath tray
- Candles (multiple sizes)
- Orchids, eucalyptus plants
- Fluffy towels, bath robe

### Lighting
```
Key:        Backlit mirror (4000K, dimmable)
Fill:       Recessed ceiling (3000K)
Accent:     Candles (animated flames), under-tub LED strip
Steam:      Volumetric fog (shower area)
Window:     Frosted daylight or night glow
```

### Special Effects
- Steam particles (Niagara)
- Water caustics on surfaces
- Animated candle flames
- Wet surface reflections (SSR high)

### Character Positions
- In bathtub (various poses)
- Shower silhouette
- At vanity mirror
- Wrapped in towel/robe

---

## 4. ROOFTOP TERRACE (Evening)

### Description
Urban rooftop with lounge furniture, string lights, city skyline backdrop.

### Layout (10m x 12m outdoor)
```
┌─────────────────────────────────────────┐
│ ░░░░░░░░░░ CITY SKYLINE ░░░░░░░░░░░░░░░ │
│          (Parallax backdrop)            │
│                                         │
│   ◇─────◇─────◇  String Lights  ◇─────◇ │
│                                         │
│   ┌──────────┐      ┌─────────────┐    │
│   │ DAYBED   │      │   OUTDOOR   │    │
│   │ (Wicker) │      │    SOFA     │    │
│   └──────────┘      └─────────────┘    │
│                                         │
│      ○ Fire Pit       ○ Coffee Table   │
│      (Gas, real)                        │
│                                         │
│  ┌──────┐   ⬡ Potted    ┌──────────┐   │
│  │ Bar  │     Plants    │ Hot Tub  │   │
│  │      │               │ (Option) │   │
│  └──────┘               └──────────┘   │
│                                         │
│ ▓▓▓▓▓▓▓▓▓▓▓ BUILDING EDGE ▓▓▓▓▓▓▓▓▓▓▓▓ │
└─────────────────────────────────────────┘
```

### Key Assets
- Wicker daybed with cushions
- Outdoor sectional sofa
- Fire pit table (gas)
- String lights (warm white)
- Potted plants (olive trees, grasses)
- Outdoor bar with stools
- Optional: Built-in hot tub

### Lighting
```
Key:        Golden hour sun or city ambient
Practicals: String lights (2700K), fire pit (1800K)
Accent:     Uplights on plants, pool/tub underwater LEDs
Sky:        HDRI sunset/night city with stars
```

### Time of Day Variants
1. **Golden Hour** - Warm sun, long shadows
2. **Blue Hour** - Deep blue sky, city lights emerging
3. **Night** - Full city lights, stars, string lights prominent

---

## 5. COZY HOME OFFICE / STREAMING STUDIO

### Description
Aesthetic home office setup with ring light, plants, and warm ambiance.

### Layout (4m x 5m)
```
┌───────────────────────────────┐
│                               │
│  ┌─────────────────────────┐  │
│  │     DESK SETUP          │  │
│  │  ┌───┐ ┌───────┐ ┌───┐  │  │
│  │  │Mon│ │Monitor│ │Mon│  │  │
│  │  └───┘ └───────┘ └───┘  │  │
│  │         ○ Chair         │  │
│  └─────────────────────────┘  │
│                               │
│  ◎ Ring      ⬡ Shelf with   │
│    Light       Plants/Books  │
│                               │
│  ○ Bean    ┌────────────┐    │
│    Bag     │  Accent    │    │
│            │  Wall      │    │
│            │ (LED/Neon) │    │
│            └────────────┘    │
└───────────────────────────────┘
```

### Key Assets
- Modern desk (white/wood)
- Gaming/streaming chair
- Monitor stand with plants
- Ring light (18")
- LED accent panels
- Bookshelf with decor
- Neon sign (custom text)
- Bean bag chair
- Acoustic panels (aesthetic)

### Lighting
```
Key:        Ring light (5500K, dimmable)
Fill:       LED panels (RGB capable)
Accent:     Neon sign, monitor glow, plant grow lights
Background: LED strip behind desk (customizable color)
```

### Special Features
- RGB lighting sync with mood
- "Streaming" indicator light
- Webcam-style camera angle
- Multiple background options

---

## 6. FOREST CLEARING (Nature)

### Description
Magical forest clearing with soft dappled light, wildflowers, and a cozy blanket setup.

### Layout (Outdoor, ~15m diameter clearing)
```
         🌲      🌲
    🌲              🌲
        ░░░░░░░░░░░
   🌲  ░ CLEARING  ░  🌲
       ░  ┌─────┐  ░
       ░  │Blank│  ░
  🌲   ░  │ et  │  ░   🌲
       ░  └─────┘  ░
       ░   ⬡ ⬡ ⬡  ░
   🌲  ░ Flowers  ░  🌲
        ░░░░░░░░░░░
    🌲      ║      🌲
           ║ Path
         🌲
```

### Key Assets (Megascans Forest Pack)
- Procedural trees (oak, birch mix)
- Ground cover (grass, ferns, moss)
- Wildflowers (poppies, daisies)
- Large blanket with pillows
- Picnic basket
- Lanterns (for evening)
- Fallen log (seating)
- Stream nearby (audio)

### Lighting
```
Daylight:   Directional sun through canopy (dappled)
Fill:       Sky light (blue-ish)
God rays:   Volumetric fog, light shafts
Evening:    Warm sunset through trees, lantern practicals
Night:      Moonlight, fireflies (Niagara particles)
```

### Atmosphere Effects
- Dust particles in light shafts
- Fireflies at dusk (particle system)
- Leaves falling (occasional)
- Bird sounds, stream audio

### Character Positions
- Lying on blanket
- Sitting against tree
- Standing in flowers
- Walking on path

---

## 7. BEACH SUNSET

### Description
Private beach cove during golden hour/sunset with cabana and ocean waves.

### Layout
```
   ～～～～～～～～～～～～～～～～～
   ～～～～～ OCEAN ～～～～～～～～～
   ～～～～～～～～～～～～～～～～～
   ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
   ▒▒▒▒▒▒▒▒ SHORELINE ▒▒▒▒▒▒▒▒▒▒
   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
   ░░░  ┌─────────┐      ○      ░░
   ░░░  │ CABANA  │    Lounge   ░░
   ░░░  │ (Sheer) │    Chair    ░░
   ░░░  └─────────┘              ░░
   ░░░       ○ Daybed    🌴      ░░
   ░░░                    🌴     ░░
   ░░░░░░░░ SAND ░░░░░░░░░░░░░░░░░
```

### Key Assets
- Cabana with sheer curtains
- Daybed (weathered wood)
- Beach lounge chairs
- Palm trees
- Beach towels
- Bonfire pit (evening)
- Tiki torches
- Ocean material (Gerstner waves)

### Lighting
```
Sun:        Directional, golden hour angle (15-25°)
Sky:        Gradient sunset HDRI
Bounce:     Sand reflection (warm)
Water:      Reflections, caustics
Evening:    Bonfire, tiki torches, stars emerging
```

### Special Effects
- Ocean waves (material + audio)
- Wet sand reflections
- Wind on fabric (cloth sim)
- Seagulls (distant audio)

---

## 8. PENTHOUSE POOL (Indoor/Outdoor)

### Description
Luxury infinity pool with city views, mood lighting, and lounge area.

### Layout (15m x 20m)
```
┌─────────────────────────────────────────┐
│ ░░░░░░░░░ GLASS WALL / SKYLINE ░░░░░░░░ │
│                                          │
│   ╔═══════════════════════════════╗     │
│   ║                               ║     │
│   ║      INFINITY POOL            ║     │
│   ║       (Underwater LEDs)       ║     │
│   ║                               ║     │
│   ╚═══════════════════════════════╝     │
│                                          │
│   ○ Lounger  ○ Lounger  ○ Lounger       │
│                                          │
│   ┌────────┐         ┌──────────────┐   │
│   │Hot Tub │    ⬡    │   DAYBED     │   │
│   │(Raised)│  Plants │  (Cabana)    │   │
│   └────────┘         └──────────────┘   │
│                                          │
│   ○ Bar Area      ◇ Fire Features       │
└─────────────────────────────────────────┘
```

### Key Assets
- Infinity edge pool
- Raised hot tub/jacuzzi
- Designer loungers
- Poolside daybed with canopy
- Bar with stools
- Fire bowls
- Tropical plants
- Towel station

### Lighting
```
Underwater:  RGB LEDs (color cycling)
Accent:      Fire bowls, pool edge lights
Ambient:     City glow through glass
Practicals:  Bar lighting, canopy strings
```

### Water Effects
- Pool reflections (SSR/RTX)
- Caustics on ceiling
- Ripple animations
- Steam from hot tub (particles)

---

## 9. JAPANESE ONSEN (Hot Spring)

### Description
Traditional Japanese hot spring bath with bamboo, rocks, and zen garden view.

### Layout (Outdoor, 8m x 10m)
```
┌────────────────────────────────────┐
│  ⬡ Bamboo   ░░ ZEN GARDEN ░░  🗻  │
│    Grove         Raked Sand  View │
│                                    │
│   ╔══════════════════════╗        │
│   ║                      ║   ⬡    │
│   ║   NATURAL ONSEN     ║ Maple  │
│   ║    (Rock pool)       ║  Tree  │
│   ║                      ║        │
│   ╚══════════════════════╝        │
│         ⌇ Rocks ⌇                 │
│                                    │
│   ┌──────────┐    ┌───────────┐   │
│   │ Changing │    │  Wooden   │   │
│   │   Area   │    │   Deck    │   │
│   └──────────┘    └───────────┘   │
│           ◇ Lanterns              │
└────────────────────────────────────┘
```

### Key Assets
- Natural rock pool
- Bamboo grove
- Japanese maple tree
- Stone lanterns
- Wooden deck
- Zen garden (raked patterns)
- Distant mountain backdrop
- Wooden buckets, towels
- Shoji screens

### Lighting
```
Day:        Soft overcast, dappled through bamboo
Dusk:       Warm lantern glow, blue hour sky
Night:      Lanterns, moon reflection on water
Steam:      Volumetric rising from hot water
```

---

## 10. YACHT DECK (Mediterranean)

### Description
Luxury yacht deck with sunbeds, champagne, and Mediterranean sea backdrop.

### Layout
```
┌────────────────────────────────────┐
│          ⚓ YACHT BOW ⚓           │
│    ┌────────────────────────┐     │
│    │      SUNBED AREA       │     │
│    │  ○ Lounger ○ Lounger  │     │
│    └────────────────────────┘     │
│                                    │
│  ◇ Champagne    ┌─────────────┐   │
│    On Ice       │   JACUZZI   │   │
│                 │   (Deck)    │   │
│    ○ Seating    └─────────────┘   │
│                                    │
│    ┌───────────────────────┐      │
│    │    COVERED LOUNGE     │      │
│    │      (Shade)          │      │
│    └───────────────────────┘      │
│                                    │
│ ～～～ MEDITERRANEAN SEA ～～～～～ │
└────────────────────────────────────┘
```

### Key Assets
- Yacht deck (teak wood)
- Designer sunbeds
- Built-in jacuzzi
- Bar/lounge area
- Champagne bucket
- Towels, sunglasses
- Yacht railings
- Ocean horizon

### Motion
- Subtle deck sway
- Ocean waves (material)
- Ripples in jacuzzi
- Fabric in wind

---

## 11. WINTER CABIN (Cozy)

### Description
Rustic mountain cabin with fireplace, fur throws, and snow-covered windows.

### Layout (6m x 7m main room)
```
┌────────────────────────────────────┐
│ ░░ SNOW-COVERED WINDOWS ░░░░░░░░░ │
│ ░░ (Mountain View)      ░░░░░░░░░ │
│                                    │
│   ┌────────────────┐    🔥🔥🔥    │
│   │   LARGE BED    │   FIREPLACE  │
│   │ (Fur throws,   │    (Stone)   │
│   │  thick duvet)  │              │
│   └────────────────┘    ○ Armchair│
│                         (Leather) │
│  ⬡ Candles                        │
│                    ┌────────────┐ │
│   ○ Bearskin       │ Hot Cocoa  │ │
│     Rug            │   Table    │ │
│                    └────────────┘ │
│  Wood beams visible on ceiling    │
└────────────────────────────────────┘
```

### Key Assets
- Log cabin interior (exposed beams)
- Stone fireplace (large, animated fire)
- King bed with fur throws
- Bearskin rug
- Leather armchair
- Wooden side tables
- Candles (many)
- Hot cocoa setup
- Snow on window panes

### Lighting
```
Key:        Fireplace (warm, flickering, 1800K)
Fill:       Snow bounce from windows (cool, 7000K)
Accent:     Candles throughout
Ambient:    Very warm overall, high contrast
```

### Atmosphere
- Fire particle effects
- Crackling fire audio
- Distant wind audio
- Breath visible (cold indication)

---

## 12. BOUDOIR / DRESSING ROOM

### Description
Glamorous dressing room with Hollywood mirror, lingerie displays, and soft lighting.

### Layout (5m x 6m)
```
┌────────────────────────────────────┐
│                                     │
│  ┌─────────────────────────────┐   │
│  │    HOLLYWOOD VANITY MIRROR  │   │
│  │    (LED bulb border)        │   │
│  │         ○ Chair             │   │
│  └─────────────────────────────┘   │
│                                     │
│  ┌────────┐          ┌──────────┐  │
│  │Wardrobe│    ⬡     │ Chaise   │  │
│  │(Glass) │  Mirror  │ Lounge   │  │
│  │        │ (Full)   │ (Velvet) │  │
│  └────────┘          └──────────┘  │
│                                     │
│  ◇ Perfumes    ○ Mannequin         │
│    Display       (Dressed)          │
│                                     │
│  ⬡ Chandelier (Crystal, dimmed)    │
└────────────────────────────────────┘
```

### Key Assets
- Hollywood vanity (bulb mirror)
- Glass wardrobe
- Velvet chaise lounge
- Full-length mirror
- Crystal chandelier
- Perfume display
- Jewelry boxes
- Mannequin with outfit
- Silk robes on hooks

### Lighting
```
Key:        Vanity mirror bulbs (4000K, dimmable)
Fill:       Chandelier (3000K, low)
Accent:     Wardrobe interior lights
Ambient:    Pink/warm tint overall
```

---

## Technical Implementation Notes

### Performance Optimization

1. **LOD System**: Each environment uses 3 LOD levels
   - LOD0: Full Nanite detail (foreground)
   - LOD1: Reduced mesh (mid-ground)
   - LOD2: Imposters (background)

2. **Streaming Levels**: Environments load as sublevels
   ```cpp
   UGameplayStatics::LoadStreamLevel(World, LevelName, true, true, LatentInfo);
   ```

3. **Material Instances**: All materials use shared parents for batching

4. **Light Complexity**: Max 4 shadow-casting lights per scene

### Level Streaming Blueprint

```cpp
// In EnvironmentManager
void LoadEnvironment(UEnvironmentPreset* Preset)
{
    // Unload current
    if (CurrentLevelInstance.IsValid())
    {
        UGameplayStatics::UnloadStreamLevel(...);
    }

    // Load new
    UGameplayStatics::LoadStreamLevel(
        GetWorld(),
        Preset->LevelAsset.GetAssetName(),
        true, true, LatentInfo
    );

    // Apply lighting
    ApplyLightingPreset(Preset->Lighting);

    // Position character
    Character->SetActorTransform(Preset->CharacterSpawnTransform);
}
```

### Quick-Switch System

Environments can be switched in <3 seconds using:
1. Pre-loaded LOD2 backgrounds
2. Async level streaming
3. Fade transition (0.5s)
4. Lighting interpolation

---

## Asset Sources (Recommended)

| Source | Use For |
|--------|---------|
| Quixel Megascans | Surfaces, props, nature |
| Kitbash3D | Architecture, furniture |
| TurboSquid | Specific props |
| HDRI Haven | Sky/environment maps |
| Substance Source | Custom materials |

---

## Summary Table

| Environment | Mood | Best Time | Key Feature |
|-------------|------|-----------|-------------|
| Luxury Bedroom | Intimate | Night | Silk bed, city view |
| Living Room | Relaxed | Evening | Fireplace, sectional |
| Spa Bathroom | Sensual | Any | Copper tub, steam |
| Rooftop | Romantic | Sunset | City lights, fire pit |
| Office | Playful | Day | RGB lights, streaming |
| Forest | Magical | Golden hour | Dappled light, flowers |
| Beach | Dreamy | Sunset | Ocean, cabana |
| Penthouse Pool | Luxurious | Night | Infinity edge, LEDs |
| Japanese Onsen | Zen | Dusk | Steam, lanterns |
| Yacht | Glamorous | Day | Mediterranean, champagne |
| Winter Cabin | Cozy | Night | Fireplace, fur |
| Boudoir | Glamorous | Evening | Hollywood mirror |
