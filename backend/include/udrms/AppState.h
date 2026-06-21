#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QString>
#include <QVector>

namespace udrms {

// Campus grouping used by the GUI to scope dormitory access for non-global
// administrators.
struct Neighborhood {
    QString id;
    QString name;
    QVector<QString> dormitoryIds;
};

// Demo/admin login profile stored in the local app-state file. Passwords are
// plain text because this is a coursework demo, not production authentication.
struct AdminProfile {
    QString username;
    QString password;
    QString displayName;
    bool fullAccess = false;
    QSet<QString> neighborhoodIds;
};

// Small JSON helpers shared by the GUI app-state loader/saver. They keep
// validation errors consistent with the backend domain exceptions.
QJsonArray stringListToJson(const QVector<QString> &values);
QJsonArray stringSetToJson(const QSet<QString> &values);
QVector<QString> stringArrayFromJson(const QJsonObject &object, const char *field);
QString requiredAppString(const QJsonObject &object, const char *field);
bool requiredAppBool(const QJsonObject &object, const char *field);
QString defaultAppStatePath();

QJsonObject neighborhoodToJson(const Neighborhood &neighborhood);
Neighborhood neighborhoodFromJson(const QJsonObject &object);

QJsonObject adminProfileToJson(const AdminProfile &profile);
AdminProfile adminProfileFromJson(const QJsonObject &object);

} // namespace udrms
