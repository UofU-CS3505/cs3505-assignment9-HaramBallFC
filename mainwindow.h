#pragma once

#include "models/Lesson.h"
#include "models/LessonRepository.h"

#include <QLabel>
#include <QMainWindow>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QStackedWidget;
class HomePage;
class ModeSelectionPage;
class FanModePage;
class PlayerModePage;
class LessonMenuPage;
class LessonViewerPage;
class BracketPage;
class PenaltyGamePage;
class QuizPage;
class JugglingGame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showHomePage();
    void showModeSelectionPage();
    void showFanModePage();
    void showPlayerModePage();
    void showFanLessons();
    void showPlayerLessons();
    void showBracketPage();
    void openLessonById(int lessonId); // Open a lesson by ID.
    void returnToLessonMenu(); // Go back to the lesson menu.
    void showQuizPage(int lessonId); // Open the quiz for a lesson.

private:
    PenaltyGamePage *m_penaltyGamePage;

    enum class PageId {
        Home = 0,
        ModeSelection,
        FanMode,
        PlayerMode,
        LessonMenu,
        LessonViewer,
        Bracket
    }; // Used for stacked widget page navigation.

    void buildPageStack();
    void connectNavigation();
    void setCurrentPage(PageId pageId); // Switch visible page.
    void loadLessonsForMode(LessonMode mode); // Load lessons for selected mode.
    void repositionVolumeWidget(); // Keep volume widget aligned.

protected:
    void resizeEvent(QResizeEvent *event) override; // Update layout on resize.

    Ui::MainWindow *ui;
    QStackedWidget *m_pageStack;

    HomePage *m_homePage;
    ModeSelectionPage *m_modeSelectionPage;
    FanModePage *m_fanModePage;
    PlayerModePage *m_playerModePage;
    LessonMenuPage *m_lessonMenuPage;
    LessonViewerPage *m_lessonViewerPage;
    BracketPage *m_bracketPage;
    QuizPage *m_quizPage;
    JugglingGame *m_jugglingGame;

    QWidget *m_volumeWidget;
    QLabel  *m_volIcon;
    QSlider *m_volumeSlider;

    LessonRepository m_lessonRepository; // Stores and provides lesson data.
    LessonMode m_currentMode;
    int m_currentLessonId;
};
