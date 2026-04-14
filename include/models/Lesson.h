#pragma once

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
