#pragma once

#include "models/Lesson.h"

#include <QVector>

class LessonRepository
{
public:
    LessonRepository();

    QVector<Lesson> allLessons() const;
    QVector<Lesson> lessonsForMode(LessonMode mode) const;
    Lesson lessonById(int lessonId) const;

private:
    void loadSampleLessons();

    QVector<Lesson> m_lessons;
};
