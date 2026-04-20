#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// PenaltyGamePage = Box2D-based kick + UI + quiz-ish scoring.

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QPixmap>
#include <QRect>
#include <QSlider>
#include <QStringList>
#include <QVector>
#include <QString>

#include "Box2D/Box2D.h"

class PenaltyGamePage : public QWidget
{
    Q_OBJECT

public:
    explicit PenaltyGamePage(QWidget *parent = nullptr);

signals:
    void backClicked();

private slots:
    void updateWorld();
    void resetBall();
    void shootBall();

protected:
    void paintEvent(QPaintEvent *event) override;
    //void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;

private:
    b2World world;
    b2Body* ball;
    QTimer timer;
    QString answerTarget;

    bool scored;
    bool dragging;
    bool ballVisible;

    QPoint dragStart;
    QPoint dragCurrent;

    QPushButton *resetButton;
    QPushButton *backButton;

    QPushButton *shootButton;
    QSlider *angleSlider;
    QSlider *powerSlider;



    QPixmap fieldPixmap;
    QPixmap playerPixmap;
    QRect playerRect;

    //for penalty game
    QStringList questions;
    QVector<bool> correctAnswers;

    int currentQuestionIndex;
    int score;
    bool gameOver;
    QString resultText;


    int worldToScreenX(float x);
    int worldToScreenY(float y);
};
