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
         "Iraq (first since 1986).\n\n"

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
         "This lesson will cover the main rules of association football that are usually misinterpreted in the context of contemporary matches.",

         "Section 1: The Offside Rule\n"
         "The offside rule can be listed among the most widely misunderstood rules of soccer. A player is in "
         "an offside position if at the very moment the ball is passed to him/her, he/she is closer to the "
         "goal line of the opponent than the ball itself and the second-to-last opponent (or the last one who "
         "is usually the goalkeeper). This requirement aims to avoid the tactic called goal hanging that "
         "implies positioning a player near the goal to intercept an unopposed ball.\n\n"
         "However, being in an offside position is not considered an infraction by itself. A penalty occurs "
         "if a player is actively involved in the play by means of:\n"
         "- interfering with play (playing the ball)\n"
         "- interfering with an opponent (blocking a way or obstructing a line of vision)\n"
         "- gaining an advantage by being in that position (for instance, receiving the ball after rebounding "
         "from the goalpost or the goalkeeper)\n\n"
         "There are some circumstances under which a player is not considered offside such as if:\n"
         "- He/she occupies his/her own half of the field.\n"
         "- He/she is even with the second-to-last opponent.\n"
         "- He/she gets the ball directly from a goal kick, throw-in, or corner kick.\n\n"
         "Advancements in technology made determining the offside situation easier. The Video Assistant "
         "Referee (VAR) made its debut at the FIFA World Cup held in Russia in 2018. It has been used ever "
         "since then at all following events. The semi-automated offside technology (SAOT) has premiered at "
         "the World Cup tournament held in Qatar in 2022 and uses twelve camera-tracking units installed "
         "under the stadium roof and a sensor unit placed in the ball. Decisions on offside situations are "
         "provided within seconds now. All sixteen venues hosting the 2026 World Cup will be equipped with SAOT.\n\n"

         "Section 2: Fouls and Penalties\n"
         "Foul is the act performed by one of the players against another, judged by the referee as unfair. "
         "According to the Laws of the Game developed and governed by the International Football Association "
         "Board (IFAB), there are several fouls that include:\n"
         "- kicking or attempting to kick an opponent\n"
         "- tripping or attempting to trip an opponent\n"
         "- jumping at, charging, or striking an opponent\n"
         "- tackling an opponent and contacting him/her before touching the ball\n"
         "- holding an opponent or spitting at any person\n"
         "- deliberately handling the ball (except for the goalkeeper within his/her own penalty area)\n\n"
         "If the foul is committed in areas beyond the penalty area (the 18-yard box), a direct free kick is "
         "awarded from the spot of the foul. If the foul takes place in the penalty area, a penalty kick is "
         "awarded. The kick is performed from the penalty spot, which is 12 yards (11 m) away from the goal "
         "line. The kicker and the goalkeeper are the only players who can remain within the penalty area "
         "while taking a penalty kick. Traditionally, about 75-80 percent of penalty kicks were scored, "
         "making them one of the best scoring opportunities in football.\n\n"
         "Some disciplinary measures control player behavior:\n"
         "- yellow card (a caution): given to players for reckless foul, time-wasting, dissent or unsporting "
         "behavior. A player who receives two yellow cards during one match receives a red card.\n"
         "- red card (a sending off): given to players for serious foul play, violent conduct, denying an "
         "obvious goal-scoring opportunity or using abusive language. A player who is given a red card is "
         "sent off the field. His team is supposed to play with 10 members for the rest of the match.\n\n"
         "In the 2010 FIFA World Cup final between Spain and the Netherlands, referee Howard Webb showed 14 "
         "yellow and one red cards, thus providing the record for the most ill-disciplined World Cup final "
         "match. In particular, the notorious foul performed by Nigel de Jong on Xabi Alonso is considered "
         "the worst foul recorded in World Cup finals.\n\n"

         "Section 3: Extra Time and Penalty Shootout\n"
         "At knockout stages of the World Cup tournament (Round of 32, Round of 16, Quarter-finals, "
         "Semi-finals, and the Final), it is required to define a winner. If there is a draw after 90 minutes "
         "of regular play time, the match proceeds to extra time, consisting of two 15-minute halves (30 "
         "minutes altogether). Halftime takes place in the middle of the extra time period, however, without "
         "giving participants a full break.\n\n"
         "In case of tie after extra time, a penalty shootout is performed, with each side nominating 5 "
         "players for performing alternating kicks. If the teams' scores are equal after 5 rounds of kicks, "
         "a sudden-death shootout starts, during which teams perform alternative kicks until one side scores "
         "and the other fails.\n\n"
         "Here are some notable cases of World Cup penalty shootouts:\n"
         "- 1994 Final (Brazil vs. Italy): Roberto Baggio missed the high kick, allowing Brazil to win. The "
         "event is considered one of the best known in World Cup history.\n"
         "- 2006 Final (Italy vs. France): Zinedine Zidane was sent off the field for headbutting Marco "
         "Materazzi. Italy won 5-3 in the shootout.\n"
         "- 2022 Final (Argentina vs. France): The final match of 2022 is considered one of the greatest in "
         "World Cup history. The shootout ended up with the victory of Argentina for the score of 4-2 after "
         "3-3 draw. Lionel Messi won the World Cup at the age of 35.\n\n"
         "The number of teams participating in the 2026 World Cup will grow up to 48 teams, introducing the "
         "Round of 32 knockout phase.",

         "Rules", LessonMode::General},

        // ── Lesson 4: History of the World Cup ──
        {4, "Lesson 4: History of the World Cup",
         "How the World Cup began, evolved over time, and why it is the most important soccer tournament.",

         "Section 1: Beginning of the World Cup\n"
         "The FIFA World Cup was created by Jules Rimet, the president of FIFA from 1921 to 1954. The idea "
         "was to bring together national teams from around the world in a single tournament. The first World "
         "Cup was held in Uruguay in 1930, with only 13 teams participating. Uruguay was chosen as the host "
         "because they were the reigning Olympic champions and offered to pay travel expenses for all teams.\n\n"
         "The inaugural tournament was dominated by South American teams. Only 4 European nations made the "
         "long boat journey across the Atlantic: France, Belgium, Romania, and Yugoslavia. Uruguay won the "
         "first final, beating Argentina 4-2 at the Estadio Centenario in Montevideo in front of 68,346 fans. "
         "The original trophy was named the Jules Rimet Trophy in honor of the founder.\n\n"
         "The tournament was suspended during World War II (1942 and 1946). When it returned in 1950, Brazil "
         "hosted the event and built the famous Maracana Stadium in Rio de Janeiro. The 1950 final between "
         "Uruguay and Brazil drew an estimated 199,854 spectators, a world record for a football match that "
         "still stands today. Uruguay's shocking victory is known in Brazil as the 'Maracanazo.'\n\n"

         "Section 2: Growth of the Tournament\n"
         "The World Cup has grown dramatically since its beginnings:\n"
         "- 1930: 13 teams participated in Uruguay\n"
         "- 1934-1938: 16 teams, hosted in Europe (Italy and France)\n"
         "- 1954-1970: 16 teams, the era of Pele and Brazil's dominance (3 titles in 4 tournaments)\n"
         "- 1982: Expanded to 24 teams for the first time (Spain)\n"
         "- 1998: Expanded to 32 teams (France), which remained the format for 24 years\n"
         "- 2026: Expanding to 48 teams for the first time (USA, Mexico, Canada)\n\n"
         "Television transformed the World Cup into a global event. The 1966 final between England and West "
         "Germany was one of the first matches broadcast live worldwide. By the 1994 World Cup in the United "
         "States, the tournament drew a total attendance of 3,587,538 fans across 52 matches, a record that "
         "still stands. The 2022 World Cup final between Argentina and France was watched by an estimated "
         "1.5 billion viewers worldwide, making it one of the most-watched events in television history.\n\n"
         "The hosting has also expanded geographically. South Korea and Japan co-hosted in 2002, the first "
         "World Cup held in Asia. South Africa hosted in 2010, the first on the African continent. Qatar "
         "hosted in 2022, the first in the Middle East and the first held in winter (November-December) "
         "due to extreme summer heat.\n\n"

         "Section 3: Successful Countries\n"
         "Only 8 countries have ever won the FIFA World Cup:\n"
         "- Brazil: 5 titles (1958, 1962, 1970, 1994, 2002). The only team to qualify for every single "
         "World Cup since 1930. Pele won 3 World Cups and is considered one of the greatest players ever.\n"
         "- Germany: 4 titles (1954, 1974, 1990, 2014). Known for consistency and strong tournament "
         "performances. Their 2014 victory included a historic 7-1 demolition of Brazil in the semi-final.\n"
         "- Italy: 4 titles (1934, 1938, 1982, 2006). Despite their success, Italy failed to qualify for "
         "both the 2018 and 2022 World Cups and also missed 2026.\n"
         "- Argentina: 3 titles (1978, 1986, 2022). Diego Maradona's 'Hand of God' and 'Goal of the Century' "
         "in the 1986 quarter-final against England are two of the most famous moments in sports history. "
         "Lionel Messi won his first World Cup in 2022 at age 35.\n"
         "- France: 2 titles (1998, 2018). Kylian Mbappe scored a hat trick in the 2022 final, becoming "
         "only the second player after Geoff Hurst (1966) to do so in a World Cup final.\n"
         "- Uruguay: 2 titles (1930, 1950). Won the first ever World Cup on home soil.\n"
         "- England: 1 title (1966). Won their only World Cup on home soil at Wembley Stadium.\n"
         "- Spain: 1 title (2010). Won with their famous tiki-taka passing style.\n\n"
         "All 8 World Cup winners come from either Europe or South America. No team from Africa, Asia, "
         "North America, or Oceania has ever won the tournament.\n\n"

         "Section 4: 2026 World Cup Changes\n"
         "The 2026 FIFA World Cup will be the largest in history with several major changes:\n\n"
         "Format: 48 teams will compete, up from 32. Teams are divided into 12 groups of 4. The top 2 from "
         "each group plus the 8 best third-place teams advance to a Round of 32 knockout stage. This means "
         "104 total matches, up from 64 in previous tournaments.\n\n"
         "Host countries: The tournament is jointly hosted by the United States (11 venues), Mexico (3 venues), "
         "and Canada (2 venues). This is the first World Cup hosted by 3 countries. Key venues include MetLife "
         "Stadium (New York/New Jersey) for the final, the Rose Bowl (Los Angeles), AT&T Stadium (Dallas), "
         "and Estadio Azteca (Mexico City), which becomes the first stadium to host matches in 3 different "
         "World Cups (1970, 1986, 2026).\n\n"
         "Impact: The expansion allows more countries to participate than ever before. Nations like Curacao "
         "(population 156,000, the smallest country ever to qualify), Cape Verde, Jordan, and Uzbekistan will "
         "make their World Cup debuts. The tournament is expected to draw over 6 million fans in total "
         "attendance, breaking the 1994 record.",

         "History", LessonMode::General},

        {5, "Lesson 5: Quiz to Review the Lessons",
         "Review all Fan Mode lessons with a short quiz.",
         "Section 1: Fan Mode Review Quiz\n"
         "This is a placeholder for the Fan Mode review quiz.\n\n"
         "Users will be able to test what they learned from the first four lessons here.",

         "Review Quiz", LessonMode::Fan},

        {6, "Lesson 6: Juggling Game Review",
         "A short interactive juggling game to review Fan Mode content.",
         "Section 1: Juggling Game Review\n"
         "This is a placeholder for the Fan Mode juggling game.\n\n"
         "Later, this page can launch a mini-game where the user keeps the ball in the air while reviewing key ideas from the lessons.",

         "Game Review", LessonMode::Fan}
    };
}
