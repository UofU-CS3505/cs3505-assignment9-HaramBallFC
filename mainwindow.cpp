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

#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QSlider>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "SoundManager.h"

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
    , m_volumeWidget(nullptr)
    , m_volIcon(nullptr)
    , m_volumeSlider(nullptr)
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

void MainWindow::showHomePage()        { setCurrentPage(PageId::Home); }
void MainWindow::showModeSelectionPage(){ setCurrentPage(PageId::ModeSelection); }
void MainWindow::showFanModePage()     { setCurrentPage(PageId::FanMode); }
void MainWindow::showPlayerModePage()  { setCurrentPage(PageId::PlayerMode); }
void MainWindow::showBracketPage()     { setCurrentPage(PageId::Bracket); }

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

void MainWindow::returnToLessonMenu() { setCurrentPage(PageId::LessonMenu); }

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

    m_homePage          = new HomePage(m_pageStack);
    m_modeSelectionPage = new ModeSelectionPage(m_pageStack);
    m_fanModePage       = new FanModePage(m_pageStack);
    m_playerModePage    = new PlayerModePage(m_pageStack);
    m_lessonMenuPage    = new LessonMenuPage(m_pageStack);
    m_lessonViewerPage  = new LessonViewerPage(m_pageStack);
    m_bracketPage       = new BracketPage(m_pageStack);

    QuizRepository quizRepository;
    QStringList facts = quizRepository.jugglingFacts();
    m_jugglingGame = new JugglingGame(facts, m_pageStack);

    m_pageStack->addWidget(m_homePage);
    m_pageStack->addWidget(m_modeSelectionPage);
    m_pageStack->addWidget(m_fanModePage);
    m_pageStack->addWidget(m_playerModePage);
    m_pageStack->addWidget(m_lessonMenuPage);
    m_pageStack->addWidget(m_lessonViewerPage);
    m_pageStack->addWidget(m_bracketPage);
    m_pageStack->addWidget(m_jugglingGame);

    m_penaltyGamePage = new PenaltyGamePage(this);
    m_pageStack->addWidget(m_penaltyGamePage);

    m_quizPage = new QuizPage(m_pageStack);
    m_pageStack->addWidget(m_quizPage);

    layout->addWidget(m_pageStack);
    layout->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(central);

    // Floating volume slider — bottom-right corner, overlays all pages
    m_volumeWidget = new QWidget(central);
    m_volumeWidget->setAttribute(Qt::WA_StyledBackground, true);
    m_volumeWidget->setFixedSize(190, 40);
    m_volumeWidget->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(15,30,53,220);"
        "  border-top: 3px solid #2E4E7A; border-left: 3px solid #2E4E7A;"
        "  border-bottom: 3px solid #060D1A; border-right: 3px solid #060D1A;"
        "}"
    );

    QHBoxLayout *volLayout = new QHBoxLayout(m_volumeWidget);
    volLayout->setContentsMargins(10, 8, 10, 8);
    volLayout->setSpacing(8);

    m_volIcon = new QLabel("\U0001F50A", m_volumeWidget);
    m_volIcon->setStyleSheet(
        "QLabel { font-size: 16px; background: transparent; border: none;"
        "font-family: 'Apple Color Emoji', 'Segoe UI Emoji', sans-serif; }"
    );
    m_volIcon->setFixedWidth(22);
    m_volIcon->setAlignment(Qt::AlignCenter);

    m_volumeSlider = new QSlider(Qt::Horizontal, m_volumeWidget);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(35);
    m_volumeSlider->setToolTip("Music volume");
    m_volumeSlider->setStyleSheet(
        "QSlider { background: transparent; border: none; }"
        "QSlider::groove:horizontal {"
        "  height: 4px; background: #0A1628; border: 1px solid #060D1A;"
        "}"
        "QSlider::sub-page:horizontal {"
        "  background: #D4A843; border: 1px solid #7A6010;"
        "}"
        "QSlider::handle:horizontal {"
        "  width: 10px; height: 18px;"
        "  margin-top: -8px; margin-bottom: -8px;"
        "  background: #D4A843;"
        "  border-top: 2px solid #F0C860; border-left: 2px solid #F0C860;"
        "  border-bottom: 2px solid #7A6010; border-right: 2px solid #7A6010;"
        "}"
        "QSlider::handle:horizontal:hover { background: #F0C860; }"
    );

    volLayout->addWidget(m_volIcon);
    volLayout->addWidget(m_volumeSlider, 1);

    m_volumeWidget->raise();
    repositionVolumeWidget();

    connect(m_volumeSlider, &QSlider::valueChanged, this, [this](int value) {
        SoundManager::instance().setBgmVolume(value / 100.0f);
        m_volIcon->setText(value == 0 ? "\U0001F507" : "\U0001F50A");
    });

    // Start background music
    SoundManager::instance().startBgm();
}

