# Animation Assets - Complete Specification

## Overview

This document details all required animations for the AI Digital Human, with frame counts, keyframe descriptions, and creation guidelines.

---

## 🎬 ANIMATION CATEGORIES

| Category | Count | Priority |
|----------|-------|----------|
| Idle Variations | 5 | Critical |
| Talking/Gestures | 8 | Critical |
| Reactions (Tips/Events) | 12 | Critical |
| Emotions | 10 | High |
| Poses | 8 | High |
| Transitions | 6 | Medium |
| Special Actions | 10 | Medium |
| Dance | 4 | Low |

**Total: ~63 animations**

---

## 1️⃣ IDLE ANIMATIONS (Looping)

### AM_Idle_Neutral_01
```
Duration: 8 seconds (loop)
FPS: 30
Frame Count: 240

Description:
- Subtle weight shift every 2-3 seconds
- Occasional blink (handled by code)
- Micro-movements in fingers
- Slight head tilt variation
- Natural breathing (chest rise/fall)

Keyframes:
0f: Neutral pose
60f: Slight weight to left leg
120f: Return to center
180f: Slight weight to right leg
240f: Return to neutral (loop point)

Body Parts Focus:
- Spine: 2-3° sway
- Hips: 1-2cm shift
- Shoulders: Subtle rise/fall with breath
- Hands: Relaxed, micro-movements
```

### AM_Idle_Neutral_02
```
Duration: 10 seconds (loop)
Variation: Arms position different, more hip sway
```

### AM_Idle_Confident
```
Duration: 8 seconds (loop)
Variation: Hands on hips, confident stance
```

### AM_Idle_Shy
```
Duration: 8 seconds (loop)
Variation: One arm holding other elbow, weight on one leg
```

### AM_Idle_Playful
```
Duration: 8 seconds (loop)
Variation: Slight bounce, more animated, ready to engage
```

---

## 2️⃣ TALKING ANIMATIONS

### AM_Talk_Idle
```
Duration: 6 seconds (loop)
Description: Subtle body movement while speaking
- Small hand gestures
- Head nods
- Weight shifts
- Shoulder shrugs

Use: Background animation during speech
```

### AM_Talk_Gesture_Point
```
Duration: 2 seconds
Description: Point at camera/viewer
Keyframes:
0f: Neutral
15f: Raise arm, extend finger
30f: Hold point
45f: Return
60f: Neutral
```

### AM_Talk_Gesture_Open
```
Duration: 2 seconds
Description: Open palm gesture (welcoming)
```

### AM_Talk_Gesture_Think
```
Duration: 3 seconds
Description: Finger on chin, looking up/aside
```

### AM_Talk_Gesture_Explain
```
Duration: 2.5 seconds
Description: Both hands moving, explaining something
```

### AM_Talk_Gesture_Shrug
```
Duration: 1.5 seconds
Description: Shoulder shrug "I don't know"
```

### AM_Talk_Nod_Yes
```
Duration: 1 second
Description: Agreeable head nod
```

### AM_Talk_Nod_No
```
Duration: 1.2 seconds
Description: Gentle head shake
```

---

## 3️⃣ REACTION ANIMATIONS (Tips/Events)

### AM_React_Tip_Small
```
Duration: 2 seconds
Trigger: Tips $1-9

Keyframes:
0f: Current pose
10f: Slight surprise, smile begins
20f: Hand to chest (grateful)
30f: Small bow/nod
45f: Return with smile maintained
60f: Blend out

Facial: Smile, eyebrow raise
Audio: "Thank you!" or similar
```

### AM_React_Tip_Medium
```
Duration: 3 seconds
Trigger: Tips $10-49

Description: More enthusiastic thank you
- Clasp hands together
- Bigger smile
- Slight bounce
```

### AM_React_Tip_Large
```
Duration: 4 seconds
Trigger: Tips $50-99

Description: Excited reaction
- Happy jump or excited movement
- Blow kiss
- Wave enthusiastically
```

### AM_React_Tip_Huge
```
Duration: 5 seconds
Trigger: Tips $100+

Description: Over-the-top celebration
- Jump with arms up
- Spin/dance
- Heart hands
- Extended celebration
```

### AM_React_Subscribe
```
Duration: 3 seconds
Trigger: New subscription

Description:
- Heart hands
- Excited wave
- Grateful gesture
```

### AM_React_Follow
```
Duration: 2 seconds
Trigger: New follower

Description:
- Wave
- Smile
- Welcoming gesture
```

### AM_React_Raid
```
Duration: 4 seconds
Trigger: Raid/host

Description:
- Big wave
- Welcome gesture
- Excited reaction
```

