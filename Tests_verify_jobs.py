from pathlib import Path

ROOT = Path(__file__).parent
PUBLIC = ROOT / "Source/METROVERSE/Public"
PRIVATE = ROOT / "Source/METROVERSE/Private"

types = (PUBLIC / "MetroverseRPTypes.h").read_text()
ps_h = (PUBLIC / "MetroversePlayerState.h").read_text()
ps_cpp = (PRIVATE / "MetroversePlayerState.cpp").read_text()
pc_h = (PUBLIC / "MetroversePlayerController.h").read_text()
pc_cpp = (PRIVATE / "MetroversePlayerController.cpp").read_text()
contract_h = (PUBLIC / "MetroverseJobContract.h").read_text()
contract_cpp = (PRIVATE / "MetroverseJobContract.cpp").read_text()

for job in ("DeliveryCourier", "BusOperator", "TaxiDriver", "TruckHauler", "AirportGroundCrew"):
    assert job in types and job in contract_cpp, f"Missing job type: {job}"
assert "ServerStartJob" in pc_h and "ServerStartJob_Implementation" in pc_cpp
assert "ServerCompleteJob" in pc_h and "ServerCompleteJob_Implementation" in pc_cpp
assert "IsPlayerAtPickup" in pc_cpp and "IsPlayerAtDropoff" in pc_cpp
assert "MinimumDurationSeconds" in pc_cpp and "MaximumDurationSeconds" in pc_cpp
assert "ServerCompleteJob(int64 CurrencyReward, int32 ExperienceReward)" in ps_h
assert "SoftCurrency += CurrencyReward" in ps_cpp
assert "ExperiencePoints += ExperienceReward" in ps_cpp
assert "DOREPLIFETIME(AMetroversePlayerState, CurrentJob)" in ps_cpp
assert "DOREPLIFETIME(AMetroversePlayerState, ExperiencePoints)" in ps_cpp
assert "bReplicates = true" in contract_cpp
assert "CurrencyReward" in contract_h and "ExperienceReward" in contract_h
print("METROVERSE job-contract static checks: PASS")
