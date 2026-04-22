#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// PlayerModePage displays the Player mode menu
// and lets the user open lessons, go back, or start the game.

#include <QWidget>

class PlayerModePage : public QWidget
{
    Q_OBJECT

public:
    // Creates the Player mode page.
    explicit PlayerModePage(QWidget *parent = nullptr);

signals:
    // Opens the Player mode lessons page.
    void openLessonsRequested();

    // Returns to the previous page.
    void backRequested();

    // Starts the Player mode game.
    void playGameClicked();
};
