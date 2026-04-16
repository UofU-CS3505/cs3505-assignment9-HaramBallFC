#pragma once

#include <QString>
#include <QVector>

struct QuizQuestion
{
    int lessonId = -1;
    QString question;
    QVector<QString> choices; // always 4 options
    int correctIndex = 0;     // 0-based index into choices
};