### AM_React_Gift
```
Duration: 3 seconds
Trigger: Gift received

Description:
- Surprised look
- Happy acceptance
- Thank you gesture
```

### AM_React_Milestone
```
Duration: 5 seconds
Trigger: Goal reached

Description:
- Celebration dance
- Jump
- Clap
```

### AM_React_Request_Accepted
```
Duration: 2 seconds
Description: Agree/accept a request (thumbs up, nod)
```

### AM_React_Request_Denied
```
Duration: 2 seconds
Description: Politely decline (head shake, apologetic)
```

### AM_React_Thinking
```
Duration: 3 seconds (can loop)
Description: Processing request, thinking pose
```

---

## 4️⃣ EMOTION ANIMATIONS

### AM_Emotion_Happy
```
Duration: 3 seconds
Entry into happy state
- Smile grows
- Eyes brighten
- Slight bounce
- Open body language
```

### AM_Emotion_Sad
```
Duration: 3 seconds
- Shoulders drop
- Head tilts down
- Slower movement
- Sigh
```

### AM_Emotion_Excited
```
Duration: 2 seconds
- Quick movements
- Bounce/jump
- Wide eyes
- Fast gestures
```

### AM_Emotion_Flirty
```
Duration: 3 seconds
- Slow wink
- Hair touch
- Lip bite (subtle)
- Tilted head
- Shoulder drop
```

### AM_Emotion_Shy
```
Duration: 2.5 seconds
- Look away
- Slight blush (material)
- Touch face/hair
- Nervous laugh gesture
```

### AM_Emotion_Surprised
```
Duration: 1.5 seconds
- Wide eyes
- Step back
- Hands up
- Gasp gesture
```

### AM_Emotion_Thinking
```
Duration: 4 seconds (loop)
- Finger on chin
- Eyes up/aside
- Slight pacing
- Contemplative pose
```

### AM_Emotion_Confused
```
Duration: 2 seconds
- Head tilt
- Furrowed brow
- Shrug gesture
```

### AM_Emotion_Love
```
Duration: 3 seconds
- Heart hands
- Dreamy expression
- Gentle sway
```

### AM_Emotion_Grateful
```
Duration: 2.5 seconds
- Hand on heart
- Small bow
- Warm smile
```

---

## 5️⃣ POSE ANIMATIONS

### AM_Pose_Casual_01
```
Duration: Hold (2s entry, infinite hold, 2s exit)
Description: Casual standing pose, one leg bent
Hand position: One on hip
```

### AM_Pose_Casual_02
```
Variation: Arms crossed comfortably, slight smile
```

### AM_Pose_Confident
```
Description: Power pose, both hands on hips
Chin slightly up, shoulders back
```

### AM_Pose_Cute
```
Description: Playful pose
- Peace sign near face
- Or hands framing face
- Head tilt
```

### AM_Pose_Elegant
```
Description: Model pose
- One arm down, one across body
- Chin up
- Elongated neck
```

### AM_Pose_Sexy_01
```
Description: Alluring pose
- Hip pop
- Shoulder drop
- Smoldering look
- Hand on hip or in hair
```

### AM_Pose_Sexy_02
```
Variation: Different arm positions, more curves
```

### AM_Pose_Sitting
```
Description: Seated pose (for chair/bed)
- Legs crossed or together
- Hands in lap or on surface
```

---

## 6️⃣ TRANSITION ANIMATIONS

### AM_Trans_Sit_To_Stand
```
Duration: 2 seconds
From seated to standing
```

### AM_Trans_Stand_To_Sit
```
Duration: 2 seconds
From standing to seated
```

### AM_Trans_Lie_Down
```
Duration: 3 seconds
From standing/sitting to lying on bed
```

### AM_Trans_Get_Up
```
Duration: 3 seconds
From lying to standing
```

### AM_Trans_Turn_Around
```
Duration: 2.5 seconds
180° turn with flair
```

### AM_Trans_Move_To_Point
```
Duration: Variable
Walk/move to different position in scene
```

---

## 7️⃣ SPECIAL ACTIONS

### AM_Action_Wave
```
Duration: 2 seconds
Friendly wave at camera
```

### AM_Action_Blow_Kiss
```
Duration: 2 seconds
Blow kiss gesture
```

### AM_Action_Wink
```
Duration: 1 second
Flirty wink
```

### AM_Action_Hair_Flip
```
Duration: 2 seconds
Flip/toss hair
```

### AM_Action_Giggle
```
Duration: 2 seconds
Giggling with hand near mouth
```

### AM_Action_Stretch
```
Duration: 3 seconds
Morning stretch, arms up
```

