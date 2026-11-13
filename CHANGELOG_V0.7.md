# METROVERSE RP UE5 — v0.7 Inter-city transit foundation

## Added
- `AMetroverseTransitNetwork`, a configurable eight-city station/anchor catalogue.
- Metro City hub connections plus an outer-city ring route graph.
- Blueprint-callable `RequestCityTravel` on the player controller.
- Server-side route, current-city, and station-proximity checks before changing city and teleporting the pawn.
- Configurable city station offsets and arrival radius.

## Honest implementation status
This is a code foundation for a shared-world transit test. It does not yet include visible train/airport/road vehicles, travel UI, timetables, fare payments, map streaming, loading screens, or a production transit experience. City anchors are widely spaced coordinates; place the corresponding city generators at those anchors to test. The player controller must be the configured controller class and one `MetroverseTransitNetwork` actor must be placed in the level.

Unreal compilation, multiplayer travel testing, packaged builds, and Android performance have not been verified in this environment.
