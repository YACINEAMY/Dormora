#pragma once

#include "udrms/Person.h"

#include <optional>

namespace udrms {

// Student profile plus optional accommodation assignment.
class Student final : public Person {
public:
    Student() = default;
    Student(QString id, QString fullName, int academicYear);

    int academicYear() const;
    std::optional<QString> dormitoryId() const;
    std::optional<int> roomNumber() const;
    bool isAssigned() const;

    void setAcademicYear(int academicYear);
    // Stores the student's assignment only; Dormitory/University own the room
    // membership checks that keep both sides of the assignment in sync.
    void assignToRoom(const QString &dormitoryId, int roomNumber);
    void clearAssignment();

    QJsonObject toJson() const override;
    static Student fromJson(const QJsonObject &object);

private:
    int m_academicYear = 1;
    std::optional<QString> m_dormitoryId;
    std::optional<int> m_roomNumber;
};

} // namespace udrms
