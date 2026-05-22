#include "udrms/DomainError.h"
#include "udrms/University.h"

#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QCoreApplication>
#include <QDateEdit>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFrame>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QShortcut>
#include <QSaveFile>
#include <QScreen>
#include <QScrollArea>
#include <QSet>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTableWidget>
#include <QTimer>
#include <QTemporaryDir>
#include <QVBoxLayout>

#include <algorithm>
#include <functional>

using namespace udrms;

namespace {

const char *kAppStyle = R"(
* {
    font-family: "Segoe UI", Arial, sans-serif;
    font-size: 13px;
    color: #13231D;
}
QMainWindow {
    background: transparent;
}
QFrame#windowChrome {
    background: #F7FAF8;
    border: 1px solid #C9DED3;
    border-radius: 22px;
}
QWidget#appRoot, QWidget#loginRoot {
    background: #F7FAF8;
    border-radius: 21px;
}
QWidget#studentRoot {
    background: #FFFCF7;
    border-radius: 21px;
}
QWidget#contentPane {
    background: #F7FAF8;
    border-top-right-radius: 21px;
    border-bottom-right-radius: 21px;
}
QWidget#studentContentPane {
    background: #FFFCF7;
    border-top-right-radius: 21px;
    border-bottom-right-radius: 21px;
}
QFrame#titleControls {
    background: transparent;
    border: none;
}
QPushButton#windowButton {
    background: rgba(15,81,50,0.08);
    border: none;
    border-radius: 13px;
    min-width: 26px;
    max-width: 26px;
    min-height: 26px;
    max-height: 26px;
    padding: 0;
    font-weight: 800;
}
QPushButton#windowButton:hover {
    background: rgba(25,135,84,0.18);
}
QPushButton#windowButtonWide {
    background: rgba(15,81,50,0.08);
    border: none;
    border-radius: 13px;
    min-width: 42px;
    max-width: 42px;
    min-height: 26px;
    max-height: 26px;
    padding: 0;
    font-weight: 800;
}
QPushButton#windowButtonWide:hover {
    background: rgba(25,135,84,0.18);
}
QPushButton#closeButton {
    background: rgba(180,35,24,0.10);
    color: #B42318;
    border: none;
    border-radius: 13px;
    min-width: 26px;
    max-width: 26px;
    min-height: 26px;
    max-height: 26px;
    padding: 0;
    font-weight: 800;
}
QPushButton#closeButton:hover {
    background: #B42318;
    color: white;
}
QFrame#sidebar {
    background: #123D32;
    border: none;
    border-top-left-radius: 21px;
    border-bottom-left-radius: 21px;
}
QFrame#studentSidebar {
    background: #2F5145;
    border: none;
    border-top-left-radius: 21px;
    border-bottom-left-radius: 21px;
}
QPushButton#sidebarToggle {
    background: rgba(255,255,255,0.10);
    color: #F7FAF8;
    border: 1px solid rgba(255,255,255,0.14);
    border-radius: 8px;
    min-height: 34px;
    padding: 0 10px;
    font-weight: 800;
}
QPushButton#sidebarToggle:hover {
    background: rgba(255,255,255,0.18);
}
QFrame#loginBrand {
    background: #123D32;
    border-top-left-radius: 21px;
    border-bottom-left-radius: 21px;
}
QLabel#brand {
    color: white;
    font-size: 24px;
    font-weight: 800;
}
QLabel#brandSub {
    color: #BFE4D2;
    font-size: 12px;
    font-weight: 600;
}
QFrame#brandMark {
    background: #F6C76B;
    border: 2px solid rgba(255,255,255,0.40);
    border-radius: 16px;
    min-width: 42px;
    max-width: 42px;
    min-height: 42px;
    max-height: 42px;
}
QLabel#brandMarkLetter {
    color: #123D32;
    font-size: 22px;
    font-weight: 900;
}
QFrame#brandMiniLine {
    background: #123D32;
    border-radius: 2px;
    min-height: 4px;
    max-height: 4px;
}
QLabel#loginBadge {
    background: rgba(255,255,255,0.12);
    border: 1px solid rgba(255,255,255,0.20);
    border-radius: 14px;
    color: #E8FFF3;
    font-size: 12px;
    font-weight: 700;
    padding: 6px 10px;
}
QPushButton[nav="true"] {
    background: transparent;
    color: #D9F2E4;
    border: none;
    border-radius: 8px;
    padding: 12px 14px;
    text-align: left;
    font-weight: 600;
}
QPushButton[nav="true"]:checked {
    background: #1D7A57;
    color: white;
}
QPushButton[nav="true"]:hover {
    background: rgba(255,255,255,0.10);
}
QLabel#pageTitle {
    font-size: 26px;
    font-weight: 800;
}
QLabel#pageKicker {
    color: #536579;
    font-size: 14px;
}
QFrame.card, QFrame[class="card"], QGroupBox {
    background: white;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
}
QFrame#studentCard {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
}
QFrame#studentInlinePanel, QFrame#todayMenuItem, QFrame#mealRow {
    background: #F7FAF8;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
}
QLabel.cardTitle, QLabel[class="cardTitle"] {
    font-size: 17px;
    font-weight: 700;
}
QLabel.muted, QLabel[class="muted"] {
    color: #667085;
}
QLabel.metricValue, QLabel[class="metricValue"] {
    font-size: 32px;
    font-weight: 800;
    color: #13231D;
}
QLabel.metricLabel, QLabel[class="metricLabel"] {
    color: #536579;
    font-weight: 700;
}
QPushButton {
    background: white;
    border: 1px solid #D4E1DA;
    border-radius: 8px;
    padding: 9px 14px;
    font-weight: 650;
}
QPushButton:hover {
    background: #F3FBF7;
}
QPushButton.primary, QPushButton[class="primary"] {
    background: #1D7A57;
    border-color: #1D7A57;
    color: white;
}
QPushButton.primary:hover, QPushButton[class="primary"]:hover {
    background: #176648;
}
QPushButton.danger, QPushButton[class="danger"] {
    color: #B42318;
    border-color: #F1B8B3;
}
QPushButton#suggestionButton {
    background: #F6FAF8;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
    color: #13231D;
    font-weight: 600;
    padding: 7px 10px;
    text-align: left;
}
QPushButton#suggestionButton:hover {
    background: #E6F4EE;
    border-color: #BFD8CD;
}
QPushButton:disabled, QLineEdit:disabled, QComboBox:disabled, QSpinBox:disabled {
    color: #98A2B3;
    background: #F2F5F3;
    border-color: #E2E8E4;
}
QPushButton:focus, QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDateEdit:focus {
    border: 1px solid #1D7A57;
}
QLineEdit, QComboBox, QSpinBox, QDateEdit {
    background: white;
    border: 1px solid #D4E1DA;
    border-radius: 8px;
    padding: 8px 12px;
    min-height: 30px;
}
QTableWidget {
    background: white;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
    gridline-color: transparent;
    selection-background-color: #E2F3EA;
    selection-color: #13231D;
    outline: 0;
}
QTableWidget::item {
    padding: 8px 10px;
    border-bottom: 1px solid #EEF3F0;
}
QTableWidget::item:selected {
    background: #E2F3EA;
}
QHeaderView::section {
    background: #F3F8F5;
    color: #536579;
    border: none;
    border-bottom: 1px solid #D9E5DF;
    padding: 10px;
    font-weight: 700;
}
QScrollArea {
    background: transparent;
    border: none;
}
QScrollArea > QWidget > QWidget {
    background: transparent;
}
QScrollBar:vertical {
    background: transparent;
    width: 8px;
}
QScrollBar::handle:vertical {
    background: #D2E0D8;
    border-radius: 4px;
    min-height: 32px;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    background: transparent;
}
QStatusBar {
    background: #F7FAF8;
    color: #123D32;
}
QLabel#appStatus {
    background: #EEF6F1;
    border-top: 1px solid #D9E5DF;
    color: #315B4B;
    font-size: 12px;
    padding: 1px 18px;
}
)";

QLabel *label(const QString &text, const QString &objectName = {}, QWidget *parent = nullptr)
{
    auto *result = new QLabel(text, parent);
    if (!objectName.isEmpty()) {
        result->setObjectName(objectName);
    }
    result->setWordWrap(true);
    return result;
}

QFrame *card(QWidget *parent = nullptr)
{
    auto *frame = new QFrame(parent);
    frame->setProperty("class", "card");
    frame->setFrameShape(QFrame::NoFrame);
    return frame;
}

QFrame *brandMark(QWidget *parent = nullptr)
{
    auto *mark = new QFrame(parent);
    mark->setObjectName("brandMark");
    auto *layout = new QVBoxLayout(mark);
    layout->setContentsMargins(8, 6, 8, 7);
    layout->setSpacing(3);
    auto *letter = new QLabel("D", mark);
    letter->setObjectName("brandMarkLetter");
    letter->setAlignment(Qt::AlignCenter);
    layout->addWidget(letter, 1);
    auto *line = new QFrame(mark);
    line->setObjectName("brandMiniLine");
    layout->addWidget(line);
    return mark;
}

QString assignmentText(const Student &student)
{
    if (!student.isAssigned()) {
        return "Unassigned";
    }
    return student.dormitoryId().value() + " / Room " + QString::number(student.roomNumber().value());
}

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

QString defaultAppStatePath()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("udrms-data.json");
}

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

} // namespace

class DormitoryWindow final : public QMainWindow {
public:
    explicit DormitoryWindow(QString dataFilePath = {})
        : m_dataFilePath(dataFilePath.isEmpty() ? defaultAppStatePath() : std::move(dataFilePath))
    {
        loadOrSeedData();
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        m_enableAnimations = !QCoreApplication::arguments().contains("--screenshot");
        qApp->setFont(QFont("Segoe UI", 10));
        qApp->setStyleSheet(kAppStyle);
        statusBar()->setSizeGripEnabled(false);
        statusBar()->hide();
        auto *fullscreenShortcut = new QShortcut(QKeySequence(Qt::Key_F11), this);
        connect(fullscreenShortcut, &QShortcut::activated, this, [this] { toggleFullScreen(); });
        buildLoginUi();
        setWindowTitle("Dormora");
        resize(1280, 780);
        setMinimumSize(980, 620);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            m_resizeEdges = resizeEdgesAt(event->position().toPoint());
            if (m_resizeEdges != 0 && !isMaximized() && !isFullScreen()) {
                m_resizingWindow = true;
                m_resizeStartPosition = event->globalPosition().toPoint();
                m_resizeStartGeometry = geometry();
            } else {
                m_draggingWindow = !isMaximized() && !isFullScreen()
                    && (event->position().y() < 80 || event->position().x() < (m_sidebarCollapsed ? 90 : 260));
                m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            }
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (m_resizingWindow && (event->buttons() & Qt::LeftButton)) {
            resizeFromMouse(event->globalPosition().toPoint());
            event->accept();
            return;
        }
        if (m_draggingWindow && (event->buttons() & Qt::LeftButton)) {
            move(event->globalPosition().toPoint() - m_dragPosition);
            event->accept();
            return;
        }
        updateResizeCursor(event->position().toPoint());
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        m_draggingWindow = false;
        m_resizingWindow = false;
        m_resizeEdges = 0;
        unsetCursor();
        QMainWindow::mouseReleaseEvent(event);
    }

private:
    enum class AuthRole {
        None,
        Admin,
        Student,
    };

    University m_university;
    QVector<Neighborhood> m_neighborhoods;
    QHash<QString, AdminProfile> m_adminProfiles;
    AuthRole m_role = AuthRole::None;
    QString m_dataFilePath;
    QString m_studentPortalPassword = "student123";
    QString m_currentAdminUsername;
    QString m_currentStudentId;
    QPoint m_dragPosition;
    QPoint m_resizeStartPosition;
    QRect m_resizeStartGeometry;
    bool m_draggingWindow = false;
    bool m_resizingWindow = false;
    int m_resizeEdges = 0;
    bool m_sidebarCollapsed = false;
    bool m_enableAnimations = true;

    QStackedWidget *m_stack = nullptr;
    QLineEdit *m_loginUserInput = nullptr;
    QLineEdit *m_loginPasswordInput = nullptr;
    QLabel *m_loginFeedback = nullptr;
    QLabel *m_statusLabel = nullptr;

    QLabel *m_residentsMetric = nullptr;
    QLabel *m_availableRoomsMetric = nullptr;
    QLabel *m_mealsMetric = nullptr;
    QTableWidget *m_roomMatrix = nullptr;
    QTableWidget *m_studentTable = nullptr;
    QLineEdit *m_studentSearchInput = nullptr;
    QComboBox *m_studentFilterInput = nullptr;
    QComboBox *m_studentYearFilterInput = nullptr;
    QComboBox *m_studentDormitoryFilterInput = nullptr;
    QWidget *m_studentSuggestionPanel = nullptr;
    QVBoxLayout *m_studentSuggestionList = nullptr;
    QLabel *m_studentCountLabel = nullptr;
    QDialog *m_studentProfileDialog = nullptr;
    QLabel *m_profileNameLabel = nullptr;
    QLabel *m_profileMetaLabel = nullptr;
    QLabel *m_profileAssignmentLabel = nullptr;
    QLabel *m_profileRoomLabel = nullptr;
    QLabel *m_profileAccessLabel = nullptr;
    QLabel *m_profileResidentStatusLabel = nullptr;
    QLabel *m_profileStatusLabel = nullptr;
    QLineEdit *m_profileNameInput = nullptr;
    QSpinBox *m_profileYearInput = nullptr;
    QVBoxLayout *m_todayMenuSummary = nullptr;
    QTableWidget *m_neighborhoodTable = nullptr;
    QGridLayout *m_residentList = nullptr;
    QVBoxLayout *m_menuCards = nullptr;

    QLineEdit *m_studentIdInput = nullptr;
    QLineEdit *m_studentNameInput = nullptr;
    QSpinBox *m_academicYearInput = nullptr;
    QComboBox *m_assignStudentInput = nullptr;
    QComboBox *m_assignDormitoryInput = nullptr;
    QSpinBox *m_assignRoomInput = nullptr;

    QComboBox *m_menuDormitoryInput = nullptr;
    QDateEdit *m_menuDateInput = nullptr;
    QLineEdit *m_breakfastInput = nullptr;
    QLineEdit *m_lunchInput = nullptr;
    QLineEdit *m_dinnerInput = nullptr;
    QLineEdit *m_neighborhoodIdInput = nullptr;
    QLineEdit *m_neighborhoodNameInput = nullptr;
    QComboBox *m_copyNeighborhoodInput = nullptr;
    QComboBox *m_adminAccessInput = nullptr;
    QComboBox *m_neighborhoodAccessInput = nullptr;
    QString m_selectedStudentId;
    bool m_studentProfileDirty = false;
    bool m_restoringStudentSelection = false;
    bool m_loadingStudentProfile = false;

