# V1.0 — Vehicle catalog expansion

- Added a data catalog of 120 fictional cars and 20 fictional motorcycles (140 total).
- Added CSV for Unreal DataTable import, JSON index for tooling, and `FMetroverseVehicleCatalogRow` schema.
- Added validation test for counts, unique IDs and required specification fields.
- Added Unreal Editor import and asset-mapping instructions.

## Honest scope
This release expands the vehicle **catalog/data layer**. It does not create 140 finished drivable 3D models. Each catalog entry still needs an original/licensed mesh and appropriate physics, wheel, sound, animation and Blueprint setup. Unreal compilation, in-editor import, driving, multiplayer replication and performance remain unverified.
