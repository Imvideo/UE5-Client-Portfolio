# Project Axiom
UE5 C++ Client Portfolio Project  
**Focus:** gameplay systems, performance optimization, and tools

---

## 0. Quick Links
- 🎥 Demo Video: (link)
- 🧰 Tool Demo Video: (link)
- 📄 Build Download: (link) *(optional)*
- 🧾 Docs / Wiki: (link) *(optional)*

---

## 1. Overview
- **Genre:** Top-down action roguelike (single player)
- **Engine:** Unreal Engine 5 (C++)
- **Duration:** 2026.02 ~ YYYY.MM (ongoing)
- **Team:** Solo
- **Goal:** Build production-like client systems (data-driven skills, debugging tools, performance)

---

## 2. Features (What you can try)
- Player movement / dash
- Basic combat + skills
- Enemy AI (FSM)
- Wave spawner + progression
- Debug UI (spawn, god mode, fps)
- Object pooling
- Profiling & optimization (before/after)

---

## 3. Architecture
### 3.1 Module / Folder Structure
- `Source/ProjectAxiom/`
  - `Core/` (common utilities, interfaces)
  - `Gameplay/` (player, enemies, combat)
  - `Ability/` (data-driven skills)
  - `UI/` (HUD, debug UI)
  - `Systems/` (spawner, save/load, pooling)
  - `Tools/` (editor/debug tools)

### 3.2 Key Design Decisions
- Why data-driven skills?
- Why component-based architecture?
- How to minimize allocations and avoid runtime spikes?

*(Add diagrams/screenshots here later)*

---

## 4. Data-Driven Skill System (Main Highlight)
### 4.1 Data Model
- Skill definition via **DataTable** (or DataAsset)
- Fields: cooldown, damage, range, effect, hitbox type, etc.

### 4.2 Runtime Flow
1. Input → AbilityComponent
2. Validate (cooldown/resources)
3. Spawn executor (hitbox/projectile)
4. Apply damage / effects
5. Update UI / logs

### 4.3 Extending Skills
- Adding a new skill = add data + implement (or reuse) executor

---

## 5. Debugging & Tools (Highlight)
- In-game debug UI
  - spawn enemy, toggle invincible, show FPS/frame time
- (Optional) Editor tool
  - edit skill table, validate values, export/import

---

## 6. Performance (Before / After)
### 6.1 Profiling Setup
- Unreal Insights / stat commands used
- Test scenario description

### 6.2 Improvements (Numbers)
| Item | Before | After | Notes |
|---|---:|---:|---|
| Effect spawn |  |  | pooling |
| Frame spikes |  |  | async load |
| CPU time |  |  | tick reduction |

*(Add screenshots here later)*

---

## 7. Troubleshooting Logs (Portfolio Asset)
- [Bug] 제목 - 원인/해결 요약 링크
- [Perf] 제목 - 개선 전/후 링크

---

## 8. How to Run
### 8.1 Requirements
- UE version:
- Platform:

### 8.2 Build / Launch
1. `git clone ...`
2. Open `.uproject`
3. Generate project files
4. Build & Run

---

## 9. Roadmap
- [ ] Week 01: project setup + movement
- [ ] Week 02: combat loop
- [ ] Week 03: ability skeleton
- ...

---

## 10. Contact
- GitHub: (link)
- Email: (optional)
