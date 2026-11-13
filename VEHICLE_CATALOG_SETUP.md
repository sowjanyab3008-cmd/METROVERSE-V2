# Vehicle catalog — V1.0

The catalog contains **120 cars + 20 motorcycles = 140 entries**. Names and specifications are fictional game-balance data, not licensed real-world vehicle models. The catalog does not include 140 3D meshes, sounds, animations, physics assets or finished Blueprint vehicles.

## Import into Unreal Engine 5.6
1. Open the project in Unreal Editor after compiling the C++ module (the new row struct is `FMetroverseVehicleCatalogRow`).
2. In the Content Browser, choose **Import** and select `Content/Data/MetroverseVehicleCatalog.csv`.
3. Choose `MetroverseVehicleCatalogRow` as the DataTable row structure when prompted.
4. Save the DataTable as `DT_MetroverseVehicleCatalog`.
5. Use the DataTable in a vehicle dealership / garage UI. Filter `Category` to `Car` or `Bike`; use `BasePriceMC` for the fictional MC economy.
6. Create Blueprint children of the appropriate vehicle pawn, assign licensed/original meshes, physics assets, wheel setups and sounds, then map catalog IDs to those assets.

## Catalog fields
`VehicleID` is the stable unique key. `TopSpeedKmh`, `Acceleration0To100Sec` and `Handling` are tuning targets for gameplay balancing, not measured real-world performance. `BasePriceMC` is fictional in-game currency.

## Validation
Run `python Tests_verify_vehicle_catalog.py` from the project root. It checks row count, category counts, unique IDs and required fields.
