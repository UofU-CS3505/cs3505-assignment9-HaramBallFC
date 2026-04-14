#include "models/LessonRepository.h"

LessonRepository::LessonRepository()
{
    loadSampleLessons();
}

QVector<Lesson> LessonRepository::allLessons() const
{
    return m_lessons;
}

QVector<Lesson> LessonRepository::lessonsForMode(LessonMode mode) const
{
    QVector<Lesson> filteredLessons;

    for (const Lesson &lesson : m_lessons) {
        bool matchesFan =
            (mode == LessonMode::Fan) &&
            (lesson.mode == LessonMode::General || lesson.mode == LessonMode::Fan);

        bool matchesPlayer =
            (mode == LessonMode::Player) &&
            (lesson.mode == LessonMode::General || lesson.mode == LessonMode::Player);

        bool matchesGeneral =
            (mode == LessonMode::General) &&
            (lesson.mode == LessonMode::General);

        if (matchesFan || matchesPlayer || matchesGeneral) {
            filteredLessons.push_back(lesson);
        }
    }

    return filteredLessons;
}

Lesson LessonRepository::lessonById(int lessonId) const
{
    for (const Lesson &lesson : m_lessons) {
        if (lesson.id == lessonId) {
            return lesson;
        }
    }

    Lesson missingLesson;
    missingLesson.title = "Lesson Not Found";
    missingLesson.description = "The selected lesson could not be loaded.";
    missingLesson.content = "TODO: Handle invalid lesson selection more gracefully later.";
    missingLesson.category = "System";
    return missingLesson;
}

void LessonRepository::loadSampleLessons()
{
    m_lessons = {
        {1, "Soccer Basics", "An introduction to the sport, field, and main objectives of play.",
         "Placeholder lesson content.", "Basics", LessonMode::General},
        {2, "Rules and Gameplay", "A starter lesson covering common rules and how a match flows.",
         "Placeholder lesson content.", "Rules", LessonMode::General},
        {3, "World Cup History", "A fan-focused lesson about the tournament and its legacy.",
         "Placeholder lesson content.", "History", LessonMode::Fan},
        {4, "How to Watch Like a Fan", "A fan-oriented lesson about noticing patterns and key moments.",
         "Placeholder lesson content.", "Fan Experience", LessonMode::Fan},
        {5, "Player Positions and Roles", "A player-focused lesson introducing responsibilities by position.",
         "Placeholder lesson content.", "Positions", LessonMode::Player},
        {6, "Movement and Strategy", "A player-oriented lesson on spacing and decision-making.",
         "Placeholder lesson content.", "Strategy", LessonMode::Player}
    };
}
