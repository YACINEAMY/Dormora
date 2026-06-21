#include "udrms/DomainError.h"
#include "udrms/AppState.h"
#include "udrms/University.h"

#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QCoreApplication>
#include <QDateEdit>
#include <QDialog>
#include <QDir>
#include <QElapsedTimer>
#include <QEvent>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QFrame>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QAbstractAnimation>
#include <QEasingCurve>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QPixmap>
#include <QProgressBar>
#include <QShortcut>
#include <QSaveFile>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QSet>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStatusBar>
#include <QStyle>
#include <QTableWidget>
#include <QTimer>
#include <QTemporaryDir>
#include <QVBoxLayout>

#include <algorithm>
#include <exception>
#include <functional>
#include <initializer_list>

using namespace udrms;

namespace {

const char *kAppStyle = R"(
* {
    font-family: "Segoe UI", "Inter", Arial, sans-serif;
    font-size: 13px;
    color: #18212A;
}
QMainWindow {
    background: transparent;
}
QFrame#windowChrome {
    background: #F3F5F6;
    border: 1px solid #C8D1D4;
    border-radius: 6px;
}
QFrame#windowChrome[fullscreenChrome="true"] {
    border: none;
    border-radius: 0;
}
QWidget#appRoot, QWidget#loginRoot {
    background: #F3F5F6;
    border-radius: 5px;
}
QWidget#appRoot[fullscreenChrome="true"], QWidget#loginRoot[fullscreenChrome="true"] {
    border-radius: 0;
}
QWidget#studentRoot {
    background: #F6F3EE;
    border-radius: 5px;
}
QWidget#studentRoot[fullscreenChrome="true"] {
    border-radius: 0;
}
QWidget#contentPane {
    background: #F3F5F6;
    border-top-right-radius: 5px;
    border-bottom-right-radius: 5px;
}
QWidget#contentPane[fullscreenChrome="true"] {
    border-top-right-radius: 0;
    border-bottom-right-radius: 0;
}
QWidget#studentContentPane {
    background: #F6F3EE;
    border-top-right-radius: 5px;
    border-bottom-right-radius: 5px;
}
QWidget#studentContentPane[fullscreenChrome="true"] {
    border-top-right-radius: 0;
    border-bottom-right-radius: 0;
}
QFrame#titleControls {
    background: rgba(15, 35, 45, 0.06);
    border: 1px solid rgba(15, 35, 45, 0.10);
    border-radius: 6px;
    padding: 3px;
}
QPushButton#windowButton {
    background: transparent;
    color: #3D5360;
    border: none;
    border-radius: 8px;
    min-width: 30px;
    max-width: 30px;
    min-height: 30px;
    max-height: 30px;
    padding: 0;
    font-size: 14px;
    font-weight: 700;
}
QPushButton#windowButton:hover {
    background: rgba(31, 117, 92, 0.14);
    color: #143B34;
}
QPushButton#windowButton:pressed {
    background: rgba(25,135,84,0.24);
}
QPushButton#windowButtonWide {
    background: transparent;
    color: #315B4A;
    border: none;
    border-radius: 8px;
    min-width: 30px;
    max-width: 30px;
    min-height: 30px;
    max-height: 30px;
    padding: 0;
    font-size: 14px;
    font-weight: 700;
}
QPushButton#windowButtonWide:hover {
    background: rgba(25,135,84,0.14);
    color: #123D32;
}
QPushButton#closeButton {
    background: transparent;
    color: #B42318;
    border: none;
    border-radius: 8px;
    min-width: 30px;
    max-width: 30px;
    min-height: 30px;
    max-height: 30px;
    padding: 0;
    font-size: 15px;
    font-weight: 800;
}
QPushButton#closeButton:hover {
    background: #B42318;
    color: white;
}
QFrame#sidebar {
    background: #11282E;
    border: none;
    border-top-left-radius: 11px;
    border-bottom-left-radius: 11px;
}
QFrame#sidebar[fullscreenChrome="true"] {
    border-top-left-radius: 0;
    border-bottom-left-radius: 0;
}
QFrame#studentSidebar {
    background: #2F3E39;
    border: none;
    border-top-left-radius: 11px;
    border-bottom-left-radius: 11px;
}
QFrame#studentSidebar[fullscreenChrome="true"] {
    border-top-left-radius: 0;
    border-bottom-left-radius: 0;
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
    background: #11282E;
    border-top-left-radius: 11px;
    border-bottom-left-radius: 11px;
}
QFrame#loginBrand[fullscreenChrome="true"] {
    border-top-left-radius: 0;
    border-bottom-left-radius: 0;
}
QLabel#brand {
    color: white;
    font-size: 24px;
    font-weight: 750;
}
QLabel#brandSub {
    color: #BBD8D1;
    font-size: 12px;
    font-weight: 600;
}
QFrame#brandMark {
    background: #F8FBFA;
    border: 1px solid rgba(255,255,255,0.45);
    border-radius: 25px;
    min-width: 50px;
    max-width: 50px;
    min-height: 50px;
    max-height: 50px;
}
QLabel#loginBadge {
    background: rgba(255,255,255,0.12);
    border: 1px solid rgba(255,255,255,0.20);
    border-radius: 4px;
    color: #E8FFF3;
    font-size: 12px;
    font-weight: 700;
    padding: 6px 10px;
}
QPushButton[nav="true"] {
    background: transparent;
    color: #DCE9E6;
    border: none;
    border-radius: 4px;
    padding: 10px 14px;
    text-align: left;
    font-weight: 600;
}
QPushButton[nav="true"]:checked {
    background: #1F7560;
    color: white;
}
QPushButton[nav="true"]:hover {
    background: rgba(255,255,255,0.10);
}
QPushButton[compactNav="true"] {
    text-align: center;
    padding: 0;
    font-size: 18px;
    font-weight: 800;
}
QLabel#pageTitle {
    font-size: 22px;
    font-weight: 750;
}
QLabel#pageKicker {
    color: #5D6975;
    font-size: 14px;
}
QFrame.card, QFrame[class="card"], QGroupBox {
    background: #FFFFFF;
    border: 1px solid #D4DDDF;
    border-radius: 5px;
}
QFrame#pageHeader {
    background: #FFFFFF;
    border: 1px solid #D4DDDF;
    border-radius: 5px;
}
QFrame#metricCard {
    background: #FFFFFF;
    border: 1px solid #D4DDDF;
    border-top: 3px solid #1F7560;
    border-radius: 5px;
}
QFrame#studentCard {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
}
QFrame#studentHeroCard {
    background: #123D32;
    border: none;
    border-radius: 8px;
}
QLabel#studentHeroKicker {
    color: #A9D8CA;
    font-size: 12px;
    font-weight: 800;
}
QLabel#studentHeroTitle {
    color: #FFFFFF;
    font-size: 26px;
    font-weight: 800;
}
QLabel#studentHeroText {
    color: #DCEBE6;
    font-size: 14px;
    font-weight: 600;
}
QLabel#studentHeroMetric {
    color: #FFFFFF;
    font-size: 30px;
    font-weight: 800;
}
QFrame#studentInfoTile {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-top: 3px solid #1F7560;
    border-radius: 8px;
}
QFrame#studentMealCard {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
}
QFrame#studentWeekMenuCard {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-radius: 8px;
}
QFrame#studentWeekMenuRow {
    background: #F7FAF9;
    border: 1px solid #E2ECE7;
    border-radius: 6px;
}
QLabel#studentMealPhoto {
    background: #F7FAF9;
    border: 1px solid #D9E5DF;
    border-radius: 7px;
    color: #667085;
}
QFrame#studentInlinePanel, QFrame#todayMenuItem, QFrame#mealRow {
    background: #F7FAF9;
    border: 1px solid #DDE6E7;
    border-radius: 5px;
}
QLabel#mealImagePreview, QLabel#mealThumbnail {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-radius: 6px;
    color: #667085;
}
QCalendarWidget {
    background: #FFFFFF;
    border: 1px solid #D9E5DF;
    border-radius: 5px;
}
QCalendarWidget QToolButton {
    background: #FFFFFF;
    border: none;
    color: #13231D;
    padding: 4px 8px;
}
QCalendarWidget QToolButton:hover {
    background: #F3FBF7;
}
QCalendarWidget QAbstractItemView {
    background: #FFFFFF;
    color: #13231D;
    selection-background-color: #1D7A57;
    selection-color: #FFFFFF;
    outline: none;
}
QLabel.cardTitle, QLabel[class="cardTitle"] {
    font-size: 17px;
    font-weight: 700;
}
QLabel.muted, QLabel[class="muted"] {
    color: #667085;
}
QLabel.metricValue, QLabel[class="metricValue"] {
    font-size: 34px;
    font-weight: 760;
    color: #17252D;
}
QLabel.metricLabel, QLabel[class="metricLabel"] {
    color: #536579;
    font-weight: 700;
}
QPushButton {
    background: white;
    border: 1px solid #D2DDDF;
    border-radius: 5px;
    padding: 8px 13px;
    font-weight: 650;
}
QPushButton:hover {
    background: #F2F7F7;
}
QPushButton.primary, QPushButton[class="primary"] {
    background: #1F7560;
    border-color: #1F7560;
    color: white;
}
QPushButton.primary:hover, QPushButton[class="primary"]:hover {
    background: #195F50;
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
    border: 1px solid #1F7560;
}
QLineEdit, QComboBox, QSpinBox, QDateEdit {
    background: white;
    border: 1px solid #D2DDDF;
    border-radius: 5px;
    padding: 8px 12px;
    min-height: 30px;
}
QComboBox, QDateEdit {
    padding: 8px 36px 8px 12px;
}
QSpinBox {
    padding: 8px 40px 8px 12px;
}
QComboBox::drop-down, QDateEdit::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 30px;
    border-left: 1px solid #DDE8E2;
    border-top-right-radius: 5px;
    border-bottom-right-radius: 5px;
    background: #F6FAF8;
}
QComboBox::down-arrow, QDateEdit::down-arrow {
    image: url(:/icons/chevron-down.svg);
    width: 12px;
    height: 12px;
}
QSpinBox::up-button {
    subcontrol-origin: border;
    subcontrol-position: top right;
    width: 30px;
    border-left: 1px solid #DDE8E2;
    border-bottom: 1px solid #DDE8E2;
    border-top-right-radius: 5px;
    background: #F6FAF8;
}
QSpinBox::down-button {
    subcontrol-origin: border;
    subcontrol-position: bottom right;
    width: 30px;
    border-left: 1px solid #DDE8E2;
    border-bottom-right-radius: 5px;
    background: #F6FAF8;
}
QSpinBox::up-button:hover, QSpinBox::down-button:hover {
    background: #E6F4EE;
}
QSpinBox::up-arrow {
    image: url(:/icons/chevron-up.svg);
    width: 12px;
    height: 12px;
}
QSpinBox::down-arrow {
    image: url(:/icons/chevron-down.svg);
    width: 12px;
    height: 12px;
}
QComboBox QAbstractItemView {
    background: white;
    border: 1px solid #C9DED3;
    border-radius: 5px;
    padding: 4px;
    selection-background-color: #E6F4EE;
    selection-color: #123D32;
    outline: 0;
}
QComboBox QAbstractItemView::item {
    min-height: 28px;
    padding: 6px 10px;
    border-radius: 6px;
}
QTableWidget {
    background: white;
    border: 1px solid #D8E1E2;
    border-radius: 5px;
    gridline-color: transparent;
    selection-background-color: #E0F0EC;
    selection-color: #18212A;
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
    background: #EEF4F4;
    color: #5D6975;
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
QDialog {
    background: #F3F5F6;
    color: #18212A;
}
QDialog QLabel {
    background: transparent;
    color: #18212A;
}
QDialog QScrollArea {
    background: transparent;
    border: none;
}
QMessageBox {
    background: #FFFFFF;
    color: #18212A;
    min-width: 600px;
}
QMessageBox QLabel {
    background: transparent;
    color: #18212A;
    font-size: 13px;
}
QMessageBox QLabel#qt_msgbox_label {
    min-width: 440px;
}
QMessageBox QPushButton {
    background: #FFFFFF;
    border: 1px solid #BFD8CD;
    border-radius: 5px;
    color: #123D32;
    min-width: 74px;
    padding: 8px 16px;
}
QMessageBox QPushButton:hover {
    background: #E6F4EE;
    border-color: #89B8A7;
}
QLabel#appStatus {
    background: #EEF5F4;
    border-top: 1px solid #D9E5DF;
    color: #315B4B;
    font-size: 12px;
    padding: 1px 18px;
}
QLabel.counterValue, QLabel[class="counterValue"] {
    background: #EEF5F4;
    border: 1px solid #C9DED3;
    border-radius: 5px;
    color: #123D32;
    font-size: 18px;
    font-weight: 760;
    padding: 10px 12px;
}
QFrame#todayMenuItem {
    min-height: 92px;
}
QWidget#startupLoadingScreen {
    background: transparent;
}
QFrame#startupLoadingCard {
    background: #F4F7F8;
    border: 1px solid #CBD8DA;
    border-radius: 18px;
}
QLabel#loadingBrandName {
    color: #123D32;
    font-size: 30px;
    font-weight: 800;
}
QLabel#loadingMessage {
    color: #315B4B;
    font-size: 13px;
    font-weight: 650;
}
QProgressBar#startupProgress {
    background: #E4F0EA;
    border: none;
    border-radius: 4px;
    height: 8px;
    max-height: 8px;
    text-align: center;
}
QProgressBar#startupProgress::chunk {
    background: #198754;
    border-radius: 4px;
}
)";

constexpr int kStartupLoadingMinimumMs = 240;

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

void prepareMessageBox(QMessageBox &box)
{
    box.setMinimumWidth(600);

    for (QLabel *candidate : box.findChildren<QLabel *>()) {
        if (candidate->text().trimmed().isEmpty()) {
            continue;
        }
        candidate->setWordWrap(true);
        candidate->setMinimumWidth(440);
        candidate->setMaximumWidth(560);
    }

    if (box.width() < 600) {
        box.resize(600, box.height());
    }
}

int execPreparedMessageBox(QMessageBox &box)
{
    prepareMessageBox(box);
    box.show();
    qApp->processEvents();
    prepareMessageBox(box);
    prepareMessageBox(box);
    return box.exec();
}

void showWarningMessage(QWidget *parent, const QString &title, const QString &message)
{
    QMessageBox box(QMessageBox::Warning, title, message, QMessageBox::Ok, parent);
    execPreparedMessageBox(box);
}

void showInformationMessage(QWidget *parent, const QString &title, const QString &message)
{
    QMessageBox box(QMessageBox::Information, title, message, QMessageBox::Ok, parent);
    execPreparedMessageBox(box);
}

QMessageBox::StandardButton askQuestionMessage(QWidget *parent,
                                               const QString &title,
                                               const QString &message,
                                               QMessageBox::StandardButtons buttons,
                                               QMessageBox::StandardButton defaultButton)
{
    QMessageBox box(QMessageBox::Question, title, message, buttons, parent);
    box.setDefaultButton(defaultButton);
    return static_cast<QMessageBox::StandardButton>(execPreparedMessageBox(box));
}

