# Assignment Compliance Checklist

This checklist maps the project to the requirements from the professor's University Dormitory & Restaurant Management System PDF.

## Objectives

| PDF objective | Project implementation |
| --- | --- |
| Model a real university dormitory system with OOP | Backend domain classes model students, rooms, dormitories, restaurants, and the university service layer. |
| Apply encapsulation, composition, inheritance, and polymorphism | Fields are private, `Dormitory` owns `Room` and `Restaurant`, `Student` inherits from `Person`, and `Person::toJson()` is virtual. |
| Show restaurant as part of the dormitory | `Dormitory` stores exactly one `Restaurant` object. |
| Organize C++ project with multiple source/header files | Backend headers are in `backend/include/udrms/`; backend implementations are in `backend/src/`; tests are in `backend/tests/`. |
| Provide a graphical user interface | `frontend/src/gui_main.cpp` implements the Qt Widgets GUI with login, admin, and student workflows. |
| Use appropriate data structures | `QHash` supports fast ID lookups, `QMap` stores menus by date, `QSet` tracks room residents, and `QVector` returns stable ordered lists. |
| Test functionality | `backend/tests/backend_tests.cpp` and CTest cover room assignment, capacity limits, restaurant access, persistence, and GUI smoke checks. |
| Create UML diagrams | Pending final diagram review before submission. |

## Required Features

| Feature | Status | Evidence |
| --- | --- | --- |
| Add/remove students | Complete | `University::addStudent`, `University::removeStudent`, GUI resident desk. |
| Store student ID, full name, academic year | Complete | `Student` class and JSON persistence. |
| Assign students to dormitories and rooms | Complete | `University::assignStudentToRoom`, `Dormitory::assignStudent`. |
| Display student details and accommodation status | Complete | Admin resident table and student portal. |
| Manage multiple dormitories | Complete | `University` owns multiple `Dormitory` objects. |
| Define dormitory capacity and rooms | Complete | `Dormitory::capacity`, `Dormitory::addRoom`. |
| Assign/remove students from rooms | Complete | `assignStudentToRoom` and `removeStudentFromRoom`. |
| Display occupied and available rooms | Complete | `Dormitory::occupiedRooms`, `Dormitory::availableRooms`, GUI tables. |
| Track room occupancy and prevent over-occupancy | Complete | `Room::addStudent` validates capacity; backend tests verify this. |
| One restaurant per dormitory | Complete | `Dormitory` contains one `Restaurant`. |
| Manage daily breakfast, lunch, and dinner menus | Complete | `Restaurant::setMenu`, GUI meal desk. |
| Display meal images | Complete | Bundled Qt resources under `frontend/resources/meal-*.png` and GUI meal cards. |
| Let residents view menus | Complete | `University::restaurantMenuForStudent`, student portal today's dining cards, and next-7-days schedule. |
| Track meals served per day | Complete | Optional requirement implemented through `recordMealServed`. |
| Validate input and handle incorrect entries | Complete | Domain exceptions and GUI status/error messages. |
| Save/load data across executions | Complete | `JsonStorage` and local app-state persistence. |

## Evaluation Criteria

- **Functionality:** covered by backend logic, GUI workflows, and tests.
- **OOP principles:** visible in the class design, ownership boundaries, and validation methods.
- **Graphical interface:** Qt Widgets provides forms, buttons, tables, dialogs, and clear navigation.
- **UML documentation:** pending final diagram review before submission.
- **Code organization:** backend files are split by class; GUI, resources, helper scripts, examples, and docs are separated.
- **Error handling:** invalid IDs, capacities, assignments, menus, JSON, and access attempts throw `DomainError` and are surfaced in the GUI.
- **Performance:** ID-based `QHash` lookups and set/map containers avoid slow full-list scans for common operations.

## Notes

- The app works offline through local JSON persistence.
- Demo credentials are intended for coursework demonstration only, not production deployment.
