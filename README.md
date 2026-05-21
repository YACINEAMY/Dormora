# University Dormitory & Restaurant Management System

This is the backend foundation for the OOP project described in the assignment PDF. It is written in C++ with Qt Core only, so a Qt Widgets or QML graphical interface can be added later without changing the domain model.

## Implemented Backend

- Student management with ID, full name, academic year, and accommodation status.
- Dormitory management with multiple dormitories, capacity limits, rooms, assignment, and removal.
- Room management with room number, capacity, occupancy tracking, and over-occupancy prevention.
- Restaurant management as composition: every dormitory owns exactly one restaurant.
- Daily menus for breakfast, lunch, and dinner.
- Optional meal count tracking per date.
- JSON save/load persistence for students, dormitories, rooms, restaurants, menus, and assignments.
- Atomic file writes and integrity checks before saving or after loading data.
- Domain validation through `DomainError`.
- Backend tests covering each component plus assignment, capacity checks, resident-only restaurant access, meal counts, persistence, and invalid persisted data.

## Build

From this folder in PowerShell:

```powershell
$env:PATH='C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;' + $env:PATH
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\6.10.2\mingw_64
cmake --build build
ctest --test-dir build --output-on-failure
.\build\udrms_demo.exe
.\build\udrms_gui.exe
```

## Structure

- `include/udrms`: public backend headers.
- `src`: implementation and console demo entrypoint.
- `tests`: backend test executable.
- `docs/uml-class-diagram.puml`: UML class diagram source.
- `udrms_gui`: basic Qt Widgets mockup executable for future GUI work.

## Login Credentials

The GUI starts with role-detecting login. There is no role dropdown.

- Admin: `admin` / `admin123`
- Student: any seeded student ID, for example `S1001` / `student123`

Admins get the full dashboard, student management, and restaurant editor. Students get a restricted portal showing only their accommodation and menu access.

## GUI Integration Later

The GUI should call `University` as the main service object. Forms can catch `DomainError` and display its message to the user. Persistence should stay behind `JsonStorage`, so GUI screens do not need to know the JSON format.

The current GUI is intentionally a mockup. It uses seeded sample data and exercises the real backend for adding students, assigning rooms, removing assignments, and editing daily restaurant menus.
