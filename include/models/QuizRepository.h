#pragma once

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
