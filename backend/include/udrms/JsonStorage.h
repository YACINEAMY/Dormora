#pragma once

#include "udrms/University.h"

#include <QString>

namespace udrms {

// JSON file persistence boundary for the University aggregate.
class JsonStorage final {
public:
    // Saves through QSaveFile so a failed write does not replace existing data.
    static void saveUniversity(const University &university, const QString &filePath);
    static University loadUniversity(const QString &filePath);
};

} // namespace udrms
