# METROVERSE RP — UE5 source status (v0.9)

## Present in source
- Native Unreal Engine 5.6 C++ project scaffold.
- Third-person character movement foundation, sprint, jump, mouse look and first-/third-person camera toggle.
- Replicated RP PlayerState data foundations, organisation rank ladders, interview eligibility and reprimand helper logic.
- Eight roleplay chat channels with server-routed chat and basic access/rate checks.
- Procedural city greybox generator with eight city identities and per-city layout presets.
- Inter-city transit foundation with eight configurable station anchors and server-validated travel requests.
- Server-validated pickup-to-destination job contracts for five job categories, with in-session currency and XP rewards.
- v0.9 Chaos wheeled-vehicle pawn foundation, replicated driver reference, clamped driving-input API and server-validated enter/exit possession flow.

## Vehicle framework boundaries
- Requires a Blueprint child with a vehicle skeletal mesh, physics asset, wheel classes and Chaos Vehicle Movement wheel setup.
- Enhanced Input bindings, seat/door animation, safe exit obstruction checks, fuel, damage, garages and ownership are not supplied.
- Vehicle replication, anti-cheat and multiplayer physics have not been runtime tested.

## Not present / not verified
- No compiled Unreal build, packaged PC executable or Android APK.
- No finished realistic character mesh, facial rig, animation Blueprint or licensed art pack.
- No finished city maps, interiors, traffic simulation, pedestrian AI, production vehicle fleet, phone or complete UI.
- No database-backed account persistence, production authentication, dedicated-server cluster, anti-cheat, complete moderation workflow or public deployment.
- No Android touch controls, device performance profile or mobile testing.
- No Unreal Editor / Unreal Build Tool compilation or multiplayer runtime test has been performed.

## Security / release warning
Server authority and validation are still required for rank, faction, money, session IDs, reprimands, vehicle ownership and driving state. Production release needs authenticated admin roles, persistent database transactions, audit storage, moderation/report tools, abuse testing and privacy review. Never trust client-supplied faction, rank, balance, session membership or moderation values.
