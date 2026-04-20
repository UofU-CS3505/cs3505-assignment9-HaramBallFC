#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// ModeSelectionPage = menu to pick which mode to enter.

#include <QWidget>

class ModeSelectionPage : public QWidget
{
    Q_OBJECT

public:
    explicit ModeSelectionPage(QWidget *parent = nullptr);

signals:
    void fanModeSelected();
    void playerModeSelected();
    void bracketSelected();
    void backRequested();
};
