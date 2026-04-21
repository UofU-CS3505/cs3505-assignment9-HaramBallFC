#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// JugglingGame is the page wrapper around the juggling mini-game.

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <Box2D/Box2D.h>

class JugglingGame : public QWidget
{
    Q_OBJECT
    
    b2World* world;
    b2Body* ball;
    
    QTimer* timer;
    
    QString currentFact;
    
    int jugglesCount;
    
    QVector<QString> facts;
    
    void keyPressEvent(QKeyEvent*) override;
    
    explicit Jugl
    
protected:
    void paintEvent(QPaintEvent*) override;
    
private slots:
    void update();
    

    b2World* world;
    b2Body* ball;

    QTimer* timer;

    QString currentFact;

    int jugglesCount;

    QVector<QString> facts;

    void keyPressEvent(QKeyEvent*) override;


protected:
    void paintEvent(QPaintEvent*) override;

private slots:
    void tick();

public:
    explicit JugglingGame(QVector<QString> facts, QWidget *parent = nullptr);

signals:
    void backRequested();
};
