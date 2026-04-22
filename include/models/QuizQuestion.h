#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// QuizQuestion = one multiple-choice question (always 4 choices).

#include <QString>
#include <QVector>

// Represents a single mutiple choice quiz question tied to a lesson
struct QuizQuestion
{
    int lessonId = -1;
    QString question;
    QVector<QString> choices; // always 4 options
    int correctIndex = 0;     // 0-based index into choices
};
