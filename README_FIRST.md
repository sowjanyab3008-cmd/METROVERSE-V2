# METROVERSE RP — Unreal Engine 5 source project

This package is an evolving native UE5 C++ development project for the METROVERSE RP game concept. It is source code, not a finished game or a packaged executable.

## Current source features
- V1.6 starter-world bootstrap: when the game starts in an empty level, the GameMode creates a shared ground plane, eight greybox city districts, a transit network, six Metro City faction headquarters, a PlayerStart and a starter delivery contract.
- Primitive placeholder avatar so the player is visible without importing a character mesh.
- Keyboard job interaction: press E near the pickup marker to accept the delivery, reach the drop-off marker, then press E again to claim the reward.
- Third-person character foundation with first-/third-person camera toggle, walk, sprint and jump input.
- Replicated RP player identity and organisation data foundations.
- Organisation rank ladders, senior interview eligibility and reprimand logic helpers.
- Eight roleplay chat channels with server-side routing and basic access/rate checks.
- Player appearance and XP/level data foundations.
- Procedural city greybox generator with eight city identities and per-city layout presets: Metro City, Northpoint, Southbay, Eastford, Westhaven, Port Azure, Ironworks and Hillcrest.
- Inter-city transit foundation with eight configurable station anchors, central-hub and outer-ring routes, and server-validated player travel requests.
- Server-validated pickup-to-destination job contracts for five job categories, with replicated in-session currency, XP and level rewards.
- V1.6 starter Metro City now spawns six visible greybox faction headquarters: Government, Police, Hospital, News Network, FIB and Administration.
- Replicated HQ labels plus a Blueprint faction directory with organisation descriptions and headquarters locations.
- Player-controller faction application and review requests, plus promotion requests whose rank thresholds are selected by the server rather than supplied by clients.

## Try the V1.6 starter world
1. Open the project in Unreal Engine 5.6 on Windows and let the C++ module compile.
2. Open any empty level (or create a new Basic level) and press Play. The GameMode should spawn the starter district automatically.
3. Controls: WASD move, mouse look, Left Shift sprint, Space jump, V toggle first-/third-person camera, E interact with the delivery job.
4. Start beside the pickup marker, press E, walk to the second marker, and press E again after the minimum journey time. The reward message appears on screen.
5. Eight greybox city districts are spawned in a shared-world layout. The transit network connects the central hub and outer-city ring. Six faction HQ blockouts are spawned around Metro City. Faction application, promotion and review requests are exposed to Blueprint through `AMetroversePlayerController`.
6. The avatar, city and faction HQs are greybox placeholders, not final art.

This is a source-level starter slice; it still needs an Unreal compile and PIE/standalone validation on a Windows PC before it can be called confirmed playable. V1.6 adds the eight-city world layout, headquarters and faction API foundation on top of that slice.

## Open the project
- Requires Unreal Engine 5.6 and a Windows development PC with a compatible C++ toolchain (Visual Studio with the game development workload).
- Open `METROVERSE.uproject` in Unreal Editor and allow Unreal Build Tool to compile the C++ module.
- If prompted, regenerate project files using the `.uproject` context menu on Windows.

## Test the city blockout
- See `CHANGELOG_V0.5.md` for instructions to add `MetroverseCityGenerator` to a level.
- The generated district uses engine primitive meshes as temporary geometry. Replace these with original/licensed modular road, building, facade and street-prop assets for final art.

## V1.0 vehicle catalog
- Data-driven catalog with 120 fictional car entries and 20 fictional motorcycle entries (140 total).
- CSV is prepared for import as an Unreal DataTable using `FMetroverseVehicleCatalogRow`; JSON is included for tooling/UI prototypes.
- Entries include class, body style, seats, tuning targets, fuel/drive type and fictional MC price.
- The catalog is not 140 finished 3D vehicles: meshes, sounds, wheel/physics assets, animations, Blueprints and runtime dealership integration still require Unreal Editor work and testing. See `VEHICLE_CATALOG_SETUP.md`.

