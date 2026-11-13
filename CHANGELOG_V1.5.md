# METROVERSE RP — V1.5 Starter Playable Slice (source-level)

## Added
- GameMode startup bootstrap creates a PlayerStart, large ground plane, Metro City greybox generator and starter delivery job in an empty level.
- Temporary primitive avatar components (torso, head, arms and legs) make the player visible without a custom skeletal mesh. The placeholder avatar is hidden in first-person camera mode.
- `E` interaction input: accept the nearest delivery contract when close to its pickup; press `E` again to request completion while a contract is active. Existing server-side timing, pickup/drop-off proximity, expiry and reward checks remain authoritative.
- On-screen control hints and job result feedback.

## Controls
- WASD: move
- Mouse: look
- Left Shift: sprint
- Space: jump
- V: first-/third-person camera
- E: accept/complete delivery job

## Validation status
- Source changes and configuration were inspected and packaged.
- Unreal Engine 5.6 is not installed in this build environment; no C++ compile, editor launch, PIE/standalone session, packaged executable, vehicle driving or multiplayer test was performed.
- Treat this as a source-level playable-slice implementation awaiting engine validation, not as a confirmed playable build.

## Known limitations
- One greybox city and one delivery contract only.
- Placeholder primitive avatar; no skeletal animation, character customization, finished environment art, audio or UI widgets.
- Vehicle framework remains a foundation and is not yet drivable out of the box.
- Progress is in-session only; no account/backend persistence or public multiplayer server.
