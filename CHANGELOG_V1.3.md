# METROVERSE RP V1.3 — Garage foundation

## Added
- Owned vehicle records now include a garage location and stored/active state.
- Server-only store and retrieve operations for owned vehicles.
- Eight named garage locations aligned with the eight city presets.
- Configurable garage capacity (1–50 slots; default 10) with server-side capacity checks.
- Vehicle purchase creates an owned record that is not yet stored at a garage; the player/server gameplay flow must store it before it appears as garaged.

## Validation
- Added `Tests/verify_v1_3_garage.py` for static source checks.
- These checks do not compile Unreal C++, create a garage UI, spawn a vehicle, or verify multiplayer behavior.

## Still needed
- Garage/dealership UMG interface and map interaction.
- Spawn/despawn integration and vehicle return rules.
- Database persistence, anti-duplication transaction handling, and multiplayer testing.
- Unreal Engine 5.6 compile and packaged build.
