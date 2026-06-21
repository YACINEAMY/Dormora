#include "udrms/DomainError.h"
#include "udrms/JsonStorage.h"

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QTemporaryDir>

#include <cstdlib>
#include <functional>
#include <iostream>

using namespace udrms;

static void require(bool condition, const char *message)
{
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        std::exit(1);
    }
}

static bool throwsDomainError(const std::function<void()> &operation)
{
    try {
        operation();
    } catch (const DomainError &) {
        return true;
    }
    return false;
}

static void verifyStudentComponent()
{
    Student student("S1", "Nadia Mansouri", 1);

    require(student.id() == "S1", "student stores ID");
    require(student.fullName() == "Nadia Mansouri", "student stores full name");
    require(student.academicYear() == 1, "student stores academic year");
    require(!student.isAssigned(), "new student starts unassigned");

    student.setFullName("Nadia B.");
    student.setAcademicYear(2);
    student.assignToRoom("D1", 10);

    require(student.fullName() == "Nadia B.", "student name updates");
    require(student.academicYear() == 2, "student academic year updates");
    require(student.isAssigned(), "student assignment updates status");
    require(student.dormitoryId().value() == "D1", "student stores assigned dormitory");
    require(student.roomNumber().value() == 10, "student stores assigned room");

    student.clearAssignment();
    require(!student.isAssigned(), "student assignment clears");

    require(throwsDomainError([] { Student("", "Name", 1); }), "student rejects empty ID");
    require(throwsDomainError([] { Student("S2", "", 1); }), "student rejects empty name");
    require(throwsDomainError([] { Student("S2", "Name", 0); }), "student rejects invalid academic year");
}

static void verifyRoomComponent()
{
    Room room(10, 2);

    require(room.number() == 10, "room stores number");
    require(room.capacity() == 2, "room stores capacity");
    require(room.isEmpty(), "new room starts empty");

    room.addStudent("S2");
    room.addStudent("S1");

    require(room.occupancy() == 2, "room tracks occupancy");
    require(room.isFull(), "room detects full state");
    require(room.studentIds().first() == "S1", "room student IDs are sorted for stable output");

    require(throwsDomainError([&] { room.addStudent("S3"); }), "room rejects over-occupancy");
    require(throwsDomainError([&] { room.addStudent("S1"); }), "room rejects duplicate student");

    room.removeStudent("S2");
    require(!room.isFull(), "room updates capacity after removal");
    require(throwsDomainError([&] { room.removeStudent("S2"); }), "room rejects removing missing student");

    require(throwsDomainError([] { Room(0, 1); }), "room rejects invalid number");
    require(throwsDomainError([] { Room(1, 0); }), "room rejects invalid capacity");
}

static void verifyRestaurantComponent()
{
    Restaurant restaurant("North Restaurant");
    const QDate today(2026, 5, 19);

    restaurant.setMenu(today, {"Milk", "Pasta", "Fish"});
    const auto menu = restaurant.menuForDate(today);
    require(menu.has_value(), "restaurant stores daily menu");
    require(menu->breakfast == "Milk" && menu->lunch == "Pasta" && menu->dinner == "Fish",
            "restaurant returns complete daily menu");

    const QJsonObject menuWithImages{
        {"breakfast", "Milk"},
        {"lunch", "Pasta"},
        {"dinner", "Fish"},
        {"breakfastImageUrl", ":/icons/meal-breakfast.png"},
        {"lunchImageUrl", ":/icons/meal-lunch.png"},
        {"dinnerImageUrl", ":/icons/meal-dinner.png"},
    };
    const QJsonObject roundTrippedMenu = DailyMenu::fromJson(menuWithImages).toJson();
    require(roundTrippedMenu.value("breakfastImageUrl").toString() == ":/icons/meal-breakfast.png",
            "daily menu preserves breakfast image path");
    require(roundTrippedMenu.value("lunchImageUrl").toString() == ":/icons/meal-lunch.png",
            "daily menu preserves lunch image path");
    require(roundTrippedMenu.value("dinnerImageUrl").toString() == ":/icons/meal-dinner.png",
            "daily menu preserves dinner image path");

    require(!restaurant.menuForDate(today.addDays(1)).has_value(), "restaurant returns no menu for missing date");

    restaurant.recordMealServed(today);
    restaurant.recordMealServed(today, 3);
    require(restaurant.mealsServedOn(today) == 4, "restaurant accumulates meal count");

    require(throwsDomainError([] { Restaurant(""); }), "restaurant rejects empty name");
    require(throwsDomainError([&] { restaurant.setMenu(QDate(), {"", "", ""}); }), "restaurant rejects invalid menu date");
    require(throwsDomainError([&] { restaurant.setMenu(today, {"Milk", "", "Fish"}); }),
            "restaurant rejects incomplete menus");
    require(throwsDomainError([&] { restaurant.recordMealServed(today, 0); }), "restaurant rejects invalid meal count");
}

