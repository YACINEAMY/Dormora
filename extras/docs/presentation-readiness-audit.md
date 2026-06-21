# Presentation Readiness Audit

Date: 2026-06-21

This audit compares Dormora against the University Dormitory & Restaurant Management System PDF and separates urgent submission requirements from optional improvements.

## Final Verdict

The implementation is ready for review after the release ZIPs are generated from the current workspace. UML class and sequence documentation is included under `extras/docs/`.

The required PDF features are covered:

- C++ OOP backend with separate headers and source files.
- Qt graphical interface.
- Student, room, dormitory, and restaurant management.
- Room capacity checks and assignment rules.
- Restaurant menus connected to dormitory residents, including today's meals and the next 7 days in the student portal.
- Local JSON persistence.
- Error handling through `DomainError` and GUI messages.
- Automated tests through CTest.

## Needed Changes Before Submission

These are the changes that would block or weaken submission if missing.

| Item | Status | Notes |
| --- | --- | --- |
| Remove extra cloud/database code | Done | Project is now offline and focused on the PDF requirements. |
| Provide UML class diagram | Done | `extras/docs/uml-class-diagram.puml` and `.svg`. |
| Provide sequence diagram for a key operation | Done | `extras/docs/uml-room-assignment-sequence.puml`. |
| Map implementation to PDF requirements | Done | `extras/docs/assignment-compliance.md` maps features directly to the PDF. |
| Provide interview explanation/Q&A | Done | `extras/docs/interview-preparation.md` contains oral defense answers. |
| Verify build and tests | Done | Run `ctest --test-dir build --output-on-failure`; all tests must pass. |
| Package source for teacher review | Done by packaging script | `extras/scripts/package-release.ps1` creates `release/Dormora-source.zip`. |
| Package executable for direct testing | Done by packaging script | `release/Dormora-binary.zip` includes the executable and Qt runtime files. |

No blocker remains for the coursework repository submission.

## Better If Done Later

These are improvements that would make the project cleaner, but they are not required by the PDF and should not delay today's submission.

| Improvement | Why it helps | Risk today |
| --- | --- | --- |
| Split `frontend/src/gui_main.cpp` into smaller GUI classes | Easier long-term maintenance. | Medium; could introduce bugs before submission. |
| Add rendered PNG/PDF versions of diagrams | Easier for teachers who do not use PlantUML. | Low, but should happen after the diagrams are finalized. |
| Add password hashing and real authentication | More realistic security. | High; not required for coursework demo credentials. |
| Add database storage | More production-like persistence. | High; the PDF only requires file persistence. |
| Add more granular GUI tests | Better confidence in every click path. | Medium; existing backend and smoke tests already cover core behavior. |
| Add localization or print/export reports | Nice presentation features. | Low value for the current assignment. |

## PDF Requirement Checklist

| PDF requirement | Project evidence |
| --- | --- |
| Model a dormitory system using OOP | `Person`, `Student`, `Room`, `Restaurant`, `Dormitory`, `University`. |
| Encapsulation | Private data members and public validation methods. |
| Composition | `Dormitory` owns rooms and exactly one restaurant. |
| Inheritance | `Student` inherits from `Person`. |
| Basic polymorphism | `Student::toJson()` overrides `Person::toJson()`. |
| Multiple source/header files | `backend/include/udrms/` and `backend/src/`. |
| Qt graphical interface | `frontend/src/gui_main.cpp`. |
| Data structures | `QHash`, `QSet`, `QMap`, `QVector`. |
| Testing | `backend/tests/backend_tests.cpp` and CTest GUI self-tests. |
| Student management | Add, remove, search, edit, assign, and display students. |
| Dormitory management | Multiple dormitories, rooms, capacity, occupied/available states. |
| Room management | Capacity and duplicate assignment checks. |
| Restaurant management | One restaurant per dormitory, daily menus, meal count tracking. |
| Resident menu access | `University::restaurantMenuForStudent`, student portal today's meals, and next-7-days schedule. |
| GUI validation | `DomainError` messages shown through the GUI. |
| UML documentation | `extras/docs/uml-class-diagram.puml`, `.svg`, and `uml-room-assignment-sequence.puml`. |
| File handling | JSON save/load and app-state persistence. |

## How To Present The Project

1. Start with the PDF objective: model dormitories, rooms, students, and restaurant services.
2. Explain composition: `Dormitory` contains `Room` and `Restaurant`.
3. Explain inheritance: `Student` extends `Person`.
4. Run the app and login as `admin` / `admin123`.
5. Show the Residents page: search a student and explain profile/room assignment.
6. Show the Meals page: menus belong to dormitory restaurants.
7. Reset a student password as `admin`, then login with that student ID and generated password to show the student portal, today's meals, and the next 7 days of menus.
8. Mention tests: backend and GUI self-tests pass through CTest.

## Final Verification Commands

From PowerShell:

```powershell
$env:PATH='C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;' + $env:PATH
cmake --build build
ctest --test-dir build --output-on-failure
.\extras\scripts\package-release.ps1
```

Expected result:

- Build finishes successfully.
- CTest reports all tests passed.
- `release/Dormora-source.zip` and `release/Dormora-binary.zip` are created.
