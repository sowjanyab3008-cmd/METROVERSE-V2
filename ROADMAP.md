# METROVERSE RP — UE5 production roadmap

## Milestone 0 — Native project foundation
- [x] Create UE5 C++ project scaffold
- [x] Add basic character movement/camera code
- [ ] Compile in Unreal Editor
- [ ] Assign a proper skeletal mesh and animation Blueprint

## Milestone 1 — Character quality
- Character creator and preview
- Rigged original human character
- Facial features, hair, skin tones and outfit slots
- Idle, walk, jog, sprint, jump and turn animations
- First-person arms/camera and third-person body visibility

## Milestone 2 — First city district
- Modular road and sidewalk kit
- Original buildings and interiors
- Street props, signs, vegetation and traffic lights
- Day/night lighting and weather prototype
- Collision, navigation and performance pass

## Milestone 3 — Vehicles and life
- One original car with enter/exit and driving
- Vehicle camera and basic handling
- Pedestrian/traffic prototype
- First jobs and safe spawn locations

## Milestone 4 — Multiplayer foundation
- Dedicated authoritative server design
- Player identity and saved profiles
- Replicated movement and vehicle state
- Chat channels, moderation and reporting
- Database-backed persistence

## Milestone 5 — Roleplay systems
- Government, four Police Departments, three Hospitals, News Network, FIB
- Rank permissions, interviews, reprimands and audit logs
- Jobs, businesses, fictional currency and events

## Platform strategy
Develop and validate the core game on PC first. Then profile and optimise for Android using mobile renderer settings, scalable assets, reduced draw calls, simplified shadows and device-specific quality presets. A high-end PC visual preset cannot be assumed to run on Android.

## v0.5 development note — first district blockout
A procedural city greybox actor has been added. It can generate a configurable connected street grid with sidewalks, varied building masses, facade bands and basic street-furniture placeholders. It is intended for gameplay scale/layout testing before replacing geometry with final art assets. See `CHANGELOG_V0.5.md`.

## v0.9 development note — vehicle possession foundation
- [x] Add Chaos wheeled-vehicle pawn C++ foundation
- [x] Add replicated driver state and clamped driving-input API
- [x] Add server-validated enter/exit possession requests
- [ ] Create and configure a vehicle Blueprint with wheel/physics assets
- [ ] Bind Enhanced Input and add seat/door/exit safety logic
- [ ] Compile and test vehicle replication with multiple clients


## V1.0 vehicle catalog follow-up
- [x] Add 120 car + 20 bike catalog records.
- [x] Add DataTable row schema and CSV/JSON exports.
- [ ] Import DataTable and compile in UE 5.6.
- [ ] Create/assign original or licensed vehicle meshes and physics assets.
- [ ] Integrate dealership, garage, ownership, spawn/despawn and persistence.
- [ ] Test vehicle handling and replication with multiple clients.

## V1.1 — life and faction foundations
- Added 24 life-activity data rows and a DataTable schema.
- Added replicated vehicle ownership records with server-side purchase/sale methods; dealership UI, garage spawning, persistence and customization remain future work.
- Added faction application and promotion request records with server-side submission/review methods.
- Added proposed rank progression balance data for five factions; service-hour/activity thresholds still require runtime tracking and enforcement.
- Static validation scripts pass. Unreal compile and in-editor/network tests remain outstanding.

## V1.3 — garage ownership foundation
- [x] Add garage location and stored-state fields to owned vehicle records.
- [x] Add server-only store/retrieve methods for eight city garages.
- [x] Add configurable garage slot limit with server-side validation.
- [ ] Build dealership/garage UMG screens and vehicle preview.
- [ ] Connect garage actions to actual vehicle spawn/despawn and save data.
- [ ] Compile and test with multiple clients in Unreal Engine 5.6.
