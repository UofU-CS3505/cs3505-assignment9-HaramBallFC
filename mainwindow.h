// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// AI was used to make this file
// MainWindow handles page navigation, lesson flow,
// and shared UI elements like volume controls.

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

    // Load lessons for each mode and show the lesson menu.
    void showFanLessons();
    void showPlayerLessons();

    void showBracketPage();

    // Open a lesson, quiz, or special activity.
    void openLessonById(int lessonId);

    // Return from lesson content back to the lesson menu.
    void returnToLessonMenu();

    // Open the quiz page for the selected lesson.
    void showQuizPage(int lessonId);

private:
    PenaltyGamePage *m_penaltyGamePage;

    // Identifies each page in the stacked widget.
    enum class PageId {
        Home = 0,
        ModeSelection,
        FanMode,
        PlayerMode,
        LessonMenu,
        LessonViewer,
        Bracket
    };

    // Builds the page stack and shared widgets.
    void buildPageStack();

    // Connects page signals to navigation actions.
    void connectNavigation();

    // Switches to the requested page.
    void setCurrentPage(PageId pageId);

    // Loads lessons for the current mode.
    void loadLessonsForMode(LessonMode mode);

    // Keeps the floating volume widget in position.
    void repositionVolumeWidget();

protected:
    // Updates floating widget placement on resize.
    void resizeEvent(QResizeEvent *event) override;

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

    // Floating volume controls.
    QWidget *m_volumeWidget;
    QLabel  *m_volIcon;
    QSlider *m_volumeSlider;

    // Lesson data and current lesson state.
    LessonRepository m_lessonRepository;
    LessonMode m_currentMode;
    int m_currentLessonId;
};
