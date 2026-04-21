#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// JugglingGame is the page wrapper around the juggling mini-game.

#include <QWidget>
#include <QStringList>

/**
 * @brief JugglingGame is a QWidget page wrapper that hosts the JugglingGameCanvas
 *        alongside the page title and a back button. It is responsible only for
 *        layout and navigation, all physics and game logic live in JugglingGameCanvas.
 *        Facts are received from QuizRepository and passed directly into the canvas
 *        at construction time via dependency injection.
 */
class JugglingGame : public QWidget
{
    Q_OBJECT

public:
    explicit JugglingGame(const QStringList &facts, QWidget *parent = nullptr);

signals:
    void backRequested();
};
