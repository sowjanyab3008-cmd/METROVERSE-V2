# METROVERSE RP V1.4.1 — Test hardening patch

## Fixes
- Resale cap calculation now divides before multiplying, avoiding signed 64-bit overflow for unusually large purchase-price values while preserving floor(70%).
- Vehicle sale lookup now compares vehicle IDs case-insensitively, matching the duplicate-ID policy used at purchase.
- Expanded the dealership economy static test to cover both regressions.

## Verification scope
- Included Python source/data validation scripts were run against this package.
- These checks do not compile Unreal C++, launch Unreal Editor, import DataTables, or exercise gameplay/network behavior. Unreal Engine 5.6 and a compatible Windows C++ toolchain are still required for those tests.
