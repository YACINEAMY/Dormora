#include "udrms/Restaurant.h"

#include "udrms/DomainError.h"

#include <QJsonArray>
#include <QVector>

#include <algorithm>

namespace udrms {

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
        object.value("breakfast").toString(),
        object.value("lunch").toString(),
        object.value("dinner").toString(),
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
    Restaurant restaurant(object.value("name").toString());

    const QJsonArray menus = object.value("menus").toArray();
    for (const QJsonValue &value : menus) {
        const QJsonObject menuObject = value.toObject();
        const QDate date = QDate::fromString(menuObject.value("date").toString(), Qt::ISODate);
        restaurant.setMenu(date, DailyMenu::fromJson(menuObject));
    }

    const QJsonArray mealsServed = object.value("mealsServed").toArray();
    for (const QJsonValue &value : mealsServed) {
        const QJsonObject mealObject = value.toObject();
        restaurant.recordMealServed(
            QDate::fromString(mealObject.value("date").toString(), Qt::ISODate),
            mealObject.value("count").toInt());
    }

    return restaurant;
}

} // namespace udrms
