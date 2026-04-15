#pragma once

#include "models/Lesson.h"
#include "models/LessonRepository.h"

#include <QMainWindow>

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
class PenaltyGamePage;

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
    void openLessonById(int lessonId);
    void returnToLessonMenu();

private:
    PenaltyGamePage *m_penaltyGamePage;
    enum class PageId {
        Home = 0,
        ModeSelection,
        FanMode,
        PlayerMode,
        LessonMenu,
        LessonViewer

    };



    void buildPageStack();
    void connectNavigation();
    void setCurrentPage(PageId pageId);
    void loadLessonsForMode(LessonMode mode);

    Ui::MainWindow *ui;

    QStackedWidget *m_pageStack;

    HomePage *m_homePage;
    ModeSelectionPage *m_modeSelectionPage;
    FanModePage *m_fanModePage;
    PlayerModePage *m_playerModePage;
    LessonMenuPage *m_lessonMenuPage;
    LessonViewerPage *m_lessonViewerPage;

    LessonRepository m_lessonRepository;
    LessonMode m_currentMode;
    int m_currentLessonId;
};
