#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// PlayerModePage = simple menu page for Player mode.

#include <QWidget>

class PlayerModePage : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerModePage(QWidget *parent = nullptr);

signals:
    void openLessonsRequested();
    void backRequested();
    void playGameClicked();
};
