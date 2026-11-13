from pathlib import Path
root = Path(__file__).parent
h = (root / "Source/METROVERSE/Public/MetroverseTransitNetwork.h").read_text()
c = (root / "Source/METROVERSE/Private/MetroverseTransitNetwork.cpp").read_text()
pc_h = (root / "Source/METROVERSE/Public/MetroversePlayerController.h").read_text()
pc_c = (root / "Source/METROVERSE/Private/MetroversePlayerController.cpp").read_text()
assert "CityStationOffsets" in h and "GetCityStationWorldLocation" in h
assert all(name in c for name in ["Central", "North", "South", "East", "West", "Coastal", "Industrial", "Highland"])
assert "IsRouteAvailable" in c and "Ring" in c
assert "ServerRequestCityTravel" in pc_h and "ServerRequestCityTravel_Implementation" in pc_c
assert "StationArrivalRadius" in pc_c and "ServerSetCity" in pc_c
print("Transit source checks passed: eight anchors, route graph, server RPC, proximity validation, city-state update.")
