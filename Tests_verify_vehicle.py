from pathlib import Path

ROOT = Path(__file__).parent
vehicle_h = (ROOT / "Source/METROVERSE/Public/MetroverseVehiclePawn.h").read_text()
vehicle_cpp = (ROOT / "Source/METROVERSE/Private/MetroverseVehiclePawn.cpp").read_text()
controller_h = (ROOT / "Source/METROVERSE/Public/MetroversePlayerController.h").read_text()
controller_cpp = (ROOT / "Source/METROVERSE/Private/MetroversePlayerController.cpp").read_text()
build_cs = (ROOT / "Source/METROVERSE/METROVERSE.Build.cs").read_text()
uproject = (ROOT / "METROVERSE.uproject").read_text()

checks = {
    "Chaos vehicle pawn base": "AWheeledVehiclePawn" in vehicle_h,
    "replicated driver state": "ReplicatedUsing=OnRep_DriverController" in vehicle_h and "DOREPLIFETIME(AMetroverseVehiclePawn, DriverController)" in vehicle_cpp,
    "clamped driving inputs": all(x in vehicle_cpp for x in ("SetThrottleInput", "SetSteeringInput", "SetBrakeInput", "SetHandbrakeInput")),
    "server enter and exit RPCs": all(x in controller_h for x in ("ServerRequestEnterVehicle", "ServerRequestExitVehicle")),
    "server validates entry distance and occupancy": "CanAcceptDriver()" in controller_cpp and "MaximumEntryDistance" in controller_cpp,
    "previous character restored": "PreviousPawnBeforeVehicle" in controller_cpp and "Possess(CharacterPawn)" in controller_cpp,
    "ChaosVehicles module enabled": '"ChaosVehicles"' in build_cs and '"ChaosVehicles", "Enabled": true' in uproject,
}
for name, passed in checks.items():
    print(f"{'PASS' if passed else 'FAIL'}: {name}")
if not all(checks.values()):
    raise SystemExit(1)
print(f"{len(checks)} vehicle source checks passed. This is static source verification, not an Unreal compile or runtime test.")
