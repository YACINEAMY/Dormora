# Dormora Interview Preparation

This file is written as a study guide for an oral review or project interview. It explains the project in plain language and gives direct answers to questions that may come up.

## Short Project Explanation

Dormora is a C++ and Qt implementation of a University Dormitory and Restaurant Management System. It manages students, dormitories, rooms, room assignment, restaurant menus, meal access, and saved data. The backend is separated into OOP classes such as `Student`, `Room`, `Restaurant`, `Dormitory`, and `University`, while the frontend is a Qt Widgets desktop application.

The main idea is that a university owns multiple dormitories, each dormitory owns rooms and exactly one restaurant, and students can be assigned to rooms. Restaurant access is linked to accommodation: only assigned residents can access their dormitory restaurant menu.

## One-Minute Presentation

My project is called Dormora. It models a university dormitory and restaurant management system using C++ and Qt. The backend is organized around domain classes: `Student` inherits from `Person`, `Dormitory` contains rooms and one restaurant, `Room` controls occupancy, `Restaurant` stores daily menus, and `University` coordinates the whole system. The GUI lets an admin manage students, room assignments, neighborhoods, and restaurant menus, while a student account can view accommodation and meal information. Data is saved locally as JSON, so changes persist between executions. The project also includes tests for important rules such as room capacity, assignment consistency, restaurant access, and JSON persistence.

## Important Files To Show

- `backend/include/udrms/Student.h` and `backend/src/Student.cpp`: student profile and assignment data.
- `backend/include/udrms/Room.h` and `backend/src/Room.cpp`: room capacity and occupancy rules.
- `backend/include/udrms/Dormitory.h` and `backend/src/Dormitory.cpp`: dormitory composition with rooms and restaurant.
- `backend/include/udrms/Restaurant.h` and `backend/src/Restaurant.cpp`: daily menus and meals served.
- `backend/include/udrms/University.h` and `backend/src/University.cpp`: main service layer.
- `frontend/src/gui_main.cpp`: Qt Widgets GUI.
- `backend/tests/backend_tests.cpp`: automated backend and rule tests.
- `extras/docs/assignment-compliance.md`: requirement checklist mapped to the PDF.

## How The Main Classes Work

### `Person`

`Person` is a base class for shared identity data. It stores an ID and full name. `Student` inherits from it, which demonstrates inheritance and avoids repeating identity fields.

### `Student`

`Student` stores academic year and optional accommodation assignment. The assignment is optional because a student can exist in the system before being assigned to a dormitory room.

### `Room`

`Room` stores a room number, capacity, and a set of student IDs. It prevents duplicate students and prevents adding students when the room is full.

### `Restaurant`

`Restaurant` stores daily menus using dates. Each menu has breakfast, lunch, dinner, and optional image paths. It also tracks the number of meals served per day.

### `Dormitory`

`Dormitory` owns rooms and one restaurant. This is composition: the restaurant is part of the dormitory, which matches the assignment PDF.

### `University`

`University` is the coordinator class. It stores all students and dormitories and exposes operations like adding students, assigning rooms, removing students from rooms, setting menus, and validating data integrity.

### `JsonStorage` and `AppState`

`JsonStorage` saves and loads the core `University` data. `AppState` supports the GUI state, such as neighborhoods and admin profiles. This gives persistence across executions.

## OOP Concepts

### Encapsulation

Class data is private. Other code must use public methods such as `addStudent`, `assignStudentToRoom`, `setMenu`, or `addRoom`. These methods validate input before changing state.

### Inheritance

`Student` inherits from `Person`. `Person` contains shared identity behavior, and `Student` adds academic year and room assignment.

### Composition

`Dormitory` contains `Room` objects and exactly one `Restaurant`. These are parts of the dormitory, so composition is the correct relationship.

### Polymorphism

`Person::toJson()` is virtual and `Student::toJson()` overrides it. This shows basic polymorphism through a base class function with derived behavior.

## Data Structures

- `QHash<QString, Student>`: fast student lookup by ID.
- `QHash<QString, Dormitory>`: fast dormitory lookup by ID.
- `QHash<int, Room>`: fast room lookup by room number inside a dormitory.
- `QSet<QString>`: room resident IDs without duplicates.
- `QMap<QDate, DailyMenu>`: restaurant menus sorted by date.
- `QVector`: stable ordered lists for GUI display, JSON output, and tests.

These choices support the PDF requirement about scalability and good performance.

## Room Assignment Flow

1. Admin selects a student, dormitory, and room in the GUI.
2. GUI calls `University::assignStudentToRoom`.
3. `University` finds the target student and dormitory.
4. `Dormitory::assignStudent` finds the room.
5. `Room::addStudent` checks capacity and duplicates.
6. If accepted, `Student::assignToRoom` stores the assignment.
7. GUI refreshes the student and room views.

If a room is full or a student/room does not exist, a `DomainError` is thrown and the GUI displays an error message.

## Restaurant Access Flow

Restaurant access is tied to room assignment. `University::restaurantMenuForStudent` first checks whether the student is assigned to a dormitory. If not, it throws an error. If assigned, it returns the menu from that student's dormitory restaurant.

This reflects the PDF requirement that the restaurant is part of the dormitory and serves resident students.

## Persistence