    enum ResizeEdge {
        LeftEdge = 0x1,
        RightEdge = 0x2,
        TopEdge = 0x4,
        BottomEdge = 0x8,
    };

    struct ExampleStudentSeed {
        QString id;
        QString name;
        int year;
        QString dormitoryId;
        int roomNumber;
    };

    struct ExampleMenuSeed {
        int dayOffset;
        const char *dormitoryId;
        const char *breakfast;
        const char *lunch;
        const char *dinner;
    };

    void loadOrSeedData()
    {
        if (QFile::exists(m_dataFilePath)) {
            loadAppState();
            if (ensureExampleDataVolume()) {
                saveAppState();
            }
            return;
        }

        seedData();
        saveAppState();
    }

    void seedData()
    {
        m_university = University();
        m_neighborhoods.clear();
        m_adminProfiles.clear();
        m_studentPortalPassword = "student123";

        Dormitory north("D1", "North Dormitory", 100, Restaurant("North Restaurant"));
        for (int roomNumber = 101; roomNumber <= 150; ++roomNumber) {
            north.addRoom(Room(roomNumber, 2));
        }

        Dormitory south("D2", "South Dormitory", 100, Restaurant("South Restaurant"));
        for (int roomNumber = 201; roomNumber <= 250; ++roomNumber) {
            south.addRoom(Room(roomNumber, 2));
        }

        m_university.addDormitory(north);
        m_university.addDormitory(south);
        addExampleStudents(true);
        addExampleMenus(false);

        const QDate today = QDate::currentDate();
        m_university.recordStudentMeal("S1001", today);
        m_university.recordStudentMeal("S1002", today);
        m_university.recordStudentMeal("S1004", today);
        m_university.recordStudentMeal("S1016", today);

        m_neighborhoods.append({"NORTH", "North Campus Neighborhood", {"D1"}});
        m_neighborhoods.append({"SOUTH", "South Campus Neighborhood", {"D2"}});

        m_adminProfiles.insert("admin", {"admin", "admin123", "Global Administrator", true, {}});
        m_adminProfiles.insert("northadmin", {"northadmin", "north123", "North Neighborhood Admin", false, {"NORTH"}});
        m_adminProfiles.insert("southadmin", {"southadmin", "south123", "South Neighborhood Admin", false, {"SOUTH"}});
    }

    bool ensureExampleDataVolume()
    {
        bool changed = addExampleStudents(false);
        changed = addExampleMenus(true) || changed;
        return changed;
    }

    bool addExampleStudents(bool assignRooms)
    {
        bool changed = false;
        for (const ExampleStudentSeed &seed : exampleStudentSeeds()) {
            const QString studentId = seed.id;
            if (!m_university.hasStudent(studentId)) {
                m_university.addStudent(Student(studentId, seed.name, seed.year));
                changed = true;
            }

            if (!assignRooms || seed.dormitoryId.isEmpty()) {
                continue;
            }

            Student &student = m_university.student(studentId);
            const QString dormitoryId = seed.dormitoryId;
            if (!student.isAssigned()
                && m_university.hasDormitory(dormitoryId)
                && m_university.dormitory(dormitoryId).hasRoom(seed.roomNumber)
                && !m_university.dormitory(dormitoryId).room(seed.roomNumber).isFull()) {
                m_university.assignStudentToRoom(studentId, dormitoryId, seed.roomNumber);
                changed = true;
            }
        }
        return changed;
    }

    bool addExampleMenus(bool preserveExisting)
    {
        bool changed = false;
        const QDate today = QDate::currentDate();
        for (const ExampleMenuSeed &seed : exampleMenuSeeds()) {
            const QString dormitoryId = QString::fromLatin1(seed.dormitoryId);
            if (!m_university.hasDormitory(dormitoryId)) {
                continue;
            }

            const QDate date = today.addDays(seed.dayOffset);
            if (preserveExisting && m_university.dormitory(dormitoryId).restaurant().menuForDate(date).has_value()) {
                continue;
            }

            m_university.setRestaurantMenu(dormitoryId, date, {
                QString::fromUtf8(seed.breakfast),
                QString::fromUtf8(seed.lunch),
                QString::fromUtf8(seed.dinner),
            });
            changed = true;
        }
        return changed;
    }

    static QVector<ExampleStudentSeed> exampleStudentSeeds()
    {
        QVector<ExampleStudentSeed> seeds = {
            {"S1001", "Amina Benali", 1, "D1", 101},
            {"S1002", "Karim Haddad", 2, "D1", 101},
            {"S1003", "Lina Saadi", 3, "D1", 102},
            {"S1004", "Yanis Merad", 1, "D2", 201},
            {"S1005", "Nadia Mansouri", 2, "D1", 102},
            {"S1006", "Omar Rahmani", 4, "D1", 103},
            {"S1007", "Sara Bouchard", 1, "D1", 103},
            {"S1008", "Mehdi Kaci", 3, "D1", 104},
            {"S1009", "Leila Haddouche", 2, "D1", 104},
            {"S1010", "Rayan Saad", 1, "D1", 105},
            {"S1011", "Maya Cherif", 4, "D1", 105},
            {"S1012", "Ilyes Hamdi", 2, "D1", 106},
            {"S1013", "Selma Ait Amar", 3, "D1", 106},
            {"S1014", "Adam Bouzid", 1, "D1", 107},
            {"S1015", "Ines Ferhat", 2, "D1", 108},
            {"S1016", "Nour Belkacem", 3, "D2", 201},
            {"S1017", "Samir Touati", 4, "D2", 202},
            {"S1018", "Rania Ouali", 1, "D2", 202},
            {"S1019", "Walid Ziane", 2, "D2", 203},
            {"S1020", "Lamia Bensaid", 3, "D2", 203},
            {"S1021", "Bilal Mansour", 1, "D2", 204},
            {"S1022", "Yasmine Abbas", 2, "D2", 204},
            {"S1023", "Hugo Martin", 3, "D2", 205},
            {"S1024", "Claire Dubois", 4, "D2", 205},
            {"S1025", "Amel Ghazi", 1, {}, 0},
            {"S1026", "Tariq Salhi", 2, {}, 0},
            {"S1027", "Meriem Lounis", 3, {}, 0},
            {"S1028", "Sofiane Haddar", 4, {}, 0},
            {"S1029", "Elena Rossi", 2, {}, 0},
            {"S1030", "Noah Laurent", 1, {}, 0},
        };

        static constexpr const char *firstNames[] = {
            "Adel", "Sofia", "Malik", "Nora", "Ibrahim", "Lina", "Nassim", "Aya",
            "Farid", "Dalia", "Younes", "Mina", "Samy", "Rim", "Khaled", "Lara",
            "Anis", "Celia", "Taha", "Hana",
        };
        static constexpr const char *lastNames[] = {
            "Belaid", "Haddad", "Mokrani", "Saadi", "Kacem", "Bouzid", "Rahmani", "Cherif",
            "Ferhat", "Toumi", "Bensaid", "Zeroual", "Amrani", "Mansouri", "Ouali", "Brahimi",
            "Lounis", "Abbassi", "Meziane", "Taleb",
        };

        for (int number = 1031; number <= 1100; ++number) {
            const int index = number - 1031;
            const QString id = "S" + QString::number(number);
            const QString fullName = QString::fromLatin1(firstNames[index % 20])
                + " " + QString::fromLatin1(lastNames[(index * 7) % 20]);
            const bool northCampus = index % 2 == 0;
            const QString dormitoryId = northCampus ? QStringLiteral("D1") : QStringLiteral("D2");
            const int roomNumber = (northCampus ? 109 : 209) + (index / 4);
            seeds.append({
                id,
                fullName,
                (index % 4) + 1,
                dormitoryId,
                roomNumber,
            });
        }

        return seeds;
    }

    static QVector<ExampleMenuSeed> exampleMenuSeeds()
    {
        return {
            {0, "D1", "Coffee, eggs, and toast", "Chicken rice bowl with salad", "Lentil soup and grilled vegetables"},
            {0, "D2", "Mint tea, msemen, and yogurt", "Couscous with vegetables", "Vegetable stew with bread"},
            {1, "D1", "Oatmeal with dates and milk", "Turkey sandwich with tomato soup", "Pasta primavera and fruit"},
            {1, "D2", "Croissants, jam, and orange juice", "Beef tagine with rice", "Chickpea salad and baked potatoes"},
            {2, "D1", "Cheese omelet and fruit", "Tuna pasta salad", "Roast chicken with carrots"},
            {2, "D2", "Baghrir, honey, and milk", "Lentil dal with flatbread", "Fish fillet with herb rice"},
            {3, "D1", "Greek yogurt with granola", "Beef kofta wrap", "Tomato soup with grilled cheese"},
            {3, "D2", "Scrambled eggs and baguette", "Chicken couscous", "Vegetable lasagna"},
            {4, "D1", "Pancakes with banana", "Falafel bowl with hummus", "Beef stew with potatoes"},
            {4, "D2", "Coffee, toast, and cheese", "Grilled chicken salad", "Harira soup and dates"},
            {5, "D1", "Breakfast burrito", "Salmon rice plate", "Mushroom risotto"},
            {5, "D2", "Semolina porridge and fruit", "Pasta bolognese", "Stuffed peppers and salad"},
            {6, "D1", "French toast and berries", "Chicken shawarma plate", "Vegetable curry with rice"},
            {6, "D2", "Boiled eggs, olives, and bread", "Lamb meatballs with couscous", "Spinach quiche and soup"},
            {7, "D1", "Avocado toast and boiled egg", "Pesto pasta with chicken", "Black bean chili and cornbread"},
            {7, "D2", "Milk, cereal, and apple slices", "Shrimp rice skillet", "Roasted vegetable couscous"},
            {8, "D1", "Fruit smoothie and muffin", "Chicken caesar wrap", "Baked fish with green beans"},
            {8, "D2", "Omelet with peppers", "Vegetarian tajine", "Creamy tomato pasta"},
            {9, "D1", "Toast, jam, and yogurt", "Beef burger with salad", "Chicken noodle soup"},
            {9, "D2", "Dates, bread, and labneh", "Rice with chickpeas and vegetables", "Turkey meatloaf with mash"},
        };
    }

    QJsonObject appStateToJson() const
    {
        QJsonArray neighborhoods;
        for (const Neighborhood &neighborhood : m_neighborhoods) {
            neighborhoods.append(neighborhoodToJson(neighborhood));
        }

        QJsonArray adminProfiles;
        QVector<QString> usernames(m_adminProfiles.keyBegin(), m_adminProfiles.keyEnd());
        std::sort(usernames.begin(), usernames.end());
        for (const QString &username : usernames) {
            adminProfiles.append(adminProfileToJson(m_adminProfiles.value(username)));
        }

        return {
            {"university", m_university.toJson()},
            {"neighborhoods", neighborhoods},
            {"adminProfiles", adminProfiles},
            {"studentPortalPassword", m_studentPortalPassword},
        };
    }

    void loadAppState()
    {
        QFile file(m_dataFilePath);
        if (!file.open(QIODevice::ReadOnly)) {
            throw DomainError("Could not open app data file: " + file.errorString());
        }

        QJsonParseError parseError;
        const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
            throw DomainError("App data file is not valid JSON: " + parseError.errorString());
        }

        const QJsonObject root = document.object();
        const QJsonValue universityValue = root.value("university");
        if (!universityValue.isObject()) {
            throw DomainError("App state field 'university' must be an object.");
        }

        const QJsonValue neighborhoodsValue = root.value("neighborhoods");
        if (!neighborhoodsValue.isArray()) {
            throw DomainError("App state field 'neighborhoods' must be an array.");
        }

        const QJsonValue adminProfilesValue = root.value("adminProfiles");
        if (!adminProfilesValue.isArray()) {
            throw DomainError("App state field 'adminProfiles' must be an array.");
        }

        University loadedUniversity = University::fromJson(universityValue.toObject());
        QVector<Neighborhood> loadedNeighborhoods;
        for (const QJsonValue &value : neighborhoodsValue.toArray()) {
            if (!value.isObject()) {
                throw DomainError("App state neighborhoods entries must be objects.");
            }
            loadedNeighborhoods.append(neighborhoodFromJson(value.toObject()));
        }

        QHash<QString, AdminProfile> loadedProfiles;
        for (const QJsonValue &value : adminProfilesValue.toArray()) {
            if (!value.isObject()) {
                throw DomainError("App state admin profile entries must be objects.");
            }
            const AdminProfile profile = adminProfileFromJson(value.toObject());
            loadedProfiles.insert(profile.username.toLower(), profile);
        }
        if (loadedProfiles.isEmpty()) {
            throw DomainError("App state must contain at least one admin profile.");
        }

        const QString loadedStudentPassword = requiredAppString(root, "studentPortalPassword");
        if (loadedStudentPassword.isEmpty()) {
            throw DomainError("Student portal password cannot be empty.");
        }