### AM_Action_Yawn
```
Duration: 2.5 seconds
Tired yawn (cute)
```

### AM_Action_Drink
```
Duration: 3 seconds
Sip from glass (prop interaction)
```

### AM_Action_Mirror_Check
```
Duration: 3 seconds
Check self in mirror, adjust hair
```

### AM_Action_Phone_Check
```
Duration: 2 seconds
Look at phone (prop)
```

---

## 8️⃣ DANCE ANIMATIONS

### AM_Dance_Chill_01
```
Duration: 8 seconds (loop)
Description: Slow, chill dance
- Gentle sway
- Hip movement
- Relaxed arms
```

### AM_Dance_Chill_02
```
Duration: 8 seconds (loop)
Variation: Different arm styling
```

### AM_Dance_Energetic_01
```
Duration: 6 seconds (loop)
Description: More active dance
- Bigger movements
- Arm choreography
- Steps/turns
```

### AM_Dance_Sexy
```
Duration: 8 seconds (loop)
Description: Sensual dance
- Slow hip movements
- Body waves
- Hair play
- Floor work optional
```

---

## 📋 ANIMATION CREATION GUIDELINES

### Skeleton Requirements
```
Base Skeleton: metahuman_base_skel
Root Bone: root
Pelvis: pelvis
Spine Chain: spine_01 → spine_02 → spine_03 → spine_04 → spine_05
Neck/Head: neck_01 → neck_02 → head
```

### Export Settings (from Maya/Blender)
```
Format: FBX 2020
Scale: 1.0 (cm)
Skeleton: Reference to existing
Bake Animation: Yes
Resample: 30 FPS
Remove Namespace: Yes
```

### Quality Standards
```
- No foot sliding
- No knee popping
- Natural arcs on all movements
- Weight shifts before movement
- Anticipation before actions
- Follow-through after actions
- Breathing visible in all idles
- Fingers posed (no flat hands)
```

### Blend Compatibility
```
All animations should:
- Start from neutral T-pose derivatives
- End in neutral or defined pose
- Have clean first/last frame for looping
- Include root motion if moving
```

---

## 🔄 ANIMATION STATE MACHINE

```
                    ┌─────────────┐
                    │    IDLE     │
                    │ (variations)│
                    └──────┬──────┘
                           │
          ┌────────────────┼────────────────┐
          │                │                │
          ▼                ▼                ▼
    ┌──────────┐    ┌──────────┐    ┌──────────┐
    │ TALKING  │    │ REACTING │    │  POSING  │
    │(gestures)│    │(to events)│   │ (holds)  │
    └────┬─────┘    └────┬─────┘    └────┬─────┘
         │               │               │
         └───────────────┴───────────────┘
                         │
                         ▼
                    ┌──────────┐
                    │ SPECIAL  │
                    │ (dance,  │
                    │  actions)│
                    └────┬─────┘
                         │
                         ▼
                    ┌──────────┐
                    │TRANSITION│
                    │(sit/stand│
                    │  etc.)   │
                    └──────────┘
```

---

## 📊 PRIORITY ORDER FOR CREATION

### Phase 1 (Must Have)
1. `AM_Idle_Neutral_01` - Primary idle
2. `AM_Talk_Idle` - Speaking base
3. `AM_React_Tip_Small/Medium/Large` - Monetization
4. `AM_Action_Wave` - Greeting
5. `AM_Emotion_Happy` - Primary emotion

### Phase 2 (Should Have)
6. `AM_Idle_Neutral_02` - Variation
7. `AM_React_Subscribe` - Sub reaction
8. `AM_Action_Blow_Kiss` - Fan favorite
9. `AM_Pose_Casual_01` - First pose
10. `AM_Emotion_Flirty` - Key emotion

### Phase 3 (Nice to Have)
11-30: Remaining idles, reactions, emotions

### Phase 4 (Polish)
31-50: Transitions, special actions

### Phase 5 (Extra)
51-63: Dance, additional variations

---

## 💡 SOURCES FOR ANIMATIONS

### Motion Capture
- **Rokoko** - Affordable mocap suits
- **Move.ai** - Phone-based mocap
- **RADiCAL** - AI mocap from video

### Animation Libraries
- **Mixamo** - Free/Adobe (retarget needed)
- **ActorCore** - Professional mocap
- **Reallusion** - iClone animations
- **Mocap Online** - Purchase packs

### Custom Creation
- **Maya** - Industry standard
- **Blender** - Free, excellent for UE5
- **Cascadeur** - Physics-based animation

### AI Animation
- **DeepMotion** - AI pose/animation
- **Plask** - Browser-based AI mocap
