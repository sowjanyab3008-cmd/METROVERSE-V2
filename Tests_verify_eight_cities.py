from pathlib import Path
root = Path(__file__).parent
types = (root / "Source/METROVERSE/Public/MetroverseRPTypes.h").read_text()
header = (root / "Source/METROVERSE/Public/MetroverseCityGenerator.h").read_text()
source = (root / "Source/METROVERSE/Private/MetroverseCityGenerator.cpp").read_text()
expected = ["Central", "North", "South", "East", "West", "Coastal", "Industrial", "Highland"]
for city in expected:
    assert city in types, f"Missing city enum: {city}"
    assert f"EMetroverseCity::{city}" in source, f"Missing city preset: {city}"
for fn in ["GetCityDisplayName", "GetCityLandmark", "GetCityTheme", "GetCityPreset"]:
    assert fn in header and fn in source, f"Missing city API: {fn}"
assert "CityToGenerate" in header
assert "PresetBlocks" in source and "PresetSeed" in source
print("PASS: eight city enum values and eight named presets present")
print("PASS: city display, landmark and theme APIs present")
print("PASS: per-city deterministic greybox settings present")
print("NOTE: static source checks only; Unreal compile and runtime tests not performed")
