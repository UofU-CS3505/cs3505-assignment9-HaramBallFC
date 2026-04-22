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
    // Constructs the repository and loads sample lesson data
    LessonRepository();

    // Returns every lesson in the repository
    QVector<Lesson> allLessons() const;

    // Returns lessons visible to the given mode
    QVector<Lesson> lessonsForMode(LessonMode mode) const;

    // Returns the lesson with the given ID, or a placeholder "not found" lesson if the ID is invalid
    Lesson lessonById(int lessonId) const;

private:
    // Populates m_lessons with hard-coded sample data
    void loadSampleLessons();

    // All loaded lessons
    QVector<Lesson> m_lessons;
};
