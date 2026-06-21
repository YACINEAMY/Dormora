#include "udrms/JsonStorage.h"

#include "udrms/DomainError.h"

#include <QFile>
#include <QJsonDocument>
#include <QSaveFile>

namespace udrms {

void JsonStorage::saveUniversity(const University &university, const QString &filePath)
{
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        throw DomainError("Could not open data file for writing: " + file.errorString());
    }

    // Validate before writing so corrupted relationships are not persisted.
    university.validateIntegrity();

    const QByteArray data = QJsonDocument(university.toJson()).toJson(QJsonDocument::Indented);
    if (file.write(data) != data.size()) {
        throw DomainError("Could not write data file: " + file.errorString());
    }
    if (!file.commit()) {
        throw DomainError("Could not commit data file: " + file.errorString());
    }
}

University JsonStorage::loadUniversity(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw DomainError("Could not open data file for reading: " + file.errorString());
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        throw DomainError("Data file is not valid JSON: " + parseError.errorString());
    }

    return University::fromJson(document.object());
}

} // namespace udrms
