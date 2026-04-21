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
    QLabel* m_reasonLabel;   // shows why the run ended

    QPixmap m_bgPixmap;      // pitch background
    QPixmap m_playerPixmap;  // player sprite

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
