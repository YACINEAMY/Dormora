#include "udrms/AppState.h"

#include "udrms/DomainError.h"

#include <QCoreApplication>
#include <QDir>

#include <algorithm>

namespace udrms {

QJsonArray stringListToJson(const QVector<QString> &values)
{
    QJsonArray array;
    for (const QString &value : values) {
        array.append(value);
    }
    return array;
}

QJsonArray stringSetToJson(const QSet<QString> &values)
{
    QVector<QString> sorted(values.cbegin(), values.cend());
    // QSet has no guaranteed order. Sort before saving to keep app-state JSON
    // readable and stable between runs.
    std::sort(sorted.begin(), sorted.end());
    return stringListToJson(sorted);
}

QVector<QString> stringArrayFromJson(const QJsonObject &object, const char *field)
{
    const QJsonValue value = object.value(field);
    if (!value.isArray()) {
        throw DomainError(QString("App state field '%1' must be an array.").arg(field));
    }

    QVector<QString> result;
    for (const QJsonValue &entry : value.toArray()) {
        if (!entry.isString()) {
            throw DomainError(QString("App state field '%1' must contain only strings.").arg(field));
        }
        result.append(entry.toString());
    }
    return result;
}

QString requiredAppString(const QJsonObject &object, const char *field)
{
    const QJsonValue value = object.value(field);
    if (!value.isString()) {
        throw DomainError(QString("App state field '%1' must be a string.").arg(field));
    }
    return value.toString();
}

bool requiredAppBool(const QJsonObject &object, const char *field)
{
    const QJsonValue value = object.value(field);
    if (!value.isBool()) {
        throw DomainError(QString("App state field '%1' must be a boolean.").arg(field));
    }
    return value.toBool();
}

QString defaultAppStatePath()
{
    // Store the GUI state next to the executable so the packaged app can save
    // data without requiring a separate configured path.
    return QDir(QCoreApplication::applicationDirPath()).filePath("udrms-data.json");
}

QJsonObject neighborhoodToJson(const Neighborhood &neighborhood)
{
    return {
        {"id", neighborhood.id},
        {"name", neighborhood.name},
        {"dormitoryIds", stringListToJson(neighborhood.dormitoryIds)},
    };
}

Neighborhood neighborhoodFromJson(const QJsonObject &object)
{
    return {
        requiredAppString(object, "id"),
        requiredAppString(object, "name"),
        stringArrayFromJson(object, "dormitoryIds"),
    };
}

QJsonObject adminProfileToJson(const AdminProfile &profile)
{
    return {
        {"username", profile.username},
        {"password", profile.password},
        {"displayName", profile.displayName},
        {"fullAccess", profile.fullAccess},
        {"neighborhoodIds", stringSetToJson(profile.neighborhoodIds)},
    };
}

AdminProfile adminProfileFromJson(const QJsonObject &object)
{
    const QVector<QString> neighborhoodIds = stringArrayFromJson(object, "neighborhoodIds");
    return {
        requiredAppString(object, "username"),
        requiredAppString(object, "password"),
        requiredAppString(object, "displayName"),
        requiredAppBool(object, "fullAccess"),
        QSet<QString>(neighborhoodIds.cbegin(), neighborhoodIds.cend()),
    };
}

} // namespace udrms
