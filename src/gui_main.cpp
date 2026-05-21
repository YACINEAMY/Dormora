#include "udrms/DomainError.h"
#include "udrms/University.h"

#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QCoreApplication>
#include <QDateEdit>
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
#include <QSaveFile>
#include <QScreen>
#include <QScrollArea>
#include <QSet>
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
        buildLoginUi();
        setWindowTitle("Dormora");
        resize(1280, 780);
        setMinimumSize(1100, 680);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            m_draggingWindow = event->position().y() < 80 || event->position().x() < 260;
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (m_draggingWindow && (event->buttons() & Qt::LeftButton)) {
            move(event->globalPosition().toPoint() - m_dragPosition);
            event->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        m_draggingWindow = false;
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
    bool m_draggingWindow = false;
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
    QLabel *m_studentCountLabel = nullptr;
    QLabel *m_profileNameLabel = nullptr;
    QLabel *m_profileMetaLabel = nullptr;
    QLabel *m_profileAssignmentLabel = nullptr;
    QLabel *m_profileRoomLabel = nullptr;
    QLabel *m_profileAccessLabel = nullptr;
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

    void loadOrSeedData()
    {
        if (QFile::exists(m_dataFilePath)) {
            loadAppState();
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

        Dormitory north("D1", "North Dormitory", 6, Restaurant("North Restaurant"));
        north.addRoom(Room(101, 2));
        north.addRoom(Room(102, 2));
        north.addRoom(Room(103, 2));

        Dormitory south("D2", "South Dormitory", 4, Restaurant("South Restaurant"));
        south.addRoom(Room(201, 2));
        south.addRoom(Room(202, 2));

        m_university.addDormitory(north);
        m_university.addDormitory(south);
        m_university.addStudent(Student("S1001", "Amina Benali", 1));
        m_university.addStudent(Student("S1002", "Karim Haddad", 2));
        m_university.addStudent(Student("S1003", "Lina Saadi", 3));
        m_university.addStudent(Student("S1004", "Yanis Merad", 1));
        m_university.assignStudentToRoom("S1001", "D1", 101);
        m_university.assignStudentToRoom("S1002", "D1", 101);
        m_university.assignStudentToRoom("S1003", "D1", 102);
        m_university.assignStudentToRoom("S1004", "D2", 202);

        const QDate today = QDate::currentDate();
        m_university.setRestaurantMenu("D1", today, {"Coffee and eggs", "Chicken and rice", "Soup and salad"});
        m_university.setRestaurantMenu("D2", today, {"Tea and bread", "Couscous", "Vegetable stew"});
        m_university.recordStudentMeal("S1001", today);
        m_university.recordStudentMeal("S1002", today);

        m_neighborhoods.append({"NORTH", "North Campus Neighborhood", {"D1"}});
        m_neighborhoods.append({"SOUTH", "South Campus Neighborhood", {"D2"}});

        m_adminProfiles.insert("admin", {"admin", "admin123", "Global Administrator", true, {}});
        m_adminProfiles.insert("northadmin", {"northadmin", "north123", "North Neighborhood Admin", false, {"NORTH"}});
        m_adminProfiles.insert("southadmin", {"southadmin", "south123", "South Neighborhood Admin", false, {"SOUTH"}});
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

    void buildLoginUi()
    {
        auto *root = new QWidget(this);
        root->setObjectName("loginRoot");
        auto *outer = new QHBoxLayout(root);
        outer->setContentsMargins(0, 0, 0, 0);
        outer->setSpacing(0);

        auto *brandPanel = new QFrame(root);
        brandPanel->setObjectName("loginBrand");
        brandPanel->setFixedWidth(430);
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
        auto *close = new QPushButton(QString::fromUtf8("×"), controls);
        close->setObjectName("closeButton");

        connect(minimize, &QPushButton::clicked, this, [this] { showMinimized(); });
        connect(close, &QPushButton::clicked, this, [this] { this->close(); });
        layout->addWidget(minimize);
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

    void addPersistenceProbeForTest()
    {
        m_university.addStudent(Student("S9999", "Persistence Probe", 1));
        saveAppState();
    }

    bool hasStudentForTest(const QString &studentId) const
    {
        return m_university.hasStudent(studentId);
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
        sidebar->setFixedWidth(238);

        auto *layout = new QVBoxLayout(sidebar);
        layout->setContentsMargins(22, 28, 22, 22);
        layout->setSpacing(11);
        layout->addWidget(buildBrandLockup(sidebar, "Campus living, organized."));
        layout->addSpacing(22);

        auto *group = new QButtonGroup(sidebar);
        group->setExclusive(true);

        const QStringList items = {"Dashboard", "Residents", "Meals", "Neighborhoods"};
        for (int i = 0; i < items.size(); ++i) {
            auto *button = new QPushButton(items[i], sidebar);
            button->setProperty("nav", true);
            button->setCheckable(true);
            button->setMinimumHeight(44);
            group->addButton(button, i);
            layout->addWidget(button);
            connect(button, &QPushButton::clicked, this, [this, i] { m_stack->setCurrentIndex(i); });
            if (i == 0) {
                button->setChecked(true);
            }
        }

        layout->addStretch();
        auto *logoutButton = new QPushButton("Log out", sidebar);
        logoutButton->setProperty("nav", true);
        logoutButton->setMinimumHeight(44);
        connect(logoutButton, &QPushButton::clicked, this, [this] { logout(); });
        layout->addWidget(logoutButton);
        auto *hint = label("Dormora Console\nCampus operations", "brandSub", sidebar);
        if (!m_currentAdminUsername.isEmpty()) {
            const AdminProfile profile = m_adminProfiles.value(m_currentAdminUsername);
            hint->setText(profile.displayName + "\n" + (profile.fullAccess ? "Full access" : "Neighborhood access"));
        }
        layout->addWidget(hint);
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
        sidebar->setFixedWidth(238);
        auto *sideLayout = new QVBoxLayout(sidebar);
        sideLayout->setContentsMargins(22, 28, 22, 22);
        sideLayout->setSpacing(11);
        sideLayout->addWidget(buildBrandLockup(sidebar, "Resident portal"));
        sideLayout->addSpacing(22);
        sideLayout->addWidget(label("Room status\nMeal access\nResident record", "brandSub", sidebar));
        sideLayout->addStretch();
        auto *logoutButton = new QPushButton("Log out", sidebar);
        logoutButton->setProperty("nav", true);
        logoutButton->setMinimumHeight(44);
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

        auto *split = new QHBoxLayout();
        split->setSpacing(18);
        split->addWidget(buildStudentSearchCard(), 6);
        split->addWidget(buildStudentProfileCard(), 5);
        layout->addLayout(split, 1);

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
        layout->setContentsMargins(18, 14, 18, 14);
        layout->setSpacing(8);
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
        auto *clearSearch = new QPushButton("Clear", box);
        filterRow->addWidget(m_studentFilterInput, 1);
        filterRow->addWidget(clearSearch);
        layout->addLayout(filterRow);

        m_studentTable = new QTableWidget(box);
        setupTable(m_studentTable, {"ID", "Full Name", "Year", "Assignment"});
        applyColumnWeights(m_studentTable, {72, 165, 55, 145});
        m_studentTable->setMinimumHeight(170);
        layout->addWidget(m_studentTable, 1);

        auto *addBox = new QFrame(box);
        addBox->setObjectName("studentInlinePanel");
        auto *addLayout = new QVBoxLayout(addBox);
        addLayout->setContentsMargins(12, 10, 12, 10);
        addLayout->setSpacing(6);
        addLayout->addWidget(classLabel("Add Student", "cardTitle"));
        m_studentIdInput = new QLineEdit(addBox);
        m_studentIdInput->setPlaceholderText("S1005");
        m_studentNameInput = new QLineEdit(addBox);
        m_studentNameInput->setPlaceholderText("Full name");
        m_academicYearInput = new QSpinBox(addBox);
        m_academicYearInput->setRange(1, 8);
        auto *addGrid = new QGridLayout();
        addGrid->setContentsMargins(0, 0, 0, 0);
        addGrid->setHorizontalSpacing(10);
        addGrid->setVerticalSpacing(8);
        addGrid->addWidget(fieldLabel("Student ID", m_studentIdInput), 0, 0);
        addGrid->addWidget(fieldLabel("Academic Year", m_academicYearInput), 0, 1);
        addGrid->addWidget(fieldLabel("Full Name", m_studentNameInput), 1, 0, 1, 2);
        addLayout->addLayout(addGrid);
        auto *addButton = new QPushButton("Add student", addBox);
        addButton->setProperty("class", "primary");
        addButton->setObjectName("addStudentButton");
        addLayout->addWidget(addButton);
        layout->addWidget(addBox);

        connect(m_studentSearchInput, &QLineEdit::textChanged, this, [this] { refreshStudents(); });
        connect(m_studentFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshStudents(); });
        connect(clearSearch, &QPushButton::clicked, this, [this] {
            m_studentSearchInput->clear();
            m_studentFilterInput->setCurrentIndex(0);
        });
        connect(m_studentTable, &QTableWidget::itemSelectionChanged, this, [this] { selectStudentFromTable(); });
        connect(addButton, &QPushButton::clicked, this, [this] { addStudent(); });
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
        profileLayout->setSpacing(6);

        m_profileNameLabel = classLabel("No student selected", "cardTitle");
        m_profileMetaLabel = classLabel("Search for a student by name or ID.", "muted");
        m_profileAssignmentLabel = classLabel("Assignment will appear here.", "muted");
        m_profileRoomLabel = classLabel("Room details will appear here.", "muted");
        m_profileAccessLabel = classLabel("Access scope will appear here.", "muted");
        profileLayout->addWidget(m_profileNameLabel);
        profileLayout->addWidget(m_profileMetaLabel);
        profileLayout->addWidget(m_profileAssignmentLabel);
        profileLayout->addWidget(m_profileRoomLabel);
        profileLayout->addWidget(m_profileAccessLabel);

        m_profileNameInput = new QLineEdit(box);
        m_profileNameInput->setPlaceholderText("Full name");
        m_profileNameInput->setFixedHeight(36);
        m_profileYearInput = new QSpinBox(box);
        m_profileYearInput->setRange(1, 8);
        m_profileYearInput->setFixedHeight(36);
        profileLayout->addWidget(fieldLabel("Modify Full Name", m_profileNameInput));
        profileLayout->addWidget(fieldLabel("Modify Academic Year", m_profileYearInput));

        auto *save = new QPushButton("Save changes", box);
        save->setProperty("class", "primary");
        save->setMinimumHeight(36);
        connect(save, &QPushButton::clicked, this, [this] { saveSelectedStudent(); });
        profileLayout->addWidget(save);

        auto *utilityRow = new QHBoxLayout();
        utilityRow->setSpacing(8);
        auto *reset = new QPushButton("Reset edits", box);
        auto *duplicate = new QPushButton("Duplicate", box);
        auto *deleteButton = new QPushButton("Delete", box);
        deleteButton->setProperty("class", "danger");
        reset->setFixedHeight(34);
        duplicate->setFixedHeight(34);
        deleteButton->setFixedHeight(34);
        utilityRow->addWidget(reset);
        utilityRow->addWidget(duplicate);
        utilityRow->addWidget(deleteButton);
        profileLayout->addLayout(utilityRow);
        connect(reset, &QPushButton::clicked, this, [this] { resetSelectedStudentEdits(); });
        connect(duplicate, &QPushButton::clicked, this, [this] { duplicateSelectedStudent(); });
        connect(deleteButton, &QPushButton::clicked, this, [this] { deleteSelectedStudent(); });

        profileLayout->addSpacing(4);
        profileLayout->addWidget(classLabel("Accommodation Actions", "cardTitle"));
        m_assignDormitoryInput = new QComboBox(box);
        m_assignDormitoryInput->setFixedHeight(36);
        m_assignRoomInput = new QSpinBox(box);
        m_assignRoomInput->setRange(1, 9999);
        m_assignRoomInput->setFixedHeight(36);
        profileLayout->addWidget(fieldLabel("Dormitory", m_assignDormitoryInput));
        profileLayout->addWidget(fieldLabel("Room Number", m_assignRoomInput));

        auto *actionRow = new QHBoxLayout();
        actionRow->setSpacing(8);
        auto *assign = new QPushButton("Assign", box);
        assign->setProperty("class", "primary");
        auto *remove = new QPushButton("Remove", box);
        remove->setProperty("class", "danger");
        assign->setFixedHeight(34);
        remove->setFixedHeight(34);
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

    void saveSelectedStudent()
    {
        runAction([this] {
            Student &student = selectedStudent();
            if (m_profileNameInput->text().trimmed().isEmpty()) {
                throw DomainError("Student full name is required.");
            }
            student.setFullName(m_profileNameInput->text().trimmed());
            student.setAcademicYear(m_profileYearInput->value());
            setStudentPanelMessage("Student profile updated.");
        });
    }

    void resetSelectedStudentEdits()
    {
        updateSelectedStudentProfile();
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

    void refreshStudents()
    {
        if (m_studentTable == nullptr) {
            return;
        }
        m_studentTable->setRowCount(0);
        const QString query = m_studentSearchInput == nullptr ? QString() : m_studentSearchInput->text().trimmed().toLower();
        const QString filter = m_studentFilterInput == nullptr ? QStringLiteral("all") : m_studentFilterInput->currentData().toString();
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
        if ((m_selectedStudentId.isEmpty() || m_studentTable->selectedItems().isEmpty()) && m_studentTable->rowCount() > 0) {
            m_studentTable->selectRow(0);
        }
        if (m_studentTable->rowCount() == 0) {
            m_selectedStudentId.clear();
        }
        updateSelectedStudentProfile();
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
        if (m_studentTable == nullptr || m_studentTable->selectedItems().isEmpty()) {
            return;
        }
        const int row = m_studentTable->currentRow();
        if (row < 0) {
            return;
        }
        m_selectedStudentId = m_studentTable->item(row, 0)->text();
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
            m_profileNameLabel->setText("No student selected");
            m_profileMetaLabel->setText("Search for a student by name or ID.");
            m_profileAssignmentLabel->setText("Assignment will appear here.");
            m_profileRoomLabel->setText("Room details will appear here.");
            m_profileAccessLabel->setText("Access scope will appear here.");
            m_profileNameInput->clear();
            m_profileYearInput->setValue(1);
            setProfileControlsEnabled(false);
            return;
        }

        const Student &student = m_university.student(m_selectedStudentId);
        m_profileNameLabel->setText(student.fullName());
        m_profileMetaLabel->setText(student.id() + " - Academic year " + QString::number(student.academicYear()));
        m_profileAssignmentLabel->setText(assignmentText(student));
        m_profileRoomLabel->setText(roomDetailText(student));
        m_profileAccessLabel->setText(accessDetailText(student));
        m_profileNameInput->setText(student.fullName());
        m_profileYearInput->setValue(student.academicYear());
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
            return "No accommodation assigned yet.";
        }

        const Dormitory &dormitory = m_university.dormitory(student.dormitoryId().value());
        const Room &room = dormitory.room(student.roomNumber().value());
        return dormitory.name() + " - room capacity "
            + QString::number(room.occupancy()) + " / " + QString::number(room.capacity());
    }

    QString accessDetailText(const Student &student) const
    {
        if (!student.isAssigned()) {
            return "Visible because this student is unassigned.";
        }

        const QString dormitoryId = student.dormitoryId().value();
        const Neighborhood *neighborhood = findNeighborhoodForDormitory(dormitoryId);
        const QString neighborhoodText = neighborhood == nullptr
            ? "No neighborhood"
            : neighborhood->id + " - " + neighborhood->name;
        return "Scope: " + neighborhoodText + (currentAdminCanAccessDormitory(dormitoryId) ? " (editable)" : " (view only)");
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