static void verifyDormitoryComponent()
{
    Dormitory dormitory("D1", "North Dormitory", 3, Restaurant("North Restaurant"));
    dormitory.addRoom(Room(11, 1));
    dormitory.addRoom(Room(10, 2));

    require(dormitory.id() == "D1", "dormitory stores ID");
    require(dormitory.totalRoomCapacity() == 3, "dormitory tracks total room capacity");
    require(dormitory.rooms().first().number() == 10, "dormitory rooms are sorted for stable output");

    Student student("S1", "Nadia Mansouri", 1);
    dormitory.assignStudent(student, 10);

    require(student.isAssigned(), "dormitory assigns student");
    require(dormitory.totalOccupancy() == 1, "dormitory occupancy increments");
    require(dormitory.occupiedRooms().size() == 1, "dormitory lists occupied rooms");

    dormitory.removeStudent(student);
    require(!student.isAssigned(), "dormitory clears student assignment on removal");
    require(dormitory.totalOccupancy() == 0, "dormitory occupancy decrements");

    dormitory.removeRoom(11);
    require(!dormitory.hasRoom(11), "dormitory removes empty room");

    require(throwsDomainError([&] { dormitory.addRoom(Room(10, 1)); }), "dormitory rejects duplicate room");
    require(throwsDomainError([&] { dormitory.addRoom(Room(12, 2)); }), "dormitory rejects capacity overflow");
    require(throwsDomainError([] { Dormitory("", "Dorm", 1, Restaurant("R")); }), "dormitory rejects empty ID");
}

static University makeUniversityFixture()
{
    University university;
    Dormitory dormitory("D1", "North Dormitory", 2, Restaurant("North Restaurant"));
    dormitory.addRoom(Room(10, 1));
    dormitory.addRoom(Room(11, 1));

    university.addDormitory(dormitory);
    university.addStudent(Student("S1", "Nadia Mansouri", 1));
    university.addStudent(Student("S2", "Youcef Amrani", 2));
    university.addStudent(Student("S3", "Lina Saadi", 3));
    return university;
}

static void verifyUniversityComponent()
{
    University university = makeUniversityFixture();

    require(university.hasStudent("S1"), "university finds students by ID");
    require(university.hasDormitory("D1"), "university finds dormitories by ID");
    require(university.students().first().id() == "S1", "university students are sorted for stable output");

    university.assignStudentToRoom("S1", "D1", 10);
    require(university.student("S1").isAssigned(), "university assigns student to room");
    require(university.dormitory("D1").room(10).occupancy() == 1, "university updates room occupancy");

    require(throwsDomainError([&] { university.assignStudentToRoom("S2", "D1", 10); }),
            "university rejects full room assignment");

    university.assignStudentToRoom("S2", "D1", 11);
    require(university.dormitory("D1").availableRooms().isEmpty(), "university detects no available rooms");

    university.setRestaurantMenu("D1", QDate(2026, 5, 19), {"Milk", "Pasta", "Fish"});
    const auto menu = university.restaurantMenuForStudent("S1", QDate(2026, 5, 19));
    require(menu.has_value() && menu->lunch == "Pasta", "resident can read restaurant menu");

    require(throwsDomainError([&] { university.restaurantMenuForStudent("S3", QDate(2026, 5, 19)); }),
            "unassigned student cannot access restaurant menu");

    university.recordStudentMeal("S1", QDate(2026, 5, 19));
    require(university.dormitory("D1").restaurant().mealsServedOn(QDate(2026, 5, 19)) == 1,
            "university records resident meal");

    university.removeStudentFromRoom("S1");
    require(!university.student("S1").isAssigned(), "university removes student from room");
    university.removeStudent("S1");
    require(!university.hasStudent("S1"), "university removes student record");

    require(throwsDomainError([&] { university.addStudent(Student("S2", "Duplicate", 1)); }),
            "university rejects duplicate student");
    require(throwsDomainError([&] { university.removeDormitory("D1"); }),
            "university rejects removing occupied dormitory");

    university.validateIntegrity();
}

