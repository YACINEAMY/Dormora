# Dormora

<p align="center">
  <img src="frontend/resources/icons/dormora-logo.png" alt="Dormora Logo" width="140">
</p>

<p align="center">
  <strong>University Dormitory and Restaurant Management System</strong>
</p>

<p align="center">
  C++17 · Qt Widgets · CMake · JSON Persistence · CTest
</p>

---

## Overview

**Dormora** is a C++ / Qt desktop application for managing a university dormitory and restaurant system.

It was built as an Object-Oriented Programming coursework project, with a focus on clear class design, data validation, local persistence, and a usable graphical interface.

The system allows administrators to manage students, dormitories, rooms, room assignments, restaurants, menus, meal counts, and scoped admin access. Students can also sign in to view their accommodation details, today’s dining cards, and the next 7 days of menus for their assigned residence hall.

---

## App Preview

<p align="center">
  <img src="extras/docs/screenshots/login.png" alt="Dormora Login Screen" width="700">
</p>

<p align="center">
  <img src="extras/docs/screenshots/admin-dashboard.png" alt="Dormora Admin Dashboard" width="700">
</p>

<p align="center">
  <img src="extras/docs/screenshots/student-dashboard.png" alt="Dormora Student Dashboard" width="700">
</p>

---

## Project Highlights

* C++17 backend organized into separate domain classes.
* Qt Widgets graphical interface with login pages, dashboards, forms, tables, dialogs, and validation messages.
* Local JSON persistence for students, dormitories, rooms, restaurants, menus, admin accounts, and application state.
* Admin and student login routing.
* Dormitory neighborhoods with scoped administrator access.
* Room capacity validation to prevent over-occupancy.
* Daily breakfast, lunch, and dinner menu management.
* Student dining dashboard showing today’s meals and the next 7 days of menus.
* Served-meal counter per dormitory and date.
* Bundled meal images matched to menu names.
* Automated backend and GUI self-tests using CTest.

---

## Main Features

### Student Management

* Add, edit, duplicate, search, and delete students.
* Store student ID, full name, academic year, and accommodation status.
* Assign students to dormitory rooms.
* Remove students from assigned rooms.
* View student profiles with room, dormitory, and access information.

### Dormitory and Room Management

* Manage multiple dormitories.
* Store room numbers and capacities.
* Track room occupancy.
* Display available, occupied, and full rooms.
* Enforce room capacity before assigning students.

### Restaurant Management

* Link each dormitory to its own restaurant.
* Manage daily breakfast, lunch, and dinner menus.
* Display today’s menu to residents based on their assigned dormitory.
* Show the next 7 days of menus.
* Track meals served per dormitory and date.
* Use bundled meal images for common menu items.

### Administration

* Global administrator account with full access.
* Scoped administrators with access to specific campus neighborhoods.
* Admin account creation from the GUI.
* Access checks before changing students, rooms, menus, or neighborhoods.

---

## Technology Stack

| Category        | Technology                    |
| --------------- | ----------------------------- |
| Language        | C++17                         |
| GUI Framework   | Qt Widgets                    |
| Build System    | CMake                         |
| Persistence     | JSON files using Qt JSON APIs |
| Testing         | CTest                         |
| Platform Tested | Windows                       |
| Qt Version Used | Qt 6.10.2 with MinGW          |

---

## Repository Layout

```text
backend/
  include/udrms/     Domain class headers
  src/               Backend implementation and console demo
  tests/             Backend unit tests

frontend/
  src/               Qt Widgets GUI entry point
  resources/         Icons and bundled meal images

extras/
  docs/              Supporting documentation for presentation/submission
  examples/          Example JSON data
  scripts/           Packaging helper scripts

CMakeLists.txt       Main CMake configuration
README.md            Project overview and setup guide
Launch Dormora.bat   Quick launcher for the built GUI
```

---

## UML Class Diagram

The UML class diagram is available in several formats:

* `extras/docs/uml-class-diagram.pdf` for direct upload or printing.
* `extras/docs/uml-class-diagram.drawio` for editing in Draw.io / diagrams.net.
* `extras/docs/uml-class-diagram.svg` and `.puml` as source/export formats.

---

## Requirements

* CMake 3.21 or newer.
* Qt 6 with the Core and Widgets modules.
* A C++17 compiler compatible with Qt.

The project was developed and tested on Windows using Qt `6.10.2` with MinGW.

---

## Build and Run

From PowerShell:

```powershell
$env:PATH='C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;' + $env:PATH
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\6.10.2\mingw_64
cmake --build build
.\build\udrms_gui.exe
```

You can also launch the built application using:

```powershell
.\Launch Dormora.bat
```

---

## Run Tests

```powershell
ctest --test-dir build --output-on-failure
```

Useful direct commands:

```powershell
.\build\udrms_tests.exe
.\build\udrms_demo.exe
.\build\udrms_gui.exe --smoke-test
```

---

## Test Credentials

These accounts are included for local coursework demonstration:

| Role               | Username / ID                       | Password             |
| ------------------ | ----------------------------------- | -------------------- |
| Global admin       | `admin`                             | `admin123`           |
| North campus admin | `northadmin`                        | `north123`           |
| South campus admin | `southadmin`                        | `south123`           |
| Student            | Any student ID, for example `S1001` | Generated in the GUI |

---

## Data Storage

Dormora stores local data in JSON format. This makes the project simple to review, easy to run without a database server, and suitable for coursework submission.

Generated local data and build output are ignored by Git.

Student passwords are generated when a student account is created. The global administrator can also reset a student’s password from the student profile dialog. The new temporary password is shown once and can then be used for student login.

---

## Packaging

After building the app and deploying the Qt runtime files, release packages can be created with:

```powershell
.\extras\scripts\package-release.ps1
```

The script prepares source and binary ZIP files under:

```text
release/
```

---

## Notes

* This is a coursework project, not a production system.
* Passwords are stored only for demonstration and testing.
* A real deployment would require password hashing, stronger authentication, role hardening, and database-backed storage.

---

## Development Note

Most of the core implementation was written manually as part of the coursework project. AI tools were used as support for reviewing code, fixing bugs, checking for missed issues, and improving project documentation.
