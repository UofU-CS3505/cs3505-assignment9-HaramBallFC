// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// MainWindow = handles page navigation and shared app UI.

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
    void showFanLessons(); // Show fan lessons.
    void showPlayerLessons(); // Show player lessons.
    void showBracketPage();
    void openLessonById(int lessonId); // Open selected lesson.
    void returnToLessonMenu(); // Return to lesson menu.
    void showQuizPage(int lessonId); // Open lesson quiz.

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
    }; // Page IDs for stacked widget.

    void buildPageStack();
    void connectNavigation();
    void setCurrentPage(PageId pageId); // Switch current page.
    void loadLessonsForMode(LessonMode mode); // Load lessons by mode.
    void repositionVolumeWidget(); // Keep volume widget in place.

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

    LessonRepository m_lessonRepository; // Lesson data source.
    LessonMode m_currentMode;
    int m_currentLessonId;
};