static void verifyPersistenceComponent()
{
    University university = makeUniversityFixture();
    university.assignStudentToRoom("S1", "D1", 10);
    university.assignStudentToRoom("S2", "D1", 11);
    university.setRestaurantMenu("D1", QDate(2026, 5, 19), {"Milk", "Pasta", "Fish"});
    university.recordStudentMeal("S1", QDate(2026, 5, 19));

    QTemporaryDir tempDir;
    require(tempDir.isValid(), "temporary directory is available");

    const QString dataPath = QDir(tempDir.path()).filePath("university.json");
    JsonStorage::saveUniversity(university, dataPath);
    const University loaded = JsonStorage::loadUniversity(dataPath);

    require(loaded.student("S1").roomNumber().value() == 10, "student assignment survives persistence");
    require(loaded.dormitory("D1").room(11).containsStudent("S2"), "room students survive persistence");
    require(loaded.restaurantMenuForStudent("S1", QDate(2026, 5, 19))->dinner == "Fish",
            "menu survives persistence");
    require(loaded.dormitory("D1").restaurant().mealsServedOn(QDate(2026, 5, 19)) == 1,
            "meal counts survive persistence");

    QFile corruptFile(QDir(tempDir.path()).filePath("corrupt.json"));
    require(corruptFile.open(QIODevice::WriteOnly | QIODevice::Truncate), "can write corrupt test file");
    corruptFile.write("{ bad json");
    corruptFile.close();

    require(throwsDomainError([&] {
        JsonStorage::loadUniversity(corruptFile.fileName());
    }), "persistence rejects malformed JSON");

    QJsonObject inconsistent = university.toJson();
    QJsonArray students = inconsistent.value("students").toArray();
    QJsonObject firstStudent = students.first().toObject();
    firstStudent["roomNumber"] = 999;
    students.replace(0, firstStudent);
    inconsistent["students"] = students;

    require(throwsDomainError([&] {
        University::fromJson(inconsistent);
    }), "university rejects inconsistent persisted assignments");

    QJsonObject partialStudentAssignment{
        {"id", "S9"},
        {"fullName", "Incomplete Assignment"},
        {"academicYear", 1},
        {"dormitoryId", "D1"},
    };
    require(throwsDomainError([&] {
        Student::fromJson(partialStudentAssignment);
    }), "student JSON rejects partial assignment fields");

    QJsonObject missingRoomStudentsArray{
        {"number", 12},
        {"capacity", 1},
    };
    require(throwsDomainError([&] {
        Room::fromJson(missingRoomStudentsArray);
    }), "room JSON requires students array");

    QJsonObject missingRestaurantMealsArray{
        {"name", "North Restaurant"},
        {"menus", QJsonArray{}},
    };
    require(throwsDomainError([&] {
        Restaurant::fromJson(missingRestaurantMealsArray);
    }), "restaurant JSON requires meals-served array");
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    verifyStudentComponent();
    verifyRoomComponent();
    verifyRestaurantComponent();
    verifyDormitoryComponent();
    verifyUniversityComponent();
    verifyPersistenceComponent();

    std::cout << "All backend component tests passed.\n";
    return 0;
}
