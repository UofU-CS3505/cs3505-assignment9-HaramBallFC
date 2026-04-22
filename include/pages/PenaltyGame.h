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
#include <QLabel>

#include "Box2D/Box2D.h"

class PenaltyGamePage : public QWidget
{
    Q_OBJECT

public:
    explicit PenaltyGamePage(QWidget *parent = nullptr);

signals:
    //Quit button slot for quitting game
    void backClicked();

private slots:
    //Update world method for handling gameplay
    void updateWorld();

    //Reset ball slot for reset ball button
    void resetBall();

    //Shoot ball slot for shoot button
    void shootBall();

protected:
    //Paint event for handling and painting game
    void paintEvent(QPaintEvent *event) override;

private:
    //Box2D world object for setting up world simulation
    b2World world;

    //Box2D Soccer ball object for game
    b2Body* ball;

    //QTimer for starting game
    QTimer timer;

    //QString for detecting answer for hit box
    QString answerTarget;

    //Flag for when player scores
    bool scored;

    //Flag for when ball is visible for goal and hit box purposes
    bool ballVisible;

    //Reset button for returning ball to shooting position
    QPushButton *resetButton;

    //Quit button to return to Player Mode
    QPushButton *backButton;

    //Shoot button for shooting ball into goal
    QPushButton *shootButton;

    //Angle slider for direction where ball shoots
    QSlider *angleSlider;

    //Power slider for shot intensity
    QSlider *powerSlider;

    //Power slider label
    QLabel *powerLabel;

    //Aim slider label
    QLabel *aimLabel;

    //QPixmap for field image
    QPixmap fieldPixmap;

    //QPixmap for player image
    QPixmap playerPixmap;

    //QRectangle for player mapping
    QRect playerRect;

    //List of questions for game
    QStringList questions;

    //Correct answer vector to map to correct answers
    QVector<bool> correctAnswers;

    //Active questions during game play
    QStringList activeQuestions;

    //Active answers during the game
    QVector<bool> activeAnswers;

    //Question Index of question bank
    int currentQuestionIndex;

    //Score int
    int score;

    //Flag that triggers when game over
    bool gameOver;

    //Displays result in game through QString
    QString resultText;


    // Convert to Screen coordinates
    int worldToScreenX(float x);

    // Convert to Screen coordinates
    int worldToScreenY(float y);

    //Resize Event trigger when window changed
    void resizeEvent(QResizeEvent * event) override;
};


