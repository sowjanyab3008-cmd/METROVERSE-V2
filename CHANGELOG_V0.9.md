# METROVERSE RP — v0.9 vehicle framework

## Added
- Added `AMetroverseVehiclePawn`, based on Unreal's Chaos wheeled-vehicle pawn foundation.
- Added replicated driver ownership state and a third-person vehicle camera boom.
- Added clamped throttle, steering, brake and handbrake input entry points for Blueprint/input binding.
- Added server RPCs for entering and exiting a vehicle. The server validates vehicle existence, world, occupancy and player-to-vehicle distance before possession changes.
- Preserved the player's previous character pawn while driving and restores possession on exit.
- Enabled the UE ChaosVehicles plugin/module in the project configuration.

## Blueprint setup required
1. Open the project in Unreal Engine 5.6 and compile the C++ module.
2. Create a Blueprint child of `MetroverseVehiclePawn`.
3. Assign an original/licensed vehicle skeletal mesh and a compatible physics asset.
4. Configure the Chaos Vehicle Movement component's wheel setups, wheel classes, mass, torque curve, steering and suspension.
5. Add Enhanced Input actions for throttle, steering, brake, handbrake, enter and exit. Bind the driving actions to `SetDriverInputs`; call `RequestEnterVehicle` / `RequestExitVehicle` on the player controller.
6. Add a safe exit-location check and a proper seat/door animation before production use.
7. Test with two clients on a dedicated server before considering multiplayer vehicle handling verified.

## Not included / not verified
- No finished vehicle mesh, wheel assets, sound, damage, fuel, garage, ownership, traffic AI or vehicle purchase flow.
- No automatic input mapping or door/seat animation is supplied; these are Blueprint integration tasks.
- Vehicle physics replication and anti-cheat hardening have not been tested with live clients.
- No Unreal compile, packaged build, Android performance test or dedicated-server test has been run in this environment.
