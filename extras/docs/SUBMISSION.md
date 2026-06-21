# Dormora Submission Guide

## What To Send

Send these two files from the `release/` folder:

- `Dormora-source.zip` - source code, documentation, examples, and build scripts.
- `Dormora-binary.zip` - runnable Windows build with the Qt DLLs and plugins needed by the executable.

The binary package includes `Launch Dormora.bat`. Double-click it to start the app.

## Project Summary

Dormora is a C++/Qt implementation of a University Dormitory and Restaurant Management System. It manages students, dormitories, rooms, restaurant menus, room assignment, resident meal access, next-week student dining schedules, and local data persistence.

The code is separated into:

- `backend/` for the OOP domain model and tests.
- `frontend/` for the Qt Widgets interface and bundled visual resources.
- `extras/docs/` for the project report, requirement mapping, and interview preparation.
- `extras/scripts/` for optional helper scripts.

## Local Test Logins

| Role | Username | Password |
| --- | --- | --- |
| Global admin | `admin` | `admin123` |
| North campus admin | `northadmin` | `north123` |
| South campus admin | `southadmin` | `south123` |
| Student | Any student ID, for example `S1001` | Generated or reset by the global admin |

Student passwords are not shared demo passwords. To test a student account, login as `admin`, open the student's profile, use **Reset student password**, and sign in with the temporary password shown by the GUI.

## How To Build From Source

Requirements:

- CMake 3.21 or newer.
- Qt 6 with Core and Widgets.
- A C++17 compiler compatible with the installed Qt version.

PowerShell example for Qt 6.10.2 MinGW:

```powershell
$env:PATH='C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;' + $env:PATH
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\6.10.2\mingw_64
cmake --build build
ctest --test-dir build --output-on-failure
```

Run the app:

```powershell
.\build\udrms_gui.exe
```

## How To Recreate The Release ZIPs

After building and deploying Qt runtime files, run:

```powershell
.\extras\scripts\package-release.ps1
```

This recreates:

- `release/Dormora-source.zip`
- `release/Dormora-binary.zip`

## Main Review Files

- `backend/include/udrms/*.h` - class interfaces.
- `backend/src/*.cpp` - class implementations.
- `frontend/src/gui_main.cpp` - Qt Widgets user interface.
- `backend/tests/backend_tests.cpp` - backend component tests.
- `extras/docs/project-report.md` - concise written report.
- `extras/docs/assignment-compliance.md` - requirement checklist.
- `extras/docs/presentation-readiness-audit.md` - final required-vs-optional change list.
- `extras/docs/interview-preparation.md` - oral review explanations and likely Q&A.
- `extras/docs/uml-class-diagram.pdf` - UML class diagram ready to upload.
- `extras/docs/uml-class-diagram.drawio` - editable UML class diagram for Draw.io / diagrams.net.
- `extras/docs/uml-class-diagram.puml` and `.svg` - UML class diagram source/export formats.
- `extras/docs/uml-room-assignment-sequence.puml` - optional room assignment sequence diagram.

## Notes

The app works offline through local JSON data. No external database or web service is required for review.
