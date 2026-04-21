#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages/FanModePage.h"
#include "pages/HomePage.h"
#include "pages/LessonMenuPage.h"
#include "pages/LessonViewerPage.h"
#include "pages/ModeSelectionPage.h"
#include "pages/PlayerModePage.h"
#include "pages/BracketPage.h"
#include "pages/PenaltyGame.h"
#include "pages/QuizPage.h"
#include "pages/JugglingGame.h"
#include "models/QuizRepository.h"

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
    , m_bracketPage(nullptr)
    , m_quizPage(nullptr)
    , m_currentMode(LessonMode::General)
    , m_currentLessonId(-1)
    , m_penaltyGamePage(nullptr)
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

void MainWindow::showBracketPage()
{
    setCurrentPage(PageId::Bracket);
}

void MainWindow::openLessonById(int lessonId)
{
    if (m_currentMode == LessonMode::Fan && lessonId == 5) {
        showQuizPage(lessonId);
        return;
    }

    if (m_currentMode == LessonMode::Fan && lessonId == 6) {
        m_pageStack->setCurrentWidget(m_jugglingGame);
        return;
    }

    Lesson lesson = m_lessonRepository.lessonById(lessonId);
    m_currentLessonId = lessonId;
    m_lessonViewerPage->setLesson(lesson, m_currentMode);
    setCurrentPage(PageId::LessonViewer);
}

void MainWindow::returnToLessonMenu()
{
    setCurrentPage(PageId::LessonMenu);
}

void MainWindow::showQuizPage(int lessonId)
{
    m_quizPage->startQuiz(lessonId);
    m_pageStack->setCurrentWidget(m_quizPage);
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
    m_bracketPage = new BracketPage(m_pageStack);

    QuizRepository quizRepository;
    QStringList facts = quizRepository.jugglingFacts();

    m_jugglingGame = new JugglingGame(facts, m_pageStack);

    m_pageStack->addWidget(m_homePage);          // 0 = Home
    m_pageStack->addWidget(m_modeSelectionPage); // 1 = ModeSelection
    m_pageStack->addWidget(m_fanModePage);        // 2 = FanMode
    m_pageStack->addWidget(m_playerModePage);     // 3 = PlayerMode
    m_pageStack->addWidget(m_lessonMenuPage);     // 4 = LessonMenu
    m_pageStack->addWidget(m_lessonViewerPage);   // 5 = LessonViewer
    m_pageStack->addWidget(m_bracketPage);
    m_pageStack->addWidget(m_jugglingGame);

    m_penaltyGamePage = new PenaltyGamePage(this);
    m_pageStack->addWidget(m_penaltyGamePage);   // 7 (uses setCurrentWidget)

    m_quizPage = new QuizPage(m_pageStack);
    m_pageStack->addWidget(m_quizPage);          // 8 = Quiz



    layout->addWidget(m_pageStack);
    layout->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(central);
}

void MainWindow::connectNavigation()
{
    //Connections for Home Page according to UI
    connect(m_homePage, &HomePage::startClicked,
            this, &MainWindow::showModeSelectionPage);

    //Connections for Mode Selection (2nd) Page according to UI
    connect(m_modeSelectionPage, &ModeSelectionPage::fanModeSelected,
            this, &MainWindow::showFanModePage);
    connect(m_modeSelectionPage, &ModeSelectionPage::playerModeSelected,
            this, &MainWindow::showPlayerModePage);
    connect(m_modeSelectionPage, &ModeSelectionPage::bracketSelected,
            this, &MainWindow::showBracketPage);
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

    connect(m_playerModePage, &PlayerModePage::playGameClicked,
            this, [this]() {
                m_pageStack->setCurrentWidget(m_penaltyGamePage);
            });

    connect(m_penaltyGamePage, &PenaltyGamePage::backClicked,
            this, [this]() {
                m_pageStack->setCurrentWidget(m_playerModePage);
            });

    connect(m_lessonMenuPage, &LessonMenuPage::lessonSelected,
            this, &MainWindow::openLessonById);

    connect(m_lessonMenuPage, &LessonMenuPage::backRequested,
            this, [this]() {
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
    connect(m_lessonViewerPage, &LessonViewerPage::quizRequested,
            this, &MainWindow::showQuizPage);

    connect(m_quizPage, &QuizPage::backRequested,
            this, &MainWindow::returnToLessonMenu);
    connect(m_quizPage, &QuizPage::homeRequested,
            this, &MainWindow::showHomePage);
    connect(m_jugglingGame, &JugglingGame::backRequested,
            this, &MainWindow::returnToLessonMenu);

    // Bracket placeholder
    connect(m_bracketPage, &BracketPage::backRequested,
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