## V1.1 life, vehicle ownership and faction career foundations
- Server-side vehicle ownership record purchase/sale methods and replicated owned-vehicle records.
- 24 life-activity definitions across work, civic, social, media, public service, business and hobbies.
- Replicated faction application and promotion request records, with server-side submission and senior/admin review methods.
- Proposed 50-row faction rank progression balance table for five organisations. V1.6 adds server-side promotion thresholds for ranks 2–10 using those progression values.
- See `LIFE_AND_FACTION_SYSTEMS_SETUP.md` for setup and limitations. These additions have source-level/data validation only; they have not been compiled or runtime-tested in Unreal.

## V1.4.1 test hardening patch
- Vehicle resale cap arithmetic avoids signed 64-bit multiplication overflow.
- Vehicle sale lookup is case-insensitive, matching purchase duplicate detection.
- See `CHANGELOG_V1.4.1.md`.

## V1.6 world and faction update
- See `CHANGELOG_V1.6.md` for the six starter headquarters and faction API additions.
- `UMetroverseFactionLibrary` supplies Blueprint-friendly names, descriptions and headquarters strings.
- `AMetroversePlayerController` now exposes application submission, application review, promotion requests and promotion review. Promotion thresholds are selected on the server; do not expose a client override for these values.
- These are gameplay/source foundations. The HQs have no interiors or NPC recruiters, and the faction flow has no finished UI or persistent backend yet.

## Important status
- No Unreal compile, packaged Windows build, Android APK, live multiplayer test, or device performance test has been completed here. V1.5 starter-world code is not yet verified in Unreal.
- No realistic character mesh, facial rig, animation blueprint, finished city art, database-backed account service, or production server cluster is included yet. City districts and HQs are primitive greybox geometry and still require UE multiplayer replication validation.
- Do not expose admin secrets or trust client-supplied rank, faction, balance, reprimand or session data in a public server.

For transit setup, see `CHANGELOG_V0.7.md`. For job-contract setup, see `CHANGELOG_V0.8.md`. Place one `MetroverseTransitNetwork` actor in the level, place each city generator at its matching station offset, and call `RequestCityTravel` from the player controller when the player is at a station. See `CITY_CATALOG.md`, `RP_SYSTEMS_STATUS.md` and `ROADMAP.md` for detailed scope and remaining work.

## v0.9 vehicle framework
A Chaos Vehicles-based pawn and server-validated enter/exit possession flow are now included. See `CHANGELOG_V0.9.md` for Blueprint setup steps. A vehicle Blueprint still needs a valid skeletal mesh, physics asset, wheel setup and Enhanced Input bindings. This source addition is not a finished drivable-car experience until it is compiled and tested in Unreal Engine.

## V1.2 progression integration note
`AMetroversePlayerState` now exposes server-only C++ foundations to complete a life activity with a reward and per-activity cooldown, record validated faction service minutes, and gate promotion requests against rule values supplied by trusted server code. Load the matching faction/rank row from `Content/Data/FactionPromotionRules.csv` and pass its `MinimumServiceHours`, `MinimumCompletedActivities`, and `MaximumActiveReprimands` to `ServerRequestPromotion`. Do not accept reward values or promotion thresholds directly from an untrusted client. Persist cooldown timestamps and progression fields in a backend/save system before production use.

## V1.3 garage update
Owned vehicle records now support a garage location and stored/active state. Server-only methods can store and retrieve a player's owned vehicle at one of eight named city garages, with a configurable slot limit. This is gameplay logic scaffolding only: no garage screen, vehicle spawning, persistent database, or Unreal multiplayer test is included yet. See `CHANGELOG_V1.3.md` and run `python Tests/verify_v1_3_garage.py` for static checks.

## V1.4 dealership economy safeguards
- Owned vehicles retain their recorded purchase price; duplicate IDs/plates are checked case-insensitively.
- Sale requires garage storage and the requested resale payout cannot exceed 70% of recorded purchase price.
- See `DEALERSHIP_ECONOMY_V1.4.md` for the UMG integration path and security requirements.
