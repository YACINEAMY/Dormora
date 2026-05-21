#pragma once

#include <QDate>
#include <QHash>
#include <QJsonObject>
#include <QMap>
#include <QString>
#include <optional>

namespace udrms {

// One day's menu. Empty meal labels are rejected when a menu is saved.
struct DailyMenu {
    QString breakfast;
    QString lunch;
    QString dinner;

    QJsonObject toJson() const;
    static DailyMenu fromJson(const QJsonObject &object);
};

// Restaurant owned by a dormitory. It stores dated menus and served-meal counts.
class Restaurant final {
public:
    Restaurant() = default;
    explicit Restaurant(QString name);

    QString name() const;
    void setName(const QString &name);

    // Replaces the menu for the date; all three meal fields must be non-empty.
    void setMenu(const QDate &date, const DailyMenu &menu);
    std::optional<DailyMenu> menuForDate(const QDate &date) const;

    void recordMealServed(const QDate &date, int count = 1);
    int mealsServedOn(const QDate &date) const;

    QJsonObject toJson() const;
    static Restaurant fromJson(const QJsonObject &object);

private:
    QString m_name;
    QMap<QDate, DailyMenu> m_menus;
    QHash<QDate, int> m_mealsServed;
};

} // namespace udrms
