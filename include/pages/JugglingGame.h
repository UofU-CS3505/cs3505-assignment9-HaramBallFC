#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// JugglingGame is the page wrapper around the juggling mini-game.

#include <QWidget>
#include <QStringList>

class JugglingGame : public QWidget
{
    Q_OBJECT

public:
    explicit JugglingGame(const QStringList &facts, QWidget *parent = nullptr);

signals:
    void backRequested();
};
