# METROVERSE RP V1.4 — Dealership economy safeguards

## Added
- Purchase price stored on every owned vehicle record.
- Case-insensitive duplicate vehicle ID and registration plate protection.
- Vehicle must be stored in a garage before it can be sold.
- Resale amount is capped at 70% of the recorded purchase price.
- Dealership UMG integration notes and source validation test.

## Validation
- Run `python Tests/verify_v1_4_dealership_economy.py`.
- This is static source validation only; no Unreal compile or gameplay test has been performed.
