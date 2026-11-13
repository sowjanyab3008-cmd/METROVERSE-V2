# METROVERSE RP — V1.6 World & Factions Foundation

## World additions
- Starter world now spawns all eight greybox city districts in a shared-world layout: Metro City, Northpoint, Southbay, Eastford, Westhaven, Port Azure, Ironworks and Hillcrest.
- The shared map ground plane was expanded to contain all eight city locations; the existing transit network is spawned and connects the central hub with the outer-city ring.
- Odd-sized city layouts reserve an open central transit plaza so the station arrival point is not inside a generated building.
- Metro City spawns six physical greybox faction headquarters around its district: Government, Police, Hospital, News Network, FIB and Administration.
- Headquarters use engine primitive buildings and text signs as temporary blockout assets.
- Headquarters actor identity and sign text are replicated so clients can see the faction labels in a networked session.
- Added `Content/Data/FactionDirectory.csv` with faction names, headquarters and role descriptions.

## Faction systems additions
- Added `UMetroverseFactionLibrary` Blueprint function library for faction display names, descriptions, headquarters and maximum rank.
- Added player-controller request/review wrappers for faction applications and promotion requests.
- Promotion thresholds are selected server-side from the included rank progression values. Clients submit only the requested next rank and cannot provide weaker requirements.
- Application and promotion review still use the existing server-authoritative permission checks in `AMetroversePlayerState`.

## Validation and limitations
- Static tests verify the new source declarations, HQ spawn entries, replicated faction label data, CSV rows and server-selected promotion thresholds.
- Unreal Engine 5.6 is not available in the build environment. No C++ compile, editor launch, PIE, multiplayer session or packaged build was performed.
- HQs are simple cube blockouts, not finished buildings. There are no interiors, NPC recruiters, faction UI panels, database persistence or live public servers yet.
