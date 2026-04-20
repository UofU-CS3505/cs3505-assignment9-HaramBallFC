#pragma once

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <Box2D/Box2D.h>
#include <QSettings>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class JugglingGameCanvas : public QWidget
{
    Q_OBJECT

    b2World* world;
    b2Body* ball;
    b2Body* ground;

    QTimer* timer;

    QWidget* m_overlay;
    QLabel* m_scoreLabel;
    QLabel* m_reasonLabel;

    QPixmap m_bgPixmap;
    QPixmap m_playerPixmap;

    int jugglesCount;

    bool gameOver;
    int highScore;

    void resetGame();

    void repositionGround();

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
