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
    // Constructs the mode selection page and optionally assigns a parent widget.
    explicit ModeSelectionPage(QWidget *parent = nullptr);

signals:
    // Emitted when the user chooses Fan Mode.
    void fanModeSelected();

    // Emitted when the user chooses Player Mode.
    void playerModeSelected();

    // Emitted when the user chooses the Bracket option.
    void bracketSelected();

    // Emitted when the user requests navigation to the previous/back page.
    void backRequested();
};
