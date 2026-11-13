# METROVERSE RP V1.4 — Dealership economy safeguards

## Implemented in source
- Owned vehicle records now retain the server-recorded purchase price.
- Vehicle ID and registration plate duplicate checks are case-insensitive.
- A vehicle must be stored in an approved garage before it can be sold.
- Server sale requests above 70% of the recorded purchase price are rejected.
- Existing currency overflow protection remains in place.

## Dealership UI integration guide
1. Import `Content/Data/MetroverseVehicleCatalog.csv` as a DataTable using `FMetroverseVehicleCatalogRow`.
2. Build a UMG showroom with category filters (Car/Motorcycle), class filters, vehicle details and the listed `BasePriceMC`.
3. The server must resolve the selected `VehicleID` against its trusted DataTable and pass the canonical name, plate and price to `ServerPurchaseVehicle`; never accept a client-provided price as authoritative.
4. Display owned vehicles from the replicated `OwnedVehicles` array. Enable Sell only for vehicles stored at a garage.
5. Compute the offered resale value on trusted server code, at or below 70% of the recorded purchase price, then call `ServerSellVehicle`.
6. Add server-side persistence and transaction logging before public multiplayer release.

## Validation and limitations
- `Tests/verify_v1_4_dealership_economy.py` checks the source safeguards.
- These are source-level safeguards, not a completed dealership screen or live economy test.
- Unreal Engine 5.6 compilation, DataTable import, UMG implementation, persistence, multiplayer security testing and packaged builds remain outstanding.