QFrame *brandMark(QWidget *parent = nullptr)
{
    auto *mark = new QFrame(parent);
    mark->setObjectName("brandMark");
    auto *layout = new QVBoxLayout(mark);
    layout->setContentsMargins(3, 3, 3, 3);
    auto *logo = new QLabel(mark);
    logo->setAlignment(Qt::AlignCenter);
    logo->setPixmap(QPixmap(":/icons/dormora-logo.svg").scaled(44, 44, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setMinimumSize(44, 44);
    logo->setMaximumSize(44, 44);
    layout->addWidget(logo);
    return mark;
}

QWidget *buildStartupLoadingScreen()
{
    auto *screen = new QWidget(nullptr, Qt::SplashScreen | Qt::FramelessWindowHint);
    screen->setObjectName("startupLoadingScreen");
    screen->setAttribute(Qt::WA_TranslucentBackground);
    screen->resize(460, 300);

    auto *outer = new QVBoxLayout(screen);
    outer->setContentsMargins(12, 12, 12, 12);

    auto *card = new QFrame(screen);
    card->setObjectName("startupLoadingCard");
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(42, 38, 42, 34);
    layout->setSpacing(14);

    auto *logo = new QLabel(card);
    logo->setObjectName("loadingLogo");
    logo->setAlignment(Qt::AlignCenter);
    logo->setPixmap(QPixmap(":/icons/dormora-logo.svg").scaled(72, 72, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setMinimumSize(72, 72);
    logo->setMaximumHeight(78);
    layout->addWidget(logo, 0, Qt::AlignHCenter);

    auto *brand = label("Dormora", "loadingBrandName", card);
    brand->setAlignment(Qt::AlignCenter);
    layout->addWidget(brand);

    auto *message = label("Preparing your campus workspace...", "loadingMessage", card);
    message->setAlignment(Qt::AlignCenter);
    layout->addWidget(message);

    auto *progress = new QProgressBar(card);
    progress->setObjectName("startupProgress");
    progress->setRange(0, 0);
    progress->setTextVisible(false);
    progress->setMinimumWidth(260);
    layout->addSpacing(6);
    layout->addWidget(progress);

    outer->addWidget(card);
    return screen;
}

void centerOnPrimaryScreen(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }
    if (const QScreen *screen = QApplication::primaryScreen()) {
        const QRect available = screen->availableGeometry();
        widget->move(available.center() - widget->rect().center());
    }
}

bool startupLoadingScreenSelfTest()
{
    qApp->setFont(QFont("Times New Roman", 10));
    qApp->setStyleSheet(kAppStyle);

    QWidget *loading = buildStartupLoadingScreen();
    centerOnPrimaryScreen(loading);
    loading->show();
    qApp->processEvents();

    const auto *logo = loading->findChild<QLabel *>("loadingLogo");
    const auto *brand = loading->findChild<QLabel *>("loadingBrandName");
    const auto *message = loading->findChild<QLabel *>("loadingMessage");
    const auto *progress = loading->findChild<QProgressBar *>("startupProgress");
    const QPixmap logoPixmap = logo == nullptr ? QPixmap() : logo->pixmap();
    const bool healthy = loading->isVisible()
        && logo != nullptr
        && !logoPixmap.isNull()
        && brand != nullptr
        && brand->text() == "Dormora"
        && message != nullptr
        && message->text().contains("campus workspace")
        && progress != nullptr
        && progress->minimum() == 0
        && progress->maximum() == 0;

    loading->close();
    delete loading;
    return healthy;
}

bool messageBoxStyleSelfTest()
{
    qApp->setFont(QFont("Times New Roman", 10));
    qApp->setStyleSheet(kAppStyle);

    QMessageBox box(QMessageBox::Warning,
                    "Rule feedback",
                    "Global administrators already have access to every neighborhood.");
    box.setStandardButtons(QMessageBox::Ok);
    prepareMessageBox(box);
    box.show();
    qApp->processEvents();

    QLabel *messageLabel = nullptr;
    for (QLabel *candidate : box.findChildren<QLabel *>()) {
        if (candidate->text().contains("Global administrators")) {
            messageLabel = candidate;
            break;
        }
    }

    const QString style = qApp->styleSheet();
    const QColor textColor = box.palette().color(QPalette::WindowText);
    const QColor backgroundColor = box.palette().color(QPalette::Window);
    const bool hasExplicitRules = style.contains("QMessageBox {")
        && style.contains("QMessageBox QLabel")
        && style.contains("QMessageBox QPushButton");
    const bool readablePalette = textColor.lightness() < 110 && backgroundColor.lightness() > 180;
    const bool readableLayout = box.width() >= 500
        && messageLabel != nullptr
        && messageLabel->wordWrap()
        && messageLabel->minimumWidth() >= 440
        && messageLabel->maximumWidth() >= 520;

    box.close();
    return hasExplicitRules && readablePalette && readableLayout;
}

QString assignmentText(const Student &student)
{
    if (!student.isAssigned()) {
        return "Unassigned";
    }
    return student.dormitoryId().value() + " / Room " + QString::number(student.roomNumber().value());
}

QIcon sidebarLineIcon(const QString &name)
{
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor("#F7FAF8"), 2.4);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    if (name == "overview") {
        painter.drawRoundedRect(QRectF(7, 7, 8, 8), 2, 2);
        painter.drawRoundedRect(QRectF(18, 7, 7, 12), 2, 2);
        painter.drawRoundedRect(QRectF(7, 18, 8, 7), 2, 2);
        painter.drawRoundedRect(QRectF(18, 22, 7, 3), 1, 1);
    } else if (name == "residents") {
        painter.drawEllipse(QPointF(12, 11), 4, 4);
        painter.drawArc(QRectF(6, 17, 12, 10), 0, 180 * 16);
        painter.drawEllipse(QPointF(22, 13), 3, 3);
        painter.drawArc(QRectF(17, 19, 10, 8), 0, 180 * 16);
    } else if (name == "meals") {
        painter.drawEllipse(QRectF(7, 8, 14, 14));
        painter.drawEllipse(QRectF(11, 12, 6, 6));
        painter.drawLine(QPointF(24, 7), QPointF(24, 25));
        painter.drawLine(QPointF(21, 8), QPointF(21, 15));
        painter.drawLine(QPointF(27, 8), QPointF(27, 15));
        painter.drawLine(QPointF(21, 15), QPointF(27, 15));
    } else if (name == "neighborhoods") {
        painter.drawRoundedRect(QRectF(7, 7, 7, 7), 1.5, 1.5);
        painter.drawRoundedRect(QRectF(18, 7, 7, 7), 1.5, 1.5);
        painter.drawRoundedRect(QRectF(7, 18, 7, 7), 1.5, 1.5);
        painter.drawRoundedRect(QRectF(18, 18, 7, 7), 1.5, 1.5);
    } else if (name == "logout") {
        painter.drawRoundedRect(QRectF(10, 7, 12, 18), 2, 2);
        painter.drawLine(QPointF(18, 16), QPointF(27, 16));
        painter.drawLine(QPointF(23, 12), QPointF(27, 16));
        painter.drawLine(QPointF(23, 20), QPointF(27, 16));
    }

    return QIcon(pixmap);
}

QIcon adminNavIcon(int index)
{
    static const QStringList names = {"overview", "residents", "meals", "neighborhoods"};
    return sidebarLineIcon(names.value(index, "overview"));
}

QIcon logoutNavIcon()
{
    return sidebarLineIcon("logout");
}

} // namespace

class DormitoryWindow final : public QMainWindow {
public:
    explicit DormitoryWindow(QString dataFilePath = {})
        : m_usesDefaultDataPath(dataFilePath.isEmpty())
        , m_dataFilePath(m_usesDefaultDataPath ? defaultAppStatePath() : std::move(dataFilePath))
    {
        loadOrSeedData();
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        m_enableAnimations = !QCoreApplication::arguments().contains("--screenshot");
        qApp->setFont(QFont("Times New Roman", 10));
        qApp->setStyleSheet(kAppStyle);
        statusBar()->setSizeGripEnabled(false);
        statusBar()->hide();
        auto *fullscreenShortcut = new QShortcut(QKeySequence(Qt::Key_F11), this);
        connect(fullscreenShortcut, &QShortcut::activated, this, [this] { toggleFullScreen(); });
        buildLoginUi();
        setWindowTitle("Dormora");
        setWindowIcon(QIcon(":/icons/dormora-logo.svg"));
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

    void changeEvent(QEvent *event) override
    {
        QMainWindow::changeEvent(event);
        if (event->type() == QEvent::WindowStateChange) {
            applyChromeState();
        }
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
    bool m_usesDefaultDataPath = false;
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
    QFrame *m_windowChrome = nullptr;
    QWidget *m_contentRoot = nullptr;
    QLineEdit *m_loginUserInput = nullptr;
    QLineEdit *m_loginPasswordInput = nullptr;
    QLabel *m_loginFeedback = nullptr;
    QLabel *m_statusLabel = nullptr;
    QWidget *m_sidebarWidget = nullptr;

    QLabel *m_residentsMetric = nullptr;
    QLabel *m_availableRoomsMetric = nullptr;
    QLabel *m_mealsMetric = nullptr;
    QTableWidget *m_roomMatrix = nullptr;
    QLineEdit *m_roomSearchInput = nullptr;
    QComboBox *m_roomDormitoryFilterInput = nullptr;
    QComboBox *m_roomOccupancyFilterInput = nullptr;
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
    QLineEdit *m_neighborhoodSearchInput = nullptr;
    QComboBox *m_neighborhoodDormitoryFilterInput = nullptr;
    QComboBox *m_neighborhoodAdminFilterInput = nullptr;
    QGridLayout *m_residentList = nullptr;
    QVBoxLayout *m_menuCards = nullptr;

    QLineEdit *m_studentIdInput = nullptr;
    QLineEdit *m_studentNameInput = nullptr;
    QSpinBox *m_academicYearInput = nullptr;
    QComboBox *m_assignStudentInput = nullptr;
    QComboBox *m_assignDormitoryInput = nullptr;
    QComboBox *m_profileAssignDormitoryInput = nullptr;
    QSpinBox *m_profileAssignRoomInput = nullptr;
    QSpinBox *m_assignRoomInput = nullptr;

    QComboBox *m_menuDormitoryInput = nullptr;
    QLineEdit *m_menuSearchInput = nullptr;
    QComboBox *m_menuDormitoryFilterInput = nullptr;
    QComboBox *m_menuMealFilterInput = nullptr;
    QDateEdit *m_menuDateInput = nullptr;
    QComboBox *m_mealCounterDormitoryInput = nullptr;
    QDateEdit *m_mealCounterDateInput = nullptr;
    QSpinBox *m_mealCounterAmountInput = nullptr;
    QLabel *m_mealCounterValueLabel = nullptr;
    QLineEdit *m_breakfastInput = nullptr;
    QLineEdit *m_lunchInput = nullptr;
    QLineEdit *m_dinnerInput = nullptr;
    QLineEdit *m_breakfastImageUrlInput = nullptr;
    QLineEdit *m_lunchImageUrlInput = nullptr;
    QLineEdit *m_dinnerImageUrlInput = nullptr;
    QLineEdit *m_neighborhoodIdInput = nullptr;
    QLineEdit *m_neighborhoodNameInput = nullptr;
    QComboBox *m_copyNeighborhoodInput = nullptr;
    QComboBox *m_adminAccessInput = nullptr;
    QComboBox *m_neighborhoodAccessInput = nullptr;
    QLineEdit *m_newAdminUsernameInput = nullptr;
    QLineEdit *m_newAdminDisplayNameInput = nullptr;
    QLineEdit *m_newAdminPasswordInput = nullptr;
    QComboBox *m_newAdminScopeInput = nullptr;
    QComboBox *m_newAdminNeighborhoodInput = nullptr;
    QLabel *m_newAdminStatusLabel = nullptr;
    QString m_selectedStudentId;
    bool m_studentProfileDirty = false;
    bool m_restoringStudentSelection = false;
    bool m_loadingStudentProfile = false;
    bool m_loadingMenuEditor = false;
    QVector<QPushButton *> m_navButtons;
    mutable QVector<Dormitory> m_cachedVisibleDormitories;
    mutable QVector<Student> m_cachedVisibleStudents;
    mutable bool m_visibilityCacheDirty = true;

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
        const char *breakfastImageUrl;
        const char *lunchImageUrl;
        const char *dinnerImageUrl;
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

        Dormitory north("D1", "North Residence Hall", 100, Restaurant("North Dining Hall"));
        for (int roomNumber = 101; roomNumber <= 150; ++roomNumber) {
            north.addRoom(Room(roomNumber, 2));
        }

        Dormitory south("D2", "South Residence Hall", 100, Restaurant("South Dining Hall"));
        for (int roomNumber = 201; roomNumber <= 250; ++roomNumber) {
            south.addRoom(Room(roomNumber, 2));
        }

        Dormitory east("D3", "East Residence Hall", 96, Restaurant("East Dining Hall"));
        for (int roomNumber = 301; roomNumber <= 348; ++roomNumber) {
            east.addRoom(Room(roomNumber, 2));
        }

        Dormitory west("D4", "West Residence Hall", 96, Restaurant("West Dining Hall"));
        for (int roomNumber = 401; roomNumber <= 448; ++roomNumber) {
            west.addRoom(Room(roomNumber, 2));
        }

        m_university.addDormitory(north);
        m_university.addDormitory(south);
        m_university.addDormitory(east);
        m_university.addDormitory(west);
        addExampleStudents(true);
        addExampleMenus(false);

        const QDate today = QDate::currentDate();
        recordOperationalMealCounts(today);

        m_neighborhoods.append({"NORTH", "North Campus", {"D1"}});
        m_neighborhoods.append({"SOUTH", "South Campus", {"D2"}});
        m_neighborhoods.append({"EAST", "East Campus", {"D3"}});
        m_neighborhoods.append({"WEST", "West Campus", {"D4"}});

        m_adminProfiles.insert("admin", {"admin", "admin123", "Residence Operations Director", true, {}});
        m_adminProfiles.insert("northadmin", {"northadmin", "north123", "North Campus Manager", false, {"NORTH"}});
        m_adminProfiles.insert("southadmin", {"southadmin", "south123", "South Campus Manager", false, {"SOUTH"}});
        m_adminProfiles.insert("eastadmin", {"eastadmin", "east123", "East Campus Manager", false, {"EAST"}});
        m_adminProfiles.insert("westadmin", {"westadmin", "west123", "West Campus Manager", false, {"WEST"}});
    }

    bool ensureExampleDataVolume()
    {
        bool changed = ensureOperationalScaffolding();
        changed = addExampleStudents(true) || changed;
        changed = addExampleMenus(true) || changed;
        return changed;
    }

    bool ensureOperationalScaffolding()
    {
        bool changed = false;
        const auto ensureDormitory = [&](const QString &id, const QString &name, int firstRoom, int lastRoom, const QString &restaurantName) {
            if (m_university.hasDormitory(id)) {
                return false;
            }
            Dormitory dormitory(id, name, (lastRoom - firstRoom + 1) * 2, Restaurant(restaurantName));
            for (int roomNumber = firstRoom; roomNumber <= lastRoom; ++roomNumber) {
                dormitory.addRoom(Room(roomNumber, 2));
            }
            m_university.addDormitory(dormitory);
            return true;
        };

        changed = ensureDormitory("D3", "East Residence Hall", 301, 348, "East Dining Hall") || changed;
        changed = ensureDormitory("D4", "West Residence Hall", 401, 448, "West Dining Hall") || changed;

        const auto ensureNeighborhood = [&](const QString &id, const QString &name, const QVector<QString> &dormitoryIds) {
            if (findNeighborhood(id) == nullptr) {
                m_neighborhoods.append({id, name, dormitoryIds});
                return true;
            }
            return false;
        };

        changed = ensureNeighborhood("NORTH", "North Campus", {"D1"}) || changed;
        changed = ensureNeighborhood("SOUTH", "South Campus", {"D2"}) || changed;
        changed = ensureNeighborhood("EAST", "East Campus", {"D3"}) || changed;
        changed = ensureNeighborhood("WEST", "West Campus", {"D4"}) || changed;

        const auto ensureAdmin = [&](const QString &username, const QString &password, const QString &displayName, bool fullAccess, const QSet<QString> &neighborhoodIds) {
            if (!m_adminProfiles.contains(username)) {
                m_adminProfiles.insert(username, {username, password, displayName, fullAccess, neighborhoodIds});
                return true;
            }
            return false;
        };

        changed = ensureAdmin("eastadmin", "east123", "East Campus Manager", false, {"EAST"}) || changed;
        changed = ensureAdmin("westadmin", "west123", "West Campus Manager", false, {"WEST"}) || changed;
        if (mealsRecordedOn(QDate::currentDate()) == 0) {
            recordOperationalMealCounts(QDate::currentDate());
            changed = true;
        }
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
            const QString breakfastText = QString::fromUtf8(seed.breakfast);
            const QString lunchText = QString::fromUtf8(seed.lunch);
            const QString dinnerText = QString::fromUtf8(seed.dinner);
            const QString breakfastImageUrl = mealImageForDescription(breakfastText, QStringLiteral("breakfast"));
            const QString lunchImageUrl = mealImageForDescription(lunchText, QStringLiteral("lunch"));
            const QString dinnerImageUrl = mealImageForDescription(dinnerText, QStringLiteral("dinner"));
            if (preserveExisting) {
                const auto existingMenu = m_university.dormitory(dormitoryId).restaurant().menuForDate(date);
                const auto needsBundledImage = [](const QString &value) {
                    const QString trimmed = value.trimmed();
                    return trimmed.isEmpty()
                        || trimmed.startsWith("http://")
                        || trimmed.startsWith("https://")
                        || trimmed.startsWith(":/icons/meal-");
                };
                if (existingMenu.has_value()
                    && !needsBundledImage(existingMenu->breakfastImageUrl)
                    && !needsBundledImage(existingMenu->lunchImageUrl)
                    && !needsBundledImage(existingMenu->dinnerImageUrl)) {
                    continue;
                }
                if (existingMenu.has_value()) {
                    DailyMenu upgradedMenu = existingMenu.value();
                    if (needsBundledImage(upgradedMenu.breakfastImageUrl)) {
                        upgradedMenu.breakfastImageUrl = breakfastImageUrl;
                    }
                    if (needsBundledImage(upgradedMenu.lunchImageUrl)) {
                        upgradedMenu.lunchImageUrl = lunchImageUrl;
                    }
                    if (needsBundledImage(upgradedMenu.dinnerImageUrl)) {
                        upgradedMenu.dinnerImageUrl = dinnerImageUrl;
                    }
                    m_university.setRestaurantMenu(dormitoryId, date, upgradedMenu);
                    changed = true;
                    continue;
                }
            }

            m_university.setRestaurantMenu(dormitoryId, date, {
                breakfastText,
                lunchText,
                dinnerText,
                breakfastImageUrl,
                lunchImageUrl,
                dinnerImageUrl,
            });
            changed = true;
        }
        return changed;
    }

    void recordOperationalMealCounts(const QDate &date)
    {
        const QVector<QString> students = {
            "S1001", "S1002", "S1003", "S1004", "S1005", "S1006", "S1007", "S1008",
            "S1016", "S1017", "S1018", "S1019", "S1020", "S1021", "S1022", "S1023",
            "S1031", "S1032", "S1033", "S1034", "S1035", "S1036", "S1037", "S1038",
            "S1041", "S1042", "S1043", "S1044", "S1045", "S1046", "S1047", "S1048",
        };
        for (const QString &studentId : students) {
            if (m_university.hasStudent(studentId) && m_university.student(studentId).isAssigned()) {
                m_university.recordStudentMeal(studentId, date);
            }
        }
    }

    int mealsRecordedOn(const QDate &date) const
    {
        int count = 0;
        for (const Dormitory &dormitory : m_university.dormitories()) {
            count += dormitory.restaurant().mealsServedOn(date);
        }
        return count;
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
            const int campus = index % 4;
            const QString dormitoryId = QStringLiteral("D%1").arg(campus + 1);
            const int firstRoom = 109 + (campus * 100);
            const int roomNumber = firstRoom + (index / 8);
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
        static constexpr const char *breakfastImage = ":/icons/meal-breakfast.png";
        static constexpr const char *lunchImage = ":/icons/meal-lunch.png";
        static constexpr const char *dinnerImage = ":/icons/meal-dinner.png";
        return {
            {0, "D1", "Coffee, eggs, and toast", "Chicken rice bowl with salad", "Lentil soup and grilled vegetables", breakfastImage, lunchImage, dinnerImage},
            {0, "D2", "Mint tea, msemen, and yogurt", "Couscous with vegetables", "Vegetable stew with bread", breakfastImage, lunchImage, dinnerImage},
            {0, "D3", "Greek yogurt with figs", "Lemon chicken pasta", "Vegetable curry with rice", breakfastImage, lunchImage, dinnerImage},
            {0, "D4", "Omelet, olives, and bread", "Salmon rice plate", "Harira soup with dates", breakfastImage, lunchImage, dinnerImage},
            {1, "D1", "Oatmeal with dates and milk", "Turkey sandwich with tomato soup", "Pasta primavera and fruit", breakfastImage, lunchImage, dinnerImage},
            {1, "D2", "Croissants, jam, and orange juice", "Beef tagine with rice", "Chickpea salad and baked potatoes", breakfastImage, lunchImage, dinnerImage},
            {1, "D3", "Baghrir, honey, and milk", "Falafel bowl with hummus", "Chicken couscous", breakfastImage, lunchImage, dinnerImage},
            {1, "D4", "Fruit smoothie and muffin", "Pesto pasta with chicken", "Spinach quiche and soup", breakfastImage, lunchImage, dinnerImage},
            {2, "D1", "Cheese omelet and fruit", "Tuna pasta salad", "Roast chicken with carrots", breakfastImage, lunchImage, dinnerImage},
            {2, "D2", "Baghrir, honey, and milk", "Lentil dal with flatbread", "Fish fillet with herb rice", breakfastImage, lunchImage, dinnerImage},
            {2, "D3", "Pancakes with banana", "Chicken shawarma plate", "Black bean chili and cornbread", breakfastImage, lunchImage, dinnerImage},
            {2, "D4", "Toast, jam, and yogurt", "Vegetarian tajine", "Turkey meatloaf with mash", breakfastImage, lunchImage, dinnerImage},
            {3, "D1", "Greek yogurt with granola", "Beef kofta wrap", "Tomato soup with grilled cheese", breakfastImage, lunchImage, dinnerImage},
            {3, "D2", "Scrambled eggs and baguette", "Chicken couscous", "Vegetable lasagna", breakfastImage, lunchImage, dinnerImage},
            {3, "D3", "Avocado toast and boiled egg", "Shrimp rice skillet", "Baked fish with green beans", breakfastImage, lunchImage, dinnerImage},
            {3, "D4", "Semolina porridge and fruit", "Grilled chicken salad", "Mushroom risotto", breakfastImage, lunchImage, dinnerImage},
            {4, "D1", "Pancakes with banana", "Falafel bowl with hummus", "Beef stew with potatoes", breakfastImage, lunchImage, dinnerImage},
            {4, "D2", "Coffee, toast, and cheese", "Grilled chicken salad", "Harira soup and dates", breakfastImage, lunchImage, dinnerImage},
            {4, "D3", "Milk, cereal, and apple slices", "Rice with chickpeas and vegetables", "Creamy tomato pasta", breakfastImage, lunchImage, dinnerImage},
            {4, "D4", "French toast and berries", "Beef burger with salad", "Roasted vegetable couscous", breakfastImage, lunchImage, dinnerImage},
            {5, "D1", "Breakfast burrito", "Salmon rice plate", "Mushroom risotto", breakfastImage, lunchImage, dinnerImage},
            {5, "D2", "Semolina porridge and fruit", "Pasta bolognese", "Stuffed peppers and salad", breakfastImage, lunchImage, dinnerImage},
            {5, "D3", "Boiled eggs, olives, and bread", "Chicken caesar wrap", "Fish fillet with herb rice", breakfastImage, lunchImage, dinnerImage},
            {5, "D4", "Greek yogurt with granola", "Beef kofta wrap", "Vegetable lasagna", breakfastImage, lunchImage, dinnerImage},
            {6, "D1", "French toast and berries", "Chicken shawarma plate", "Vegetable curry with rice", breakfastImage, lunchImage, dinnerImage},
            {6, "D2", "Boiled eggs, olives, and bread", "Lamb meatballs with couscous", "Spinach quiche and soup", breakfastImage, lunchImage, dinnerImage},
            {6, "D3", "Coffee, eggs, and toast", "Couscous with vegetables", "Pasta primavera and fruit", breakfastImage, lunchImage, dinnerImage},
            {6, "D4", "Mint tea, msemen, and yogurt", "Tuna pasta salad", "Lentil soup and grilled vegetables", breakfastImage, lunchImage, dinnerImage},
            {7, "D1", "Avocado toast and boiled egg", "Pesto pasta with chicken", "Black bean chili and cornbread", breakfastImage, lunchImage, dinnerImage},
            {7, "D2", "Milk, cereal, and apple slices", "Shrimp rice skillet", "Roasted vegetable couscous", breakfastImage, lunchImage, dinnerImage},
            {7, "D3", "Cheese omelet and fruit", "Turkey sandwich with tomato soup", "Chickpea salad and baked potatoes", breakfastImage, lunchImage, dinnerImage},
            {7, "D4", "Croissants, jam, and orange juice", "Chicken rice bowl with salad", "Vegetable stew with bread", breakfastImage, lunchImage, dinnerImage},
            {8, "D1", "Fruit smoothie and muffin", "Chicken caesar wrap", "Baked fish with green beans", breakfastImage, lunchImage, dinnerImage},
            {8, "D2", "Omelet with peppers", "Vegetarian tajine", "Creamy tomato pasta", breakfastImage, lunchImage, dinnerImage},
            {8, "D3", "Toast, jam, and yogurt", "Beef tagine with rice", "Roast chicken with carrots", breakfastImage, lunchImage, dinnerImage},
            {8, "D4", "Baghrir, honey, and milk", "Lentil dal with flatbread", "Tomato soup with grilled cheese", breakfastImage, lunchImage, dinnerImage},
            {9, "D1", "Toast, jam, and yogurt", "Beef burger with salad", "Chicken noodle soup", breakfastImage, lunchImage, dinnerImage},
            {9, "D2", "Dates, bread, and labneh", "Rice with chickpeas and vegetables", "Turkey meatloaf with mash", breakfastImage, lunchImage, dinnerImage},
            {9, "D3", "Scrambled eggs and baguette", "Chicken couscous", "Harira soup and dates", breakfastImage, lunchImage, dinnerImage},
            {9, "D4", "Pancakes with banana", "Falafel bowl with hummus", "Beef stew with potatoes", breakfastImage, lunchImage, dinnerImage},
        };
    }

    static QString mealImageForDescription(const QString &description, const QString &mealType)
    {
        const QString text = description.toLower();
        const bool breakfast = mealType == "breakfast";
        const auto has = [&text](std::initializer_list<const char *> words) {
            for (const char *word : words) {
                if (text.contains(QString::fromLatin1(word))) {
                    return true;
                }
            }
            return false;
        };
        const auto savoryImage = [&]() -> QString {
            if (has({"couscous"})) {
                return QStringLiteral(":/icons/meal-couscous.png");
            }
            if (has({"fish", "salmon", "tuna", "shrimp"})) {
                return QStringLiteral(":/icons/meal-fish-rice.png");
            }
            if (has({"pasta", "bolognese", "pesto", "primavera", "lasagna", "risotto"})) {
                return QStringLiteral(":/icons/meal-pasta.png");
            }
            if (has({"soup", "harira", "lentil", "chili"})) {
                return QStringLiteral(":/icons/meal-soup.png");
            }
            if (has({"falafel", "hummus", "chickpea"})) {
                return QStringLiteral(":/icons/meal-falafel-hummus.png");
            }
            if (has({"curry", "dal"})) {
                return QStringLiteral(":/icons/meal-curry-rice.png");
            }
            if (has({"burger"})) {
                return QStringLiteral(":/icons/meal-burger.png");
            }
            if (has({"wrap", "sandwich", "shawarma", "burrito", "kofta"})) {
                return QStringLiteral(":/icons/meal-wrap-sandwich.png");
            }
            if (has({"tagine", "tajine", "stew", "meatball", "meatloaf", "roast"})) {
                return QStringLiteral(":/icons/meal-tagine-stew.png");
            }
            if (has({"salad", "vegetable", "potato", "pepper", "beans", "quiche"})) {
                return QStringLiteral(":/icons/meal-salad-vegetables.png");
            }
            if (has({"chicken", "rice"})) {
                return QStringLiteral(":/icons/meal-chicken-rice.png");
            }
            return {};
        };

        if (!breakfast) {
            const QString image = savoryImage();
            if (!image.isEmpty()) {
                return image;
            }
        }

        if (has({"msemen", "baghrir", "semolina", "mint tea"})) {
            return QStringLiteral(":/icons/meal-north-african-breakfast.png");
        }
        if (has({"pancake", "french toast"})) {
            return QStringLiteral(":/icons/meal-pancakes.png");
        }
        if (has({"yogurt", "granola", "fig", "fruit", "smoothie"})) {
            return QStringLiteral(":/icons/meal-yogurt-fruit.png");
        }
        if (has({"oatmeal", "porridge", "cereal"})) {
            return QStringLiteral(":/icons/meal-porridge-cereal.png");
        }
        if (has({"croissant", "muffin", "labneh", "dates", "bread"})) {
            return QStringLiteral(":/icons/meal-pastry.png");
        }
        if (breakfast || has({"egg", "omelet", "toast", "coffee", "cheese"})) {
            return QStringLiteral(":/icons/meal-eggs-toast.png");
        }

        const QString image = savoryImage();
        if (!image.isEmpty()) {
            return image;
        }

        return breakfast
            ? QStringLiteral(":/icons/meal-eggs-toast.png")
            : QStringLiteral(":/icons/meal-chicken-rice.png");
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

        applyAppStateJson(document.object());
    }

    void applyAppStateJson(const QJsonObject &root)
    {
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

        const QJsonObject state = appStateToJson();
        const QByteArray data = QJsonDocument(state).toJson(QJsonDocument::Indented);
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
        applyChromeState();
    }

    void toggleSidebar()
    {
        m_sidebarCollapsed = !m_sidebarCollapsed;
        replaceSidebarOnly();
    }

    void replaceSidebarOnly()
    {
        if (m_contentRoot == nullptr || m_role == AuthRole::None) {
            return;
        }

        auto *rootLayout = qobject_cast<QHBoxLayout *>(m_contentRoot->layout());
        if (rootLayout == nullptr) {
            return;
        }

        const int currentPage = m_stack == nullptr ? 0 : m_stack->currentIndex();
        const int oldWidth = m_sidebarWidget == nullptr ? (m_sidebarCollapsed ? 238 : 76) : m_sidebarWidget->width();
        QWidget *oldSidebar = m_sidebarWidget;
        QWidget *newSidebar = m_role == AuthRole::Admin ? buildSidebar() : buildStudentSidebar();
        const int targetWidth = m_sidebarCollapsed ? 76 : 238;
        newSidebar->setFixedWidth(oldWidth);

        if (oldSidebar != nullptr) {
            rootLayout->replaceWidget(oldSidebar, newSidebar);
            oldSidebar->hide();
            oldSidebar->deleteLater();
        } else {
            rootLayout->insertWidget(0, newSidebar);
        }
        m_sidebarWidget = newSidebar;

        if (m_role == AuthRole::Admin) {
            setCurrentAdminPage(currentPage);
        }
        applyChromeState();

        if (!m_enableAnimations || oldWidth == targetWidth) {
            newSidebar->setFixedWidth(targetWidth);
            return;
        }

        auto *group = new QParallelAnimationGroup(newSidebar);
        auto *minAnimation = new QPropertyAnimation(newSidebar, "minimumWidth", group);
        auto *maxAnimation = new QPropertyAnimation(newSidebar, "maximumWidth", group);
        for (QPropertyAnimation *animation : {minAnimation, maxAnimation}) {
            animation->setDuration(170);
            animation->setStartValue(oldWidth);
            animation->setEndValue(targetWidth);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            group->addAnimation(animation);
        }
        connect(group, &QParallelAnimationGroup::finished, newSidebar, [newSidebar, targetWidth] {
            newSidebar->setFixedWidth(targetWidth);
        });
        group->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void applyChromeState()
    {
        const bool fullscreen = isFullScreen();
        setFullscreenChromeProperty(m_windowChrome, fullscreen);
        setFullscreenChromeProperty(m_contentRoot, fullscreen);
    }

    void setFullscreenChromeProperty(QWidget *widget, bool fullscreen)
    {
        if (widget == nullptr) {
            return;
        }

        auto updateSurface = [fullscreen](QWidget *surface) {
            surface->setProperty("fullscreenChrome", fullscreen);
            surface->style()->unpolish(surface);
            surface->style()->polish(surface);
            surface->update();
        };
        auto shouldUpdate = [](QWidget *surface) {
            const QString objectName = surface->objectName();
            return objectName == "windowChrome"
                || objectName == "appRoot"
                || objectName == "loginRoot"
                || objectName == "studentRoot"
                || objectName == "contentPane"
                || objectName == "studentContentPane"
                || objectName == "sidebar"
                || objectName == "studentSidebar"
                || objectName == "loginBrand";
        };

        if (shouldUpdate(widget)) {
            updateSurface(widget);
        }
        for (QWidget *child : widget->findChildren<QWidget *>()) {
            if (shouldUpdate(child)) {
                updateSurface(child);
            }
        }
    }

    void buildLoginUi()
    {
        m_sidebarWidget = nullptr;
        m_stack = nullptr;

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
        brandLayout->setContentsMargins(38, 42, 38, 42);
        brandLayout->setSpacing(12);
        brandLayout->addWidget(buildBrandLockup(brandPanel, "Residence Operations"));
        brandLayout->addSpacing(34);
        brandLayout->addWidget(label("Housing Office", "loginBadge", brandPanel), 0, Qt::AlignLeft);
        brandLayout->addSpacing(18);
        auto *headline = label("Dormitory and restaurant records", "brand", brandPanel);
        headline->setStyleSheet("QLabel#brand { font-size: 31px; line-height: 38px; }");
        brandLayout->addWidget(headline);
        auto *sub = label("Room allocation, resident status, meal schedules, and campus access controls.", "brandSub", brandPanel);
        sub->setStyleSheet("QLabel#brandSub { font-size: 14px; line-height: 21px; }");
        brandLayout->addWidget(sub);
        auto *scope = label("Active academic year: 2025-2026\nResidence halls: 4\nPortal mode: Staff and resident access", "brandSub", brandPanel);
        scope->setStyleSheet("QLabel#brandSub { color: #D9E8E5; line-height: 20px; }");
        brandLayout->addSpacing(14);
        brandLayout->addWidget(scope);
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
        loginLayout->addWidget(label("Sign in", "pageTitle", loginCard));
        loginLayout->addWidget(label("Use an operations account or a registered student ID.", "pageKicker", loginCard));
        loginLayout->addWidget(statusPill("Secure campus access", "#E0F0EC", "#1F7560"), 0, Qt::AlignLeft);

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

        auto *button = new QPushButton("Open workspace", loginCard);
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
        minimize->setText(QString(QChar(0x2212)));
        minimize->setToolTip("Minimize");
        auto *close = new QPushButton(QString::fromUtf8("×"), controls);
        close->setObjectName("closeButton");
        close->setText(QString(QChar(0x00D7)));
        close->setToolTip("Close");
        auto *maximize = new QPushButton("[]", controls);
        maximize->setObjectName("windowButton");
        maximize->setText(QString(QChar(0x25A1)));
        maximize->setToolTip("Maximize or restore");
        auto *fullscreen = new QPushButton("F11", controls);
        fullscreen->setObjectName("windowButtonWide");
        fullscreen->setText(QString(QChar(0x26F6)));
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
        m_windowChrome = chrome;
        m_contentRoot = content;
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
        applyChromeState();

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
            invalidateVisibilityCache();
            buildAdminUi();
            refreshAll();
            showStatus("Signed in as " + m_adminProfiles.value(adminKey).displayName + ".");
            return;
        }

        const QString studentId = user.toUpper();
        if (m_university.hasStudent(studentId) && password == m_studentPortalPassword) {
            m_role = AuthRole::Student;
            m_currentStudentId = studentId;
            invalidateVisibilityCache();
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
        invalidateVisibilityCache();
        buildAdminUi();
        refreshAll();
        showStatus("Signed in as " + m_adminProfiles.value(m_currentAdminUsername).displayName + ".");
    }

    void loginAsStudentForTest(const QString &studentId)
    {
        m_role = AuthRole::Student;
        m_currentStudentId = studentId;
        invalidateVisibilityCache();
        buildStudentPortalUi();
        showStatus("Signed in as " + m_university.student(studentId).fullName() + ".");
    }

    void showPageForTest(int index)
    {
        setCurrentAdminPage(index);
    }

    void collapseSidebarForTest()
    {
        if (!m_sidebarCollapsed) {
            toggleSidebar();
        }
    }

    bool defaultLaunchIsMaximizedForTest()
    {
        showMaximized();
        qApp->processEvents();
        const bool result = isMaximized();
        hide();
        return result;
    }

    void addPersistenceProbeForTest()
    {
        m_university.addStudent(Student("S9999", "Persistence Probe", 1));
        invalidateVisibilityCache();
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

    bool dropdownsHealthyForTest() const
    {
        const QVector<QComboBox *> combos = {
            m_roomDormitoryFilterInput,
            m_roomOccupancyFilterInput,
            m_studentFilterInput,
            m_studentYearFilterInput,
            m_studentDormitoryFilterInput,
            m_profileAssignDormitoryInput,
            m_menuDormitoryInput,
            m_mealCounterDormitoryInput,
            m_menuDormitoryFilterInput,
            m_menuMealFilterInput,
            m_neighborhoodDormitoryFilterInput,
            m_neighborhoodAdminFilterInput,
            m_copyNeighborhoodInput,
            m_adminAccessInput,
            m_neighborhoodAccessInput,
            m_newAdminScopeInput,
            m_newAdminNeighborhoodInput,
        };
        for (QComboBox *combo : combos) {
            if (combo == nullptr || combo->property("polishedDropdown").toBool() != true || combo->count() == 0) {
                return false;
            }
        }
        return m_roomDormitoryFilterInput->count() >= 2
            && m_roomOccupancyFilterInput->count() == 4
            && m_studentFilterInput->count() == 3
            && m_studentYearFilterInput->count() >= 5
            && m_studentDormitoryFilterInput->count() >= 3
            && m_profileAssignDormitoryInput->count() >= 2
            && m_menuDormitoryInput->count() >= 2
            && m_mealCounterDormitoryInput->count() >= 2
            && m_menuDormitoryFilterInput->count() >= 2
            && m_menuMealFilterInput->count() == 4
            && m_neighborhoodDormitoryFilterInput->count() >= 2
            && m_neighborhoodAdminFilterInput->count() >= 3
            && m_copyNeighborhoodInput->count() >= 2
            && m_adminAccessInput->count() >= 3
            && m_neighborhoodAccessInput->count() >= 2
            && m_newAdminScopeInput->count() == 2
            && m_newAdminNeighborhoodInput->count() >= 2;
    }

    bool credentialRoutingHealthyForTest()
    {
        if (authenticateCredentialsForTest("admin", "wrong") != AuthRole::None
            || m_loginFeedback == nullptr
            || !m_loginFeedback->text().contains("Credentials not recognized")) {
            return false;
        }

        if (authenticateCredentialsForTest("ADMIN", "admin123") != AuthRole::Admin
            || m_currentAdminUsername != "admin"
            || !m_currentStudentId.isEmpty()
            || m_stack == nullptr
            || m_stack->count() != 4) {
            return false;
        }

        if (authenticateCredentialsForTest("s1001", m_studentPortalPassword) != AuthRole::Student
            || m_currentStudentId != "S1001"
            || !m_currentAdminUsername.isEmpty()) {
            return false;
        }

        return authenticateCredentialsForTest("S1001", "bad-password") == AuthRole::None;
    }

    bool adminPermissionBoundariesHealthyForTest()
    {
        loginAsAdminForTest("admin");
        if (!currentAdminHasFullAccess()
            || !currentAdminCanAccessDormitory("D1")
            || !currentAdminCanAccessDormitory("D2")
            || visibleDormitories().size() < 2) {
            return false;
        }

        loginAsAdminForTest("northadmin");
        if (currentAdminHasFullAccess()
            || !currentAdminCanAccessDormitory("D1")
            || currentAdminCanAccessDormitory("D2")) {
            return false;
        }

        for (const Dormitory &dormitory : visibleDormitories()) {
            if (dormitory.id() != "D1") {
                return false;
            }
        }
        for (const Student &student : visibleStudents()) {
            if (student.isAssigned() && student.dormitoryId().value() != "D1") {
                return false;
            }
        }

        showPageForTest(3);
        refreshAll();
        return m_neighborhoodIdInput != nullptr
            && m_adminAccessInput != nullptr
            && !m_neighborhoodIdInput->isEnabled()
            && !m_adminAccessInput->isEnabled();
    }

    bool restaurantAccessRulesHealthyForTest()
    {
        loginAsStudentForTest("S1001");
        const bool studentPortalLayoutHealthy = findChild<QFrame *>("studentHeroCard") != nullptr
            && findChildren<QFrame *>("studentInfoTile").size() >= 3
            && findChildren<QFrame *>("studentMealCard").size() >= 3
            && findChild<QFrame *>("studentWeekMenuCard") != nullptr
            && findChildren<QFrame *>("studentWeekMenuRow").size() >= 7;
        if (!studentPortalLayoutHealthy) {
            return false;
        }

        const QDate today = QDate::currentDate();
        const auto assignedMenu = m_university.restaurantMenuForStudent("S1001", today);
        if (!assignedMenu.has_value() || assignedMenu->breakfast.isEmpty() || assignedMenu->lunch.isEmpty() || assignedMenu->dinner.isEmpty()) {
            return false;
        }

        QString unassignedStudentId;
        for (const Student &student : m_university.students()) {
            if (!student.isAssigned()) {
                unassignedStudentId = student.id();
                break;
            }
        }
        if (unassignedStudentId.isEmpty()) {
            m_university.addStudent(Student("S-UNASSIGNED-TEST", "Unassigned Test Student", 1));
            unassignedStudentId = "S-UNASSIGNED-TEST";
        }

        try {
            (void)m_university.restaurantMenuForStudent(unassignedStudentId, today);
        } catch (const DomainError &) {
            return true;
        }
        return false;
    }

    bool restaurantMealCounterHealthyForTest()
    {
        loginAsAdminForTest("admin");
        showPageForTest(2);
        refreshAll();

        if (m_mealCounterDormitoryInput == nullptr
            || m_mealCounterDateInput == nullptr
            || m_mealCounterAmountInput == nullptr
            || m_mealCounterValueLabel == nullptr) {
            return false;
        }

        const QDate today = QDate::currentDate();
        selectComboData(m_mealCounterDormitoryInput, QStringLiteral("D1"));
        m_mealCounterDateInput->setDate(today);
        const int before = m_university.dormitory("D1").restaurant().mealsServedOn(today);
        m_mealCounterAmountInput->setValue(3);
        recordAdditionalMealsServed();

        const int after = m_university.dormitory("D1").restaurant().mealsServedOn(today);
        return after == before + 3
            && m_mealCounterValueLabel->text().contains(QString::number(after))
            && m_mealsMetric != nullptr
            && m_mealsMetric->text().toInt() >= after;
    }

    bool adminAccountCreatorHealthyForTest()
    {
        loginAsAdminForTest("admin");
        showPageForTest(3);
        refreshAll();

        if (m_newAdminUsernameInput == nullptr
            || m_newAdminDisplayNameInput == nullptr
            || m_newAdminPasswordInput == nullptr
            || m_newAdminScopeInput == nullptr
            || m_newAdminNeighborhoodInput == nullptr
            || m_adminAccessInput == nullptr) {
            return false;
        }

        m_newAdminUsernameInput->setText("trialoperator");
        m_newAdminDisplayNameInput->setText("Trial Operator");
        m_newAdminPasswordInput->setText("trial123");
        selectComboData(m_newAdminScopeInput, QStringLiteral("scoped"));
        selectComboData(m_newAdminNeighborhoodInput, QStringLiteral("EAST"));
        createAdminAccount();

        if (!m_adminProfiles.contains("trialoperator")
            || m_adminProfiles.value("trialoperator").fullAccess
            || !m_adminProfiles.value("trialoperator").neighborhoodIds.contains("EAST")
            || m_adminAccessInput->findData("trialoperator") < 0) {
            return false;
        }

        loginAsAdminForTest("trialoperator");
        return m_currentAdminUsername == "trialoperator"
            && !currentAdminHasFullAccess()
            && currentAdminCanAccessDormitory("D3")
            && !currentAdminCanAccessDormitory("D1")
            && visibleDormitories().size() == 1;
    }

    bool uiHardeningHealthyForTest()
    {
        loginAsAdminForTest("admin");
        showPageForTest(2);
        refreshAll();

        const bool mealCounterHealthy = m_mealCounterValueLabel != nullptr
            && m_mealCounterValueLabel->property("class").toString() == "counterValue"
            && m_mealCounterValueLabel->text().startsWith("Meals served:")
            && m_mealCounterValueLabel->sizeHint().height() <= 56;

        showPageForTest(0);
        refreshAll();
        const bool menuSummaryHealthy = m_todayMenuSummary != nullptr
            && m_todayMenuSummary->count() >= visibleDormitories().size();

        const bool maximizedHealthy = defaultLaunchIsMaximizedForTest();
        const QString style = qApp->styleSheet();
        const bool dialogStyleHealthy = style.contains("QDialog {")
            && style.contains("QLabel.counterValue");
        const bool spinBoxArrowsHealthy = style.contains("QSpinBox::up-arrow")
            && style.contains(":/icons/chevron-up.svg")
            && style.contains("QSpinBox::down-arrow")
            && style.contains(":/icons/chevron-down.svg");

        collapseSidebarForTest();
        QStackedWidget *stackBeforeToggle = m_stack;
        const int pageBeforeToggle = m_stack == nullptr ? -1 : m_stack->currentIndex();
        toggleSidebar();
        const bool sidebarTogglePreservesContent = m_stack == stackBeforeToggle
            && m_stack != nullptr
            && m_stack->currentIndex() == pageBeforeToggle;
        collapseSidebarForTest();
        bool compactNavHealthy = m_sidebarCollapsed && m_navButtons.size() == 4;
        for (int i = 0; i < m_navButtons.size(); ++i) {
            compactNavHealthy = compactNavHealthy
                && m_navButtons[i] != nullptr
                && m_navButtons[i]->property("compactNav").toBool()
                && m_navButtons[i]->text().isEmpty()
                && !m_navButtons[i]->icon().isNull()
                && m_navButtons[i]->iconSize() == QSize(26, 26);
        }
        bool compactLogoutHealthy = false;
        for (QPushButton *button : findChildren<QPushButton *>()) {
            if (button->toolTip() == "Log out"
                && button->property("compactNav").toBool()
                && button->text().isEmpty()
                && !button->icon().isNull()
                && button->iconSize() == QSize(26, 26)) {
                compactLogoutHealthy = true;
                break;
            }
        }

        return mealCounterHealthy
            && menuSummaryHealthy
            && maximizedHealthy
            && dialogStyleHealthy
            && spinBoxArrowsHealthy
            && sidebarTogglePreservesContent
            && compactNavHealthy
            && compactLogoutHealthy;
    }

    bool listFiltersAndMealImagesHealthyForTest()
    {
        loginAsAdminForTest("admin");
        showPageForTest(0);
        refreshAll();

        if (m_roomSearchInput == nullptr
            || m_roomOccupancyFilterInput == nullptr
            || m_roomDormitoryFilterInput == nullptr
            || m_roomMatrix == nullptr) {
            return false;
        }

        m_roomSearchInput->setText("North");
        selectComboData(m_roomOccupancyFilterInput, QStringLiteral("full"));
        refreshRoomMatrix();
        if (m_roomMatrix->rowCount() == 0) {
            return false;
        }
        for (int row = 0; row < m_roomMatrix->rowCount(); ++row) {
            if (!m_roomMatrix->item(row, 0)->text().contains("North", Qt::CaseInsensitive)
                || m_roomMatrix->item(row, 4)->text() != "Full") {
                return false;
            }
        }

        showPageForTest(1);
        if (m_studentSearchInput == nullptr
            || m_studentFilterInput == nullptr
            || m_studentYearFilterInput == nullptr
            || m_studentDormitoryFilterInput == nullptr
            || m_studentTable == nullptr) {
            return false;
        }
        m_studentSearchInput->setText("Amina");
        selectComboData(m_studentFilterInput, QStringLiteral("assigned"));
        selectComboData(m_studentYearFilterInput, 1);
        selectComboData(m_studentDormitoryFilterInput, QStringLiteral("D1"));
        refreshStudents();
        if (m_studentTable->rowCount() != 1
            || m_studentTable->item(0, 1)->text() != "Amina Benali") {
            return false;
        }

        showPageForTest(2);
        if (m_menuSearchInput == nullptr
            || m_menuDormitoryFilterInput == nullptr
            || m_menuMealFilterInput == nullptr
            || m_menuCards == nullptr) {
            return false;
        }
        m_menuSearchInput->setText("kofta");
        selectComboData(m_menuDormitoryFilterInput, QStringLiteral("D1"));
        selectComboData(m_menuMealFilterInput, QStringLiteral("lunch"));
        if (m_menuDateInput != nullptr) {
            m_menuDateInput->setDate(QDate::currentDate().addDays(3));
        }
        refreshMenuCards();
        if (m_menuCards->count() < 2) {
            return false;
        }

        const auto menu = m_university.dormitory("D1").restaurant().menuForDate(QDate::currentDate().addDays(3));
        if (!menu.has_value()
            || menu->breakfastImageUrl != mealImageForDescription(menu->breakfast, QStringLiteral("breakfast"))
            || menu->lunchImageUrl != mealImageForDescription(menu->lunch, QStringLiteral("lunch"))
            || menu->dinnerImageUrl != mealImageForDescription(menu->dinner, QStringLiteral("dinner"))
            || QPixmap(menu->lunchImageUrl).isNull()) {
            return false;
        }

        showPageForTest(3);
        if (m_neighborhoodSearchInput == nullptr
            || m_neighborhoodDormitoryFilterInput == nullptr
            || m_neighborhoodAdminFilterInput == nullptr
            || m_neighborhoodTable == nullptr) {
            return false;
        }
        m_neighborhoodSearchInput->setText("East");
        selectComboData(m_neighborhoodDormitoryFilterInput, QStringLiteral("D3"));
        selectComboData(m_neighborhoodAdminFilterInput, QStringLiteral("eastadmin"));
        refreshNeighborhoods();
        return m_neighborhoodTable->rowCount() == 1
            && m_neighborhoodTable->item(0, 0)->text() == "EAST";
    }

    void openFirstStudentProfileForTest()
    {
        if (m_studentTable != nullptr && m_studentTable->rowCount() > 0) {
            m_studentTable->selectRow(0);
            selectStudentFromTable();
            openSelectedStudentProfile();
            refreshCombos();
        }
    }

private:
    AuthRole authenticateCredentialsForTest(const QString &user, const QString &password)
    {
        if (m_role != AuthRole::None) {
            logout();
        }
        if (m_loginUserInput == nullptr || m_loginPasswordInput == nullptr) {
            return AuthRole::None;
        }
        m_loginUserInput->setText(user);
        m_loginPasswordInput->setText(password);
        authenticate();
        qApp->processEvents();
        return m_role;
    }

    void logout()
    {
        m_role = AuthRole::None;
        m_currentAdminUsername.clear();
        m_currentStudentId.clear();
        invalidateVisibilityCache();
        buildLoginUi();
    }

    void setCurrentAdminPage(int index)
    {
        if (m_stack == nullptr || index < 0 || index >= m_stack->count()) {
            return;
        }
        m_stack->setCurrentIndex(index);
        for (int i = 0; i < m_navButtons.size(); ++i) {
            if (m_navButtons[i] != nullptr) {
                m_navButtons[i]->setChecked(i == index);
            }
        }
    }

    void buildAdminUi()
    {

        auto *root = new QWidget(this);
        root->setObjectName("appRoot");
        auto *rootLayout = new QHBoxLayout(root);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);

        m_stack = new QStackedWidget(root);
        m_sidebarWidget = buildSidebar();
        rootLayout->addWidget(m_sidebarWidget);

        m_stack->addWidget(buildDashboardPage());
        m_stack->addWidget(buildStudentsPage());
        m_stack->addWidget(buildRestaurantPage());
        m_stack->addWidget(buildNeighborhoodsPage());
        connect(m_stack, &QStackedWidget::currentChanged, this, [this](int index) {
            for (int i = 0; i < m_navButtons.size(); ++i) {
                if (m_navButtons[i] != nullptr) {
                    m_navButtons[i]->setChecked(i == index);
                }
            }
        });
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
            layout->addWidget(buildBrandLockup(sidebar, "Residence Operations"));
        }

        auto *collapseButton = new QPushButton(m_sidebarCollapsed ? QString::fromUtf8("\u2630") : "<<", sidebar);
        collapseButton->setObjectName("sidebarToggle");
        collapseButton->setToolTip(m_sidebarCollapsed ? "Expand sidebar" : "Collapse sidebar");
        connect(collapseButton, &QPushButton::clicked, this, [this] { toggleSidebar(); });
        layout->addWidget(collapseButton);
        layout->addSpacing(m_sidebarCollapsed ? 12 : 22);

        auto *group = new QButtonGroup(sidebar);
        group->setExclusive(true);
        m_navButtons.clear();

        const QStringList items = {"Overview", "Residents", "Meals", "Neighborhoods"};
        for (int i = 0; i < items.size(); ++i) {
            auto *button = new QPushButton(m_sidebarCollapsed ? QString() : items[i], sidebar);
            button->setProperty("nav", true);
            button->setProperty("compactNav", m_sidebarCollapsed);
            button->setCheckable(true);
            button->setMinimumHeight(44);
            button->setMinimumWidth(m_sidebarCollapsed ? 48 : 0);
            if (m_sidebarCollapsed) {
                button->setIcon(adminNavIcon(i));
                button->setIconSize(QSize(26, 26));
            }
            button->setToolTip(items[i]);
            group->addButton(button, i);
            m_navButtons.append(button);
            layout->addWidget(button);
            connect(button, &QPushButton::clicked, this, [this, i] { setCurrentAdminPage(i); });
            if (i == 0) {
                button->setChecked(true);
            }
        }

        layout->addStretch();
        auto *logoutButton = new QPushButton(m_sidebarCollapsed ? QString() : "Log out", sidebar);
        logoutButton->setProperty("nav", true);
        logoutButton->setProperty("compactNav", m_sidebarCollapsed);
        logoutButton->setMinimumHeight(44);
        logoutButton->setMinimumWidth(m_sidebarCollapsed ? 48 : 0);
        if (m_sidebarCollapsed) {
            logoutButton->setIcon(logoutNavIcon());
            logoutButton->setIconSize(QSize(26, 26));
        }
        logoutButton->setToolTip("Log out");
        connect(logoutButton, &QPushButton::clicked, this, [this] { logout(); });
        layout->addWidget(logoutButton);
        if (!m_sidebarCollapsed) {
            auto *hint = label("Operations Console\nCampus records", "brandSub", sidebar);
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

        m_sidebarWidget = buildStudentSidebar();
        rootLayout->addWidget(m_sidebarWidget);

        const Student &student = m_university.student(m_currentStudentId);
        auto *page = pageContainer(student.fullName(), "Resident record, current room, and today's dining access.", true);
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        layout->addWidget(buildStudentHeroCard(student));

        auto *accessRow = new QHBoxLayout();
        accessRow->setSpacing(14);
        if (student.isAssigned()) {
            const Dormitory &dormitory = m_university.dormitory(student.dormitoryId().value());
            const Room &room = dormitory.room(student.roomNumber().value());
            accessRow->addWidget(buildStudentInfoTile("Room", QString::number(room.number()), dormitory.name(), "Assigned"), 1);
            accessRow->addWidget(buildStudentInfoTile("Occupancy", QString("%1 / %2").arg(room.occupancy()).arg(room.capacity()), "Current room residents", room.isFull() ? "Full" : "Open beds"), 1);
            accessRow->addWidget(buildStudentInfoTile("Dining", dormitory.restaurant().name(), "Access follows your dormitory", "Available"), 1);
        } else {
            accessRow->addWidget(buildStudentInfoTile("Room", "Unassigned", "No accommodation assigned", "Pending"), 1);
            accessRow->addWidget(buildStudentInfoTile("Occupancy", "-", "No room selected yet", "Pending"), 1);
            accessRow->addWidget(buildStudentInfoTile("Dining", "Unavailable", "Restaurant access requires a room", "Locked"), 1);
        }
        layout->addLayout(accessRow);

        auto *diningHeader = new QHBoxLayout();
        diningHeader->setSpacing(10);
        auto *diningCopy = new QVBoxLayout();
        diningCopy->setSpacing(3);
        diningCopy->addWidget(classLabel("Today's Dining", "cardTitle"));
        diningCopy->addWidget(classLabel(student.isAssigned()
                                             ? "Menus are attached to your residence hall."
                                             : "Dining access appears after a room assignment.",
                                         "muted"));
        diningHeader->addLayout(diningCopy, 1);
        diningHeader->addWidget(statusPill(QDate::currentDate().toString("MMM d"), "#E6F4EE", "#1D7A57"));
        layout->addLayout(diningHeader);

        if (student.isAssigned()) {
            const Dormitory &dormitory = m_university.dormitory(student.dormitoryId().value());
            const auto menu = dormitory.restaurant().menuForDate(QDate::currentDate());
            layout->addWidget(classLabel(dormitory.restaurant().name() + " - " + dormitory.name(), "muted"));

            auto *mealGrid = new QHBoxLayout();
            mealGrid->setSpacing(14);
            if (menu.has_value()) {
                mealGrid->addWidget(buildStudentMealCard("Breakfast", menu->breakfast, menu->breakfastImageUrl), 1);
                mealGrid->addWidget(buildStudentMealCard("Lunch", menu->lunch, menu->lunchImageUrl), 1);
                mealGrid->addWidget(buildStudentMealCard("Dinner", menu->dinner, menu->dinnerImageUrl), 1);
            } else {
                mealGrid->addWidget(buildStudentInfoTile("Restaurant Menu", "No menu", "No menu is set for today.", "Missing"), 1);
            }
            layout->addLayout(mealGrid, 1);
            layout->addWidget(buildStudentWeekMenuCard(dormitory));
        } else {
            layout->addWidget(buildStudentInfoTile("Restaurant Menu", "No access", "Restaurant access requires a room assignment.", "Locked"), 1);
        }

        auto *scroll = new QScrollArea(root);
        scroll->setObjectName("studentPortalScroll");
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll->setWidget(page);
        QTimer::singleShot(0, scroll, [scroll] {
            scroll->verticalScrollBar()->setValue(0);
        });
        rootLayout->addWidget(scroll, 1);
        setAppContent(root);
    }

    QWidget *buildStudentSidebar()
    {
        auto *sidebar = new QFrame(this);
        sidebar->setObjectName("studentSidebar");
        sidebar->setFixedWidth(m_sidebarCollapsed ? 76 : 238);
        auto *sideLayout = new QVBoxLayout(sidebar);
        sideLayout->setContentsMargins(m_sidebarCollapsed ? 14 : 22, 28, m_sidebarCollapsed ? 14 : 22, 22);
        sideLayout->setSpacing(11);
        if (m_sidebarCollapsed) {
            sideLayout->addWidget(brandMark(sidebar), 0, Qt::AlignHCenter);
        } else {
            sideLayout->addWidget(buildBrandLockup(sidebar, "Resident Portal"));
        }

        auto *collapseButton = new QPushButton(m_sidebarCollapsed ? QString::fromUtf8("\u2630") : "<<", sidebar);
        collapseButton->setObjectName("sidebarToggle");
        collapseButton->setToolTip(m_sidebarCollapsed ? "Expand sidebar" : "Collapse sidebar");
        connect(collapseButton, &QPushButton::clicked, this, [this] { toggleSidebar(); });
        sideLayout->addWidget(collapseButton);
        sideLayout->addSpacing(m_sidebarCollapsed ? 12 : 22);
        if (!m_sidebarCollapsed) {
            sideLayout->addWidget(label("Room status\nMeal access\nResident record", "brandSub", sidebar));
        }
        sideLayout->addStretch();
        auto *logoutButton = new QPushButton(m_sidebarCollapsed ? QString() : "Log out", sidebar);
        logoutButton->setProperty("nav", true);
        logoutButton->setProperty("compactNav", m_sidebarCollapsed);
        logoutButton->setMinimumHeight(44);
        logoutButton->setMinimumWidth(m_sidebarCollapsed ? 48 : 0);
        if (m_sidebarCollapsed) {
            logoutButton->setIcon(logoutNavIcon());
            logoutButton->setIconSize(QSize(26, 26));
        }
        logoutButton->setToolTip("Log out");
        connect(logoutButton, &QPushButton::clicked, this, [this] { logout(); });
        sideLayout->addWidget(logoutButton);
        return sidebar;
    }

    QWidget *buildStudentHeroCard(const Student &student)
    {
        auto *hero = new QFrame(this);
        hero->setObjectName("studentHeroCard");
        hero->setMinimumHeight(126);

        auto *layout = new QHBoxLayout(hero);
        layout->setContentsMargins(24, 18, 24, 18);
        layout->setSpacing(20);

        auto *copy = new QVBoxLayout();
        copy->setSpacing(6);
        copy->addWidget(label("RESIDENT DASHBOARD", "studentHeroKicker", hero));
        copy->addWidget(label("Welcome, " + student.fullName(), "studentHeroTitle", hero));
        copy->addWidget(label(student.id() + " - Academic year " + QString::number(student.academicYear()), "studentHeroText", hero));
        copy->addStretch();
        copy->addWidget(statusPill(student.isAssigned() ? "Active resident" : "Waiting for assignment",
                                   student.isAssigned() ? "#DFF4EA" : "#FFF7E6",
                                   student.isAssigned() ? "#116149" : "#9A6700"),
                        0,
                        Qt::AlignLeft);
        layout->addLayout(copy, 1);

        auto *summary = new QVBoxLayout();
        summary->setSpacing(4);
        summary->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        summary->addWidget(label(QDate::currentDate().toString("dddd"), "studentHeroText", hero), 0, Qt::AlignRight);
        summary->addWidget(label(QDate::currentDate().toString("MMM d"), "studentHeroMetric", hero), 0, Qt::AlignRight);
        if (student.isAssigned()) {
            const Dormitory &dormitory = m_university.dormitory(student.dormitoryId().value());
            summary->addWidget(label("Room " + QString::number(student.roomNumber().value()) + " - " + dormitory.name(),
                                     "studentHeroText",
                                     hero),
                               0,
                               Qt::AlignRight);
        } else {
            summary->addWidget(label("No room assigned", "studentHeroText", hero), 0, Qt::AlignRight);
        }
        layout->addLayout(summary);

        return hero;
    }

    QWidget *buildStudentInfoTile(const QString &labelText, const QString &value, const QString &detail, const QString &badge)
    {
        auto *tile = new QFrame(this);
        tile->setObjectName("studentInfoTile");
        tile->setMinimumHeight(104);

        auto *layout = new QVBoxLayout(tile);
        layout->setContentsMargins(16, 12, 16, 12);
        layout->setSpacing(4);
        layout->addWidget(classLabel(labelText.toUpper(), "metricLabel"));
        layout->addWidget(classLabel(value, "cardTitle"));
        layout->addWidget(classLabel(detail, "muted"));
        layout->addStretch();
        layout->addWidget(statusPill(badge, "#E6F4EE", "#1D7A57"), 0, Qt::AlignLeft);
        return tile;
    }

    QWidget *buildStudentMealCard(const QString &meal, const QString &description, const QString &imageUrl)
    {
        auto *box = new QFrame(this);
        box->setObjectName("studentMealCard");
        box->setMinimumHeight(214);

        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(12, 12, 12, 12);
        layout->setSpacing(7);
        const QString resolvedImageUrl = imageUrl.trimmed().isEmpty()
            ? mealImageForDescription(description, meal.toLower())
            : imageUrl;
        layout->addWidget(mealThumbnail(resolvedImageUrl, QSize(168, 98), "studentMealPhoto"), 0, Qt::AlignHCenter);
        layout->addWidget(classLabel(meal, "muted"));
        auto *title = classLabel(description.isEmpty() ? "Not set" : description, "cardTitle");
        title->setMinimumHeight(42);
        layout->addWidget(title);
        layout->addStretch();
        return box;
    }

    QWidget *buildStudentWeekMenuCard(const Dormitory &dormitory)
    {
        auto *box = new QFrame(this);
        box->setObjectName("studentWeekMenuCard");

        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(18, 16, 18, 18);
        layout->setSpacing(10);

        auto *top = new QHBoxLayout();
        top->setSpacing(10);
        auto *copy = new QVBoxLayout();
        copy->setSpacing(3);
        copy->addWidget(classLabel("Next 7 Days", "cardTitle"));
        copy->addWidget(classLabel("Upcoming breakfast, lunch, and dinner for your residence hall.", "muted"));
        top->addLayout(copy, 1);
        top->addWidget(statusPill(dormitory.id(), "#E6F4EE", "#1D7A57"));
        layout->addLayout(top);

        const QDate today = QDate::currentDate();
        for (int offset = 1; offset <= 7; ++offset) {
            layout->addWidget(buildStudentWeekMenuRow(dormitory, today.addDays(offset)));
        }
        return box;
    }

    QWidget *buildStudentWeekMenuRow(const Dormitory &dormitory, const QDate &date)
    {
        auto *row = new QFrame(this);
        row->setObjectName("studentWeekMenuRow");

        auto *layout = new QHBoxLayout(row);
        layout->setContentsMargins(12, 10, 12, 10);
        layout->setSpacing(12);

        auto *dateBlock = new QVBoxLayout();
        dateBlock->setSpacing(2);
        dateBlock->addWidget(classLabel(date.toString("ddd"), "metricLabel"));
        dateBlock->addWidget(classLabel(date.toString("MMM d"), "cardTitle"));
        layout->addLayout(dateBlock);

        const auto menu = dormitory.restaurant().menuForDate(date);
        auto *meals = new QVBoxLayout();
        meals->setSpacing(2);
        if (menu.has_value()) {
            meals->addWidget(classLabel("Breakfast: " + menu->breakfast, "muted"));
            meals->addWidget(classLabel("Lunch: " + menu->lunch, "muted"));
            meals->addWidget(classLabel("Dinner: " + menu->dinner, "muted"));
        } else {
            meals->addWidget(classLabel("No menu published for this date.", "muted"));
        }
        layout->addLayout(meals, 1);

        layout->addWidget(statusPill(menu.has_value() ? "Menu set" : "Pending",
                                     menu.has_value() ? "#E6F4EE" : "#FFF7E6",
                                     menu.has_value() ? "#1D7A57" : "#9A6700"));
        return row;
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
        auto *page = pageContainer("Operations Overview", "Capacity, assignments, and meal activity for the active residence cycle.");
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
        auto *page = pageContainer("Resident Records", "Find residents, update academic details, and manage room assignments.");
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        layout->addWidget(buildStudentSearchCard(), 1);

        return page;
    }

    QWidget *buildRestaurantPage()
    {
        auto *page = pageContainer("Dining Schedules", "Maintain daily menus and verify residence-based meal access.");
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        auto *split = new QHBoxLayout();
        split->setSpacing(18);
        auto *controlsScroll = new QScrollArea(page);
        controlsScroll->setWidgetResizable(true);
        controlsScroll->setFrameShape(QFrame::NoFrame);
        controlsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        auto *controlsContent = new QWidget(controlsScroll);
        auto *controls = new QVBoxLayout(controlsContent);
        controls->setContentsMargins(0, 0, 8, 0);
        controls->setSpacing(18);
        controls->addWidget(buildMealCounterCard());
        controls->addWidget(buildMenuEditorCard());
        controls->addStretch();
        controlsScroll->setWidget(controlsContent);
        split->addWidget(controlsScroll, 1);

        auto *preview = card(page);
        auto *previewLayout = new QVBoxLayout(preview);
        previewLayout->setContentsMargins(20, 16, 20, 16);
        previewLayout->setSpacing(10);
        previewLayout->addWidget(classLabel("Daily Menu Cards", "cardTitle"));
        m_menuSearchInput = new QLineEdit(preview);
        m_menuSearchInput->setPlaceholderText("Search menu items");
        m_menuSearchInput->setMinimumHeight(38);
        previewLayout->addWidget(m_menuSearchInput);

        auto *filterRow = new QHBoxLayout();
        filterRow->setSpacing(8);
        m_menuDormitoryFilterInput = createDropdown(preview);
        m_menuDormitoryFilterInput->addItem("Any dormitory", "all");
        m_menuMealFilterInput = createDropdown(preview);
        m_menuMealFilterInput->addItem("Any meal", "all");
        m_menuMealFilterInput->addItem("Breakfast", "breakfast");
        m_menuMealFilterInput->addItem("Lunch", "lunch");
        m_menuMealFilterInput->addItem("Dinner", "dinner");
        auto *clearMenuFilters = new QPushButton("Clear", preview);
        filterRow->addWidget(m_menuDormitoryFilterInput, 1);
        filterRow->addWidget(m_menuMealFilterInput, 1);
        filterRow->addWidget(clearMenuFilters);
        previewLayout->addLayout(filterRow);

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

        connect(m_menuSearchInput, &QLineEdit::textChanged, this, [this] { refreshMenuCards(); });
        connect(m_menuDormitoryFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshMenuCards(); });
        connect(m_menuMealFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshMenuCards(); });
        connect(clearMenuFilters, &QPushButton::clicked, this, [this] {
            m_menuSearchInput->clear();
            m_menuDormitoryFilterInput->setCurrentIndex(0);
            m_menuMealFilterInput->setCurrentIndex(0);
        });
        return page;
    }

    QWidget *buildNeighborhoodsPage()
    {
        auto *page = pageContainer("Campus Neighborhoods", "Maintain campus groups and administrator access boundaries.");
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

        auto *split = new QHBoxLayout();
        split->setSpacing(18);

        auto *tableCard = card(page);
        auto *tableLayout = new QVBoxLayout(tableCard);
        tableLayout->setContentsMargins(20, 18, 20, 20);
        tableLayout->setSpacing(14);
        tableLayout->addWidget(classLabel("Neighborhood Registry", "cardTitle"));
        m_neighborhoodSearchInput = new QLineEdit(tableCard);
        m_neighborhoodSearchInput->setPlaceholderText("Search neighborhood, dormitory, or admin");
        m_neighborhoodSearchInput->setMinimumHeight(38);
        tableLayout->addWidget(m_neighborhoodSearchInput);

        auto *filterRow = new QHBoxLayout();
        filterRow->setSpacing(8);
        m_neighborhoodDormitoryFilterInput = createDropdown(tableCard);
        m_neighborhoodDormitoryFilterInput->addItem("Any dormitory", "all");
        m_neighborhoodAdminFilterInput = createDropdown(tableCard);
        m_neighborhoodAdminFilterInput->addItem("Any admin", "all");
        auto *clearNeighborhoodFilters = new QPushButton("Clear", tableCard);
        filterRow->addWidget(m_neighborhoodDormitoryFilterInput, 1);
        filterRow->addWidget(m_neighborhoodAdminFilterInput, 1);
        filterRow->addWidget(clearNeighborhoodFilters);
        tableLayout->addLayout(filterRow);

        m_neighborhoodTable = new QTableWidget(tableCard);
        setupTable(m_neighborhoodTable, {"ID", "Name", "Dormitories", "Admins"});
        tableLayout->addWidget(m_neighborhoodTable, 1);
        split->addWidget(tableCard, 2);

        auto *controlsScroll = new QScrollArea(page);
        controlsScroll->setWidgetResizable(true);
        controlsScroll->setFrameShape(QFrame::NoFrame);
        controlsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        auto *controlsContent = new QWidget(controlsScroll);
        auto *controls = new QVBoxLayout(controlsContent);
        controls->setContentsMargins(0, 0, 8, 0);
        controls->setSpacing(18);
        controls->addWidget(buildAdminAccountCreatorCard());
        controls->addWidget(buildAccessEditorCard());
        controls->addWidget(buildNeighborhoodEditorCard());
        controls->addStretch();
        controlsScroll->setWidget(controlsContent);
        split->addWidget(controlsScroll, 1);
        layout->addLayout(split, 1);

        connect(m_neighborhoodSearchInput, &QLineEdit::textChanged, this, [this] { refreshNeighborhoods(); });
        connect(m_neighborhoodDormitoryFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshNeighborhoods(); });
        connect(m_neighborhoodAdminFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshNeighborhoods(); });
        connect(clearNeighborhoodFilters, &QPushButton::clicked, this, [this] {
            m_neighborhoodSearchInput->clear();
            m_neighborhoodDormitoryFilterInput->setCurrentIndex(0);
            m_neighborhoodAdminFilterInput->setCurrentIndex(0);
        });
        return page;
    }

    QWidget *buildNeighborhoodEditorCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(10);
        layout->addWidget(classLabel("Neighborhood Setup", "cardTitle"));

        m_neighborhoodIdInput = new QLineEdit(box);
        m_neighborhoodIdInput->setPlaceholderText("EAST");
        m_neighborhoodNameInput = new QLineEdit(box);
        m_neighborhoodNameInput->setPlaceholderText("East Campus Neighborhood");
        m_copyNeighborhoodInput = createDropdown(box);
        m_copyNeighborhoodInput->setMinimumContentsLength(18);

        layout->addWidget(fieldLabel("New ID", m_neighborhoodIdInput));
        layout->addWidget(fieldLabel("New Name", m_neighborhoodNameInput));
        layout->addWidget(fieldLabel("Source Neighborhood", m_copyNeighborhoodInput));

        auto *row = new QHBoxLayout();
        auto *add = new QPushButton("Create", box);
        add->setProperty("class", "primary");
        auto *copy = new QPushButton("Copy Settings", box);
        row->addWidget(add);
        row->addWidget(copy);
        layout->addLayout(row);

        connect(add, &QPushButton::clicked, this, [this] { addNeighborhood(); });
        connect(copy, &QPushButton::clicked, this, [this] { copyNeighborhood(); });
        return box;
    }

    QWidget *buildAdminAccountCreatorCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(10);
        layout->addWidget(classLabel("Create Admin Account", "cardTitle"));

        m_newAdminUsernameInput = new QLineEdit(box);
        m_newAdminUsernameInput->setPlaceholderText("eastoperator");
        m_newAdminDisplayNameInput = new QLineEdit(box);
        m_newAdminDisplayNameInput->setPlaceholderText("East Operations Manager");
        m_newAdminPasswordInput = new QLineEdit(box);
        m_newAdminPasswordInput->setPlaceholderText("Temporary password");
        m_newAdminPasswordInput->setEchoMode(QLineEdit::Password);
        m_newAdminScopeInput = createDropdown(box);
        m_newAdminScopeInput->addItem("Neighborhood scoped", "scoped");
        m_newAdminScopeInput->addItem("Full access", "full");
        m_newAdminNeighborhoodInput = createDropdown(box);
        m_newAdminStatusLabel = classLabel("", "muted");
        m_newAdminStatusLabel->setWordWrap(true);

        layout->addWidget(fieldLabel("Username", m_newAdminUsernameInput));
        layout->addWidget(fieldLabel("Display name", m_newAdminDisplayNameInput));
        layout->addWidget(fieldLabel("Password", m_newAdminPasswordInput));
        layout->addWidget(fieldLabel("Access type", m_newAdminScopeInput));
        layout->addWidget(fieldLabel("Starting neighborhood", m_newAdminNeighborhoodInput));
        layout->addWidget(m_newAdminStatusLabel);

        auto *create = new QPushButton("Create admin", box);
        create->setProperty("class", "primary");
        layout->addWidget(create);

        connect(create, &QPushButton::clicked, this, [this] { createAdminAccount(); });
        connect(m_newAdminScopeInput, &QComboBox::currentIndexChanged, this, [this] { refreshNewAdminScopeControls(); });
        return box;
    }

    QWidget *buildAccessEditorCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(10);
        layout->addWidget(classLabel("Admin Access", "cardTitle"));

        m_adminAccessInput = createDropdown(box);
        m_neighborhoodAccessInput = createDropdown(box);
        m_adminAccessInput->setMinimumContentsLength(18);
        m_neighborhoodAccessInput->setMinimumContentsLength(18);
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

        auto *header = new QFrame(page);
        header->setObjectName("pageHeader");
        auto *headerLayout = new QHBoxLayout(header);
        headerLayout->setContentsMargins(18, 14, 14, 14);
        headerLayout->setSpacing(14);
        auto *copy = new QVBoxLayout();
        copy->setSpacing(4);
        copy->addWidget(label(title, "pageTitle", header));
        copy->addWidget(label(subtitle, "pageKicker", header));
        headerLayout->addLayout(copy, 1);
        headerLayout->addWidget(statusPill(studentTone ? "Resident workspace" : "Staff workspace",
                                          studentTone ? "#FFF7E6" : "#E0F0EC",
                                          studentTone ? "#9A6700" : "#1F7560"));
        headerLayout->addWidget(buildWindowControls());
        layout->addWidget(header);
        return page;
    }

    QLabel *addMetric(QHBoxLayout *layout, const QString &name, const QString &value, const QString &description)
    {
        auto *box = card(this);
        box->setObjectName("metricCard");
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
        top->addWidget(classLabel("Room Occupancy", "cardTitle"));
        top->addStretch();
        top->addWidget(classLabel("Capacity controlled", "muted"));
        layout->addLayout(top);

        m_roomSearchInput = new QLineEdit(box);
        m_roomSearchInput->setPlaceholderText("Search dormitory, room, or resident ID");
        m_roomSearchInput->setMinimumHeight(38);
        layout->addWidget(m_roomSearchInput);

        auto *filterRow = new QHBoxLayout();
        filterRow->setSpacing(8);
        m_roomDormitoryFilterInput = createDropdown(box);
        m_roomDormitoryFilterInput->addItem("Any dormitory", "all");
        m_roomOccupancyFilterInput = createDropdown(box);
        m_roomOccupancyFilterInput->addItem("Any occupancy", "all");
        m_roomOccupancyFilterInput->addItem("Available", "available");
        m_roomOccupancyFilterInput->addItem("Occupied", "occupied");
        m_roomOccupancyFilterInput->addItem("Full", "full");
        auto *clearRoomFilters = new QPushButton("Clear", box);
        filterRow->addWidget(m_roomDormitoryFilterInput, 1);
        filterRow->addWidget(m_roomOccupancyFilterInput, 1);
        filterRow->addWidget(clearRoomFilters);
        layout->addLayout(filterRow);

        m_roomMatrix = new QTableWidget(box);
        setupTable(m_roomMatrix, {"Dormitory", "Room", "Capacity", "Residents", "State"});
        applyColumnWeights(m_roomMatrix, {160, 60, 75, 125, 75});
        layout->addWidget(m_roomMatrix, 1);

        connect(m_roomSearchInput, &QLineEdit::textChanged, this, [this] { refreshRoomMatrix(); });
        connect(m_roomDormitoryFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshRoomMatrix(); });
        connect(m_roomOccupancyFilterInput, &QComboBox::currentIndexChanged, this, [this] { refreshRoomMatrix(); });
        connect(clearRoomFilters, &QPushButton::clicked, this, [this] {
            m_roomSearchInput->clear();
            m_roomDormitoryFilterInput->setCurrentIndex(0);
            m_roomOccupancyFilterInput->setCurrentIndex(0);
        });
        return box;
    }

    QWidget *buildTodayMenuSummaryCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(14);
        layout->addWidget(classLabel("Today's Dining Schedule", "cardTitle"));
        auto *note = classLabel("Menus are attached to residence halls. Access follows the resident's current assignment.", "muted");
        note->setWordWrap(true);
        layout->addWidget(note);
        m_todayMenuSummary = new QVBoxLayout();
        m_todayMenuSummary->setSpacing(8);
        layout->addLayout(m_todayMenuSummary);
        layout->addWidget(classLabel("Resident access enforced", "muted"), 0, Qt::AlignLeft);
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
        m_studentFilterInput = createDropdown(box);
        m_studentFilterInput->addItem("All students", "all");
        m_studentFilterInput->addItem("Assigned only", "assigned");
        m_studentFilterInput->addItem("Unassigned only", "unassigned");
        m_studentYearFilterInput = createDropdown(box);
        m_studentYearFilterInput->addItem("Any year", 0);
        m_studentDormitoryFilterInput = createDropdown(box);
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
        m_profileNameInput->setFixedHeight(40);
        m_profileYearInput = new QSpinBox(box);
        m_profileYearInput->setRange(1, 8);
        m_profileYearInput->setFixedHeight(40);
        profileLayout->addWidget(fieldLabel("Modify Full Name", m_profileNameInput));
        profileLayout->addWidget(fieldLabel("Modify Academic Year", m_profileYearInput));
        connect(m_profileNameInput, &QLineEdit::textEdited, this, [this] { markStudentProfileDirty(); });
        connect(m_profileYearInput, &QSpinBox::valueChanged, this, [this] { markStudentProfileDirty(); });

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
        reset->setFixedHeight(36);
        duplicate->setFixedHeight(36);
        deleteButton->setFixedHeight(36);
        utilityRow->addWidget(reset);
        utilityRow->addWidget(duplicate);
        utilityRow->addWidget(deleteButton);
        profileLayout->addLayout(utilityRow);
        connect(reset, &QPushButton::clicked, this, [this] { resetSelectedStudentEdits(); });
        connect(duplicate, &QPushButton::clicked, this, [this] { duplicateSelectedStudent(); });
        connect(deleteButton, &QPushButton::clicked, this, [this] { deleteSelectedStudent(); });

        profileLayout->addWidget(classLabel("Accommodation Actions", "cardTitle"));
        m_profileAssignDormitoryInput = createDropdown(box);
        m_profileAssignDormitoryInput->setMinimumHeight(40);
        m_profileAssignRoomInput = new QSpinBox(box);
        m_profileAssignRoomInput->setRange(1, 9999);
        m_profileAssignRoomInput->setFixedHeight(40);
        profileLayout->addWidget(fieldLabel("Dormitory", m_profileAssignDormitoryInput));
        profileLayout->addWidget(fieldLabel("Room Number", m_profileAssignRoomInput));

        auto *actionRow = new QHBoxLayout();
        actionRow->setSpacing(8);
        auto *assign = new QPushButton("Assign", box);
        assign->setProperty("class", "primary");
        auto *remove = new QPushButton("Remove", box);
        remove->setProperty("class", "danger");
        assign->setFixedHeight(36);
        remove->setFixedHeight(36);
        actionRow->addWidget(assign);
        actionRow->addWidget(remove);
        profileLayout->addLayout(actionRow);
        connect(assign, &QPushButton::clicked, this, [this] { assignSelectedStudent(); });
        connect(remove, &QPushButton::clicked, this, [this] { removeSelectedAssignment(); });
        connect(m_profileAssignDormitoryInput, &QComboBox::currentIndexChanged, this, [this] { updateSuggestedRoomNumber(); });
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
        m_assignStudentInput = createDropdown(box);
        m_assignDormitoryInput = createDropdown(box);
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
        auto *scroll = new QScrollArea(box);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setWidgetResizable(true);
        scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        auto *form = new QWidget(scroll);
        auto *formLayout = new QVBoxLayout(form);
        formLayout->setContentsMargins(0, 0, 0, 0);
        formLayout->setSpacing(10);

        m_menuDormitoryInput = createDropdown(box);
        m_menuDateInput = new QDateEdit(QDate::currentDate(), box);
        m_menuDateInput->setCalendarPopup(true);
        m_menuDateInput->setDisplayFormat("yyyy-MM-dd");
        m_breakfastInput = new QLineEdit(box);
        m_lunchInput = new QLineEdit(box);
        m_dinnerInput = new QLineEdit(box);
        m_breakfastImageUrlInput = new QLineEdit(box);
        m_lunchImageUrlInput = new QLineEdit(box);
        m_dinnerImageUrlInput = new QLineEdit(box);
        m_breakfastInput->setPlaceholderText("Coffee and eggs");
        m_lunchInput->setPlaceholderText("Chicken and rice");
        m_dinnerInput->setPlaceholderText("Soup and salad");
        m_breakfastImageUrlInput->setPlaceholderText(":/icons/meal-breakfast.png");
        m_lunchImageUrlInput->setPlaceholderText(":/icons/meal-lunch.png");
        m_dinnerImageUrlInput->setPlaceholderText(":/icons/meal-dinner.png");

        formLayout->addWidget(fieldLabel("Dormitory", m_menuDormitoryInput));
        formLayout->addWidget(fieldLabel("Date", m_menuDateInput));
        formLayout->addWidget(fieldLabel("Breakfast", m_breakfastInput));
        formLayout->addWidget(buildMealImageUrlInput("Breakfast image", m_breakfastImageUrlInput));
        formLayout->addWidget(fieldLabel("Lunch", m_lunchInput));
        formLayout->addWidget(buildMealImageUrlInput("Lunch image", m_lunchImageUrlInput));
        formLayout->addWidget(fieldLabel("Dinner", m_dinnerInput));
        formLayout->addWidget(buildMealImageUrlInput("Dinner image", m_dinnerImageUrlInput));
        auto *button = new QPushButton("Save menu", box);
        button->setProperty("class", "primary");
        connect(button, &QPushButton::clicked, this, [this] { saveMenu(); });
        connect(m_menuDateInput, &QDateEdit::dateChanged, this, [this](const QDate &date) {
            Q_UNUSED(date);
            loadSelectedMenuIntoEditor();
            refreshMenuCards();
        });
        connect(m_menuDormitoryInput, &QComboBox::currentIndexChanged, this, [this] { loadSelectedMenuIntoEditor(); });
        formLayout->addWidget(button);
        formLayout->addStretch();
        scroll->setWidget(form);
        layout->addWidget(scroll, 1);
        return box;
    }

    QWidget *buildMealCounterCard()
    {
        auto *box = card(this);
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(20, 18, 20, 20);
        layout->setSpacing(10);
        layout->addWidget(classLabel("Meal Service Counter", "cardTitle"));

        m_mealCounterDormitoryInput = createDropdown(box);
        m_mealCounterDateInput = new QDateEdit(QDate::currentDate(), box);
        m_mealCounterDateInput->setCalendarPopup(true);
        m_mealCounterDateInput->setDisplayFormat("yyyy-MM-dd");
        m_mealCounterDateInput->setMinimumHeight(40);
        m_mealCounterAmountInput = new QSpinBox(box);
        m_mealCounterAmountInput->setRange(1, 500);
        m_mealCounterAmountInput->setValue(1);
        m_mealCounterAmountInput->setMinimumHeight(40);

        m_mealCounterValueLabel = classLabel("Meals served: 0", "counterValue");
        m_mealCounterValueLabel->setAlignment(Qt::AlignCenter);

        layout->addWidget(fieldLabel("Dormitory", m_mealCounterDormitoryInput));
        layout->addWidget(fieldLabel("Service date", m_mealCounterDateInput));
        layout->addWidget(fieldLabel("Add meals served", m_mealCounterAmountInput));
        layout->addWidget(m_mealCounterValueLabel);

        auto *recordButton = new QPushButton("Record meals", box);
        recordButton->setProperty("class", "primary");
        layout->addWidget(recordButton);

        connect(recordButton, &QPushButton::clicked, this, [this] { recordAdditionalMealsServed(); });
        connect(m_mealCounterDormitoryInput, &QComboBox::currentIndexChanged, this, [this] { refreshMealCounter(); });
        connect(m_mealCounterDateInput, &QDateEdit::dateChanged, this, [this](const QDate &date) {
            Q_UNUSED(date);
            refreshMealCounter();
            refreshMenuCards();
        });
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

    QWidget *buildMealImageUrlInput(const QString &name, QLineEdit *input)
    {
        auto *wrap = new QWidget(this);
        auto *layout = new QVBoxLayout(wrap);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(6);
        layout->addWidget(classLabel(name, "muted"));

        auto *row = new QHBoxLayout();
        row->setContentsMargins(0, 0, 0, 0);
        row->setSpacing(6);
        input->setMinimumHeight(38);
        input->setToolTip("Use a bundled Qt resource path such as :/icons/meal-lunch.png.");
        auto *clear = new QPushButton("Clear", wrap);
        row->addWidget(input, 1);
        row->addWidget(clear);
        layout->addLayout(row);

        connect(clear, &QPushButton::clicked, this, [this, input] {
            input->clear();
            refreshMenuCards();
        });
        connect(input, &QLineEdit::editingFinished, this, [this] { refreshMenuCards(); });
        return wrap;
    }

    QComboBox *createDropdown(QWidget *parent, const QString &placeholder = {})
    {
        auto *combo = new QComboBox(parent);
        combo->setObjectName("polishedDropdown");
        combo->setProperty("polishedDropdown", true);
        combo->setMinimumHeight(40);
        combo->setMaxVisibleItems(12);
        combo->setMinimumContentsLength(16);
        combo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        combo->setView(new QListView(combo));
        if (!placeholder.isEmpty()) {
            combo->setPlaceholderText(placeholder);
        }
        return combo;
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
        pill->setMinimumWidth(34);
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
        rebuildVisibilityCacheIfNeeded();
        return m_cachedVisibleDormitories;
    }

    QVector<Student> visibleStudents() const
    {
        rebuildVisibilityCacheIfNeeded();
        return m_cachedVisibleStudents;
    }

    void invalidateVisibilityCache() const
    {
        m_visibilityCacheDirty = true;
    }

    void rebuildVisibilityCacheIfNeeded() const
    {
        if (!m_visibilityCacheDirty) {
            return;
        }

        m_cachedVisibleDormitories.clear();
        for (const Dormitory &dormitory : m_university.dormitories()) {
            if (currentAdminCanAccessDormitory(dormitory.id())) {
                m_cachedVisibleDormitories.append(dormitory);
            }
        }

        m_cachedVisibleStudents.clear();
        for (const Student &student : m_university.students()) {
            if (!student.isAssigned() || currentAdminCanAccessDormitory(student.dormitoryId().value())) {
                m_cachedVisibleStudents.append(student);
            }
        }
        m_visibilityCacheDirty = false;
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
        dialog.resize(500, 360);
        dialog.setMinimumSize(480, 340);

        auto *layout = new QVBoxLayout(&dialog);
        layout->setContentsMargins(20, 18, 20, 18);
        layout->setSpacing(12);
        layout->addWidget(classLabel("Add Student", "cardTitle"));

        auto *idInput = new QLineEdit(&dialog);
        idInput->setMinimumHeight(40);
        idInput->setPlaceholderText(nextStudentId());
        auto *nameInput = new QLineEdit(&dialog);
        nameInput->setMinimumHeight(40);
        nameInput->setPlaceholderText("Full name");
        auto *yearInput = new QSpinBox(&dialog);
        yearInput->setRange(1, 8);
        yearInput->setValue(1);
        yearInput->setMinimumHeight(40);
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
            showWarningMessage(this, "Rule feedback", "Select a student first.");
            return;
        }

        const Student &student = m_university.student(m_selectedStudentId);
        const QMessageBox::StandardButton answer = askQuestionMessage(
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
            const QString dormitoryId = m_profileAssignDormitoryInput->currentData().toString();
            if (!currentAdminCanAccessDormitory(dormitoryId)) {
                throw DomainError("This admin cannot assign students in that dormitory.");
            }
            m_university.assignStudentToRoom(student.id(), dormitoryId, m_profileAssignRoomInput->value());
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

            const QString breakfastText = m_breakfastInput->text().trimmed();
            const QString lunchText = m_lunchInput->text().trimmed();
            const QString dinnerText = m_dinnerInput->text().trimmed();
            const auto imageOrDefault = [](const QLineEdit *input, const QString &description, const QString &mealType) {
                const QString requestedImage = input == nullptr ? QString() : input->text().trimmed();
                return requestedImage.isEmpty() ? mealImageForDescription(description, mealType) : requestedImage;
            };

            m_university.setRestaurantMenu(
                m_menuDormitoryInput->currentData().toString(),
                m_menuDateInput->date(),
                {
                    breakfastText,
                    lunchText,
                    dinnerText,
                    imageOrDefault(m_breakfastImageUrlInput, breakfastText, QStringLiteral("breakfast")),
                    imageOrDefault(m_lunchImageUrlInput, lunchText, QStringLiteral("lunch")),
                    imageOrDefault(m_dinnerImageUrlInput, dinnerText, QStringLiteral("dinner")),
                });
            showStatus("Menu saved.");
        });
    }

    void recordAdditionalMealsServed()
    {
        runAction([this] {
            const QString dormitoryId = m_mealCounterDormitoryInput->currentData().toString();
            if (dormitoryId.isEmpty()) {
                throw DomainError("Choose a dormitory before recording meals.");
            }
            if (!currentAdminCanAccessDormitory(dormitoryId)) {
                throw DomainError("This admin cannot record meals for that dormitory.");
            }

            m_university.dormitory(dormitoryId).restaurant().recordMealServed(
                m_mealCounterDateInput->date(),
                m_mealCounterAmountInput->value());
            refreshMealCounter();
            showStatus("Meal service count updated.");
        });
    }

    void loadSelectedMenuIntoEditor()
    {
        if (m_loadingMenuEditor
            || m_menuDormitoryInput == nullptr
            || m_menuDateInput == nullptr
            || m_breakfastInput == nullptr
            || m_lunchInput == nullptr
            || m_dinnerInput == nullptr
            || m_breakfastImageUrlInput == nullptr
            || m_lunchImageUrlInput == nullptr
            || m_dinnerImageUrlInput == nullptr) {
            return;
        }

        m_loadingMenuEditor = true;
        const QString dormitoryId = m_menuDormitoryInput->currentData().toString();
        const auto clearFields = [this] {
            m_breakfastInput->clear();
            m_lunchInput->clear();
            m_dinnerInput->clear();
            m_breakfastImageUrlInput->clear();
            m_lunchImageUrlInput->clear();
            m_dinnerImageUrlInput->clear();
        };

        if (dormitoryId.isEmpty()) {
            clearFields();
        } else {
            const auto menu = m_university.dormitory(dormitoryId).restaurant().menuForDate(m_menuDateInput->date());
            if (menu.has_value()) {
                m_breakfastInput->setText(menu->breakfast);
                m_lunchInput->setText(menu->lunch);
                m_dinnerInput->setText(menu->dinner);
                m_breakfastImageUrlInput->setText(menu->breakfastImageUrl);
                m_lunchImageUrlInput->setText(menu->lunchImageUrl);
                m_dinnerImageUrlInput->setText(menu->dinnerImageUrl);
            } else {
                clearFields();
            }
        }

        m_loadingMenuEditor = false;
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

    void createAdminAccount()
    {
        runAction([this] {
            requireFullAccess();
            const QString username = m_newAdminUsernameInput->text().trimmed().toLower();
            const QString displayName = m_newAdminDisplayNameInput->text().trimmed();
            const QString password = m_newAdminPasswordInput->text();
            const bool fullAccess = m_newAdminScopeInput->currentData().toString() == "full";
            const QString neighborhoodId = m_newAdminNeighborhoodInput->currentData().toString();

            if (username.isEmpty() || displayName.isEmpty() || password.trimmed().isEmpty()) {
                throw DomainError("Username, display name, and password are required.");
            }
            if (username.contains(' ')) {
                throw DomainError("Admin username cannot contain spaces.");
            }
            if (m_adminProfiles.contains(username)) {
                throw DomainError("Admin username already exists.");
            }
            if (!fullAccess && findNeighborhood(neighborhoodId) == nullptr) {
                throw DomainError("Choose a starting neighborhood for scoped admins.");
            }

            QSet<QString> neighborhoodIds;
            if (!fullAccess) {
                neighborhoodIds.insert(neighborhoodId);
            }
            m_adminProfiles.insert(username, {username, password, displayName, fullAccess, neighborhoodIds});
            m_newAdminUsernameInput->clear();
            m_newAdminDisplayNameInput->clear();
            m_newAdminPasswordInput->clear();
            m_newAdminStatusLabel->setText("Created " + displayName + ".");
            showStatus("Admin account created.");
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
            invalidateVisibilityCache();
            saveAppState();
            refreshAll();
        } catch (const DomainError &error) {
            setStudentPanelMessage(error.what(), true);
            showWarningMessage(this, "Rule feedback", error.what());
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
        refreshMealCounter();
        refreshMenuCards();
        refreshNeighborhoods();
    }

    void refreshNeighborhoods()
    {
        if (m_neighborhoodTable == nullptr) {
            return;
        }

        const QString query = m_neighborhoodSearchInput == nullptr ? QString() : m_neighborhoodSearchInput->text().trimmed().toLower();
        const QString dormitoryFilter = m_neighborhoodDormitoryFilterInput == nullptr ? QStringLiteral("all") : m_neighborhoodDormitoryFilterInput->currentData().toString();
        const QString adminFilter = m_neighborhoodAdminFilterInput == nullptr ? QStringLiteral("all") : m_neighborhoodAdminFilterInput->currentData().toString();
        m_neighborhoodTable->setRowCount(0);
        for (const Neighborhood &neighborhood : m_neighborhoods) {
            if (!currentAdminCanAccessNeighborhood(neighborhood.id)) {
                continue;
            }
            if (dormitoryFilter != "all" && !neighborhood.dormitoryIds.contains(dormitoryFilter)) {
                continue;
            }
            if (adminFilter != "all") {
                bool adminHasAccess = false;
                for (const AdminProfile &profile : m_adminProfiles) {
                    if (profile.username == adminFilter
                        && (profile.fullAccess || profile.neighborhoodIds.contains(neighborhood.id))) {
                        adminHasAccess = true;
                        break;
                    }
                }
                if (!adminHasAccess) {
                    continue;
                }
            }

            QStringList adminNames;
            for (const AdminProfile &profile : m_adminProfiles) {
                if (profile.fullAccess || profile.neighborhoodIds.contains(neighborhood.id)) {
                    adminNames.append(profile.displayName);
                }
            }
            adminNames.sort();
            const QString searchable = neighborhood.id + " " + neighborhood.name + " "
                + neighborhood.dormitoryIds.join(" ") + " " + adminNames.join(" ");
            if (!query.isEmpty() && !searchable.toLower().contains(query)) {
                continue;
            }

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
        const QString selectedNewAdminNeighborhood = m_newAdminNeighborhoodInput == nullptr ? QString() : m_newAdminNeighborhoodInput->currentData().toString();
        const QString selectedFilterDormitory = m_neighborhoodDormitoryFilterInput == nullptr ? QString() : m_neighborhoodDormitoryFilterInput->currentData().toString();
        const QString selectedFilterAdmin = m_neighborhoodAdminFilterInput == nullptr ? QString() : m_neighborhoodAdminFilterInput->currentData().toString();

        m_copyNeighborhoodInput->clear();
        m_neighborhoodAccessInput->clear();
        if (m_newAdminNeighborhoodInput != nullptr) {
            m_newAdminNeighborhoodInput->clear();
        }
        for (const Neighborhood &neighborhood : m_neighborhoods) {
            if (currentAdminCanAccessNeighborhood(neighborhood.id)) {
                QString compactName = neighborhood.name;
                compactName.remove(" Neighborhood", Qt::CaseInsensitive);
                const QString labelText = neighborhood.id + " - " + compactName;
                const QString fullText = neighborhood.id + " - " + neighborhood.name;
                m_copyNeighborhoodInput->addItem(labelText, neighborhood.id);
                m_copyNeighborhoodInput->setItemData(m_copyNeighborhoodInput->count() - 1, fullText, Qt::ToolTipRole);
                m_neighborhoodAccessInput->addItem(labelText, neighborhood.id);
                m_neighborhoodAccessInput->setItemData(m_neighborhoodAccessInput->count() - 1, fullText, Qt::ToolTipRole);
                if (m_newAdminNeighborhoodInput != nullptr) {
                    m_newAdminNeighborhoodInput->addItem(labelText, neighborhood.id);
                    m_newAdminNeighborhoodInput->setItemData(m_newAdminNeighborhoodInput->count() - 1, fullText, Qt::ToolTipRole);
                }
            }
        }

        m_adminAccessInput->clear();
        for (const AdminProfile &profile : m_adminProfiles) {
            QString compactName = profile.displayName;
            compactName.replace("Global Administrator", "Global Admin", Qt::CaseInsensitive);
            compactName.replace("Campus Manager", "Manager", Qt::CaseInsensitive);
            const QString labelText = compactName;
            const QString fullText = profile.username + " - " + profile.displayName;
            m_adminAccessInput->addItem(labelText, profile.username);
            m_adminAccessInput->setItemData(m_adminAccessInput->count() - 1, fullText, Qt::ToolTipRole);
        }
        if (m_neighborhoodDormitoryFilterInput != nullptr) {
            QSignalBlocker blocker(m_neighborhoodDormitoryFilterInput);
            m_neighborhoodDormitoryFilterInput->clear();
            m_neighborhoodDormitoryFilterInput->addItem("Any dormitory", "all");
            for (const Dormitory &dormitory : visibleDormitories()) {
                m_neighborhoodDormitoryFilterInput->addItem(dormitory.id() + " - " + dormitory.name(), dormitory.id());
            }
            selectComboData(m_neighborhoodDormitoryFilterInput, selectedFilterDormitory);
        }
        if (m_neighborhoodAdminFilterInput != nullptr) {
            QSignalBlocker blocker(m_neighborhoodAdminFilterInput);
            m_neighborhoodAdminFilterInput->clear();
            m_neighborhoodAdminFilterInput->addItem("Any admin", "all");
            for (const AdminProfile &profile : m_adminProfiles) {
                m_neighborhoodAdminFilterInput->addItem(profile.displayName, profile.username);
            }
            selectComboData(m_neighborhoodAdminFilterInput, selectedFilterAdmin);
        }
        selectComboData(m_copyNeighborhoodInput, selectedCopy);
        selectComboData(m_adminAccessInput, selectedAdmin);
        selectComboData(m_neighborhoodAccessInput, selectedAccessNeighborhood);
        if (m_newAdminNeighborhoodInput != nullptr) {
            selectComboData(m_newAdminNeighborhoodInput, selectedNewAdminNeighborhood);
        }

        const bool canManage = currentAdminHasFullAccess();
        m_neighborhoodIdInput->setEnabled(canManage);
        m_neighborhoodNameInput->setEnabled(canManage);
        m_copyNeighborhoodInput->setEnabled(canManage);
        m_adminAccessInput->setEnabled(canManage);
        m_neighborhoodAccessInput->setEnabled(canManage);
        if (m_newAdminUsernameInput != nullptr) {
            m_newAdminUsernameInput->setEnabled(canManage);
            m_newAdminDisplayNameInput->setEnabled(canManage);
            m_newAdminPasswordInput->setEnabled(canManage);
            m_newAdminScopeInput->setEnabled(canManage);
            refreshNewAdminScopeControls();
        }
    }

    void refreshNewAdminScopeControls()
    {
        if (m_newAdminScopeInput == nullptr || m_newAdminNeighborhoodInput == nullptr) {
            return;
        }
        const bool canManage = currentAdminHasFullAccess();
        const bool scoped = m_newAdminScopeInput->currentData().toString() != "full";
        m_newAdminNeighborhoodInput->setEnabled(canManage && scoped);
    }

    void refreshTodayMenuSummary()
    {
        clearLayout(m_todayMenuSummary);
        const QDate today = QDate::currentDate();
        for (const Dormitory &dormitory : visibleDormitories()) {
            const auto menu = dormitory.restaurant().menuForDate(today);
            auto *item = new QFrame(this);
            item->setObjectName("todayMenuItem");
            item->setMinimumHeight(96);
            auto *layout = new QVBoxLayout(item);
            layout->setContentsMargins(12, 9, 12, 9);
            layout->setSpacing(3);
            layout->addWidget(classLabel(dormitory.name(), "cardTitle"));
            layout->addWidget(classLabel("Meals served today: " + QString::number(dormitory.restaurant().mealsServedOn(today)), "muted"));
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
        const QString query = m_roomSearchInput == nullptr ? QString() : m_roomSearchInput->text().trimmed().toLower();
        const QString dormitoryFilter = m_roomDormitoryFilterInput == nullptr ? QStringLiteral("all") : m_roomDormitoryFilterInput->currentData().toString();
        const QString occupancyFilter = m_roomOccupancyFilterInput == nullptr ? QStringLiteral("all") : m_roomOccupancyFilterInput->currentData().toString();
        for (const Dormitory &dormitory : visibleDormitories()) {
            if (dormitoryFilter != "all" && dormitory.id() != dormitoryFilter) {
                continue;
            }
            for (const Room &room : dormitory.rooms()) {
                const QString residentText = room.studentIds().isEmpty() ? "-" : room.studentIds().join(", ");
                const bool full = room.isFull();
                const bool occupied = room.occupancy() > 0;
                const QString stateText = full ? QStringLiteral("Full") : QStringLiteral("Available");
                if (occupancyFilter == "available" && full) {
                    continue;
                }
                if (occupancyFilter == "occupied" && !occupied) {
                    continue;
                }
                if (occupancyFilter == "full" && !full) {
                    continue;
                }
                const QString searchable = dormitory.name() + " " + dormitory.id() + " "
                    + QString::number(room.number()) + " " + residentText + " " + stateText;
                if (!query.isEmpty() && !searchable.toLower().contains(query)) {
                    continue;
                }
                const int row = m_roomMatrix->rowCount();
                m_roomMatrix->insertRow(row);
                setCell(m_roomMatrix, row, 0, dormitory.name(), "#13231D");
                setCell(m_roomMatrix, row, 1, QString::number(room.number()), "#13231D");
                setCell(m_roomMatrix, row, 2, QString("%1 / %2").arg(room.occupancy()).arg(room.capacity()), "#667085");
                setCell(m_roomMatrix, row, 3, residentText, "#13231D");
                setStatusCell(m_roomMatrix, row, 4, stateText, full ? "#FDECEC" : "#E6F4EE", full ? "#B42318" : "#1D7A57");
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
        const QString selectedProfileDormitory = m_profileAssignDormitoryInput == nullptr ? QString() : m_profileAssignDormitoryInput->currentData().toString();
        const QString selectedMenuDormitory = m_menuDormitoryInput == nullptr ? QString() : m_menuDormitoryInput->currentData().toString();
        const QString selectedMealCounterDormitory = m_mealCounterDormitoryInput == nullptr ? QString() : m_mealCounterDormitoryInput->currentData().toString();
        const QString selectedRoomFilterDormitory = m_roomDormitoryFilterInput == nullptr ? QString() : m_roomDormitoryFilterInput->currentData().toString();
        const QString selectedMenuFilterDormitory = m_menuDormitoryFilterInput == nullptr ? QString() : m_menuDormitoryFilterInput->currentData().toString();

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
        if (m_profileAssignDormitoryInput != nullptr) {
            m_profileAssignDormitoryInput->clear();
        }
        if (m_menuDormitoryInput != nullptr) {
            m_menuDormitoryInput->clear();
        }
        if (m_mealCounterDormitoryInput != nullptr) {
            m_mealCounterDormitoryInput->clear();
        }
        if (m_roomDormitoryFilterInput != nullptr) {
            QSignalBlocker blocker(m_roomDormitoryFilterInput);
            m_roomDormitoryFilterInput->clear();
            m_roomDormitoryFilterInput->addItem("Any dormitory", "all");
        }
        if (m_menuDormitoryFilterInput != nullptr) {
            QSignalBlocker blocker(m_menuDormitoryFilterInput);
            m_menuDormitoryFilterInput->clear();
            m_menuDormitoryFilterInput->addItem("Any dormitory", "all");
        }
        for (const Dormitory &dormitory : visibleDormitories()) {
            const QString labelText = dormitory.id() + " - " + dormitory.name();
            if (m_assignDormitoryInput != nullptr) {
                m_assignDormitoryInput->addItem(labelText, dormitory.id());
            }
            if (m_profileAssignDormitoryInput != nullptr) {
                m_profileAssignDormitoryInput->addItem(labelText, dormitory.id());
            }
            if (m_menuDormitoryInput != nullptr) {
                m_menuDormitoryInput->addItem(labelText, dormitory.id());
            }
            if (m_mealCounterDormitoryInput != nullptr) {
                m_mealCounterDormitoryInput->addItem(labelText, dormitory.id());
            }
            if (m_roomDormitoryFilterInput != nullptr) {
                m_roomDormitoryFilterInput->addItem(labelText, dormitory.id());
            }
            if (m_menuDormitoryFilterInput != nullptr) {
                m_menuDormitoryFilterInput->addItem(labelText, dormitory.id());
            }
        }
        if (m_assignDormitoryInput != nullptr) {
            selectComboData(m_assignDormitoryInput, selectedDormitory);
        }
        if (m_profileAssignDormitoryInput != nullptr) {
            selectComboData(m_profileAssignDormitoryInput, selectedProfileDormitory);
        }
        if (m_menuDormitoryInput != nullptr) {
            selectComboData(m_menuDormitoryInput, selectedMenuDormitory);
        }
        if (m_mealCounterDormitoryInput != nullptr) {
            selectComboData(m_mealCounterDormitoryInput, selectedMealCounterDormitory);
        }
        if (m_roomDormitoryFilterInput != nullptr) {
            QSignalBlocker blocker(m_roomDormitoryFilterInput);
            selectComboData(m_roomDormitoryFilterInput, selectedRoomFilterDormitory);
        }
        if (m_menuDormitoryFilterInput != nullptr) {
            QSignalBlocker blocker(m_menuDormitoryFilterInput);
            selectComboData(m_menuDormitoryFilterInput, selectedMenuFilterDormitory);
        }
        updateSuggestedRoomNumber();
        loadSelectedMenuIntoEditor();
    }

    void updateSuggestedRoomNumber()
    {
        setSuggestedRoomNumber(m_assignDormitoryInput, m_assignRoomInput);
        setSuggestedRoomNumber(m_profileAssignDormitoryInput, m_profileAssignRoomInput);
    }

    void setSuggestedRoomNumber(QComboBox *dormitoryInput, QSpinBox *roomInput)
    {
        if (dormitoryInput == nullptr || roomInput == nullptr) {
            return;
        }
        const QString dormitoryId = dormitoryInput->currentData().toString();
        if (dormitoryId.isEmpty()) {
            return;
        }
        const QVector<Room> rooms = m_university.dormitory(dormitoryId).availableRooms();
        if (!rooms.isEmpty()) {
            roomInput->setValue(rooms.first().number());
        }
    }

    void refreshMealCounter()
    {
        if (m_mealCounterDormitoryInput == nullptr
            || m_mealCounterDateInput == nullptr
            || m_mealCounterValueLabel == nullptr) {
            return;
        }

        const QString dormitoryId = m_mealCounterDormitoryInput->currentData().toString();
        if (dormitoryId.isEmpty() || !m_university.hasDormitory(dormitoryId)) {
            m_mealCounterValueLabel->setText("Meals served: 0");
            return;
        }

        const int count = m_university.dormitory(dormitoryId).restaurant().mealsServedOn(m_mealCounterDateInput->date());
        m_mealCounterValueLabel->setText("Meals served: " + QString::number(count));
    }

    void refreshMenuCards()
    {
        if (m_menuCards == nullptr || m_menuDateInput == nullptr) {
            return;
        }
        clearLayout(m_menuCards);
        const QDate date = m_menuDateInput->date();
        const QString query = m_menuSearchInput == nullptr ? QString() : m_menuSearchInput->text().trimmed().toLower();
        const QString dormitoryFilter = m_menuDormitoryFilterInput == nullptr ? QStringLiteral("all") : m_menuDormitoryFilterInput->currentData().toString();
        const QString mealFilter = m_menuMealFilterInput == nullptr ? QStringLiteral("all") : m_menuMealFilterInput->currentData().toString();
        int visibleCount = 0;

        for (const Dormitory &dormitory : visibleDormitories()) {
            if (dormitoryFilter != "all" && dormitory.id() != dormitoryFilter) {
                continue;
            }
            const auto menu = dormitory.restaurant().menuForDate(date);
            if (menu.has_value()) {
                const QString searchable = dormitory.name() + " " + dormitory.id() + " "
                    + dormitory.restaurant().name() + " "
                    + menu->breakfast + " " + menu->lunch + " " + menu->dinner;
                if (!query.isEmpty() && !searchable.toLower().contains(query)) {
                    continue;
                }
                if (mealFilter == "breakfast" && !query.isEmpty() && !menu->breakfast.toLower().contains(query)) {
                    continue;
                }
                if (mealFilter == "lunch" && !query.isEmpty() && !menu->lunch.toLower().contains(query)) {
                    continue;
                }
                if (mealFilter == "dinner" && !query.isEmpty() && !menu->dinner.toLower().contains(query)) {
                    continue;
                }
            } else if (!query.isEmpty()) {
                continue;
            }

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
            layout->addWidget(statusPill("Meals served: " + QString::number(dormitory.restaurant().mealsServedOn(date)), "#EEF5F4", "#315B4B"));

            if (menu.has_value()) {
                if (mealFilter == "all" || mealFilter == "breakfast") {
                    layout->addWidget(mealRow("Breakfast", menu->breakfast, menu->breakfastImageUrl));
                }
                if (mealFilter == "all" || mealFilter == "lunch") {
                    layout->addWidget(mealRow("Lunch", menu->lunch, menu->lunchImageUrl));
                }
                if (mealFilter == "all" || mealFilter == "dinner") {
                    layout->addWidget(mealRow("Dinner", menu->dinner, menu->dinnerImageUrl));
                }
            } else {
                layout->addWidget(statusPill("No menu for selected date", "#FFF7E6", "#B7791F"));
            }
            m_menuCards->addWidget(item);
            ++visibleCount;
        }
        if (visibleCount == 0) {
            m_menuCards->addWidget(statusPill("No menu cards match the current filters", "#FFF7E6", "#B7791F"));
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

        const QMessageBox::StandardButton answer = askQuestionMessage(
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
            showInformationMessage(this, "Open profile", "Select a student first.");
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
        refreshCombos();

        connect(m_studentProfileDialog, &QDialog::finished, this, [this] {
            m_studentProfileDialog = nullptr;
            m_profileAssignDormitoryInput = nullptr;
            m_profileAssignRoomInput = nullptr;
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
        if (m_profileAssignDormitoryInput != nullptr) {
            m_profileAssignDormitoryInput->setEnabled(enabled);
        }
        if (m_profileAssignRoomInput != nullptr) {
            m_profileAssignRoomInput->setEnabled(enabled);
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

    QWidget *mealThumbnail(const QString &imageUrl, const QSize &thumbnailSize = QSize(72, 54), const QString &objectName = QStringLiteral("mealThumbnail"))
    {
        auto *thumbnail = new QLabel(this);
        thumbnail->setObjectName(objectName);
        thumbnail->setFixedSize(thumbnailSize);
        thumbnail->setAlignment(Qt::AlignCenter);

        const QString urlText = imageUrl.trimmed();
        if (urlText.isEmpty()) {
            thumbnail->setText("Photo");
            return thumbnail;
        }

        QPixmap pixmap(urlText);
        if (pixmap.isNull()) {
            thumbnail->setText("Image");
            thumbnail->setToolTip("Use a Qt resource path such as :/icons/meal-lunch.png.");
            return thumbnail;
        }

        thumbnail->setPixmap(pixmap.scaled(thumbnail->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        thumbnail->setToolTip(urlText);
        return thumbnail;
    }

    QWidget *mealRow(const QString &meal, const QString &description, const QString &imageUrl = {})
    {
        auto *row = new QFrame(this);
        row->setObjectName("mealRow");
        auto *layout = new QHBoxLayout(row);
        layout->setContentsMargins(10, 7, 10, 7);
        layout->setSpacing(9);
        const QString resolvedImageUrl = imageUrl.trimmed().isEmpty()
            ? mealImageForDescription(description, meal.toLower())
            : imageUrl;
        layout->addWidget(mealThumbnail(resolvedImageUrl), 0, Qt::AlignTop);
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
            m_statusLabel->setVisible(!message.trimmed().isEmpty());
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

namespace {

struct StartupLoadingHandle {
    QWidget *widget = nullptr;
    QElapsedTimer timer;
};

QString argumentValue(const QStringList &arguments, const QString &flag, const QString &fallback = {})
{
    const int index = arguments.indexOf(flag);
    return index >= 0 ? arguments.value(index + 1, fallback) : fallback;
}

QString temporaryDataFilePath(QTemporaryDir &tempDir)
{
    return tempDir.isValid() ? QDir(tempDir.path()).filePath("udrms-data.json") : QString();
}

int runSelfTestWithTempData(const std::function<bool(const QString &)> &test)
{
    // GUI self-tests use temporary data so they never modify the user's saved
    // dormitory file.
    QTemporaryDir tempDir;
    const QString testDataPath = temporaryDataFilePath(tempDir);
    if (testDataPath.isEmpty()) {
        return 1;
    }
    return test(testDataPath) ? 0 : 1;
}

bool runRequestedSelfTest(const QStringList &arguments, int &exitCode)
{
    // These command-line switches are used by CTest. Normal users do not see
    // them; they keep GUI behavior testable without manual clicking.
    if (arguments.contains("--loading-screen-self-test")) {
        exitCode = startupLoadingScreenSelfTest() ? 0 : 1;
        return true;
    }
    if (arguments.contains("--message-box-style-self-test")) {
        exitCode = messageBoxStyleSelfTest() ? 0 : 1;
        return true;
    }
    if (arguments.contains("--persistence-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow first(path);
            first.addPersistenceProbeForTest();
            DormitoryWindow second(path);
            return second.hasStudentForTest("S9999");
        });
        return true;
    }
    if (arguments.contains("--seed-data-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow seeded(path);
            return seeded.hasRichExampleSeedDataForTest();
        });
        return true;
    }
    if (arguments.contains("--dropdown-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow window(path);
            window.loginAsAdminForTest("admin");
            window.showPageForTest(1);
            window.openFirstStudentProfileForTest();
            return window.dropdownsHealthyForTest();
        });
        return true;
    }
    if (arguments.contains("--credential-routing-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow window(path);
            return window.credentialRoutingHealthyForTest();
        });
        return true;
    }
    if (arguments.contains("--admin-permissions-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow window(path);
            return window.adminPermissionBoundariesHealthyForTest();
        });
        return true;
    }
    if (arguments.contains("--admin-account-creator-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow window(path);
            return window.adminAccountCreatorHealthyForTest();
        });
        return true;
    }
    if (arguments.contains("--ui-hardening-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow window(path);
            return window.uiHardeningHealthyForTest();
        });
        return true;
    }
    if (arguments.contains("--restaurant-access-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow window(path);
            return window.restaurantAccessRulesHealthyForTest();
        });
        return true;
    }
    if (arguments.contains("--restaurant-meal-counter-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow window(path);
            return window.restaurantMealCounterHealthyForTest();
        });
        return true;
    }
    if (arguments.contains("--list-filters-self-test")) {
        exitCode = runSelfTestWithTempData([](const QString &path) {
            DormitoryWindow window(path);
            return window.listFiltersAndMealImagesHealthyForTest();
        });
        return true;
    }
    return false;
}

StartupLoadingHandle showStartupLoadingScreen(QApplication &app)
{
    // The loading screen is shown only during normal launches, not during
    // screenshots or automated tests.
    StartupLoadingHandle loading;
    loading.widget = buildStartupLoadingScreen();
    centerOnPrimaryScreen(loading.widget);
    loading.timer.start();
    loading.widget->show();
    app.processEvents();
    return loading;
}

void closeStartupLoadingScreen(StartupLoadingHandle &loading)
{
    if (loading.widget == nullptr) {
        return;
    }

    const int remainingMs = kStartupLoadingMinimumMs - static_cast<int>(loading.timer.elapsed());
    if (remainingMs > 0) {
        QEventLoop handoffLoop;
        QTimer::singleShot(remainingMs, &handoffLoop, &QEventLoop::quit);
        handoffLoop.exec();
    }

    loading.widget->close();
    loading.widget->deleteLater();
    loading.widget = nullptr;
}

void applyAutomationArguments(DormitoryWindow &window, QApplication &app, const QStringList &arguments)
{
    // Test and screenshot helpers can pre-login or open a page directly. This
    // keeps verification scripts simple while leaving normal startup unchanged.
    const int adminLoginIndex = arguments.indexOf("--login-admin");
    if (adminLoginIndex >= 0) {
        window.loginAsAdminForTest(arguments.value(adminLoginIndex + 1, "admin"));
    }

    const int studentLoginIndex = arguments.indexOf("--login-student");
    if (studentLoginIndex >= 0) {
        window.loginAsStudentForTest(arguments.value(studentLoginIndex + 1, "S1001").toUpper());
    }

    const int pageIndex = arguments.indexOf("--page");
    if (pageIndex >= 0) {
        window.showPageForTest(arguments.value(pageIndex + 1, "0").toInt());
    }

    if (arguments.contains("--collapse-sidebar")) {
        window.collapseSidebarForTest();
    }
    if (arguments.contains("--fullscreen")) {
        window.showFullScreen();
        app.processEvents();
    }
}

int saveScreenshotAndExit(DormitoryWindow &window, QApplication &app, const QString &path)
{
    window.show();
    app.processEvents();

    // Wait briefly so fade-in animations and deferred layout work finish before
    // grabbing the widget image.
    QEventLoop screenshotWaitLoop;
    QTimer::singleShot(1800, &screenshotWaitLoop, &QEventLoop::quit);
    screenshotWaitLoop.exec();

    app.processEvents();
    return window.grab().save(path) ? 0 : 1;
}

} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qApp->setFont(QFont("Times New Roman", 10));
    qApp->setStyleSheet(kAppStyle);

    const QStringList arguments = QCoreApplication::arguments();
    int selfTestExitCode = 0;
    if (runRequestedSelfTest(arguments, selfTestExitCode)) {
        return selfTestExitCode;
    }

    const bool smokeTest = arguments.contains("--smoke-test");
    const int screenshotIndex = arguments.indexOf("--screenshot");
    const bool screenshotMode = screenshotIndex >= 0;
    const QString screenshotPath = screenshotMode
        ? arguments.value(screenshotIndex + 1, "udrms-gui.png")
        : QString();

    StartupLoadingHandle startupLoading;
    if (!smokeTest && !screenshotMode) {
        startupLoading = showStartupLoadingScreen(app);
    }

    try {
        DormitoryWindow window(argumentValue(arguments, "--data-file"));
        applyAutomationArguments(window, app, arguments);

        if (smokeTest) {
            return 0;
        }
        if (screenshotMode) {
            return saveScreenshotAndExit(window, app, screenshotPath);
        }

        closeStartupLoadingScreen(startupLoading);
        window.showMaximized();
        return app.exec();
    } catch (const DomainError &error) {
        closeStartupLoadingScreen(startupLoading);
        showWarningMessage(nullptr, "Startup error", error.what());
        return 1;
    } catch (const std::exception &error) {
        closeStartupLoadingScreen(startupLoading);
        showWarningMessage(nullptr, "Unexpected startup error", error.what());
        return 1;
    }
}
