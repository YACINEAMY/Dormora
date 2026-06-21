#pragma once

#include "udrms/Dormitory.h"

#include <QHash>
#include <QJsonObject>
#include <QString>
#include <QVector>
#include <optional>

namespace udrms {

// Main service object for the backend. It coordinates students, dormitories,
// room assignment, restaurant access, and persistence integrity checks.
class University final {
public:
    void addStudent(const Student &student);
    void removeStudent(const QString &studentId);
    bool hasStudent(const QString &studentId) const;
    Student &student(const QString &studentId);
    const Student &student(const QString &studentId) const;
    QVector<Student> students() const;

    void addDormitory(const Dormitory &dormitory);
    void removeDormitory(const QString &dormitoryId);
    bool hasDormitory(const QString &dormitoryId) const;
    Dormitory &dormitory(const QString &dormitoryId);
    const Dormitory &dormitory(const QString &dormitoryId) const;
    QVector<Dormitory> dormitories() const;

    void assignStudentToRoom(const QString &studentId, const QString &dormitoryId, int roomNumber);
    void removeStudentFromRoom(const QString &studentId);

    void setRestaurantMenu(const QString &dormitoryId, const QDate &date, const DailyMenu &menu);
    std::optional<DailyMenu> restaurantMenuForStudent(const QString &studentId, const QDate &date) const;
    void recordStudentMeal(const QString &studentId, const QDate &date);

    // Verifies that student assignments and room memberships match before data
    // is saved or accepted from JSON.
    void validateIntegrity() const;

    QJsonObject toJson() const;
    static University fromJson(const QJsonObject &object);

private:
    QHash<QString, Student> m_students;
    QHash<QString, Dormitory> m_dormitories;
};

} // namespace udrms
