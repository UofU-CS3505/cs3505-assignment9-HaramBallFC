#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// FanModePage = simple menu page for Fan mode.

#include <QWidget>

class FanModePage : public QWidget
{
    Q_OBJECT

public:
    explicit FanModePage(QWidget *parent = nullptr);

signals:
    void openLessonsRequested();
    void backRequested();
};
