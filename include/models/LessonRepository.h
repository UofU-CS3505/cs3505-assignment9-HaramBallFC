#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// LessonRepository stores lesson data (hard-coded samples right now).

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
