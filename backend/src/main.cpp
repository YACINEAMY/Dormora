#include "udrms/JsonStorage.h"

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QTextStream>

using namespace udrms;

// This executable is a small backend demo, separate from the Qt GUI.
// It builds a sample university in memory, saves it, loads it back, and
// prints a few values to prove the backend can run without the interface.
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    University university;

    // A dormitory owns its rooms and its restaurant. This mirrors the
    // composition relationship shown in the UML diagram.
    Dormitory dormitory("DORM-A", "Central Residence", 4, Restaurant("Central Restaurant"));
    dormitory.addRoom(Room(101, 2));
    dormitory.addRoom(Room(102, 2));

    // University coordinates records across dormitories and students.
    university.addDormitory(dormitory);
    university.addStudent(Student("STU-001", "Amina Benali", 1));
    university.addStudent(Student("STU-002", "Karim Haddad", 2));

    // Assignment goes through University so room capacity and student state
    // stay synchronized.
    university.assignStudentToRoom("STU-001", "DORM-A", 101);
    university.assignStudentToRoom("STU-002", "DORM-A", 101);

    const QDate today = QDate::currentDate();
    university.setRestaurantMenu("DORM-A", today, {"Coffee and eggs", "Chicken and rice", "Soup and salad"});
    university.recordStudentMeal("STU-001", today);

    // Save and reload once to demonstrate persistence and integrity checks.
    const QString outputPath = QCoreApplication::applicationDirPath() + "/sample-university-data.json";
    JsonStorage::saveUniversity(university, outputPath);

    const University loaded = JsonStorage::loadUniversity(outputPath);
    const Student loadedStudent = loaded.student("STU-001");
    const auto menu = loaded.restaurantMenuForStudent("STU-001", today);

    QTextStream out(stdout);
    out << "Dormora / UDRMS backend demo\n";
    out << "Student: " << loadedStudent.id() << " - " << loadedStudent.fullName()
        << " room " << loadedStudent.roomNumber().value() << '\n';
    if (menu.has_value()) {
        out << "Today lunch: " << menu->lunch << '\n';
    }
    out << "Saved sample data to " << outputPath << '\n';

    return 0;
}
