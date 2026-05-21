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

## Needs Change

1. Split `src/gui_main.cpp`.
   The file is over 2,000 lines and mixes styling, authentication, seeded data, layout building, permissions, and actions. Move it into smaller files such as `DormitoryWindow`, `SeedData`, `AdminPages`, `StudentPages`, and `UiHelpers`.

2. Add real persistence to the GUI.
   The GUI currently seeds data in memory on every start. User edits disappear when the app closes. Connect startup/shutdown or explicit Save/Load actions to `JsonStorage`.

3. Validate JSON field types more strictly.
   The `fromJson()` methods rely heavily on Qt defaults such as empty strings and zero values. Invalid data often fails later, but the error messages are indirect. Add helpers that require fields and expected JSON types.

4. Replace hard-coded credentials before real use.
   `admin/admin123`, scoped admin passwords, and `student123` are embedded in `gui_main.cpp`. That is acceptable for a demo, but not for production or a submitted app that claims real authentication.

5. Improve GUI status feedback.
   `showStatus()` only stores the message in the window tooltip. Either wire a visible `QStatusBar`/label or remove the method and show feedback consistently near the active form.

6. Add GUI-focused tests.
   The backend is tested, but permission rules, login routing, neighborhood access, and GUI actions have only smoke-test coverage.

7. Avoid checked-in build artifacts.
   The `build` folder contains generated binaries, DLLs, screenshots, and CMake files. Keep source, docs, and sample data in the project; regenerate build outputs locally.

## Optimization Opportunities

- Cache visible dormitory/student lists during one refresh pass instead of recomputing them across multiple refresh methods.
- Avoid copying full `Dormitory`, `Room`, and `Student` objects in GUI table refreshes when references or IDs are enough.
- Extract repeated card/table/widget setup into focused helper classes once `gui_main.cpp` is split.
- Keep `validateIntegrity()` as the safety net, but avoid calling it inside hot UI refresh paths unless data was actually mutated.
