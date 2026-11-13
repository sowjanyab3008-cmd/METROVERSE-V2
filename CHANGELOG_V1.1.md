# METROVERSE RP V1.1 — life and faction foundations

- Added replicated owned-vehicle records and server-side purchase/sale mutation methods.
- Added 24 configurable life-activity definitions and a DataTable row schema.
- Added faction application and promotion request data, server-side submission/review methods, and audit-friendly reviewer notes.
- Added a 50-row proposed rank progression table for Government, Police, Hospital, News Network and FIB, plus its DataTable row schema.
- Added validation test and setup documentation.

Validation: Python data/source-presence checks passed. Unreal Engine compilation, UHT validation, runtime behavior, networking, persistence, UI, vehicle assets and packaging have not been tested.

## V1.2 immediate progression update
- Added replicated lifetime count for completed life activities.
- Added per-activity server-side cooldown timestamps and reward completion API.
- Added validated faction service-minute tracking (trusted server records up to 60 minutes per call).
- Promotion requests now accept the minimum service hours, completed-activity count, and maximum active reprimands from the faction/rank rules DataTable and enforce those thresholds before submitting.
- These are C++ gameplay foundations; the DataTable loading/lookup caller, UI, save persistence, and in-game testing remain integration work.