Data is saved as JSON. The saved data includes students, dormitories, rooms, room memberships, restaurant menus, meal counts, neighborhoods, admin profiles, and student portal password. When data is loaded, `University::validateIntegrity()` checks that student assignments and room memberships match.

## Testing

The project uses CTest. `backend/tests/backend_tests.cpp` verifies:

- Student validation.
- Room capacity and duplicate prevention.
- Dormitory assignment and removal.
- Restaurant menus and meal counts.
- Restaurant access restrictions.
- JSON persistence and corrupted JSON handling.

GUI self-tests verify startup, persistence, seed data, dropdowns, credential routing, admin permissions, restaurant access, and filters.

## Precise Questions And Answers

### Why did you create a `University` class?

`University` acts as the service layer. It coordinates students, dormitories, room assignment, restaurant access, and integrity validation. Without it, the GUI would need to directly manipulate many classes and would become harder to maintain.

### Why does `Student` inherit from `Person`?

Because a student is a type of person and shares identity fields such as ID and full name. `Student` adds academic year and accommodation assignment.

### Where is composition used?

`Dormitory` owns its rooms and its restaurant. This means `Dormitory` has a `QHash<int, Room>` and a `Restaurant` member. This matches the PDF statement that the restaurant is part of the dormitory.

### How do you prevent room over-occupancy?

`Room::addStudent` checks `isFull()` before inserting a student ID. If the room is full, it throws `DomainError`.

### How do you prevent assigning the same student twice to one room?

`Room` stores resident IDs in a `QSet`, and `Room::addStudent` rejects a student ID that already exists in the set.

### How do you keep the `Student` assignment and `Room` membership consistent?

Assignment goes through `Dormitory::assignStudent`, which updates both sides: it adds the student ID to the room and then calls `Student::assignToRoom`. Removal also updates both sides. Loaded data is checked with `University::validateIntegrity()`.

### What happens if a student is removed?

`University::removeStudent` first removes the student from their assigned room if they have one, then removes the student record from the student map.

### Why use `QHash` instead of a simple array?

Student and dormitory operations are usually ID-based. `QHash` gives fast lookup by ID, while an array would require scanning every element.

### Why use `QMap` for menus?

Menus are stored by date. `QMap<QDate, DailyMenu>` keeps dates sorted and makes retrieving a menu for a specific date clear.

### Why use `QSet` in `Room`?

A room should not contain the same student twice. `QSet` naturally prevents duplicate IDs and makes membership checks efficient.

### What is `DomainError`?

`DomainError` is the project-specific exception used when a business rule is violated, such as empty student names, invalid room capacity, full rooms, invalid menus, or unauthorized restaurant access.

### How does the GUI handle invalid input?

Backend methods throw `DomainError`. The GUI catches these errors and displays clear messages to the user instead of crashing.

### How is the restaurant connected to the dormitory?

Each `Dormitory` object contains exactly one `Restaurant` object. Menus are accessed through the assigned student's dormitory.

### Can an unassigned student view a restaurant menu?

No. `University::restaurantMenuForStudent` checks assignment first. If the student is not assigned to a dormitory, it throws an error.

### Why did you use Qt?

The assignment suggested C++ with a graphical framework such as Qt. Qt Widgets provides windows, forms, tables, buttons, dialogs, validation messages, and resource handling for images.

### Where is data saved?

The GUI saves app state to `udrms-data.json` next to the executable. The backend also has `JsonStorage` for saving and loading `University` data.

### What does `validateIntegrity()` check?

It checks that room membership and student assignment data agree. For example, if a student says they are in room 101, room 101 must also contain that student ID.

### What is the difference between aggregation and composition here?

Composition means the part belongs strongly to the whole, like rooms and restaurant inside a dormitory. Aggregation is weaker: `University` manages collections of students and dormitories, but conceptually those are separate domain objects.

### What tests prove the system works?

The backend tests prove validation, capacity rules, restaurant access, persistence, and data integrity. GUI self-tests prove the main workflows start and behave correctly.

### Is this production-ready authentication?

No. The login credentials are local demo credentials for coursework. In production, passwords should be hashed and stored in a secure authentication system.

### Why are meal images bundled?

Bundled Qt resources make the app work offline. The teacher can run the binary without internet access and still see meal images.

### What would you improve if you had more time?

I would split the large GUI file into smaller widget/controller files, add real authentication, add role management screens, and add a database backend. The current version stays focused on the OOP assignment requirements.

### Why is the GUI in one file?

For a small coursework desktop app, one GUI file keeps the build simple. The backend is properly separated into multiple source/header files, which is the most important part for the OOP model.

### Where is polymorphism visible?

`Person::toJson()` is virtual, and `Student::toJson()` overrides it. This allows derived behavior through the base class interface.

### Why do you sort returned lists?

Sorted output makes the GUI predictable, JSON files stable, and tests reliable.

### How do you know the project fulfills the PDF?

`extras/docs/assignment-compliance.md` maps each PDF requirement to a code feature or document. The project includes OOP classes, Qt GUI, local persistence, tests, data structures, and error handling. UML documentation is the remaining presentation document to finalize.

## Quick Demo Path

1. Open the app.
2. Login as admin: `admin` / `admin123`.
3. Show the Residents page and search for a student.
4. Open a student profile and explain room assignment.
5. Show the Meals page and explain that menus belong to dormitory restaurants.
6. Login as student `S1001` / `student123` and show resident-only menu access.
