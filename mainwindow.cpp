#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages/FanModePage.h"
#include "pages/HomePage.h"
#include "pages/LessonMenuPage.h"
#include "pages/LessonViewerPage.h"
#include "pages/ModeSelectionPage.h"
#include "pages/PlayerModePage.h"

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pageStack(nullptr)
    , m_homePage(nullptr)
    , m_modeSelectionPage(nullptr)
    , m_fanModePage(nullptr)
    , m_playerModePage(nullptr)
    , m_lessonMenuPage(nullptr)
    , m_lessonViewerPage(nullptr)
    , m_currentMode(LessonMode::General)
    , m_currentLessonId(-1)
{
    ui->setupUi(this);

    setWindowTitle("HaramBall FC - World Cup Learning App");
    resize(1000, 700);

    buildPageStack();
    connectNavigation();
    showHomePage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showHomePage()
{
    setCurrentPage(PageId::Home);
}

void MainWindow::showModeSelectionPage()
{
    setCurrentPage(PageId::ModeSelection);
}

void MainWindow::showFanModePage()
{
    setCurrentPage(PageId::FanMode);
}

void MainWindow::showPlayerModePage()
{
    setCurrentPage(PageId::PlayerMode);
}

void MainWindow::showFanLessons()
{
    loadLessonsForMode(LessonMode::Fan);
    setCurrentPage(PageId::LessonMenu);
}

void MainWindow::showPlayerLessons()
{
    loadLessonsForMode(LessonMode::Player);
    setCurrentPage(PageId::LessonMenu);
}

void MainWindow::openLessonById(int lessonId)
{
    Lesson lesson = m_lessonRepository.lessonById(lessonId);
    m_currentLessonId = lessonId;
    m_lessonViewerPage->setLesson(lesson);
    setCurrentPage(PageId::LessonViewer);
}

void MainWindow::returnToLessonMenu()
{
    setCurrentPage(PageId::LessonMenu);
}

void MainWindow::buildPageStack()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    m_pageStack = new QStackedWidget(central);

    m_homePage = new HomePage(m_pageStack);
    m_modeSelectionPage = new ModeSelectionPage(m_pageStack);
    m_fanModePage = new FanModePage(m_pageStack);
    m_playerModePage = new PlayerModePage(m_pageStack);
    m_lessonMenuPage = new LessonMenuPage(m_pageStack);
    m_lessonViewerPage = new LessonViewerPage(m_pageStack);

    m_pageStack->addWidget(m_homePage);
    m_pageStack->addWidget(m_modeSelectionPage);
    m_pageStack->addWidget(m_fanModePage);
    m_pageStack->addWidget(m_playerModePage);
    m_pageStack->addWidget(m_lessonMenuPage);
    m_pageStack->addWidget(m_lessonViewerPage);

    layout->addWidget(m_pageStack);
    layout->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(central);
}

void MainWindow::connectNavigation()
{
    connect(m_homePage, &HomePage::startClicked,
            this, &MainWindow::showModeSelectionPage);

    connect(m_modeSelectionPage, &ModeSelectionPage::fanModeSelected,
            this, &MainWindow::showFanModePage);
    connect(m_modeSelectionPage, &ModeSelectionPage::playerModeSelected,
            this, &MainWindow::showPlayerModePage);
    connect(m_modeSelectionPage, &ModeSelectionPage::backRequested,
            this, &MainWindow::showHomePage);

    connect(m_fanModePage, &FanModePage::openLessonsRequested,
            this, &MainWindow::showFanLessons);
    connect(m_fanModePage, &FanModePage::backRequested,
            this, &MainWindow::showModeSelectionPage);

    connect(m_playerModePage, &PlayerModePage::openLessonsRequested,
            this, &MainWindow::showPlayerLessons);
    connect(m_playerModePage, &PlayerModePage::backRequested,
            this, &MainWindow::showModeSelectionPage);

    connect(m_lessonMenuPage, &LessonMenuPage::lessonSelected,
            this, &MainWindow::openLessonById);

    connect(m_lessonMenuPage, &LessonMenuPage::backRequested, [this]() {
        if (m_currentMode == LessonMode::Fan) {
            showFanModePage();
        } else if (m_currentMode == LessonMode::Player) {
            showPlayerModePage();
        } else {
            showModeSelectionPage();
        }
    });

    connect(m_lessonMenuPage, &LessonMenuPage::homeRequested,
            this, &MainWindow::showHomePage);

    connect(m_lessonViewerPage, &LessonViewerPage::backRequested,
            this, &MainWindow::returnToLessonMenu);
    connect(m_lessonViewerPage, &LessonViewerPage::homeRequested,
            this, &MainWindow::showHomePage);
}

void MainWindow::setCurrentPage(PageId pageId)
{
    m_pageStack->setCurrentIndex(static_cast<int>(pageId));
}

void MainWindow::loadLessonsForMode(LessonMode mode)
{
    m_currentMode = mode;

    QVector<Lesson> lessons = m_lessonRepository.lessonsForMode(mode);

    if (mode == LessonMode::Fan) {
        m_lessonMenuPage->setPageTitle("Fan Mode Lessons");
    } else if (mode == LessonMode::Player) {
        m_lessonMenuPage->setPageTitle("Player Mode Lessons");
    } else {
        m_lessonMenuPage->setPageTitle("Lessons");
    }

    m_lessonMenuPage->setLessons(lessons);
}
