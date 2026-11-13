# METROVERSE RP — life activities, vehicles and faction careers (V1.1)

## Added in this package

### Vehicle ownership foundation
`AMetroversePlayerState` now has server-only C++ mutation methods for purchasing and selling owned vehicle records. Ownership (vehicle ID, display name, plate and purchase timestamp) replicates with the player state. The server caller must resolve the vehicle ID against the imported `MetroverseVehicleCatalog` DataTable and use that row's `BasePriceMC`; never accept a client-supplied price or display name. The current code stores ownership records only: spawning, garage persistence, customization, insurance and dealership UI are not yet connected.

### Life activities
Import `Content/Data/MetroverseLifeActivities.csv` as a DataTable using `FMetroverseLifeActivityRow`. It contains 24 activity ideas across work, civic service, social, media, public service, business and hobbies. Existing five job contracts remain the implemented pickup/drop-off job foundation. The new 24-row activity catalogue is design/configuration data; cooldown tracking, activity completion validation, NPC interactions and reward payout still need a runtime activity manager.

### Faction applications and promotions
`AMetroversePlayerState` now stores a replicated faction application and promotion request. Server-only methods support submitting an application, senior/admin review, requesting the next rank, and senior review. Accepted applications assign rank 1. Promotion requests currently require the next rank and zero active reprimands; a same-faction reviewer at rank 7+ (or Administration) can approve/reject. These are gameplay foundations, not a finished application UI or persistent faction database.

`Content/Data/FactionPromotionRules.csv` is a proposed progression balance sheet for Government, Police, Hospital, News Network and FIB. It lists service-hour and completed-activity targets for ranks 1–10. These numeric thresholds are configuration proposals and are NOT yet enforced by the C++ promotion methods; add server-side service/activity tracking and validate against this table before production.

## Unreal Editor setup
1. Import `MetroverseLifeActivities.csv` as a DataTable using `FMetroverseLifeActivityRow`.
2. Import `FactionPromotionRules.csv` as a DataTable using `FMetroverseFactionPromotionRuleRow`.
3. Import the 140-row vehicle CSV using `FMetroverseVehicleCatalogRow` as described in `VEHICLE_CATALOG_SETUP.md`.
4. Build the project in Unreal Engine 5.6 on Windows. Resolve any compiler/UHT issues before enabling the new features.
5. Call the mutation methods only from trusted server gameplay code. Add save/database persistence before shipping; replicated PlayerState data is not persistent storage.

## Not yet completed or verified
No Unreal compile or runtime test was run in this environment. No vehicle meshes, wheel/physics assets, garage/dealership widgets, activity NPCs, faction application widgets, database persistence, anti-cheat service, Android package or packaged PC build is included. The feature set must not be described as a finished playable implementation until those steps are complete and tested.
