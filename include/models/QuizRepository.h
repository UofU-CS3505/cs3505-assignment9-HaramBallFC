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
    // Constructs the repository and loads all questions and facts
    QuizRepository();

    // Returns quiz questions for a lesson
    QVector<QuizQuestion> questionsForLesson(int lessonId) const;

    // Returns every quiz question in the repository
    QVector<QuizQuestion> allQuestions() const;

    // Returns World Cup and soccer fun facts displayed during the juggling game
    QStringList jugglingFacts() const;

private:
    // Populates m_questions with hard-coded quiz data
    void loadQuestions();

    // Populates m_facts with hard-coded World Cup and soccer fun facts
    void loadFacts();

    QVector<QuizQuestion> m_questions;
    QStringList m_facts;
};
