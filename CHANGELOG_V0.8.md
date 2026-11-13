# METROVERSE RP UE5 — v0.8 changelog

## New: server-validated city job contracts
- Added five job categories: Delivery Courier, Bus Operator, Taxi Driver, Truck Hauler and Airport Ground Crew.
- Added configurable pickup and destination markers to `AMetroverseJobContract`.
- Added server RPC flow to accept a contract only while near its pickup marker.
- Added server-side completion checks for active contract identity, minimum/maximum elapsed time and destination proximity.
- Job rewards are read from the server-side contract actor, not supplied by the client.
- Added replicated current-job state, lifetime XP and level progression (one level per 500 XP, capped at level 100).
- Added a Blueprint event `OnJobContractResult` for a future job HUD/toast UI.

## Level setup
1. Open a city level and place one or more `MetroverseJobContract` actors.
2. Set each actor's `JobType`, `CurrencyReward`, `ExperienceReward`, `PickupOffset` and `DropoffOffset`.
3. Place the actor so both markers are reachable on foot. Vehicle requirements are not enforced yet.
4. In a PlayerController Blueprint derived from `MetroversePlayerController`, call `RequestStartJob` with the contract when the player interacts with the pickup marker.
5. Call `RequestCompleteJob` when the player interacts with the destination marker.
6. Implement `OnJobContractResult` to display the returned success/failure message.

## Controls / integration
- This milestone adds Blueprint-callable functions rather than a hard-coded key binding, so the final interaction key and UI can be designed consistently across PC and Android.
- No automatic job actors are spawned. Designers must place and configure contract actors in each city level.

## Verification and limitations
- `Tests_verify_jobs.py` performs static source assertions for the job contract flow.
- Existing source tests and ZIP integrity should be run before sharing this package.
- This is not a compiled or runtime-tested Unreal feature. A real Unreal Engine 5.6 build, multiplayer PIE test, network abuse test and reward-persistence integration remain required.
- Rewards currently update replicated in-session PlayerState fields only. They are not saved to a database and can be lost when a session ends.
- The job categories are roleplay task templates; no buses, taxis, trucks, aircraft or vehicle physics are included in this milestone.
