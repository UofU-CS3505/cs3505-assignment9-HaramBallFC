#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// QuizRepository provides quiz questions by lesson id.

#include "models/QuizQuestion.h"

#include <QVector>
#include <QStringList>

class QuizRepository
{
public:
    QuizRepository();

    QVector<QuizQuestion> questionsForLesson(int lessonId) const;
    QVector<QuizQuestion> allQuestions() const;
    QStringList jugglingFacts() const;

private:
    void loadQuestions();
    void loadFacts();

    QVector<QuizQuestion> m_questions;
    QStringList m_facts;
};
