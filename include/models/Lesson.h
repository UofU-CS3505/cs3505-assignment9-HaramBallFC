#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// Data types for lessons (content shown in Fan/Player modes).

#include <QString>

enum class LessonMode {
    General,
    Fan,
    Player
};

struct Lesson
{
    int id = -1;
    QString title;
    QString description;
    QString content;
    QString category;
    LessonMode mode = LessonMode::General;
};
