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
        // ── Lesson 1: Qualifications (placeholder for Diego) ──
        {1, "Lesson 1: Qualifications",
         "How teams qualify for the FIFA World Cup through each regional confederation.",
         "Section 1: Qualifications Overview\n"
         "The World Cup 2026 will be hosted in North America (USA, Mexico, and Canada). The qualification "
         "process began in September 2023. The reason for a lengthy process is to accomodate over 200 nations "
         "competing for limited spots. Essentially, the process is intentionally spread out as to not disrupt "
         "domestic club seasons (i.e., English Premier League, La Liga, Bundesliga, etc.). \n\n"
         "The Internatioal Federation of Association Football (Fédération Internationale de Football Association, "
         "or FIFA) has 6 regional confederations for managing the 200+ nations. They handle regional "
         "tournaments, qualifying matcfhes, and development specific to their area. The 6 main confederations "
         "make it easier for teams to play regionally before the World Cup. We will introduce each confederations "
         "in this lesson.\n\n"

         "Section 2: Asian Football Confederation (AFC)\n"
         "Countries from the AFC compete in 5 rounds. The weakest 20 teams play knockout matches "
         "first, then join stronger teams in group stages, narrowing to 3 groups of 6. Top 2 from "
         "each grop qualify (8 teams). A further round gives Iraq the Inter-confederation playoff "
         "spot (we will go over the Inter-confederation tournament at the end). So a total of 8 teams "
         "qualify directly for the World Cup. An additional one or two teams could potentially qualify "
         "through the Inter-confederation playoff tournament, bringing the total to 9 or 10 teams.\n\n"

         "Section 3: Union of European Football Associations (UEFA)\n"
         "The tournament consists of 12 groups. Each group winner qualifies directly. Second-place teams "
         "along with the four best-ranked UEFA Nations League winners enter a mini knockout playoff for "
         "the final 4 spots. The Nations Leauge offers a 'second chance' to qualify for the World Cup.\n\n"

         "Section 4: Confederation of African Football (CAF)\n"
         "Teams here were drawn into 9 groups of 6. The winner of each group directly qualifies. Runner-"
         "ups of the 4 best groups advance to the tournament playoffs. Second-place teams play a mini "
         "tournament, and the winner (Democratic Republic of the Congo (DR Congo)) advances to the "
         "Inter-confederation playoff. 9 teams secure direct qualification and a 10th spot is potentially "
         "secured through the Inter-confederation playoff.\n\n"

         "Section 5: South American Football Confederation (CONMEBOL)\n"
         "Only 10 member countries, so all teams play in a single league table. Each team played each "
         "other team twice in a home-and-away round robin format. Top 6 qualify; 7th place (Bolivia) "
         "went to the Inter-confederation playoff. 6 teams qualify directly, and a 7th team can qualify "
         "through the Inter-confederation playoff.\n\n"

         "Section 6: Confederation of North, Central America and Caribbean Association Football (CONCACAF)\n"
         "CONCACAF normally gets 6 World Cup spots, but this time 3 were taken by the hosts (USA, Mexico, Canada),  "
         "leaving only 3 direct spots + 2 playoff spots for the other 32 countries in the region to fight over. "
         "In the first round, the 4 weakest teams (ranked #29-32 on the FIFA rankings) played each other in "
         "two-legged (home and away) knockout matches. 2 winners advanced to the second round. In the second "
         "round, 2 winners from the first round joined the 28 better-ranked teams to make 30 total. The teams "
         "were split into 6 groups of 5. Each team played others only once. Top 2 from each group advanced to "
         "the 3rd round (12 teams left standing). In the 3rd round, 12 teams were split into 3 groups of 4, playing "
         "each team twice, home and away. The 3 group winners directly qualified. 2 best runners-up (Jamaica and "
         "Suriname) went to the Inter-confederation playoff (both lost there).\n\n"

         "Section 7: Oceania Football Confederation (OFC)\n"
         "Oceania's first-ever guaranteed spot. Group stage then knockout. 11 teams played in the OFC qualification "
         "tournament. In the 1st round, the four lowest-ranked teams in the FIFA Men's World Rankings played a 3-match "
         "knockout round. Winner advances to the 2nd round. In the 2nd round, the winner from the first round joint the "
         "7 highest-ranked teams in two 4-team groups. Top 2 teams from each group advanced to the 3rd round. In the 3rd "
         "round, the 4 teams that are left play a 3-match knockout round. Winner qualifies directly for the World Cup. "
         "Runner-up advances to the Inter-confederation playoff. New Zealand qualified directly, with second place (New "
         "Caledonia) going to the playoff.\n\n"

         "Section 8: Inter-confederation Playoff\n"
         "6 teams played in Mexico in March 2026 for 2 final spots. The 2 highest-ranked teams received byes to the finals "
         "while the other 4 played knockout matches. DR Congo and Iraq won theh final 2 World Cup spots.\n\n",
         "Qualifications", LessonMode::General},

        // ── Lesson 2: Countries Represented ──
        {2, "Lesson 2: Countries Represented",
         "Which countries are competing in the 2026 World Cup, including first-timers and records.",
         "Section 1: Europe (16 teams)\n"
         "Europe has won more World Cups than all other countries combined and fields the traditional favorites. "
         "Spain, France, Germany, England, and Portugal are leading contenders. Italy, a 4-time winner, failed "
         "to qualify for the 3rd straight tournament.\n\n"
         "Qualified: Spain, France, England, Germany, Portugal, Netherlands, Belgium, Croatia, Switzerland, Austria, "
         "Norway, Scotland, Sweden, Turkiye, Bosnia & Herzegovina, Czechia.\n\n"

         "Section 2: South America (6 teams)\n"
         "Although CONMEBOL has only 10 members, its teams are strong contenders. Argentina are defending champions "
         "(won the 2022 final). Brazil has won the most World Cups and is the only country to qualify for every tournament "
         "since 1930. Brazil has won the World Cup in 1958, 1962, 1970, 1994, and 2002. Uruguay won the first World Cup "
         "in 1930.\n\n"

         "Qualified: Argentina, Brazil, Colombia, Uruguay, Ecuador, Paraguay.\n\n"

         "Section 3: Africa (10 teams)\n"
         "Africa's biggest ever World Cup presence, up from 5 spots in 2022. DR Congo returns to the World Cup after a "
         "52-year absence. This is the record for the longest gap between World Cup appearances.\n\n"
         "Qualified: Morocco, Senegal, Ivory Coast, Egypt, Algeria, Ghana, South Africa, Tunisia, Cape Verde "
         "(debut), DR Congo, (first since 1974).\n"
         "Cape Verde secured their debut at the World Cup 2026 after beating Eswatini 3-0.\n\n"

         "Section 4: Asia (9 teams)\n"
         "Several nations are debuting or returning after long absences. A record 8 Arab nations qualified across "
         "Asia and Africa combined. They are Algeria, Egypt, Iraq, Jordan, Morocco, Qatar, Saudi Arabia, Tunisia.\n\n"
         "Qualified: Japan, South Korea, Australia, Iran, Saudi Arabia, Qatar, Jordan (debut), Uzbekistan (debut), "
         "Iraq (first since 1986)."

         "Section 5: North/Central America & Caribbean (6 teams)\n"
         "The host region: USA, Canada, and Mexico receieved automatic spots, leaving 3 additional qualifying places. "
         "The Caribbean is represented for the first time in decades.\n\n"
         "Qualified : USA, Mexico, Canada, Curacao (smallest country ever to qualify), Haiti (first since 1974), "
         "Panama.\n"
         "Curacao's population is 156,000. They are the smallest country ever to qualify for the World Cup. They secured "
         "their debut spot with 7 wins and 3 draws during the qualification process. They finished first in Group B.\n\n"

         "Section 6: Oceania (1 team)\n"
         "The smallest confederation received its first guaranteed spot.\n\n"
         "Qualified: New Zealand.\n"
         "New Zealand's last appearance was in 2010. They went undefeated in the group stage but still didn't advance "
         "to the knockout stage."
         "This lesson will cover the 48 participating nations by confederation.",
         "Countries", LessonMode::General},

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
