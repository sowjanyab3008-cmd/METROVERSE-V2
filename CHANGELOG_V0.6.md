# METROVERSE RP UE5 — v0.6 changelog

## Added: eight-city world framework
- Expanded `EMetroverseCity` from four entries to eight named fictional cities.
- Added city labels: Metro City, Northpoint, Southbay, Eastford, Westhaven, Port Azure, Ironworks and Hillcrest.
- Added city identity helpers for display name, signature landmark and theme.
- Extended the procedural city generator with per-city presets for block count, spacing, road width, building-height range and deterministic seed.
- Each generator actor can now be assigned to one city, enabling separate city levels/maps.
- Added `CITY_CATALOG.md` with the eight-city world plan and editor setup instructions.

## Honest limitations
- The city presets generate distinct greybox layouts from primitive meshes; they are not eight finished realistic cities.
- No seamless world streaming, inter-city transport, fast travel, city server routing or persistence has been implemented.
- Unreal compilation, editor visual inspection, multiplayer and Android/PC packaging have not been tested.
