from pathlib import Path
root = Path(__file__).parent
checks = {
    'custom player controller source': root/'Source/METROVERSE/Private/MetroversePlayerController.cpp',
    'custom player controller header': root/'Source/METROVERSE/Public/MetroversePlayerController.h',
    'player state source': root/'Source/METROVERSE/Private/MetroversePlayerState.cpp',
    'chat channel definitions': root/'Source/METROVERSE/Public/MetroverseRPTypes.h',
    'procedural city generator source': root/'Source/METROVERSE/Private/MetroverseCityGenerator.cpp',
    'procedural city generator header': root/'Source/METROVERSE/Public/MetroverseCityGenerator.h',
}
for label, path in checks.items():
    assert path.exists(), f'MISSING: {label}'
    assert path.stat().st_size > 0, f'EMPTY: {label}'
controller = checks['custom player controller source'].read_text()
state = checks['player state source'].read_text()
types = checks['chat channel definitions'].read_text()
gamemode = (root/'Source/METROVERSE/Private/MetroverseGameMode.cpp').read_text()
for channel in ['RP', 'NRP', 'Faction', 'SeniorStaff', 'Department', 'Event', 'Action', 'Interview']:
    assert f'EMetroverseChatChannel::{channel}' in state or channel in types, f'Channel policy missing: {channel}'
assert 'ServerSendChatMessage_Implementation' in controller
assert 'CanSendToChatChannel' in controller
assert 'CanReceiveFromChatChannel' in controller
assert 'MaxMessageLength = 240' in (root/'Source/METROVERSE/Public/MetroversePlayerController.h').read_text()
assert 'Now - LastChatSentAt < 0.8' in controller
assert 'AMetroversePlayerController::StaticClass()' in gamemode
assert 'bInterviewReadOnlyObserver' in state
assert 'FMath::Square(2500.0f)' in state
print('PASS: source files present')
print('PASS: all 8 chat channels declared and policy hooks present')
print('PASS: server RPC, sender/recipient checks, rate limit and message cap present')
print('PASS: custom controller assigned in GameMode')
print('PASS: event/interview/proximity restrictions present')
print('NOTE: static source checks only; Unreal compilation/runtime tests not performed')

city = checks['procedural city generator source'].read_text()
city_header = checks['procedural city generator header'].read_text()
for token in ['RebuildDistrict', 'RoadInstances->ClearInstances()', 'BuildingInstances->ClearInstances()', 'FRandomStream', 'BlocksPerSide']:
    assert token in city or token in city_header, f'City generator feature missing: {token}'
assert 'OnConstruction' in city
print('PASS: procedural city blockout generator and editable grid settings present')
