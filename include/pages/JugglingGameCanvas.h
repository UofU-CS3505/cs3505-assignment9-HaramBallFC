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
#include <QStringList>

/**
 * @brief JugglingGameCanvas is a QWidget subclass that runs the juggling mini-game.
 *        It owns the Box2D physics world, the game loop timer, and all rendering logic.
 *        Facts are injected at construction and cycled every 4 juggles. The player
 *        kicks the ball by pressing spacebar when the ball is within the player's hit box.
 *        Game over is triggered when the ball hits the ground or ceiling, and the high
 *        score is persisted between sessions using QSettings.
 */
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
    QLabel* m_factLabel;     // shows rotating facts while juggling

    QPixmap m_bgPixmap;      // pitch background
    QPixmap m_playerPixmap;  // player sprite

    QStringList m_facts;
    int m_currentFactIndex;

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
    explicit JugglingGameCanvas(const QStringList &facts, QWidget* parent = nullptr);
    int getJugglesCount() const;

signals:
    void juggleMilestone(int count);
};
