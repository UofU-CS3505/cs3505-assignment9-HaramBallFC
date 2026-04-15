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
        // ── Lesson 3: Rules and Regulations ──
        {3, "Lesson 3: Rules and Regulations",
         "The most important and commonly misunderstood soccer rules for following real matches.",

         "Section 1: Offside Rule\n"
         "The offside rule is one of the most misunderstood parts of soccer. A player is considered "
         "offside if they are closer to the opponent's goal than the defenders at the moment the "
         "ball is passed to them. This rule exists to prevent players from gaining an unfair "
         "advantage by standing near the goal and waiting for an easy pass.\n\n"
         "However, being in an offside position does not automatically mean a player is offside. "
         "The timing of the pass is what matters most. A player is NOT offside if:\n"
         "- They are on their own half of the field\n"
         "- They are level with the last defender\n"
         "- The ball is passed before they move ahead\n\n"
         "In real matches, offside decisions can be extremely close and are sometimes reviewed "
         "using video technology to ensure accuracy.\n\n"

         "Section 2: Fouls and Penalties\n"
         "A foul occurs when a player breaks the rules in a physical or unsafe way, such as:\n"
         "- Tripping an opponent\n"
         "- Pushing or holding\n"
         "- Tackling in a dangerous way\n\n"
         "If the foul takes place outside the penalty area, the opposing team is awarded a free "
         "kick. If the foul takes place inside the penalty area, the opposing team is awarded a "
         "penalty kick. Penalty kicks give a very high chance of scoring.\n\n"
         "Some fouls may also result in disciplinary action. A yellow card is a warning, while a "
         "red card removes the player from the game.\n\n"

         "Section 3: Extra Time and Penalty Shootouts\n"
         "In knockout rounds, a winner must be decided. If the score is tied at the end of regular "
         "time, the match goes into extra time (two additional halves). If the score is still tied "
         "after extra time, the match is decided by a penalty shootout.\n\n"
         "During a penalty shootout, each team takes turns shooting one-on-one against the "
         "goalkeeper. The team with the most successful shots wins. Penalty shootouts are often "
         "high-pressure situations and can decide major tournaments.",

         "Rules", LessonMode::General},

        // ── Lesson 4: History of the World Cup ──
        {4, "Lesson 4: History of the World Cup",
         "How the World Cup began, evolved over time, and why it is the most important soccer tournament.",

         "Section 1: Beginning of the World Cup\n"
         "The FIFA World Cup began in 1930. The first tournament was much smaller than it is "
         "today, with fewer teams and less global attention. Over time, the tournament grew in "
         "popularity and became the most prestigious competition in international soccer. It is "
         "held every four years, which gives teams time to qualify and prepare.\n\n"

         "Section 2: Growth of the Tournament\n"
         "The World Cup has changed significantly over time:\n"
         "- An increase in the number of participating teams\n"
         "- Growth in global viewership\n"
         "- Higher levels of competition between countries\n\n"
         "Advancements in media and broadcasting have made it possible for people all over the "
         "world to watch matches live. Today, the World Cup is one of the most watched sporting "
         "events globally.\n\n"

         "Section 3: Successful Countries\n"
         "Certain countries have had consistent success in the World Cup, including Brazil, "
         "Germany, Argentina, and Italy. Brazil has won the most World Cups, making it one of "
         "the most successful national teams in the history of the tournament. These countries "
         "are known for strong soccer traditions, skilled players, and consistent performance.\n\n"

         "Section 4: 2026 World Cup Changes\n"
         "The 2026 World Cup will introduce major changes:\n"
         "- It will be hosted by the United States, Canada, and Mexico\n"
         "- It will include 48 teams, more than any previous tournament\n"
         "- It will feature more matches and a larger tournament structure\n\n"
         "This expansion allows more countries to participate and increases global representation.",

         "History", LessonMode::General}
    };
}
