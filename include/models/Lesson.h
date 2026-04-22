#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// Data types for lessons (content shown in Fan/Player modes).

#include <QString>

// Determines which user mode a lesson is in
enum class LessonMode {
    General,
    Fan,
    Player
};

// Represents a single educational lesson about the 2026 FIFA World Cup
struct Lesson
{
    int id = -1;
    QString title;
    QString description;
    QString content;
    QString category;
    LessonMode mode = LessonMode::General;
};