        m_university = loadedUniversity;
        m_neighborhoods = loadedNeighborhoods;
        m_adminProfiles = loadedProfiles;
        m_studentPortalPassword = loadedStudentPassword;
    }

    void saveAppState() const
    {
        const QFileInfo target(m_dataFilePath);
        const QDir parentDir = target.dir();
        if (!parentDir.exists() && !QDir().mkpath(parentDir.absolutePath())) {
            throw DomainError("Could not create app data directory.");
        }

        QSaveFile file(m_dataFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            throw DomainError("Could not open app data file for writing: " + file.errorString());
        }

        const QByteArray data = QJsonDocument(appStateToJson()).toJson(QJsonDocument::Indented);
        if (file.write(data) != data.size()) {
            throw DomainError("Could not write app data file: " + file.errorString());
        }
        if (!file.commit()) {
            throw DomainError("Could not commit app data file: " + file.errorString());
        }
    }

    int resizeEdgesAt(const QPoint &position) const
    {
        if (isMaximized() || isFullScreen()) {
            return 0;
        }

        constexpr int margin = 8;
        int edges = 0;
        if (position.x() <= margin) {
            edges |= LeftEdge;
        }
        if (position.x() >= width() - margin) {
            edges |= RightEdge;
        }
        if (position.y() <= margin) {
            edges |= TopEdge;
        }
        if (position.y() >= height() - margin) {
            edges |= BottomEdge;
        }
        return edges;
    }

    void updateResizeCursor(const QPoint &position)
    {
        const int edges = resizeEdgesAt(position);
        if ((edges & LeftEdge && edges & TopEdge) || (edges & RightEdge && edges & BottomEdge)) {
            setCursor(Qt::SizeFDiagCursor);
        } else if ((edges & RightEdge && edges & TopEdge) || (edges & LeftEdge && edges & BottomEdge)) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (edges & (LeftEdge | RightEdge)) {
            setCursor(Qt::SizeHorCursor);
        } else if (edges & (TopEdge | BottomEdge)) {
            setCursor(Qt::SizeVerCursor);
        } else {
            unsetCursor();
        }
    }

    void resizeFromMouse(const QPoint &globalPosition)
    {
        QRect next = m_resizeStartGeometry;
        const QPoint delta = globalPosition - m_resizeStartPosition;
        const QSize minimum = minimumSize();

        if (m_resizeEdges & LeftEdge) {
            const int newLeft = std::min(next.right() - minimum.width() + 1, m_resizeStartGeometry.left() + delta.x());
            next.setLeft(newLeft);
        }
        if (m_resizeEdges & RightEdge) {
            next.setRight(std::max(next.left() + minimum.width() - 1, m_resizeStartGeometry.right() + delta.x()));
        }
        if (m_resizeEdges & TopEdge) {
            const int newTop = std::min(next.bottom() - minimum.height() + 1, m_resizeStartGeometry.top() + delta.y());
            next.setTop(newTop);
        }
        if (m_resizeEdges & BottomEdge) {
            next.setBottom(std::max(next.top() + minimum.height() - 1, m_resizeStartGeometry.bottom() + delta.y()));
        }
        setGeometry(next);
    }

    void toggleMaximized()
    {
        if (isFullScreen()) {
            showNormal();
            return;
        }
        isMaximized() ? showNormal() : showMaximized();
    }

    void toggleFullScreen()
    {
        isFullScreen() ? showNormal() : showFullScreen();
    }

    void toggleSidebar()
    {
        m_sidebarCollapsed = !m_sidebarCollapsed;
        if (m_role == AuthRole::Admin) {
            const int currentPage = m_stack == nullptr ? 0 : m_stack->currentIndex();
            buildAdminUi();
            if (m_stack != nullptr && currentPage >= 0 && currentPage < m_stack->count()) {
                m_stack->setCurrentIndex(currentPage);
            }
            refreshAll();
        } else if (m_role == AuthRole::Student) {
            buildStudentPortalUi();
        }
    }

    void buildLoginUi()
    {
        auto *root = new QWidget(this);
        root->setObjectName("loginRoot");
        auto *outer = new QHBoxLayout(root);
        outer->setContentsMargins(0, 0, 0, 0);
        outer->setSpacing(0);

        auto *brandPanel = new QFrame(root);
        brandPanel->setObjectName("loginBrand");
        brandPanel->setMinimumWidth(320);
        brandPanel->setMaximumWidth(430);
        auto *brandLayout = new QVBoxLayout(brandPanel);
        brandLayout->setContentsMargins(42, 44, 42, 46);
        brandLayout->setSpacing(12);
        brandLayout->addWidget(buildBrandLockup(brandPanel, "Campus living, organized."));
        brandLayout->addSpacing(34);
        brandLayout->addWidget(label("Operations console", "loginBadge", brandPanel), 0, Qt::AlignLeft);
        brandLayout->addSpacing(18);
        auto *headline = label("Campus housing starts with Dormora.", "brand", brandPanel);
        headline->setStyleSheet("QLabel#brand { font-size: 34px; line-height: 42px; }");
        brandLayout->addWidget(headline);
        auto *sub = label("One calm workspace for rooms, meals, and resident records.", "brandSub", brandPanel);
        sub->setStyleSheet("QLabel#brandSub { font-size: 14px; line-height: 21px; }");
        brandLayout->addWidget(sub);
        brandLayout->addStretch();
        outer->addWidget(brandPanel);

        auto *content = new QWidget(root);
        content->setObjectName("contentPane");
        auto *contentLayout = new QVBoxLayout(content);
        contentLayout->setContentsMargins(72, 26, 72, 64);
        contentLayout->setSpacing(18);
        contentLayout->addWidget(buildWindowControls(), 0, Qt::AlignRight);
        contentLayout->addStretch(1);

        auto *loginCard = card(content);
        loginCard->setMinimumWidth(480);
        loginCard->setMaximumWidth(540);
        auto *loginLayout = new QVBoxLayout(loginCard);
        loginLayout->setContentsMargins(34, 30, 34, 30);
        loginLayout->setSpacing(16);
        loginLayout->addWidget(label("Sign in to Dormora", "pageTitle", loginCard));
        loginLayout->addWidget(label("Use staff credentials for the management console, or a student ID for the resident portal.", "pageKicker", loginCard));

        m_loginUserInput = new QLineEdit(loginCard);
        m_loginUserInput->setPlaceholderText("Enter username or student ID");
        m_loginUserInput->setMinimumHeight(44);
        m_loginPasswordInput = new QLineEdit(loginCard);
        m_loginPasswordInput->setPlaceholderText("Password");
        m_loginPasswordInput->setEchoMode(QLineEdit::Password);
        m_loginPasswordInput->setMinimumHeight(44);
        loginLayout->addWidget(fieldLabel("Username / Student ID", m_loginUserInput));
        loginLayout->addSpacing(2);
        loginLayout->addWidget(fieldLabel("Password", m_loginPasswordInput));
        loginLayout->addSpacing(4);

        auto *button = new QPushButton("Continue to workspace", loginCard);
        button->setMinimumHeight(44);
        button->setProperty("class", "primary");
        loginLayout->addWidget(button);
        m_loginFeedback = classLabel("", "muted");
        m_loginFeedback->setStyleSheet("QLabel { color: #B42318; font-weight: 700; }");
        loginLayout->addWidget(m_loginFeedback);

        connect(button, &QPushButton::clicked, this, [this] { authenticate(); });
        connect(m_loginPasswordInput, &QLineEdit::returnPressed, this, [this] { authenticate(); });
        connect(m_loginUserInput, &QLineEdit::returnPressed, this, [this] { authenticate(); });

        contentLayout->addWidget(loginCard, 0, Qt::AlignHCenter);
        contentLayout->addStretch(2);
        outer->addWidget(content, 1);

        setAppContent(root, true);
        showStatus("");
    }

    QFrame *buildWindowControls()
    {
        auto *controls = new QFrame(this);
        controls->setObjectName("titleControls");
        auto *layout = new QHBoxLayout(controls);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(8);

        auto *minimize = new QPushButton(QString::fromUtf8("−"), controls);
        minimize->setObjectName("windowButton");
        minimize->setText("-");
        minimize->setToolTip("Minimize");
        auto *close = new QPushButton(QString::fromUtf8("×"), controls);
        close->setObjectName("closeButton");
        close->setText("X");
        close->setToolTip("Close");
        auto *maximize = new QPushButton("[]", controls);
        maximize->setObjectName("windowButton");
        maximize->setToolTip("Maximize or restore");
        auto *fullscreen = new QPushButton("F11", controls);
        fullscreen->setObjectName("windowButtonWide");
        fullscreen->setToolTip("Toggle full screen");

        connect(minimize, &QPushButton::clicked, this, [this] { showMinimized(); });
        connect(maximize, &QPushButton::clicked, this, [this] { toggleMaximized(); });
        connect(fullscreen, &QPushButton::clicked, this, [this] { toggleFullScreen(); });
        connect(close, &QPushButton::clicked, this, [this] { this->close(); });
        layout->addWidget(minimize);
        layout->addWidget(maximize);
        layout->addWidget(fullscreen);
        layout->addWidget(close);
        return controls;
    }

    QWidget *buildBrandLockup(QWidget *parent, const QString &subtitle)
    {
        auto *wrap = new QWidget(parent);
        auto *layout = new QHBoxLayout(wrap);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(12);
        layout->addWidget(brandMark(wrap), 0, Qt::AlignTop);

        auto *text = new QVBoxLayout();
        text->setContentsMargins(0, 0, 0, 0);
        text->setSpacing(2);
        text->addWidget(label("Dormora", "brand", wrap));
        text->addWidget(label(subtitle, "brandSub", wrap));
        layout->addLayout(text, 1);
        return wrap;
    }

    void setAppContent(QWidget *content, bool firstRender = false)
    {
        auto *chrome = new QFrame(this);
        chrome->setObjectName("windowChrome");
        auto *layout = new QVBoxLayout(chrome);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(content);
        m_statusLabel = new QLabel(chrome);
        m_statusLabel->setObjectName("appStatus");
        m_statusLabel->setVisible(false);
        m_statusLabel->setMinimumHeight(16);
        m_statusLabel->setMaximumHeight(16);
        layout->addWidget(m_statusLabel);
        setCentralWidget(chrome);

        if (!m_enableAnimations) {
            return;
        }

        auto *effect = new QGraphicsOpacityEffect(chrome);
        chrome->setGraphicsEffect(effect);
        auto *animation = new QPropertyAnimation(effect, "opacity", chrome);
        animation->setDuration(firstRender ? 260 : 180);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        connect(animation, &QPropertyAnimation::finished, chrome, [chrome] {
            chrome->setGraphicsEffect(nullptr);
        });
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void authenticate()
    {
        const QString user = m_loginUserInput->text().trimmed();
        const QString password = m_loginPasswordInput->text();

        const QString adminKey = user.toLower();
        if (m_adminProfiles.contains(adminKey) && m_adminProfiles.value(adminKey).password == password) {
            m_role = AuthRole::Admin;
            m_currentAdminUsername = adminKey;
            m_currentStudentId.clear();
            buildAdminUi();
            refreshAll();
            showStatus("Signed in as " + m_adminProfiles.value(adminKey).displayName + ".");
            return;
        }

        const QString studentId = user.toUpper();
        if (m_university.hasStudent(studentId) && password == m_studentPortalPassword) {
            m_role = AuthRole::Student;
            m_currentStudentId = studentId;
            buildStudentPortalUi();
            showStatus("Signed in as " + m_university.student(studentId).fullName() + ".");
            return;
        }

        m_loginFeedback->setText("Credentials not recognized. Check the username or password.");
    }

public:
    void loginAsAdminForTest(const QString &username = "admin")
    {
        const QString key = username.toLower();
        m_role = AuthRole::Admin;
        m_currentAdminUsername = m_adminProfiles.contains(key) ? key : "admin";
        m_currentStudentId.clear();
        buildAdminUi();
        refreshAll();
        showStatus("Signed in as " + m_adminProfiles.value(m_currentAdminUsername).displayName + ".");
    }

    void loginAsStudentForTest(const QString &studentId)
    {
        m_role = AuthRole::Student;
        m_currentStudentId = studentId;
        buildStudentPortalUi();
        showStatus("Signed in as " + m_university.student(studentId).fullName() + ".");
    }

    void showPageForTest(int index)
    {
        if (m_stack != nullptr && index >= 0 && index < m_stack->count()) {
            m_stack->setCurrentIndex(index);
        }
    }

    void collapseSidebarForTest()
    {
        if (!m_sidebarCollapsed) {
            toggleSidebar();
        }
    }

    void addPersistenceProbeForTest()
    {
        m_university.addStudent(Student("S9999", "Persistence Probe", 1));
        saveAppState();
    }

    bool hasStudentForTest(const QString &studentId) const
    {
        return m_university.hasStudent(studentId);
    }

    bool hasRichExampleSeedDataForTest() const
    {
        if (m_university.students().size() < 100) {
            return false;
        }

        const QDate today = QDate::currentDate();
        for (const QString &dormitoryId : {QStringLiteral("D1"), QStringLiteral("D2")}) {
            for (int offset = 0; offset < 7; ++offset) {
                if (!m_university.dormitory(dormitoryId).restaurant().menuForDate(today.addDays(offset)).has_value()) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    void logout()
    {
        m_role = AuthRole::None;
        m_currentAdminUsername.clear();
        m_currentStudentId.clear();
        buildLoginUi();
    }

    void buildAdminUi()
    {

        auto *root = new QWidget(this);
        root->setObjectName("appRoot");
        auto *rootLayout = new QHBoxLayout(root);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);

        rootLayout->addWidget(buildSidebar());

        m_stack = new QStackedWidget(root);
        m_stack->addWidget(buildDashboardPage());
        m_stack->addWidget(buildStudentsPage());
        m_stack->addWidget(buildRestaurantPage());
        m_stack->addWidget(buildNeighborhoodsPage());
        rootLayout->addWidget(m_stack, 1);

        setAppContent(root);
    }

    QWidget *buildSidebar()
    {
        auto *sidebar = new QFrame(this);
        sidebar->setObjectName("sidebar");
        sidebar->setFixedWidth(m_sidebarCollapsed ? 76 : 238);

        auto *layout = new QVBoxLayout(sidebar);
        layout->setContentsMargins(m_sidebarCollapsed ? 14 : 22, 28, m_sidebarCollapsed ? 14 : 22, 22);
        layout->setSpacing(11);
        if (m_sidebarCollapsed) {
            layout->addWidget(brandMark(sidebar), 0, Qt::AlignHCenter);
        } else {
            layout->addWidget(buildBrandLockup(sidebar, "Campus living, organized."));
        }

        auto *collapseButton = new QPushButton(m_sidebarCollapsed ? ">>" : "<<", sidebar);
        collapseButton->setObjectName("sidebarToggle");
        collapseButton->setToolTip(m_sidebarCollapsed ? "Expand sidebar" : "Collapse sidebar");
        connect(collapseButton, &QPushButton::clicked, this, [this] { toggleSidebar(); });
        layout->addWidget(collapseButton);
        layout->addSpacing(m_sidebarCollapsed ? 12 : 22);

        auto *group = new QButtonGroup(sidebar);
        group->setExclusive(true);

        const QStringList items = {"Dashboard", "Residents", "Meals", "Neighborhoods"};
        const QStringList compactItems = {"D", "R", "M", "N"};
        for (int i = 0; i < items.size(); ++i) {
            auto *button = new QPushButton(m_sidebarCollapsed ? compactItems[i] : items[i], sidebar);
            button->setProperty("nav", true);
            button->setCheckable(true);
            button->setMinimumHeight(44);
            button->setToolTip(items[i]);
            group->addButton(button, i);
            layout->addWidget(button);
            connect(button, &QPushButton::clicked, this, [this, i] { m_stack->setCurrentIndex(i); });
            if (i == 0) {
                button->setChecked(true);
            }
        }

        layout->addStretch();
        auto *logoutButton = new QPushButton(m_sidebarCollapsed ? "L" : "Log out", sidebar);
        logoutButton->setProperty("nav", true);
        logoutButton->setMinimumHeight(44);
        logoutButton->setToolTip("Log out");
        connect(logoutButton, &QPushButton::clicked, this, [this] { logout(); });
        layout->addWidget(logoutButton);
        if (!m_sidebarCollapsed) {
            auto *hint = label("Dormora Console\nCampus operations", "brandSub", sidebar);
            if (!m_currentAdminUsername.isEmpty()) {
                const AdminProfile profile = m_adminProfiles.value(m_currentAdminUsername);
                hint->setText(profile.displayName + "\n" + (profile.fullAccess ? "Full access" : "Neighborhood access"));
            }
            layout->addWidget(hint);
        }
        return sidebar;
    }

    void buildStudentPortalUi()
    {
        auto *root = new QWidget(this);
        root->setObjectName("studentRoot");
        auto *rootLayout = new QHBoxLayout(root);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);

        auto *sidebar = new QFrame(root);
        sidebar->setObjectName("studentSidebar");
        sidebar->setFixedWidth(m_sidebarCollapsed ? 76 : 238);
        auto *sideLayout = new QVBoxLayout(sidebar);
        sideLayout->setContentsMargins(m_sidebarCollapsed ? 14 : 22, 28, m_sidebarCollapsed ? 14 : 22, 22);
        sideLayout->setSpacing(11);
        if (m_sidebarCollapsed) {
            sideLayout->addWidget(brandMark(sidebar), 0, Qt::AlignHCenter);
        } else {
            sideLayout->addWidget(buildBrandLockup(sidebar, "Resident portal"));
        }

        auto *collapseButton = new QPushButton(m_sidebarCollapsed ? ">>" : "<<", sidebar);
        collapseButton->setObjectName("sidebarToggle");
        collapseButton->setToolTip(m_sidebarCollapsed ? "Expand sidebar" : "Collapse sidebar");
        connect(collapseButton, &QPushButton::clicked, this, [this] { toggleSidebar(); });
        sideLayout->addWidget(collapseButton);
        sideLayout->addSpacing(m_sidebarCollapsed ? 12 : 22);
        if (!m_sidebarCollapsed) {
            sideLayout->addWidget(label("Room status\nMeal access\nResident record", "brandSub", sidebar));
        }
        sideLayout->addStretch();
        auto *logoutButton = new QPushButton(m_sidebarCollapsed ? "L" : "Log out", sidebar);
        logoutButton->setProperty("nav", true);
        logoutButton->setMinimumHeight(44);
        logoutButton->setToolTip("Log out");
        connect(logoutButton, &QPushButton::clicked, this, [this] { logout(); });
        sideLayout->addWidget(logoutButton);
        rootLayout->addWidget(sidebar);

        const Student &student = m_university.student(m_currentStudentId);
        auto *page = pageContainer("Welcome, " + student.fullName(), "Your room status and meal access for today.", true);
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        auto *cards = new QHBoxLayout();
        cards->setSpacing(18);
        cards->addWidget(buildStudentIdentityCard(student));
        cards->addWidget(buildStudentRoomCard(student));
        layout->addLayout(cards);

        auto *menuCard = card(page);
        menuCard->setObjectName("studentCard");
        auto *menuLayout = new QVBoxLayout(menuCard);
        menuLayout->setContentsMargins(20, 18, 20, 20);
        menuLayout->setSpacing(10);
        auto *menuTop = new QHBoxLayout();
        menuTop->addWidget(classLabel("Restaurant Menu", "cardTitle"));
        menuTop->addStretch();
        if (student.isAssigned()) {
            const Dormitory &dormitory = m_university.dormitory(student.dormitoryId().value());
            const auto menu = dormitory.restaurant().menuForDate(QDate::currentDate());
            menuTop->addWidget(statusPill(QDate::currentDate().toString("MMM d"), "#E6F4EE", "#1D7A57"));
            menuLayout->addLayout(menuTop);
            menuLayout->addWidget(classLabel(dormitory.restaurant().name() + " - " + dormitory.name(), "muted"));
            if (menu.has_value()) {
                menuLayout->addWidget(mealRow("Breakfast", menu->breakfast));
                menuLayout->addWidget(mealRow("Lunch", menu->lunch));
                menuLayout->addWidget(mealRow("Dinner", menu->dinner));
            } else {
                menuLayout->addWidget(statusPill("No menu for today", "#FFF7E6", "#B7791F"));
            }
        } else {
            menuLayout->addLayout(menuTop);
            menuLayout->addWidget(statusPill("Restaurant access requires a room assignment", "#FFF7E6", "#B7791F"));
        }
        layout->addWidget(menuCard, 1);

        rootLayout->addWidget(page, 1);
        setAppContent(root);
    }

    QWidget *buildStudentIdentityCard(const Student &student)
    {
        auto *box = card(this);
        box->setObjectName("studentCard");
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(9);
        layout->addWidget(statusPill(initials(student.fullName()), "#E6F4EE", "#123D32"), 0, Qt::AlignLeft);
        layout->addWidget(classLabel(student.fullName(), "cardTitle"));
        layout->addWidget(classLabel(student.id() + " - Academic year " + QString::number(student.academicYear()), "muted"));
        layout->addStretch();
        layout->addWidget(statusPill(student.isAssigned() ? "Resident" : "Unassigned", student.isAssigned() ? "#E6F4EE" : "#FFF7E6", student.isAssigned() ? "#1D7A57" : "#B7791F"), 0, Qt::AlignLeft);
        return box;
    }

    QWidget *buildStudentRoomCard(const Student &student)
    {
        auto *box = card(this);
        box->setObjectName("studentCard");
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(9);
        layout->addWidget(classLabel("Accommodation", "cardTitle"));
        if (student.isAssigned()) {
            const Dormitory &dormitory = m_university.dormitory(student.dormitoryId().value());
            const Room &room = dormitory.room(student.roomNumber().value());
            layout->addWidget(classLabel(dormitory.name(), "cardTitle"));
            layout->addWidget(classLabel("Room " + QString::number(room.number()), "muted"));
            layout->addWidget(classLabel(QString("Occupancy %1 / %2").arg(room.occupancy()).arg(room.capacity()), "muted"));
            layout->addStretch();
            layout->addWidget(statusPill("Active assignment", "#E6F4EE", "#1D7A57"), 0, Qt::AlignLeft);
        } else {
            layout->addWidget(classLabel("No room assigned yet.", "muted"));
            layout->addStretch();
        }
        return box;
    }

    QWidget *buildDashboardPage()
    {
        auto *page = pageContainer("Dormora Dashboard", "Monitor capacity, resident distribution, and today's meal activity.");
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        auto *metrics = new QHBoxLayout();
        metrics->setSpacing(16);
        m_residentsMetric = addMetric(metrics, "Residents", "0", "Assigned and unassigned students");
        m_availableRoomsMetric = addMetric(metrics, "Open Beds", "0", "Available room capacity");
        m_mealsMetric = addMetric(metrics, "Meals Today", "0", "Recorded resident meals");
        layout->addLayout(metrics);

        auto *main = new QHBoxLayout();
        main->setSpacing(18);
        main->addWidget(buildRoomMatrixCard(), 3);
        main->addWidget(buildTodayMenuSummaryCard(), 2);
        layout->addLayout(main, 1);

        return page;
    }

    QWidget *buildStudentsPage()
    {
        auto *page = pageContainer("Resident Desk", "Search by student ID or name, then review and modify the selected resident profile.");
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        layout->addWidget(buildStudentSearchCard(), 1);

        return page;
    }

    QWidget *buildRestaurantPage()
    {
        auto *page = pageContainer("Meal Desk", "Set daily menus and review resident meal access for each dormitory restaurant.");
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        auto *split = new QHBoxLayout();
        split->setSpacing(18);
        split->addWidget(buildMenuEditorCard(), 1);

        auto *preview = card(page);
        auto *previewLayout = new QVBoxLayout(preview);
        previewLayout->setContentsMargins(20, 16, 20, 16);
        previewLayout->setSpacing(10);
        previewLayout->addWidget(classLabel("Daily Menu Cards", "cardTitle"));
        auto *scroll = new QScrollArea(preview);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        auto *content = new QWidget(scroll);
        m_menuCards = new QVBoxLayout(content);
        m_menuCards->setContentsMargins(0, 0, 0, 0);
        m_menuCards->setSpacing(10);
        scroll->setWidget(content);
        previewLayout->addWidget(scroll);
        split->addWidget(preview, 2);

        layout->addLayout(split, 1);
        return page;
    }

    QWidget *buildNeighborhoodsPage()
    {
        auto *page = pageContainer("Dorm Neighborhoods", "Group dormitories into neighborhoods, copy them, and control which admins can manage each group.");
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        auto *split = new QHBoxLayout();
        split->setSpacing(18);

        auto *tableCard = card(page);
        auto *tableLayout = new QVBoxLayout(tableCard);
        tableLayout->setContentsMargins(20, 18, 20, 20);
        tableLayout->setSpacing(14);
        tableLayout->addWidget(classLabel("Neighborhood Registry", "cardTitle"));
        m_neighborhoodTable = new QTableWidget(tableCard);
        setupTable(m_neighborhoodTable, {"ID", "Name", "Dormitories", "Admins"});
        tableLayout->addWidget(m_neighborhoodTable, 1);
        split->addWidget(tableCard, 2);

        auto *controls = new QVBoxLayout();
        controls->setSpacing(18);
        controls->addWidget(buildNeighborhoodEditorCard());
        controls->addWidget(buildAccessEditorCard());
        split->addLayout(controls, 1);
        layout->addLayout(split, 1);
        return page;
    }

    QWidget *buildNeighborhoodEditorCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(16, 12, 16, 12);
        layout->setSpacing(6);
        layout->addWidget(classLabel("Add / Copy Neighborhood", "cardTitle"));

        m_neighborhoodIdInput = new QLineEdit(box);
        m_neighborhoodIdInput->setPlaceholderText("EAST");
        m_neighborhoodNameInput = new QLineEdit(box);
        m_neighborhoodNameInput->setPlaceholderText("East Campus Neighborhood");
        m_copyNeighborhoodInput = new QComboBox(box);

        layout->addWidget(fieldLabel("New ID", m_neighborhoodIdInput));
        layout->addWidget(fieldLabel("New Name", m_neighborhoodNameInput));
        layout->addWidget(fieldLabel("Copy From", m_copyNeighborhoodInput));

        auto *row = new QHBoxLayout();
        auto *add = new QPushButton("Add empty", box);
        add->setProperty("class", "primary");
        auto *copy = new QPushButton("Copy", box);
        row->addWidget(add);
        row->addWidget(copy);
        layout->addLayout(row);

        connect(add, &QPushButton::clicked, this, [this] { addNeighborhood(); });
        connect(copy, &QPushButton::clicked, this, [this] { copyNeighborhood(); });
        return box;
    }

    QWidget *buildAccessEditorCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(10);
        layout->addWidget(classLabel("Admin Access", "cardTitle"));

        m_adminAccessInput = new QComboBox(box);
        m_neighborhoodAccessInput = new QComboBox(box);
        layout->addWidget(fieldLabel("Admin", m_adminAccessInput));
        layout->addWidget(fieldLabel("Neighborhood", m_neighborhoodAccessInput));

        auto *row = new QHBoxLayout();
        auto *grant = new QPushButton("Grant", box);
        grant->setProperty("class", "primary");
        auto *revoke = new QPushButton("Revoke", box);
        revoke->setProperty("class", "danger");
        row->addWidget(grant);
        row->addWidget(revoke);
        layout->addLayout(row);

        connect(grant, &QPushButton::clicked, this, [this] { grantNeighborhoodAccess(); });
        connect(revoke, &QPushButton::clicked, this, [this] { revokeNeighborhoodAccess(); });
        return box;
    }

    QWidget *pageContainer(const QString &title, const QString &subtitle, bool studentTone = false)
    {
        auto *page = new QWidget(this);
        page->setObjectName(studentTone ? "studentContentPane" : "contentPane");
        auto *layout = new QVBoxLayout(page);
        layout->setContentsMargins(30, 24, 30, 24);
        layout->setSpacing(14);
        auto *titleRow = new QHBoxLayout();
        titleRow->setSpacing(12);
        titleRow->addWidget(label(title, "pageTitle", page), 1);
        titleRow->addWidget(buildWindowControls());
        layout->addLayout(titleRow);
        auto *subtitleRow = new QHBoxLayout();
        subtitleRow->setSpacing(12);
        subtitleRow->addWidget(label(subtitle, "pageKicker", page), 1);
        subtitleRow->addWidget(statusPill(studentTone ? "Resident portal" : "Dormora console",
                                          studentTone ? "#FFF7E6" : "#E6F4EE",
                                          studentTone ? "#B7791F" : "#1D7A57"));
        layout->addLayout(subtitleRow);
        return page;
    }

    QLabel *addMetric(QHBoxLayout *layout, const QString &name, const QString &value, const QString &description)
    {
        auto *box = card(this);
        box->setMinimumHeight(118);
        box->setMaximumHeight(132);
        auto *inner = new QVBoxLayout(box);
        inner->setContentsMargins(18, 14, 18, 14);
        inner->setSpacing(3);
        auto *nameLabel = classLabel(name, "metricLabel");
        nameLabel->setText(name.toUpper());
        auto *valueLabel = classLabel(value, "metricValue");
        inner->addWidget(nameLabel);
        inner->addWidget(valueLabel);
        inner->addWidget(classLabel(description, "muted"));
        layout->addWidget(box);
        return valueLabel;
    }

    QWidget *buildRoomMatrixCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(18, 14, 18, 14);
        layout->setSpacing(8);
        auto *top = new QHBoxLayout();
        top->addWidget(classLabel("Dormitory Room Matrix", "cardTitle"));
        top->addStretch();
        top->addWidget(statusPill("capacity rules", "#E6F4EE", "#1D7A57"));
        layout->addLayout(top);

        m_roomMatrix = new QTableWidget(box);
        setupTable(m_roomMatrix, {"Dormitory", "Room", "Capacity", "Residents", "State"});
        applyColumnWeights(m_roomMatrix, {160, 60, 75, 125, 75});
        layout->addWidget(m_roomMatrix, 1);
        return box;
    }

    QWidget *buildTodayMenuSummaryCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(14);
        layout->addWidget(classLabel("Today Menu", "cardTitle"));
        auto *note = classLabel("Restaurant menus are tied to dormitories. Only assigned residents can access them in the backend.", "muted");
        note->setWordWrap(true);
        layout->addWidget(note);
        m_todayMenuSummary = new QVBoxLayout();
        m_todayMenuSummary->setSpacing(8);
        layout->addLayout(m_todayMenuSummary);
        layout->addWidget(statusPill("resident-only access", "#E6F4EE", "#1D7A57"), 0, Qt::AlignLeft);
        layout->addStretch();
        return box;
    }

    QWidget *buildStudentSearchCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(14);
        auto *top = new QHBoxLayout();
        top->addWidget(classLabel("Find Student", "cardTitle"));
        top->addStretch();
        m_studentCountLabel = classLabel("0 visible", "muted");
        top->addWidget(m_studentCountLabel);
        layout->addLayout(top);

        m_studentSearchInput = new QLineEdit(box);
        m_studentSearchInput->setPlaceholderText("Search by name or student ID");
        m_studentSearchInput->setMinimumHeight(38);
        layout->addWidget(m_studentSearchInput);

        auto *filterRow = new QHBoxLayout();
        filterRow->setSpacing(8);
        m_studentFilterInput = new QComboBox(box);
        m_studentFilterInput->addItem("All students", "all");
        m_studentFilterInput->addItem("Assigned only", "assigned");
        m_studentFilterInput->addItem("Unassigned only", "unassigned");
        m_studentYearFilterInput = new QComboBox(box);
        m_studentYearFilterInput->addItem("Any year", 0);
        m_studentDormitoryFilterInput = new QComboBox(box);
        m_studentDormitoryFilterInput->addItem("Any dormitory", "all");
        auto *clearSearch = new QPushButton("Clear", box);
        filterRow->addWidget(m_studentFilterInput, 1);
        filterRow->addWidget(m_studentYearFilterInput, 1);
        filterRow->addWidget(m_studentDormitoryFilterInput, 1);
        filterRow->addWidget(clearSearch);
        layout->addLayout(filterRow);

        m_studentSuggestionPanel = new QWidget(box);
        auto *suggestionOuter = new QVBoxLayout(m_studentSuggestionPanel);
        suggestionOuter->setContentsMargins(0, 0, 0, 0);
        suggestionOuter->setSpacing(6);
        suggestionOuter->addWidget(classLabel("Suggested matches", "muted"));
        m_studentSuggestionList = new QVBoxLayout();
        m_studentSuggestionList->setContentsMargins(0, 0, 0, 0);
        m_studentSuggestionList->setSpacing(6);
        suggestionOuter->addLayout(m_studentSuggestionList);
        layout->addWidget(m_studentSuggestionPanel);
        m_studentSuggestionPanel->hide();

        auto *tableHint = classLabel("Select a row, then open the full profile in a separate editor. Double-click a row to open it immediately.", "muted");
        tableHint->setWordWrap(true);
        layout->addWidget(tableHint);
        m_studentTable = new QTableWidget(box);
        setupTable(m_studentTable, {"ID", "Full Name", "Year", "Assignment"});
        applyColumnWeights(m_studentTable, {72, 165, 55, 145});
        m_studentTable->setFocusPolicy(Qt::StrongFocus);
        m_studentTable->setMinimumHeight(215);
        layout->addWidget(m_studentTable, 1);

        auto *tableActions = new QHBoxLayout();
        tableActions->setSpacing(8);
        auto *addButton = new QPushButton("Add student", box);
        addButton->setMinimumWidth(140);
        auto *openProfile = new QPushButton("Open profile", box);
        openProfile->setProperty("class", "primary");
        openProfile->setMinimumWidth(150);
        tableActions->addWidget(addButton);
        tableActions->addStretch();
        tableActions->addWidget(openProfile);
        layout->addLayout(tableActions);

        connect(m_studentSearchInput, &QLineEdit::textChanged, this, [this] { refreshStudents(); });
        connect(m_studentSearchInput, &QLineEdit::returnPressed, this, [this] {
            openStudentFromSearch(m_studentSearchInput->text());
        });
        connect(m_studentFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshStudents(); });
        connect(m_studentYearFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshStudents(); });
        connect(m_studentDormitoryFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshStudents(); });
        connect(clearSearch, &QPushButton::clicked, this, [this] {
            m_studentSearchInput->clear();
            m_studentFilterInput->setCurrentIndex(0);
            m_studentYearFilterInput->setCurrentIndex(0);
            m_studentDormitoryFilterInput->setCurrentIndex(0);
        });
        connect(m_studentTable, &QTableWidget::itemSelectionChanged, this, [this] { selectStudentFromTable(); });
        connect(m_studentTable, &QTableWidget::itemActivated, this, [this](QTableWidgetItem *item) {
            if (item != nullptr) {
                selectStudentFromTable();
                openSelectedStudentProfile();
            }
        });
        connect(openProfile, &QPushButton::clicked, this, [this] { openSelectedStudentProfile(); });
        connect(addButton, &QPushButton::clicked, this, [this] { openAddStudentDialog(); });
        return box;
    }

    QWidget *buildStudentProfileCard()
    {
        auto *box = card(this);
        box->setMinimumWidth(420);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(18, 14, 18, 14);
        layout->setSpacing(8);

        layout->addWidget(classLabel("Student Profile", "cardTitle"));
        auto *scroll = new QScrollArea(box);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        auto *content = new QWidget(scroll);
        auto *profileLayout = new QVBoxLayout(content);
        profileLayout->setContentsMargins(0, 0, 4, 0);
        profileLayout->setSpacing(4);

        m_profileNameLabel = classLabel("No student selected", "cardTitle");
        m_profileMetaLabel = classLabel("Search for a student by name or ID.", "muted");
        m_profileAssignmentLabel = classLabel("Assignment will appear here.", "muted");
        m_profileRoomLabel = classLabel("Room details will appear here.", "muted");
        m_profileAccessLabel = classLabel("Access scope will appear here.", "muted");
        m_profileResidentStatusLabel = classLabel("Resident status will appear here.", "muted");
        profileLayout->addWidget(m_profileNameLabel);
        profileLayout->addWidget(m_profileMetaLabel);
        profileLayout->addWidget(m_profileAssignmentLabel);
        profileLayout->addWidget(m_profileRoomLabel);
        profileLayout->addWidget(m_profileAccessLabel);
        m_profileResidentStatusLabel->hide();

        m_profileNameInput = new QLineEdit(box);
        m_profileNameInput->setPlaceholderText("Full name");
        m_profileNameInput->setFixedHeight(32);
        m_profileYearInput = new QSpinBox(box);
        m_profileYearInput->setRange(1, 8);
        m_profileYearInput->setFixedHeight(32);
        profileLayout->addWidget(fieldLabel("Modify Full Name", m_profileNameInput));
        profileLayout->addWidget(fieldLabel("Modify Academic Year", m_profileYearInput));
        connect(m_profileNameInput, &QLineEdit::textEdited, this, [this] { markStudentProfileDirty(); });
        connect(m_profileYearInput, &QSpinBox::valueChanged, this, [this] { markStudentProfileDirty(); });

        auto *save = new QPushButton("Save changes", box);
        save->setProperty("class", "primary");
        save->setMinimumHeight(32);
        connect(save, &QPushButton::clicked, this, [this] { saveSelectedStudent(); });
        profileLayout->addWidget(save);

        auto *utilityRow = new QHBoxLayout();
        utilityRow->setSpacing(8);
        auto *reset = new QPushButton("Reset edits", box);
        auto *duplicate = new QPushButton("Duplicate", box);
        auto *deleteButton = new QPushButton("Delete", box);
        deleteButton->setProperty("class", "danger");
        reset->setFixedHeight(32);
        duplicate->setFixedHeight(32);
        deleteButton->setFixedHeight(32);
        utilityRow->addWidget(reset);
        utilityRow->addWidget(duplicate);
        utilityRow->addWidget(deleteButton);
        profileLayout->addLayout(utilityRow);
        connect(reset, &QPushButton::clicked, this, [this] { resetSelectedStudentEdits(); });
        connect(duplicate, &QPushButton::clicked, this, [this] { duplicateSelectedStudent(); });
        connect(deleteButton, &QPushButton::clicked, this, [this] { deleteSelectedStudent(); });

        profileLayout->addWidget(classLabel("Accommodation Actions", "cardTitle"));
        m_assignDormitoryInput = new QComboBox(box);
        m_assignDormitoryInput->setFixedHeight(32);
        m_assignRoomInput = new QSpinBox(box);
        m_assignRoomInput->setRange(1, 9999);
        m_assignRoomInput->setFixedHeight(32);
        profileLayout->addWidget(fieldLabel("Dormitory", m_assignDormitoryInput));
        profileLayout->addWidget(fieldLabel("Room Number", m_assignRoomInput));

        auto *actionRow = new QHBoxLayout();
        actionRow->setSpacing(8);
        auto *assign = new QPushButton("Assign", box);
        assign->setProperty("class", "primary");
        auto *remove = new QPushButton("Remove", box);
        remove->setProperty("class", "danger");
        assign->setFixedHeight(32);
        remove->setFixedHeight(32);
        actionRow->addWidget(assign);
        actionRow->addWidget(remove);
        profileLayout->addLayout(actionRow);
        connect(assign, &QPushButton::clicked, this, [this] { assignSelectedStudent(); });
        connect(remove, &QPushButton::clicked, this, [this] { removeSelectedAssignment(); });
        connect(m_assignDormitoryInput, &QComboBox::currentIndexChanged, this, [this] { updateSuggestedRoomNumber(); });
        m_profileStatusLabel = classLabel("", "muted");
        m_profileStatusLabel->setWordWrap(true);
        profileLayout->addWidget(m_profileStatusLabel);
        scroll->setWidget(content);
        layout->addWidget(scroll, 1);

        updateSelectedStudentProfile();
        return box;
    }

    QWidget *buildAddStudentCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(10);
        layout->addWidget(classLabel("Add Student", "cardTitle"));
        m_studentIdInput = new QLineEdit(box);
        m_studentIdInput->setPlaceholderText("S1005");
        m_studentNameInput = new QLineEdit(box);
        m_studentNameInput->setPlaceholderText("Full name");
        m_academicYearInput = new QSpinBox(box);
        m_academicYearInput->setRange(1, 8);
        layout->addWidget(fieldLabel("Student ID", m_studentIdInput));
        layout->addWidget(fieldLabel("Full Name", m_studentNameInput));
        layout->addWidget(fieldLabel("Academic Year", m_academicYearInput));
        auto *button = new QPushButton("Add student", box);
        button->setProperty("class", "primary");
        button->setObjectName("addStudentButton");
        connect(button, &QPushButton::clicked, this, [this] { addStudent(); });
        layout->addWidget(button);
        return box;
    }

    QWidget *buildAllocationCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(10);
        layout->addWidget(classLabel("Room Allocation", "cardTitle"));
        m_assignStudentInput = new QComboBox(box);
        m_assignDormitoryInput = new QComboBox(box);
        m_assignRoomInput = new QSpinBox(box);
        m_assignRoomInput->setRange(1, 9999);
        layout->addWidget(fieldLabel("Student", m_assignStudentInput));
        layout->addWidget(fieldLabel("Dormitory", m_assignDormitoryInput));
        layout->addWidget(fieldLabel("Room Number", m_assignRoomInput));

        auto *row = new QHBoxLayout();
        auto *assign = new QPushButton("Assign", box);
        assign->setProperty("class", "primary");
        auto *remove = new QPushButton("Remove", box);
        remove->setProperty("class", "danger");
        row->addWidget(assign);
        row->addWidget(remove);
        layout->addLayout(row);

        connect(assign, &QPushButton::clicked, this, [this] { assignStudent(); });
        connect(remove, &QPushButton::clicked, this, [this] { removeAssignment(); });
        connect(m_assignDormitoryInput, &QComboBox::currentIndexChanged, this, [this] { updateSuggestedRoomNumber(); });
        return box;
    }

    QWidget *buildMenuEditorCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(10);
        layout->addWidget(classLabel("Daily Menu Editor", "cardTitle"));
        m_menuDormitoryInput = new QComboBox(box);
        m_menuDateInput = new QDateEdit(QDate::currentDate(), box);
        m_menuDateInput->setCalendarPopup(true);
        m_breakfastInput = new QLineEdit(box);
        m_lunchInput = new QLineEdit(box);
        m_dinnerInput = new QLineEdit(box);
        m_breakfastInput->setPlaceholderText("Coffee and eggs");
        m_lunchInput->setPlaceholderText("Chicken and rice");
        m_dinnerInput->setPlaceholderText("Soup and salad");

        layout->addWidget(fieldLabel("Dormitory", m_menuDormitoryInput));
        layout->addWidget(fieldLabel("Date", m_menuDateInput));
        layout->addWidget(fieldLabel("Breakfast", m_breakfastInput));
        layout->addWidget(fieldLabel("Lunch", m_lunchInput));
        layout->addWidget(fieldLabel("Dinner", m_dinnerInput));
        auto *button = new QPushButton("Save menu", box);
        button->setProperty("class", "primary");
        connect(button, &QPushButton::clicked, this, [this] { saveMenu(); });
        connect(m_menuDateInput, &QDateEdit::dateChanged, this, [this] { refreshMenuCards(); });
        layout->addWidget(button);
        layout->addStretch();
        return box;
    }

    QWidget *fieldLabel(const QString &name, QWidget *input)
    {
        auto *wrap = new QWidget(this);
        auto *layout = new QVBoxLayout(wrap);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(4);
        layout->addWidget(classLabel(name, "muted"));
        layout->addWidget(input);
        return wrap;
    }

    QLabel *classLabel(const QString &text, const QString &className)
    {
        auto *result = new QLabel(text, this);
        result->setProperty("class", className);
        result->setWordWrap(true);
        return result;
    }

    QLabel *statusPill(const QString &text, const QString &background, const QString &foreground)
    {
        auto *pill = new QLabel(text, this);
        pill->setAlignment(Qt::AlignCenter);
        pill->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        pill->setStyleSheet(QString("QLabel { background: %1; color: %2; border-radius: 12px; padding: 5px 10px; font-weight: 700; }")
                                .arg(background, foreground));
        return pill;
    }

    bool currentAdminHasFullAccess() const
    {
        return !m_currentAdminUsername.isEmpty() && m_adminProfiles.value(m_currentAdminUsername).fullAccess;
    }

    bool currentAdminCanAccessNeighborhood(const QString &neighborhoodId) const
    {
        if (currentAdminHasFullAccess()) {
            return true;
        }
        return !m_currentAdminUsername.isEmpty()
            && m_adminProfiles.value(m_currentAdminUsername).neighborhoodIds.contains(neighborhoodId);
    }

    QString neighborhoodForDormitory(const QString &dormitoryId) const
    {
        for (const Neighborhood &neighborhood : m_neighborhoods) {
            if (neighborhood.dormitoryIds.contains(dormitoryId)) {
                return neighborhood.id;
            }
        }
        return {};
    }

    bool currentAdminCanAccessDormitory(const QString &dormitoryId) const
    {
        const QString neighborhoodId = neighborhoodForDormitory(dormitoryId);
        return neighborhoodId.isEmpty() ? currentAdminHasFullAccess() : currentAdminCanAccessNeighborhood(neighborhoodId);
    }

    QVector<Dormitory> visibleDormitories() const
    {
        QVector<Dormitory> result;
        for (const Dormitory &dormitory : m_university.dormitories()) {
            if (currentAdminCanAccessDormitory(dormitory.id())) {
                result.append(dormitory);
            }
        }
        return result;
    }

    QVector<Student> visibleStudents() const
    {
        QVector<Student> result;
        for (const Student &student : m_university.students()) {
            if (!student.isAssigned() || currentAdminCanAccessDormitory(student.dormitoryId().value())) {
                result.append(student);
            }
        }
        return result;
    }

    Neighborhood *findNeighborhood(const QString &id)
    {
        for (Neighborhood &neighborhood : m_neighborhoods) {
            if (neighborhood.id == id) {
                return &neighborhood;
            }
        }
        return nullptr;
    }

    const Neighborhood *findNeighborhood(const QString &id) const
    {
        for (const Neighborhood &neighborhood : m_neighborhoods) {
            if (neighborhood.id == id) {
                return &neighborhood;
            }
        }
        return nullptr;
    }

    void setupTable(QTableWidget *table, const QStringList &headers)
    {
        table->setColumnCount(headers.size());
        table->setHorizontalHeaderLabels(headers);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->setVisible(false);
        table->setAlternatingRowColors(false);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(false);
        table->verticalHeader()->setDefaultSectionSize(40);
        table->setFocusPolicy(Qt::NoFocus);
        table->setWordWrap(false);
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    void applyColumnWeights(QTableWidget *table, const QVector<int> &weights)
    {
        if (table == nullptr || weights.size() != table->columnCount()) {
            return;
        }
        auto *header = table->horizontalHeader();
        for (int i = 0; i < weights.size(); ++i) {
            header->setSectionResizeMode(i, QHeaderView::Interactive);
            header->resizeSection(i, weights[i]);
        }
        header->setStretchLastSection(true);
    }

    void addStudent()
    {
        runAction([this] {
            const QString newStudentId = m_studentIdInput->text().trimmed().toUpper();
            if (newStudentId.isEmpty() || m_studentNameInput->text().trimmed().isEmpty()) {
                throw DomainError("Student ID and full name are required.");
            }
            m_university.addStudent(Student(
                newStudentId,
                m_studentNameInput->text().trimmed(),
                m_academicYearInput->value()));
            m_selectedStudentId = newStudentId;
            m_studentSearchInput->setText(newStudentId);
            m_studentIdInput->clear();
            m_studentNameInput->clear();
            setStudentPanelMessage("Student added.");
        });
    }

    void openAddStudentDialog()
    {
        QDialog dialog(this);
        dialog.setWindowTitle("Add Student");
        dialog.setModal(true);
        dialog.resize(460, 300);

        auto *layout = new QVBoxLayout(&dialog);
        layout->setContentsMargins(20, 18, 20, 18);
        layout->setSpacing(12);
        layout->addWidget(classLabel("Add Student", "cardTitle"));

        auto *idInput = new QLineEdit(&dialog);
        idInput->setPlaceholderText(nextStudentId());
        auto *nameInput = new QLineEdit(&dialog);
        nameInput->setPlaceholderText("Full name");
        auto *yearInput = new QSpinBox(&dialog);
        yearInput->setRange(1, 8);
        yearInput->setValue(1);
        layout->addWidget(fieldLabel("Student ID", idInput));
        layout->addWidget(fieldLabel("Full Name", nameInput));
        layout->addWidget(fieldLabel("Academic Year", yearInput));

        auto *status = classLabel("", "muted");
        status->setStyleSheet("color: #B42318;");
        layout->addWidget(status);

        auto *actions = new QHBoxLayout();
        actions->addStretch();
        auto *cancel = new QPushButton("Cancel", &dialog);
        auto *save = new QPushButton("Add student", &dialog);
        save->setProperty("class", "primary");
        actions->addWidget(cancel);
        actions->addWidget(save);
        layout->addLayout(actions);

        connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
        connect(save, &QPushButton::clicked, &dialog, [this, &dialog, idInput, nameInput, yearInput, status] {
            const QString newStudentId = idInput->text().trimmed().isEmpty()
                ? nextStudentId()
                : idInput->text().trimmed().toUpper();
            const QString fullName = nameInput->text().trimmed();
            if (newStudentId.isEmpty() || fullName.isEmpty()) {
                status->setText("Student ID and full name are required.");
                return;
            }

            try {
                m_university.addStudent(Student(newStudentId, fullName, yearInput->value()));
                m_selectedStudentId = newStudentId;
                saveAppState();
                refreshAll();
                if (m_studentSearchInput != nullptr) {
                    m_studentSearchInput->setText(newStudentId);
                }
                selectStudentRow(newStudentId);
                dialog.accept();
            } catch (const DomainError &error) {
                status->setText(error.what());
            }
        });

        dialog.exec();
    }

    void saveSelectedStudent()
    {
        runAction([this] {
            Student &student = selectedStudent();
            if (m_profileNameInput->text().trimmed().isEmpty()) {
                throw DomainError("Student full name is required.");
            }
            student.setFullName(m_profileNameInput->text().trimmed());
            student.setAcademicYear(m_profileYearInput->value());
            m_studentProfileDirty = false;
            setStudentPanelMessage("Student profile updated.");
        });
    }

    void resetSelectedStudentEdits()
    {
        updateSelectedStudentProfile();
        m_studentProfileDirty = false;
        setStudentPanelMessage("Unsaved edits reset.");
    }

    void duplicateSelectedStudent()
    {
        runAction([this] {
            const Student &student = selectedStudent();
            const QString newId = nextStudentId();
            m_university.addStudent(Student(newId, student.fullName() + " Copy", student.academicYear()));
            m_selectedStudentId = newId;
            m_studentSearchInput->setText(newId);
            setStudentPanelMessage("Student duplicated as " + newId + ".");
        });
    }

    void deleteSelectedStudent()
    {
        if (m_selectedStudentId.isEmpty() || !m_university.hasStudent(m_selectedStudentId)) {
            QMessageBox::warning(this, "Rule feedback", "Select a student first.");
            return;
        }

        const Student &student = m_university.student(m_selectedStudentId);
        const QMessageBox::StandardButton answer = QMessageBox::question(
            this,
            "Delete student",
            "Delete " + student.fullName() + " (" + student.id() + ")? This also clears their room assignment.",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);
        if (answer != QMessageBox::Yes) {
            return;
        }

        runAction([this] {
            const QString removedId = m_selectedStudentId;
            m_university.removeStudent(removedId);
            m_selectedStudentId.clear();
            if (m_studentSearchInput != nullptr && m_studentSearchInput->text().trimmed().compare(removedId, Qt::CaseInsensitive) == 0) {
                m_studentSearchInput->clear();
            }
            setStudentPanelMessage("Student deleted.");
        });
    }

    void assignSelectedStudent()
    {
        runAction([this] {
            Student &student = selectedStudent();
            if (student.isAssigned()) {
                throw DomainError("Remove this student's current room before assigning a new one.");
            }
            const QString dormitoryId = m_assignDormitoryInput->currentData().toString();
            if (!currentAdminCanAccessDormitory(dormitoryId)) {
                throw DomainError("This admin cannot assign students in that dormitory.");
            }
            m_university.assignStudentToRoom(student.id(), dormitoryId, m_assignRoomInput->value());
            setStudentPanelMessage("Student assigned to room.");
        });
    }

    void removeSelectedAssignment()
    {
        runAction([this] {
            Student &student = selectedStudent();
            if (student.isAssigned() && !currentAdminCanAccessDormitory(student.dormitoryId().value())) {
                throw DomainError("This admin cannot remove assignments in that dormitory.");
            }
            m_university.removeStudentFromRoom(student.id());
            setStudentPanelMessage("Student removed from room.");
        });
    }

    void assignStudent()
    {
        runAction([this] {
            if (!currentAdminCanAccessDormitory(m_assignDormitoryInput->currentData().toString())) {
                throw DomainError("This admin cannot assign students in that dormitory.");
            }
            m_university.assignStudentToRoom(
                m_assignStudentInput->currentData().toString(),
                m_assignDormitoryInput->currentData().toString(),
                m_assignRoomInput->value());
            showStatus("Student assigned to room.");
        });
    }

    void removeAssignment()
    {
        runAction([this] {
            const Student &targetStudent = m_university.student(m_assignStudentInput->currentData().toString());
            if (targetStudent.isAssigned() && !currentAdminCanAccessDormitory(targetStudent.dormitoryId().value())) {
                throw DomainError("This admin cannot remove assignments in that dormitory.");
            }
            m_university.removeStudentFromRoom(m_assignStudentInput->currentData().toString());
            showStatus("Student removed from room.");
        });
    }

    void saveMenu()
    {
        runAction([this] {
            if (!currentAdminCanAccessDormitory(m_menuDormitoryInput->currentData().toString())) {
                throw DomainError("This admin cannot edit that dormitory restaurant.");
            }
            m_university.setRestaurantMenu(
                m_menuDormitoryInput->currentData().toString(),
                m_menuDateInput->date(),
                {m_breakfastInput->text().trimmed(), m_lunchInput->text().trimmed(), m_dinnerInput->text().trimmed()});
            showStatus("Menu saved.");
        });
    }

    void addNeighborhood()
    {
        runAction([this] {
            requireFullAccess();
            const QString id = m_neighborhoodIdInput->text().trimmed().toUpper();
            const QString name = m_neighborhoodNameInput->text().trimmed();
            if (id.isEmpty() || name.isEmpty()) {
                throw DomainError("Neighborhood ID and name are required.");
            }
            if (findNeighborhood(id) != nullptr) {
                throw DomainError("Neighborhood already exists.");
            }
            m_neighborhoods.append({id, name, {}});
            m_adminProfiles[m_currentAdminUsername].neighborhoodIds.insert(id);
            m_neighborhoodIdInput->clear();
            m_neighborhoodNameInput->clear();
            showStatus("Neighborhood added.");
        });
    }

    void copyNeighborhood()
    {
        runAction([this] {
            requireFullAccess();
            const QString sourceId = m_copyNeighborhoodInput->currentData().toString();
            const Neighborhood *source = findNeighborhood(sourceId);
            if (source == nullptr) {
                throw DomainError("Choose a neighborhood to copy.");
            }

            const QString id = m_neighborhoodIdInput->text().trimmed().toUpper();
            const QString name = m_neighborhoodNameInput->text().trimmed();
            if (id.isEmpty() || name.isEmpty()) {
                throw DomainError("New neighborhood ID and name are required.");
            }
            if (findNeighborhood(id) != nullptr) {
                throw DomainError("Neighborhood already exists.");
            }

            Neighborhood copy{id, name, {}};
            QVector<Dormitory> newDormitories;
            int dormIndex = 1;
            for (const QString &sourceDormitoryId : source->dormitoryIds) {
                const Dormitory &sourceDormitory = m_university.dormitory(sourceDormitoryId);
                const QString newDormitoryId = id + "-D" + QString::number(dormIndex);
                if (m_university.hasDormitory(newDormitoryId)) {
                    throw DomainError("Copied dormitory ID already exists.");
                }
                Dormitory newDormitory(
                    newDormitoryId,
                    name + " Residence " + QString::number(dormIndex),
                    sourceDormitory.capacity(),
                    Restaurant(name + " Restaurant " + QString::number(dormIndex)));
                for (const Room &room : sourceDormitory.rooms()) {
                    newDormitory.addRoom(Room(room.number(), room.capacity()));
                }
                newDormitories.append(newDormitory);
                copy.dormitoryIds.append(newDormitoryId);
                ++dormIndex;
            }

            for (const Dormitory &newDormitory : newDormitories) {
                m_university.addDormitory(newDormitory);
            }
            m_neighborhoods.append(copy);
            m_adminProfiles[m_currentAdminUsername].neighborhoodIds.insert(id);
            m_neighborhoodIdInput->clear();
            m_neighborhoodNameInput->clear();
            showStatus("Neighborhood copied.");
        });
    }

    void grantNeighborhoodAccess()
    {
        runAction([this] {
            requireFullAccess();
            const QString username = m_adminAccessInput->currentData().toString();
            const QString neighborhoodId = m_neighborhoodAccessInput->currentData().toString();
            if (!m_adminProfiles.contains(username) || findNeighborhood(neighborhoodId) == nullptr) {
                throw DomainError("Choose a valid admin and neighborhood.");
            }
            if (m_adminProfiles[username].fullAccess) {
                throw DomainError("Global administrators already have access to every neighborhood.");
            }
            m_adminProfiles[username].neighborhoodIds.insert(neighborhoodId);
            showStatus("Neighborhood access granted.");
        });
    }

    void revokeNeighborhoodAccess()
    {
        runAction([this] {
            requireFullAccess();
            const QString username = m_adminAccessInput->currentData().toString();
            const QString neighborhoodId = m_neighborhoodAccessInput->currentData().toString();
            if (!m_adminProfiles.contains(username) || findNeighborhood(neighborhoodId) == nullptr) {
                throw DomainError("Choose a valid admin and neighborhood.");
            }
            if (m_adminProfiles[username].fullAccess) {
                throw DomainError("Global administrator access cannot be scoped.");
            }
            m_adminProfiles[username].neighborhoodIds.remove(neighborhoodId);
            showStatus("Neighborhood access revoked.");
        });
    }

    void requireFullAccess() const
    {
        if (!currentAdminHasFullAccess()) {
            throw DomainError("Only full-access administrators can manage neighborhoods and admin permissions.");
        }
    }

    void runAction(const std::function<void()> &action)
    {
        try {
            action();
            saveAppState();
            refreshAll();
        } catch (const DomainError &error) {
            setStudentPanelMessage(error.what(), true);
            QMessageBox::warning(this, "Rule feedback", error.what());
        }
    }

    void refreshAll()
    {
        refreshMetrics();
        refreshRoomMatrix();
        refreshTodayMenuSummary();
        refreshStudents();
        refreshResidentCards();
        refreshCombos();
        refreshMenuCards();
        refreshNeighborhoods();
    }

    void refreshNeighborhoods()
    {
        if (m_neighborhoodTable == nullptr) {
            return;
        }

        m_neighborhoodTable->setRowCount(0);
        for (const Neighborhood &neighborhood : m_neighborhoods) {
            if (!currentAdminCanAccessNeighborhood(neighborhood.id)) {
                continue;
            }

            QStringList adminNames;
            for (const AdminProfile &profile : m_adminProfiles) {
                if (profile.fullAccess || profile.neighborhoodIds.contains(neighborhood.id)) {
                    adminNames.append(profile.displayName);
                }
            }
            adminNames.sort();

            const int row = m_neighborhoodTable->rowCount();
            m_neighborhoodTable->insertRow(row);
            setCell(m_neighborhoodTable, row, 0, neighborhood.id, "#13231D");
            setCell(m_neighborhoodTable, row, 1, neighborhood.name, "#13231D");
            setCell(m_neighborhoodTable, row, 2, neighborhood.dormitoryIds.join(", "), "#667085");
            setCell(m_neighborhoodTable, row, 3, adminNames.join(", "), "#1D7A57");
        }

        const QString selectedCopy = m_copyNeighborhoodInput->currentData().toString();
        const QString selectedAdmin = m_adminAccessInput->currentData().toString();
        const QString selectedAccessNeighborhood = m_neighborhoodAccessInput->currentData().toString();

        m_copyNeighborhoodInput->clear();
        m_neighborhoodAccessInput->clear();
        for (const Neighborhood &neighborhood : m_neighborhoods) {
            if (currentAdminCanAccessNeighborhood(neighborhood.id)) {
                const QString labelText = neighborhood.id + " - " + neighborhood.name;
                m_copyNeighborhoodInput->addItem(labelText, neighborhood.id);
                m_neighborhoodAccessInput->addItem(labelText, neighborhood.id);
            }
        }

        m_adminAccessInput->clear();
        for (const AdminProfile &profile : m_adminProfiles) {
            m_adminAccessInput->addItem(profile.username + " - " + profile.displayName, profile.username);
        }
        selectComboData(m_copyNeighborhoodInput, selectedCopy);
        selectComboData(m_adminAccessInput, selectedAdmin);
        selectComboData(m_neighborhoodAccessInput, selectedAccessNeighborhood);

        const bool canManage = currentAdminHasFullAccess();
        m_neighborhoodIdInput->setEnabled(canManage);
        m_neighborhoodNameInput->setEnabled(canManage);
        m_copyNeighborhoodInput->setEnabled(canManage);
        m_adminAccessInput->setEnabled(canManage);
        m_neighborhoodAccessInput->setEnabled(canManage);
    }

    void refreshTodayMenuSummary()
    {
        clearLayout(m_todayMenuSummary);
        const QDate today = QDate::currentDate();
        for (const Dormitory &dormitory : visibleDormitories()) {
            const auto menu = dormitory.restaurant().menuForDate(today);
            auto *item = new QFrame(this);
            item->setObjectName("todayMenuItem");
            auto *layout = new QVBoxLayout(item);
            layout->setContentsMargins(12, 10, 12, 10);
            layout->setSpacing(5);
            auto *top = new QHBoxLayout();
            top->addWidget(classLabel(dormitory.name(), "cardTitle"));
            top->addStretch();
            top->addWidget(statusPill(dormitory.id(), "#E6F4EE", "#1D7A57"));
            layout->addLayout(top);
            if (menu.has_value()) {
                layout->addWidget(classLabel("Lunch: " + menu->lunch, "muted"));
                layout->addWidget(classLabel("Dinner: " + menu->dinner, "muted"));
            } else {
                layout->addWidget(classLabel("No menu set for today", "muted"));
            }
            m_todayMenuSummary->addWidget(item);
        }
    }

    void refreshMetrics()
    {
        int totalStudents = 0;
        int openBeds = 0;
        int mealsToday = 0;
        const QDate today = QDate::currentDate();

        totalStudents = visibleStudents().size();
        for (const Dormitory &dormitory : visibleDormitories()) {
            for (const Room &room : dormitory.rooms()) {
                openBeds += room.capacity() - room.occupancy();
            }
            mealsToday += dormitory.restaurant().mealsServedOn(today);
        }

        m_residentsMetric->setText(QString::number(totalStudents));
        m_availableRoomsMetric->setText(QString::number(openBeds));
        m_mealsMetric->setText(QString::number(mealsToday));
    }

    void refreshRoomMatrix()
    {
        m_roomMatrix->setRowCount(0);
        for (const Dormitory &dormitory : visibleDormitories()) {
            for (const Room &room : dormitory.rooms()) {
                const int row = m_roomMatrix->rowCount();
                m_roomMatrix->insertRow(row);
                setCell(m_roomMatrix, row, 0, dormitory.name(), "#13231D");
                setCell(m_roomMatrix, row, 1, QString::number(room.number()), "#13231D");
                setCell(m_roomMatrix, row, 2, QString("%1 / %2").arg(room.occupancy()).arg(room.capacity()), "#667085");
                setCell(m_roomMatrix, row, 3, room.studentIds().isEmpty() ? "-" : room.studentIds().join(", "), "#13231D");
                const bool full = room.isFull();
                setStatusCell(m_roomMatrix, row, 4, full ? "Full" : "Available", full ? "#FDECEC" : "#E6F4EE", full ? "#B42318" : "#1D7A57");
            }
        }
    }

    void refreshStudentSearchSuggestions()
    {
        if (m_studentSuggestionPanel == nullptr || m_studentSuggestionList == nullptr || m_studentSearchInput == nullptr) {
            return;
        }

        clearLayout(m_studentSuggestionList);
        const QString query = m_studentSearchInput->text().trimmed().toLower();
        if (query.size() < 2) {
            m_studentSuggestionPanel->hide();
            return;
        }

        int shown = 0;
        for (const Student &student : visibleStudents()) {
            const QString labelText = student.id() + " - " + student.fullName()
                + (student.isAssigned() ? " | " + assignmentText(student) : " | Unassigned");
            if (!student.id().toLower().contains(query)
                && !student.fullName().toLower().contains(query)
                && !labelText.toLower().contains(query)) {
                continue;
            }

            auto *button = new QPushButton(labelText, m_studentSuggestionPanel);
            button->setObjectName("suggestionButton");
            button->setProperty("studentId", student.id());
            button->setMinimumHeight(32);
            button->setCursor(Qt::PointingHandCursor);
            connect(button, &QPushButton::clicked, this, [this, id = student.id()] {
                openStudentFromSearch(id);
            });
            m_studentSuggestionList->addWidget(button);
            ++shown;
            if (shown == 4) {
                break;
            }
        }
        m_studentSuggestionPanel->setVisible(shown > 0);
    }

    void refreshStudentFilterOptions()
    {
        if (m_studentYearFilterInput == nullptr || m_studentDormitoryFilterInput == nullptr) {
            return;
        }

        const int selectedYear = m_studentYearFilterInput->currentData().toInt();
        const QString selectedDormitory = m_studentDormitoryFilterInput->currentData().toString();

        {
            QSignalBlocker yearBlocker(m_studentYearFilterInput);
            QSet<int> years;
            for (const Student &student : visibleStudents()) {
                years.insert(student.academicYear());
            }
            QList<int> sortedYears = years.values();
            std::sort(sortedYears.begin(), sortedYears.end());

            m_studentYearFilterInput->clear();
            m_studentYearFilterInput->addItem("Any year", 0);
            for (int year : sortedYears) {
                m_studentYearFilterInput->addItem("Year " + QString::number(year), year);
            }
            selectComboData(m_studentYearFilterInput, selectedYear);
        }

        {
            QSignalBlocker dormitoryBlocker(m_studentDormitoryFilterInput);
            m_studentDormitoryFilterInput->clear();
            m_studentDormitoryFilterInput->addItem("Any dormitory", "all");
            m_studentDormitoryFilterInput->addItem("Unassigned", "unassigned");
            for (const Dormitory &dormitory : visibleDormitories()) {
                m_studentDormitoryFilterInput->addItem(dormitory.name(), dormitory.id());
            }
            selectComboData(m_studentDormitoryFilterInput, selectedDormitory);
        }
    }

    QString normalizedStudentSearchQuery() const
    {
        if (m_studentSearchInput == nullptr) {
            return {};
        }

        const QString text = m_studentSearchInput->text().trimmed();
        const QString studentId = studentIdFromSearchText(text);
        return studentId.isEmpty() ? text.toLower() : studentId.toLower();
    }

    void refreshStudents()
    {
        if (m_studentTable == nullptr) {
            return;
        }
        refreshStudentFilterOptions();
        refreshStudentSearchSuggestions();
        m_studentTable->setRowCount(0);
        const QString query = normalizedStudentSearchQuery();
        const QString filter = m_studentFilterInput == nullptr ? QStringLiteral("all") : m_studentFilterInput->currentData().toString();
        const int yearFilter = m_studentYearFilterInput == nullptr ? 0 : m_studentYearFilterInput->currentData().toInt();
        const QString dormitoryFilter = m_studentDormitoryFilterInput == nullptr ? QStringLiteral("all") : m_studentDormitoryFilterInput->currentData().toString();
        int visibleCount = 0;
        for (const Student &student : visibleStudents()) {
            if (!query.isEmpty()
                && !student.id().toLower().contains(query)
                && !student.fullName().toLower().contains(query)) {
                continue;
            }
            if (filter == "assigned" && !student.isAssigned()) {
                continue;
            }
            if (filter == "unassigned" && student.isAssigned()) {
                continue;
            }
            if (yearFilter > 0 && student.academicYear() != yearFilter) {
                continue;
            }
            if (dormitoryFilter == "unassigned" && student.isAssigned()) {
                continue;
            }
            if (dormitoryFilter != "all" && dormitoryFilter != "unassigned"
                && (!student.isAssigned() || student.dormitoryId().value() != dormitoryFilter)) {
                continue;
            }
            const int row = m_studentTable->rowCount();
            m_studentTable->insertRow(row);
            setCell(m_studentTable, row, 0, student.id(), "#13231D");
            setCell(m_studentTable, row, 1, student.fullName(), "#13231D");
            setCell(m_studentTable, row, 2, QString::number(student.academicYear()), "#667085");
            setCell(m_studentTable, row, 3, assignmentText(student), student.isAssigned() ? "#1D7A57" : "#B7791F");
            ++visibleCount;
        }
        if (m_studentCountLabel != nullptr) {
            m_studentCountLabel->setText(QString::number(visibleCount) + " visible");
        }
        selectStudentRow(m_selectedStudentId);
        if (!m_studentProfileDirty
            && (m_selectedStudentId.isEmpty() || m_studentTable->selectedItems().isEmpty())
            && m_studentTable->rowCount() > 0) {
            m_studentTable->selectRow(0);
        }
        if (!m_studentProfileDirty && m_studentTable->rowCount() == 0) {
            m_selectedStudentId.clear();
        }
        if (!m_studentProfileDirty) {
            updateSelectedStudentProfile();
        }
    }

    void refreshResidentCards()
    {
        if (m_residentList == nullptr) {
            return;
        }
        clearLayout(m_residentList);
        int index = 0;
        for (const Student &student : visibleStudents()) {
            auto *item = card(this);
            auto *layout = new QHBoxLayout(item);
            layout->setContentsMargins(14, 12, 14, 12);
            layout->setSpacing(12);
            layout->addWidget(statusPill(initials(student.fullName()), "#E6F4EE", "#123D32"));
            auto *info = new QVBoxLayout();
            info->setSpacing(3);
            info->addWidget(classLabel(student.fullName(), "cardTitle"));
            info->addWidget(classLabel(student.id() + " - Academic year " + QString::number(student.academicYear()), "muted"));
            info->addWidget(classLabel(assignmentText(student), "muted"));
            layout->addLayout(info, 1);
            layout->addWidget(statusPill(student.isAssigned() ? "Resident" : "Unassigned", student.isAssigned() ? "#E6F4EE" : "#FFF7E6", student.isAssigned() ? "#1D7A57" : "#B7791F"));
            m_residentList->addWidget(item, index / 2, index % 2);
            ++index;
        }
    }

    void refreshCombos()
    {
        const QString selectedStudent = m_assignStudentInput == nullptr ? QString() : m_assignStudentInput->currentData().toString();
        const QString selectedDormitory = m_assignDormitoryInput == nullptr ? QString() : m_assignDormitoryInput->currentData().toString();
        const QString selectedMenuDormitory = m_menuDormitoryInput == nullptr ? QString() : m_menuDormitoryInput->currentData().toString();

        if (m_assignStudentInput != nullptr) {
            m_assignStudentInput->clear();
            for (const Student &student : visibleStudents()) {
                m_assignStudentInput->addItem(student.id() + " - " + student.fullName(), student.id());
            }
            selectComboData(m_assignStudentInput, selectedStudent);
        }

        if (m_assignDormitoryInput != nullptr) {
            m_assignDormitoryInput->clear();
        }
        if (m_menuDormitoryInput != nullptr) {
            m_menuDormitoryInput->clear();
        }
        for (const Dormitory &dormitory : visibleDormitories()) {
            const QString labelText = dormitory.id() + " - " + dormitory.name();
            if (m_assignDormitoryInput != nullptr) {
                m_assignDormitoryInput->addItem(labelText, dormitory.id());
            }
            if (m_menuDormitoryInput != nullptr) {
                m_menuDormitoryInput->addItem(labelText, dormitory.id());
            }
        }
        if (m_assignDormitoryInput != nullptr) {
            selectComboData(m_assignDormitoryInput, selectedDormitory);
        }
        if (m_menuDormitoryInput != nullptr) {
            selectComboData(m_menuDormitoryInput, selectedMenuDormitory);
        }
        updateSuggestedRoomNumber();
    }

    void updateSuggestedRoomNumber()
    {
        if (m_assignDormitoryInput == nullptr || m_assignRoomInput == nullptr) {
            return;
        }
        const QString dormitoryId = m_assignDormitoryInput->currentData().toString();
        if (dormitoryId.isEmpty()) {
            return;
        }
        const QVector<Room> rooms = m_university.dormitory(dormitoryId).availableRooms();
        if (!rooms.isEmpty()) {
            m_assignRoomInput->setValue(rooms.first().number());
        }
    }

    void refreshMenuCards()
    {
        if (m_menuCards == nullptr || m_menuDateInput == nullptr) {
            return;
        }
        clearLayout(m_menuCards);
        const QDate date = m_menuDateInput->date();

        for (const Dormitory &dormitory : visibleDormitories()) {
            auto *item = card(this);
            auto *layout = new QVBoxLayout(item);
            layout->setContentsMargins(16, 12, 16, 12);
            layout->setSpacing(9);
            auto *top = new QHBoxLayout();
            top->addWidget(classLabel(dormitory.restaurant().name(), "cardTitle"));
            top->addStretch();
            top->addWidget(statusPill(dormitory.id(), "#E6F4EE", "#1D7A57"));
            layout->addLayout(top);
            layout->addWidget(classLabel(dormitory.name() + " - " + date.toString("yyyy-MM-dd"), "muted"));

            const auto menu = dormitory.restaurant().menuForDate(date);
            if (menu.has_value()) {
                layout->addWidget(mealRow("Breakfast", menu->breakfast));
                layout->addWidget(mealRow("Lunch", menu->lunch));
                layout->addWidget(mealRow("Dinner", menu->dinner));
            } else {
                layout->addWidget(statusPill("No menu for selected date", "#FFF7E6", "#B7791F"));
            }
            m_menuCards->addWidget(item);
        }
        m_menuCards->addStretch();
    }

    void selectStudentFromTable()
    {
        if (m_restoringStudentSelection || m_studentTable == nullptr || m_studentTable->selectedItems().isEmpty()) {
            return;
        }
        const int row = m_studentTable->currentRow();
        if (row < 0) {
            return;
        }
        const QString requestedStudentId = m_studentTable->item(row, 0)->text();
        if (requestedStudentId == m_selectedStudentId) {
            return;
        }
        if (!confirmDiscardStudentProfileEdits()) {
            restoreStudentSelection();
            setStudentPanelMessage("Save or reset profile edits before opening another student.", true);
            return;
        }
        m_selectedStudentId = requestedStudentId;
        m_studentProfileDirty = false;
        updateSelectedStudentProfile();
    }

    void selectStudentRow(const QString &studentId)
    {
        if (m_studentTable == nullptr || studentId.isEmpty()) {
            return;
        }
        m_studentTable->clearSelection();
        for (int row = 0; row < m_studentTable->rowCount(); ++row) {
            if (m_studentTable->item(row, 0)->text() == studentId) {
                m_studentTable->selectRow(row);
                return;
            }
        }
    }

    void restoreStudentSelection()
    {
        if (m_studentTable == nullptr) {
            return;
        }

        QSignalBlocker blocker(m_studentTable);
        m_restoringStudentSelection = true;
        selectStudentRow(m_selectedStudentId);
        m_restoringStudentSelection = false;
    }

    bool confirmDiscardStudentProfileEdits()
    {
        if (!m_studentProfileDirty) {
            return true;
        }

        const QMessageBox::StandardButton answer = QMessageBox::question(
            this,
            "Unsaved profile edits",
            "This profile has unsaved edits. Discard them and open another student?",
            QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Cancel);
        return answer == QMessageBox::Discard;
    }

    void markStudentProfileDirty()
    {
        if (m_loadingStudentProfile || m_selectedStudentId.isEmpty()) {
            return;
        }

        m_studentProfileDirty = true;
        setStudentPanelMessage("Unsaved profile edits. Save changes or reset edits before opening another student.");
    }

    QString studentIdFromSearchText(const QString &text) const
    {
        const QString trimmed = text.trimmed();
        if (trimmed.isEmpty()) {
            return {};
        }

        const QString possibleId = trimmed.section(" - ", 0, 0).trimmed().toUpper();
        if (m_university.hasStudent(possibleId)) {
            return possibleId;
        }

        for (const Student &student : visibleStudents()) {
            if (student.id().compare(trimmed, Qt::CaseInsensitive) == 0
                || student.fullName().compare(trimmed, Qt::CaseInsensitive) == 0
                || (student.id() + " - " + student.fullName()).compare(trimmed, Qt::CaseInsensitive) == 0) {
                return student.id();
            }
        }
        return {};
    }

    void openStudentFromSearch(const QString &text)
    {
        const QString studentId = studentIdFromSearchText(text);
        if (studentId.isEmpty() || !m_university.hasStudent(studentId)) {
            return;
        }
        if (!confirmDiscardStudentProfileEdits()) {
            return;
        }

        {
            QSignalBlocker statusBlocker(m_studentFilterInput);
            QSignalBlocker yearBlocker(m_studentYearFilterInput);
            QSignalBlocker dormitoryBlocker(m_studentDormitoryFilterInput);
            m_studentFilterInput->setCurrentIndex(0);
            m_studentYearFilterInput->setCurrentIndex(0);
            m_studentDormitoryFilterInput->setCurrentIndex(0);
        }
        m_studentProfileDirty = false;
        m_selectedStudentId = studentId;
        if (m_studentSearchInput != nullptr) {
            QSignalBlocker searchBlocker(m_studentSearchInput);
            m_studentSearchInput->setText(studentId);
        }
        refreshStudents();
        selectStudentRow(studentId);
        m_studentSuggestionPanel->hide();
        openSelectedStudentProfile();
    }

    void openSelectedStudentProfile()
    {
        if (m_selectedStudentId.isEmpty() || !m_university.hasStudent(m_selectedStudentId)) {
            QMessageBox::information(this, "Open profile", "Select a student first.");
            return;
        }
        if (m_studentProfileDialog != nullptr && m_studentProfileDialog->isVisible()) {
            m_studentProfileDialog->raise();
            m_studentProfileDialog->activateWindow();
            return;
        }

        m_studentProfileDialog = new QDialog(this);
        m_studentProfileDialog->setWindowTitle("Student Profile - " + m_selectedStudentId);
        m_studentProfileDialog->setModal(false);
        m_studentProfileDialog->resize(760, 640);
        m_studentProfileDialog->setMinimumSize(680, 560);

        auto *dialogLayout = new QVBoxLayout(m_studentProfileDialog);
        dialogLayout->setContentsMargins(18, 18, 18, 18);
        dialogLayout->setSpacing(12);
        dialogLayout->addWidget(buildStudentProfileCard(), 1);

        connect(m_studentProfileDialog, &QDialog::finished, this, [this] {
            m_studentProfileDialog = nullptr;
            m_studentProfileDirty = false;
        });
        updateSelectedStudentProfile();
        m_studentProfileDialog->show();
    }

    Student &selectedStudent()
    {
        if (m_selectedStudentId.isEmpty() || !m_university.hasStudent(m_selectedStudentId)) {
            throw DomainError("Select a student first.");
        }
        return m_university.student(m_selectedStudentId);
    }

    void updateSelectedStudentProfile()
    {
        if (m_profileNameLabel == nullptr) {
            return;
        }

        if (m_selectedStudentId.isEmpty() || !m_university.hasStudent(m_selectedStudentId)) {
            m_loadingStudentProfile = true;
            m_profileNameLabel->setText("No student selected");
            m_profileMetaLabel->setText("Search for a student by name or ID.");
            m_profileAssignmentLabel->setText("Assignment will appear here.");
            m_profileRoomLabel->setText("Room details will appear here.");
            m_profileAccessLabel->setText("Access scope will appear here.");
            m_profileResidentStatusLabel->clear();
            m_profileNameInput->clear();
            m_profileYearInput->setValue(1);
            m_loadingStudentProfile = false;
            m_studentProfileDirty = false;
            setProfileControlsEnabled(false);
            return;
        }

        const Student &student = m_university.student(m_selectedStudentId);
        m_loadingStudentProfile = true;
        QSignalBlocker nameBlocker(m_profileNameInput);
        QSignalBlocker yearBlocker(m_profileYearInput);
        m_profileNameLabel->setText(student.fullName());
        m_profileMetaLabel->setText("ID: " + student.id()
            + " | Year " + QString::number(student.academicYear())
            + " | " + residentStatusText(student));
        m_profileAssignmentLabel->setText("Assignment: " + assignmentText(student));
        m_profileRoomLabel->setText(roomDetailText(student));
        m_profileAccessLabel->setText(accessDetailText(student));
        m_profileResidentStatusLabel->clear();
        m_profileNameInput->setText(student.fullName());
        m_profileYearInput->setValue(student.academicYear());
        m_loadingStudentProfile = false;
        m_studentProfileDirty = false;
        setProfileControlsEnabled(true);
    }

    void setProfileControlsEnabled(bool enabled)
    {
        m_profileNameInput->setEnabled(enabled);
        m_profileYearInput->setEnabled(enabled);
        if (m_assignDormitoryInput != nullptr) {
            m_assignDormitoryInput->setEnabled(enabled);
        }
        if (m_assignRoomInput != nullptr) {
            m_assignRoomInput->setEnabled(enabled);
        }
    }

    QString roomDetailText(const Student &student) const
    {
        if (!student.isAssigned()) {
            return "Room details: no accommodation assigned yet.";
        }

        const Dormitory &dormitory = m_university.dormitory(student.dormitoryId().value());
        const Room &room = dormitory.room(student.roomNumber().value());
        return "Dormitory: " + dormitory.id() + " - " + dormitory.name()
            + " | Room " + QString::number(room.number())
            + " | Capacity/occupancy "
            + QString::number(room.occupancy()) + " / " + QString::number(room.capacity());
    }

    QString accessDetailText(const Student &student) const
    {
        if (!student.isAssigned()) {
            return "Scope: unassigned resident, visible to full-access and neighborhood admins.";
        }

        const QString dormitoryId = student.dormitoryId().value();
        const Neighborhood *neighborhood = findNeighborhoodForDormitory(dormitoryId);
        const QString neighborhoodText = neighborhood == nullptr
            ? "No neighborhood"
            : neighborhood->id + " - " + neighborhood->name;
        return "Scope/access: " + neighborhoodText + (currentAdminCanAccessDormitory(dormitoryId) ? " (editable)" : " (view only)");
    }

    QString residentStatusText(const Student &student) const
    {
        if (!student.isAssigned()) {
            return "Unassigned";
        }

        const QString dormitoryId = student.dormitoryId().value();
        return "Resident in " + dormitoryId
            + (currentAdminCanAccessDormitory(dormitoryId) ? " (editable)" : " (read-only)");
    }

    const Neighborhood *findNeighborhoodForDormitory(const QString &dormitoryId) const
    {
        for (const Neighborhood &neighborhood : m_neighborhoods) {
            if (neighborhood.dormitoryIds.contains(dormitoryId)) {
                return &neighborhood;
            }
        }
        return nullptr;
    }

    QString nextStudentId() const
    {
        int maxNumber = 1000;
        for (const Student &student : m_university.students()) {
            const QString id = student.id();
            if (id.size() > 1 && id.startsWith('S')) {
                bool ok = false;
                const int number = id.mid(1).toInt(&ok);
                if (ok) {
                    maxNumber = std::max(maxNumber, number);
                }
            }
        }
        return "S" + QString::number(maxNumber + 1);
    }

    void setStudentPanelMessage(const QString &message, bool error = false)
    {
        showStatus(message);
        if (m_profileStatusLabel == nullptr) {
            return;
        }
        m_profileStatusLabel->setText(message);
        m_profileStatusLabel->setStyleSheet(error ? "color: #B42318;" : "color: #1D7A57;");
    }

    QWidget *mealRow(const QString &meal, const QString &description)
    {
        auto *row = new QFrame(this);
        row->setObjectName("mealRow");
        auto *layout = new QHBoxLayout(row);
        layout->setContentsMargins(10, 7, 10, 7);
        layout->setSpacing(9);
        layout->addWidget(statusPill(meal.left(1), "#E6F4EE", "#1D7A57"), 0, Qt::AlignTop);
        auto *textLayout = new QVBoxLayout();
        textLayout->setSpacing(2);
        textLayout->addWidget(classLabel(meal, "muted"));
        textLayout->addWidget(classLabel(description.isEmpty() ? "Not set" : description, "cardTitle"));
        layout->addLayout(textLayout, 1);
        return row;
    }

    void setCell(QTableWidget *table, int row, int column, const QString &value, const QString &color)
    {
        auto *item = new QTableWidgetItem(value);
        item->setForeground(QColor(color));
        item->setToolTip(value);
        table->setItem(row, column, item);
    }

    void setStatusCell(QTableWidget *table, int row, int column, const QString &value, const QString &background, const QString &foreground)
    {
        auto *item = new QTableWidgetItem(value);
        item->setForeground(QColor(foreground));
        item->setBackground(QColor(background));
        item->setTextAlignment(Qt::AlignCenter);
        item->setToolTip(value);
        table->setItem(row, column, item);
    }

    void showStatus(const QString &message)
    {
        setToolTip(message);
        if (m_statusLabel != nullptr) {
            m_statusLabel->setText(message);
            m_statusLabel->setVisible(false);
        }
    }

    static QString initials(const QString &name)
    {
        QString result;
        for (const QString &part : name.split(' ', Qt::SkipEmptyParts)) {
            result += part.left(1).toUpper();
            if (result.size() == 2) {
                break;
            }
        }
        return result.isEmpty() ? "ST" : result;
    }

    static void selectComboData(QComboBox *combo, const QString &value)
    {
        const int index = combo->findData(value);
        if (index >= 0) {
            combo->setCurrentIndex(index);
        }
    }

    static void selectComboData(QComboBox *combo, int value)
    {
        const int index = combo->findData(value);
        if (index >= 0) {
            combo->setCurrentIndex(index);
        }
    }

    static void clearLayout(QLayout *layout)
    {
        while (QLayoutItem *item = layout->takeAt(0)) {
            if (QWidget *widget = item->widget()) {
                widget->deleteLater();
            }
            if (QLayout *childLayout = item->layout()) {
                clearLayout(childLayout);
            }
            delete item;
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const int dataFileIndex = QCoreApplication::arguments().indexOf("--data-file");
    const QString dataFilePath = dataFileIndex >= 0
        ? QCoreApplication::arguments().value(dataFileIndex + 1)
        : QString();

    if (QCoreApplication::arguments().contains("--persistence-self-test")) {
        QTemporaryDir tempDir;
        if (!tempDir.isValid()) {
            return 1;
        }
        const QString testDataPath = QDir(tempDir.path()).filePath("udrms-data.json");
        DormitoryWindow first(testDataPath);
        first.addPersistenceProbeForTest();
        DormitoryWindow second(testDataPath);
        return second.hasStudentForTest("S9999") ? 0 : 1;
    }

    if (QCoreApplication::arguments().contains("--seed-data-self-test")) {
        QTemporaryDir tempDir;
        if (!tempDir.isValid()) {
            return 1;
        }
        const QString testDataPath = QDir(tempDir.path()).filePath("udrms-data.json");
        DormitoryWindow seeded(testDataPath);
        return seeded.hasRichExampleSeedDataForTest() ? 0 : 1;
    }

    DormitoryWindow window(dataFilePath);
    const int adminLoginIndex = QCoreApplication::arguments().indexOf("--login-admin");
    if (adminLoginIndex >= 0) {
        window.loginAsAdminForTest(QCoreApplication::arguments().value(adminLoginIndex + 1, "admin"));
    }
    const int studentLoginIndex = QCoreApplication::arguments().indexOf("--login-student");
    if (studentLoginIndex >= 0) {
        window.loginAsStudentForTest(QCoreApplication::arguments().value(studentLoginIndex + 1, "S1001").toUpper());
    }
    const int pageIndex = QCoreApplication::arguments().indexOf("--page");
    if (pageIndex >= 0) {
        window.showPageForTest(QCoreApplication::arguments().value(pageIndex + 1, "0").toInt());
    }
    if (QCoreApplication::arguments().contains("--collapse-sidebar")) {
        window.collapseSidebarForTest();
    }
    if (QCoreApplication::arguments().contains("--smoke-test")) {
        return 0;
    }
    const int screenshotIndex = QCoreApplication::arguments().indexOf("--screenshot");
    if (screenshotIndex >= 0) {
        const QString path = QCoreApplication::arguments().value(screenshotIndex + 1, "udrms-gui.png");
        window.show();
        app.processEvents();
        window.grab().save(path);
        return 0;
    }
    window.show();
    return app.exec();
}
