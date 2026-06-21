# Project Report

## Project Name

Dormora - University Dormitory & Restaurant Management System

## Purpose

This project manages student accommodation and dormitory restaurant services. It keeps track of students, dormitories, rooms, room assignments, daily menus, meal photos, and meal access. The system was built for the Object-Oriented Programming assignment and follows the structure required in the project PDF.

## Main Classes

- `Person`: base class for shared identity information.
- `Student`: stores student ID, full name, academic year, and optional room assignment.
- `Room`: stores room number, capacity, and resident IDs.
- `Restaurant`: stores daily breakfast, lunch, dinner menus, meal image paths, and meals served.
- `Dormitory`: owns rooms and one restaurant.
- `University`: coordinates students, dormitories, room allocation, restaurant access, and integrity validation.
- `JsonStorage`: saves and loads university data.

## OOP Concepts Used

- **Encapsulation:** class data is private and changed through validation methods.
- **Inheritance:** `Student` inherits from `Person`.
- **Composition:** `Dormitory` contains `Room` objects and one `Restaurant`.
- **Polymorphism:** `Person::toJson()` is virtual and overridden by `Student`.

## Data Structures

- `QHash` is used for fast lookup of students, dormitories, and rooms by ID or room number.
- `QSet` stores student IDs inside a room without duplicates.
- `QMap` stores restaurant menus by date in sorted order.
- `QVector` returns stable lists for the GUI, JSON output, and tests.

## User Interface

The GUI is built with Qt Widgets. It includes:

- Admin login and student login.
- Dashboard summaries.
- Student management and search.
- Room assignment workflows.
- Dormitory and restaurant menu views with bundled meal photos.
- Student portal for accommodation and meal information.
- Validation messages for invalid or incomplete actions.

## Persistence

The app saves and loads data through JSON so changes remain available across program executions. Default menu images are stored as Qt resources so the app can show meal photos without an internet connection.

## Testing

The backend tests verify:

- Student validation.
- Room capacity and duplicate prevention.
- Dormitory room assignment and removal.
- Restaurant menu validation and meal counting.
- Restaurant access only for assigned residents.
- JSON save/load and invalid JSON handling.

The Qt GUI also has smoke tests and workflow self-tests registered in CTest.

## Conclusion

Dormora satisfies the main implementation requirements by combining a structured C++ OOP backend, a functional Qt graphical interface, persistent data storage, bundled visual resources, and automated tests for the most important rules.
