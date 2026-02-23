# Project Axiom
UE5 C++ Client Portfolio Project  
**Focus:** gameplay systems, performance optimization, and tools

---

## 0. Quick Links
- 🎥 Demo Video: videos/day03_player_movement.mp4
- 🧾 Docs / Wiki: (TBD)

---

## 1. Overview
- **Genre:** Top-down action roguelike (single player)
- **Engine:** Unreal Engine 5 (C++)
- **Period:** 2026.02 ~ (ongoing)
- **Team:** Solo
- **Goal:** Build production-oriented client systems step by step  
  (movement → combat → skills → tools → optimization)

---

## 2. Current Features
- Player movement (Enhanced Input)
- Top-down camera
- Movement-based character rotation
- Idle / Walk / Run animation blending

<details>
<summary>🎮 [Week 1 - Day 3] 플레이어 이동 구현</summary>
  
▶️ [플레이어 이동 시연 영상](videos/week1_day3_player_movement.mp4)

</details>

<details>
<summary>🎮 [Week 1 - Day 4] 카메라 줌 기능 구현</summary>
  
▶️ [카메라 줌 시연 영상](videos/week1_day4_camera_zoom.mp4)

</details>

<details>
<summary>🎮 [Week 2 - Day 2] 적 피격 및 사망 처리</summary>
  
▶️ [적 피격 및 사망 시연 영상](videos/week2_day2_enemy_damage_death.mp4)

</details>

<details>
<summary>🎮 [Week 2 - Day 3] 적 추적 이동 및 기본 애니메이션</summary>
  
▶️ [적 추적 이동 및 기본 애니메이션 시연 영상](videos/week2_day3_enemy_chase_anim.mp4)

</details>

<details>
<summary>🎮 [Week 2 - Day 4] 적 피격 반응(히트 스턴 및 히트 플래시)</summary>

▶️ [적 피격 반응 시연 영상](videos/week2_day4_enemy_hitstun_hitflash.mp4)

</details>

<details>
<summary>🎮 [Week 2 - Day 6] 적 웨이브 스폰 시스템 구현</summary>

▶️ [적 웨이브 스폰 시스템 시연 영상](videos/week2_day6_wave_spawn.mp4)

</details>

<details>
<summary>🎮 [Week 3 - Day 1] 적 처치 코인 드랍 및 획득</summary>

▶️ [적 처치 코인 드랍 및 획득 시연 영상](videos/week3_day1_coin_drop_pickup.mp4)

</details>
---

## 3. Planned Features
- Dash & basic combat loop
- Data-driven skill system
- Enemy AI (FSM)
- Wave spawner & progression
- Debug UI (spawn, god mode, fps)
- Object pooling
- Performance profiling & optimization

---

## 4. Architecture (Planned)
### 4.1 Module / Folder Structure
```text
Source/ProjectAxiom/
 ├─ Core/        // common utilities, interfaces
 ├─ Gameplay/    // player, enemies, combat
 ├─ Ability/     // data-driven skills
 ├─ UI/          // HUD, debug UI
 ├─ Systems/     // spawner, save/load, pooling
 └─ Tools/       // editor & debug tools
```

### 4.2 Design Direction
- Component-based gameplay architecture
- Data-driven content where iteration speed matters
- Minimize runtime allocations and frame spikes

*(Details and diagrams will be added as implementation progresses)*

---

## 5. Roadmap
- [x] Week 01: project setup + player movement
- [ ] Week 02: combat loop
- [ ] Week 03: ability system skeleton
- [ ] Week 04: enemy AI & spawner
- [ ] Week 05: debug tools
- [ ] Week 06: performance optimization

---

## 6. How to Run
### Requirements
- Unreal Engine 5.7.x
- Windows 10+

### Build & Launch
1. `git clone <repo>`
2. Open `ProjectAxiom.uproject`
3. Generate project files
4. Build & Play

---

## 7. Contact
- GitHub: (link)
- Email: (optional)

