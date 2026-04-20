#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// QuizRepository provides quiz questions by lesson id.

#include "models/QuizQuestion.h"

#include <QVector>

class QuizRepository
{
public:
    QuizRepository();

    QVector<QuizQuestion> questionsForLesson(int lessonId) const;
    QVector<QuizQuestion> allQuestions() const;

private:
    void loadQuestions();

    QVector<QuizQuestion> m_questions;
};
