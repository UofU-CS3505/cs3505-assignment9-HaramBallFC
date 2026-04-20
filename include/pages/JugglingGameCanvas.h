#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// JugglingGameCanvas is the widget that draws + runs the juggling physics.

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <Box2D/Box2D.h>
#include <QSettings>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPixmap>

class JugglingGameCanvas : public QWidget
{
    Q_OBJECT

    b2World* world;
    b2Body* ball;
    b2Body* ground;

    QTimer* timer;

    QWidget* m_overlay;
    QLabel* m_scoreLabel;
<<<<<<< HEAD
    QLabel* m_reasonLabel;

    QPixmap m_bgPixmap;
    QPixmap m_playerPixmap;
=======
    QLabel* m_reasonLabel;   // shows "THE BALL FELL" or "HIT THE WALL!"

    QPixmap m_bgPixmap;      // newPitch.png background
>>>>>>> e8cb1d6d6b9cbba83895e781206c62f8c25b1382

    int jugglesCount;

    bool gameOver;
    int highScore;

    void resetGame();
    void repositionGround();   // keeps Box2D ground flush with canvas bottom

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void showEvent(QShowEvent*) override;

private slots:
    void tick();

public:
    explicit JugglingGameCanvas(QWidget* parent = nullptr);
    int getJugglesCount() const;

signals:
    void juggleMilestone(int count);
};
