# METROVERSE RP — Eight-city world catalogue (v0.6)

METROVERSE RP is planned as an eight-city roleplay world. Each city has its own identity and can be built as a separate Unreal level/sublevel. The current generator creates a distinct procedural greybox layout per city; these are not finished photorealistic maps.

| ID | City | Identity | Signature landmark |
|---|---|---|---|
| 1 | Metro City | Dense modern downtown | Civic Plaza and Central Tower |
| 2 | Northpoint | University and leafy residential districts | Northpoint University and Observatory |
| 3 | Southbay | Warm coastal-suburban city | Southbay Stadium and Waterfront Park |
| 4 | Eastford | Technology and business district | Eastford Technology Campus |
| 5 | Westhaven | Historic market and mixed-use neighbourhoods | Westhaven Grand Market |
| 6 | Port Azure | Harbour and marina city | Port Azure Harbour and Marina |
| 7 | Ironworks | Industrial and logistics centre | Ironworks Freight Terminal |
| 8 | Hillcrest | Hillside town and terraced neighbourhoods | Hillcrest Ridge and Old Town |

## Editor setup
1. Compile the C++ project in Unreal Engine 5.6 on Windows.
2. Create eight city levels (or eight persistent sublevels/world partitions as the project grows).
3. Add one `MetroverseCityGenerator` actor to each level.
4. Set `City To Generate` to a different city for each actor.
5. The actor applies that city's distinct block count, road width, building-height range and deterministic random seed.
6. Use `GetCityDisplayName`, `GetCityLandmark` and `GetCityTheme` in Blueprint UI or level setup.

## Design constraints
- The eight cities are fictional and original.
- Existing organisation coverage remains: four Police Departments and three Hospitals can be assigned to selected cities; the eight-city world does not imply every city has every faction.
- Long-distance travel, streaming, city-to-city transit, persistence and server sharding still require implementation and testing.
- Current city geometry is a greybox generated from primitive meshes, not final realistic art.