void MainWindow::connectNavigation()
{
    connect(m_homePage, &HomePage::startClicked, this, &MainWindow::showModeSelectionPage);

    connect(m_modeSelectionPage, &ModeSelectionPage::fanModeSelected,    this, &MainWindow::showFanModePage);
    connect(m_modeSelectionPage, &ModeSelectionPage::playerModeSelected, this, &MainWindow::showPlayerModePage);
    connect(m_modeSelectionPage, &ModeSelectionPage::bracketSelected,    this, &MainWindow::showBracketPage);
    connect(m_modeSelectionPage, &ModeSelectionPage::backRequested,      this, &MainWindow::showHomePage);

    connect(m_fanModePage, &FanModePage::openLessonsRequested, this, &MainWindow::showFanLessons);
    connect(m_fanModePage, &FanModePage::backRequested,        this, &MainWindow::showModeSelectionPage);

    connect(m_playerModePage, &PlayerModePage::openLessonsRequested, this, &MainWindow::showPlayerLessons);
    connect(m_playerModePage, &PlayerModePage::backRequested,        this, &MainWindow::showModeSelectionPage);
    connect(m_playerModePage, &PlayerModePage::playGameClicked, this, [this]() {
        m_pageStack->setCurrentWidget(m_penaltyGamePage);
    });

    connect(m_penaltyGamePage, &PenaltyGamePage::backClicked, this, [this]() {
        m_pageStack->setCurrentWidget(m_playerModePage);
    });

    connect(m_lessonMenuPage, &LessonMenuPage::lessonSelected, this, &MainWindow::openLessonById);
    connect(m_lessonMenuPage, &LessonMenuPage::backRequested,  this, [this]() {
        if      (m_currentMode == LessonMode::Fan)    showFanModePage();
        else if (m_currentMode == LessonMode::Player) showPlayerModePage();
        else                                          showModeSelectionPage();
    });
    connect(m_lessonMenuPage, &LessonMenuPage::homeRequested, this, &MainWindow::showHomePage);

    connect(m_lessonViewerPage, &LessonViewerPage::backRequested,  this, &MainWindow::returnToLessonMenu);
    connect(m_lessonViewerPage, &LessonViewerPage::homeRequested,  this, &MainWindow::showHomePage);
    connect(m_lessonViewerPage, &LessonViewerPage::quizRequested,  this, &MainWindow::showQuizPage);

    connect(m_quizPage, &QuizPage::backRequested, this, &MainWindow::returnToLessonMenu);
    connect(m_quizPage, &QuizPage::homeRequested, this, &MainWindow::showHomePage);

    connect(m_jugglingGame, &JugglingGame::backRequested, this, &MainWindow::returnToLessonMenu);
    connect(m_bracketPage,  &BracketPage::backRequested,  this, &MainWindow::showHomePage);
}

void MainWindow::setCurrentPage(PageId pageId)
{
    m_pageStack->setCurrentIndex(static_cast<int>(pageId));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    repositionVolumeWidget();
}

void MainWindow::repositionVolumeWidget()
{
    if (!m_volumeWidget || !centralWidget())
        return;
    const int margin = 10;
    m_volumeWidget->move(
        centralWidget()->width()  - m_volumeWidget->width()  - margin,
        centralWidget()->height() - m_volumeWidget->height() - margin);
    m_volumeWidget->raise();
}

void MainWindow::loadLessonsForMode(LessonMode mode)
{
    m_currentMode = mode;
    QVector<Lesson> lessons = m_lessonRepository.lessonsForMode(mode);

    if      (mode == LessonMode::Fan)    m_lessonMenuPage->setPageTitle("Fan Mode Lessons");
    else if (mode == LessonMode::Player) m_lessonMenuPage->setPageTitle("Player Mode Lessons");
    else                                 m_lessonMenuPage->setPageTitle("Lessons");

    m_lessonMenuPage->setLessons(lessons);
}
