#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QString>
#include <QVector>

namespace udrms {

struct Neighborhood {
    QString id;
    QString name;
    QVector<QString> dormitoryIds;
};

struct AdminProfile {
    QString username;
    QString password;
    QString displayName;
    bool fullAccess = false;
    QSet<QString> neighborhoodIds;
};

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
