#include "udrms/Restaurant.h"

#include "udrms/DomainError.h"

#include <QJsonArray>
#include <QVector>

#include <algorithm>

namespace udrms {

namespace {

QString requiredString(const QJsonObject &object, const char *field, const QString &context)
{
    const QJsonValue value = object.value(field);
    if (!value.isString()) {
        throw DomainError(context + " JSON field '" + field + "' must be a string.");
    }
    return value.toString();
}

int requiredInt(const QJsonObject &object, const char *field, const QString &context)
{
    const QJsonValue value = object.value(field);
    if (!value.isDouble()) {
        throw DomainError(context + " JSON field '" + field + "' must be a number.");
    }
    return value.toInt();
}

QJsonArray requiredArray(const QJsonObject &object, const char *field, const QString &context)
{
    const QJsonValue value = object.value(field);
    if (!value.isArray()) {
        throw DomainError(context + " JSON field '" + field + "' must be an array.");
    }
    return value.toArray();
}

QJsonObject requiredObject(const QJsonValue &value, const QString &context)
{
    if (!value.isObject()) {
        throw DomainError(context + " JSON array entries must be objects.");
    }
    return value.toObject();
}

QDate requiredIsoDate(const QJsonObject &object, const char *field, const QString &context)
{
    const QDate date = QDate::fromString(requiredString(object, field, context), Qt::ISODate);
    if (!date.isValid()) {
        throw DomainError(context + " JSON field '" + field + "' must be an ISO date.");
    }
    return date;
}

} // namespace

QJsonObject DailyMenu::toJson() const
{
    return {
        {"breakfast", breakfast},
        {"lunch", lunch},
        {"dinner", dinner},
    };
}

DailyMenu DailyMenu::fromJson(const QJsonObject &object)
{
    return {
        requiredString(object, "breakfast", "Menu"),
        requiredString(object, "lunch", "Menu"),
        requiredString(object, "dinner", "Menu"),
    };
}

Restaurant::Restaurant(QString name)
    : m_name(std::move(name))
{
    if (m_name.trimmed().isEmpty()) {
        throw DomainError("Restaurant name cannot be empty.");
    }
}

QString Restaurant::name() const
{
    return m_name;
}

void Restaurant::setName(const QString &name)
{
    if (name.trimmed().isEmpty()) {
        throw DomainError("Restaurant name cannot be empty.");
    }
    m_name = name;
}

void Restaurant::setMenu(const QDate &date, const DailyMenu &menu)
{
    if (!date.isValid()) {
        throw DomainError("Menu date is invalid.");
    }
    if (menu.breakfast.trimmed().isEmpty()
        || menu.lunch.trimmed().isEmpty()
        || menu.dinner.trimmed().isEmpty()) {
        throw DomainError("Breakfast, lunch, and dinner must all be provided.");
    }
    m_menus[date] = menu;
}

std::optional<DailyMenu> Restaurant::menuForDate(const QDate &date) const
{
    if (!m_menus.contains(date)) {
        return std::nullopt;
    }
    return m_menus.value(date);
}

void Restaurant::recordMealServed(const QDate &date, int count)
{
    if (!date.isValid()) {
        throw DomainError("Meal date is invalid.");
    }
    if (count <= 0) {
        throw DomainError("Meal count must be positive.");
    }
    m_mealsServed[date] += count;
}

int Restaurant::mealsServedOn(const QDate &date) const
{
    return m_mealsServed.value(date, 0);
}

QJsonObject Restaurant::toJson() const
{
    QJsonArray menus;
    for (auto it = m_menus.cbegin(); it != m_menus.cend(); ++it) {
        QJsonObject object = it.value().toJson();
        object["date"] = it.key().toString(Qt::ISODate);
        menus.append(object);
    }

    QJsonArray mealsServed;
    QVector<QDate> mealDates(m_mealsServed.keyBegin(), m_mealsServed.keyEnd());
    std::sort(mealDates.begin(), mealDates.end());
    for (const QDate &date : mealDates) {
        mealsServed.append(QJsonObject{
            {"date", date.toString(Qt::ISODate)},
            {"count", m_mealsServed.value(date)},
        });
    }

    return {
        {"name", m_name},
        {"menus", menus},
        {"mealsServed", mealsServed},
    };
}

Restaurant Restaurant::fromJson(const QJsonObject &object)
{
    Restaurant restaurant(requiredString(object, "name", "Restaurant"));

    const QJsonArray menus = requiredArray(object, "menus", "Restaurant");
    for (const QJsonValue &value : menus) {
        const QJsonObject menuObject = requiredObject(value, "Restaurant menu");
        const QDate date = requiredIsoDate(menuObject, "date", "Restaurant menu");
        restaurant.setMenu(date, DailyMenu::fromJson(menuObject));
    }

    const QJsonArray mealsServed = requiredArray(object, "mealsServed", "Restaurant");
    for (const QJsonValue &value : mealsServed) {
        const QJsonObject mealObject = requiredObject(value, "Restaurant meal count");
        restaurant.recordMealServed(
            requiredIsoDate(mealObject, "date", "Restaurant meal count"),
            requiredInt(mealObject, "count", "Restaurant meal count"));
    }

    return restaurant;
}

} // namespace udrms
