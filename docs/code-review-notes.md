# Code Review Notes

Review date: 2026-05-21

## Current Structure

- `include/udrms` exposes the backend domain model and persistence boundary.
- `src/*` implements the backend, console demo, and Qt Widgets GUI.
- `tests/backend_tests.cpp` is a lightweight executable test suite for the backend.
- `docs/uml-class-diagram.puml` documents the domain relationships.

## What Looks Good

- The backend has clear domain objects: `University`, `Dormitory`, `Room`, `Student`, `Restaurant`, and `JsonStorage`.
- Room assignment is guarded by domain rules and cross-checked with `University::validateIntegrity()`.
- Persistence uses `QSaveFile`, which is safer than overwriting the JSON file directly.
- Sorted accessors keep test output and JSON output stable even though the storage uses hashes/sets.
- The GUI calls the real backend for core student, room, and menu actions.

## Changes Made During Review

- Added public API comments to the backend headers so future readers can understand ownership and invariants quickly.
- Tightened `Restaurant::setMenu()` so a saved menu must include breakfast, lunch, and dinner.
- Added a backend test that catches incomplete menu data.
- Added strict JSON field/type checks in backend loaders.
- Added GUI app-state persistence for university data, neighborhoods, admin profiles, and the student portal password.
- Added a GUI persistence smoke test.
- Made GUI action status messages visible through the main-window status bar.

## GUI Aesthetic Refresh

- Normalized card, input, button, and pill styling.
- Polished sidebar and window chrome.
- Improved table density and selected-row styling.
- Reworked Resident Desk spacing and profile scrolling.
- Balanced dashboard, meal desk, student portal, and login layouts.

Remaining design debt:

- `src/gui_main.cpp` should still be split into focused UI modules.
- Real icon assets would improve the Dormora brand mark and window controls.
- More GUI behavior tests should cover permission and form flows.

## Needs Change

1. Split `src/gui_main.cpp`.
   The file is still over 2,000 lines and mixes styling, authentication, seeded data, layout building, permissions, and actions. Move it into smaller files such as `DormitoryWindow`, `SeedData`, `AdminPages`, `StudentPages`, and `UiHelpers`.

2. Replace demo authentication before production use.
   Credentials are now persisted in the app-state file and no longer exposed in the login error message, but first-run defaults still exist for the class demo. A real deployment should hash passwords and use per-user student credentials.

3. Add broader GUI-focused tests.
   The backend is tested, and persistence now has a smoke test, but permission rules, login routing, neighborhood access, and GUI actions still need direct automated coverage.

4. Avoid checked-in build artifacts.
   The `build` folder contains generated binaries, DLLs, screenshots, and CMake files. Keep source, docs, and sample data in the project; regenerate build outputs locally.

## Optimization Opportunities

- Cache visible dormitory/student lists during one refresh pass instead of recomputing them across multiple refresh methods.
- Avoid copying full `Dormitory`, `Room`, and `Student` objects in GUI table refreshes when references or IDs are enough.
- Extract repeated card/table/widget setup into focused helper classes once `gui_main.cpp` is split.
- Keep `validateIntegrity()` as the safety net, but avoid calling it inside hot UI refresh paths unless data was actually mutated.
