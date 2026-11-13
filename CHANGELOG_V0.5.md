# METROVERSE RP UE5 — v0.5 changelog

## Added: procedural first-district city blockout
- New `AMetroverseCityGenerator` actor creates a connected multi-block street grid.
- Road and sidewalk strips are generated as instanced primitive meshes.
- Deterministic randomized building footprints and heights make the skyline varied between blocks.
- Basic facade floor bands provide a temporary visual rhythm for later window and facade replacement.
- Simple street-furniture placeholder posts are placed at alternating intersections.
- Editor-exposed settings control block count, spacing, road width, building height range and random seed.
- `RebuildDistrict` can be called in the editor or from Blueprint; construction changes rebuild the instances.

## How to try it in Unreal Editor
1. Open the project with Unreal Engine 5.6 on a Windows PC and allow the C++ project to compile.
2. Create or open a test level.
3. Find `MetroverseCityGenerator` in Place Actors and drag one instance into the level.
4. Adjust Blocks Per Side, Block Spacing and building-height values in Details.
5. Use Rebuild District after changing values if needed.
6. Add a PlayerStart and set the project's GameMode / Default Pawn to the METROVERSE classes to test movement in the blockout.

## Honest limitations
This is a procedural greybox/blockout built from Unreal's basic cube mesh. It is not photorealistic architecture, a finished map, traffic, interiors, pedestrians, or a GTA V-quality city. It has not been compiled or visually tested in Unreal Editor in this environment.
