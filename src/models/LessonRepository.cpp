// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// LessonRepository = in-memory storage for lesson content.

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
        // ── Lesson 1: Qualifications ──
        {1, "Lesson 1: Qualifications",
         "How teams qualify for the FIFA World Cup through each regional confederation.",

         "Section 1: Qualifications Overview\n"
         "The 2026 FIFA World Cup is the first edition to feature 48 teams, expanded from 32. This historic "
         "increase meant more qualification spots were distributed across every confederation, giving more "
         "nations a chance to compete on the world stage for the very first time. The qualification process "
         "began in September 2023 and ran through early 2026, intentionally spread out to avoid disrupting "
         "domestic club leagues such as the English Premier League, La Liga, and the Bundesliga.\n\n"
         "FIFA (Fédération Internationale de Football Association) manages over 200 member nations through "
         "6 regional confederations. Each confederation runs its own qualifying format suited to the number "
         "and strength of its members. The host nations — USA, Mexico, and Canada — received automatic spots "
         "without needing to qualify. The remaining 45 spots were distributed as follows:\n"
         "- UEFA (Europe): 16 direct spots\n"
         "- CAF (Africa): 9 direct spots + 1 inter-confederation playoff spot\n"
         "- AFC (Asia): 8 direct spots + 1 inter-confederation playoff spot\n"
         "- CONMEBOL (South America): 6 direct spots + 1 inter-confederation playoff spot\n"
         "- CONCACAF (N/C America & Caribbean): 3 earned spots (3 hosts were automatic)\n"
         "- OFC (Oceania): 1 direct spot\n"
         "- Inter-confederation Playoff: 2 final spots from among 6 teams\n\n"

         "Section 2: Asian Football Confederation (AFC)\n"
         "47 AFC member nations competed across five qualifying rounds for a record 8 direct spots — up from "
         "5 in 2022. The weakest-ranked nations entered in the earliest rounds. In the first round, the "
         "bottom-ranked teams played knockout matches. In the second round, survivors joined stronger sides "
         "in groups. The third round narrowed to 18 teams split into 3 groups of 6. The top 2 from each "
         "group (6 teams) qualified directly. The third and fourth-place teams (6 teams) moved to the fourth "
         "round, split into 2 groups of 3. The top team from each fourth-round group (2 teams) also qualified "
         "directly, completing the 8 automatic spots. The remaining teams competed for the AFC "
         "inter-confederation playoff berth, which Iraq ultimately won.\n\n"
         "Notable AFC qualifiers: Japan (AFC's top-ranked team, reached the quarterfinals in 2022), South "
         "Korea (11th consecutive World Cup), Australia, Iran, Saudi Arabia, Qatar, and two debutants — "
         "Jordan and Uzbekistan — who qualify for the very first time.\n\n"

         "Section 3: Union of European Football Associations (UEFA)\n"
         "UEFA received the largest allocation with 16 spots, reflecting Europe's historical dominance in "
         "the sport. All 54 UEFA member nations competed in 12 qualifying groups. The 12 group winners "
         "qualified directly. The 12 runners-up, plus the 4 highest-ranked Nations League group winners "
         "who had not already qualified, entered a playoff round of 16 teams in 4 brackets. The 4 bracket "
         "winners claimed the remaining 4 spots.\n\n"
         "The UEFA Nations League — a competition that runs alongside qualifying — gives nations a secondary "
         "path to the World Cup. Nations that perform well in the Nations League but finish second in their "
         "qualifying group are rewarded with a playoff berth. This system prevents strong nations from being "
         "unfairly eliminated and ensures the best European teams reach the tournament. Italy, a four-time "
         "World Cup winner, failed to win their playoff bracket and missed a historic third consecutive "
         "World Cup — their last appearance was in 2014.\n\n"

         "Section 4: Confederation of African Football (CAF)\n"
         "Africa received the largest proportional increase of any confederation, going from 5 spots in "
         "2022 to 9 direct spots plus an inter-confederation playoff berth. This expansion reflects the "
         "growing quality and global influence of African football, highlighted by Morocco reaching the "
         "semifinals of the 2022 World Cup — the best performance by any African nation in history.\n\n"
         "54 African nations competed in 9 qualifying groups of 6 teams each. Every team in a group played "
         "each other twice (home and away). The 9 group winners qualified directly. The runners-up of the "
         "4 best-performing groups based on overall record competed in a playoff mini-tournament. DR Congo "
         "won that playoff and then won one of the 2 final spots at the inter-confederation playoff in Mexico, "
         "ending a 52-year absence — the longest gap between World Cup appearances in tournament history.\n\n"

         "Section 5: South American Football Confederation (CONMEBOL)\n"
         "CONMEBOL has only 10 member nations, so no traditional group-stage knockout format was needed. "
         "All 10 countries played in a single round-robin league table, with each team facing every other "
         "team twice (once at home, once away) for a total of 18 matches per team. The top 6 in the final "
         "standings qualified directly. 7th-place Bolivia advanced to the inter-confederation playoff but "
         "was eliminated. Chile, Peru, and Venezuela all failed to qualify.\n\n"
         "Final standings (qualifying positions): 1st Argentina (defending champions), 2nd Ecuador, "
         "3rd Brazil, 4th Uruguay, 5th Colombia, 6th Paraguay. Despite Brazil never having missed a World "
         "Cup since 1930, they had a difficult campaign and scraped through in 3rd place. Argentina, led by "
         "Lionel Messi, topped the table comfortably as they entered as 2022 world champions.\n\n"

         "Section 6: Confederation of North, Central America and Caribbean Association Football (CONCACAF)\n"
         "CONCACAF's qualification was unique because three of its own members — USA, Mexico, and Canada — "
         "were automatic hosts. This left only 3 earned spots for the remaining 32 nations. The process "
         "ran across 3 rounds. In the first round, the 4 lowest-ranked nations played two-legged knockout "
         "matches; 2 winners advanced. In the second round, those 2 joined 28 higher-ranked teams, split "
         "into 6 groups of 5. The top 2 from each group (12 teams) advanced to the third round. In the "
         "third and final round, 12 teams were placed into 3 groups of 4, playing home and away. The 3 "
         "group winners — Curacao, Haiti, and Panama — qualified directly. The 2 best runners-up (Jamaica "
         "and Suriname) went to the inter-confederation playoff, where both were eliminated.\n\n"
         "The Caribbean had a historic result: Curacao (population 156,000, the smallest nation ever to "
         "qualify for a World Cup) and Haiti both secured spots, marking the region's strongest showing in decades.\n\n"

         "Section 7: Oceania Football Confederation (OFC)\n"
         "For the first time in history, Oceania received a guaranteed World Cup spot. In every previous "
         "tournament, OFC teams had to win a two-legged inter-confederation playoff — against a team from "
         "another region — just to have a chance at qualifying. This change by FIFA recognizes Oceania's "
         "right to guaranteed representation. 11 OFC nations competed in a multi-round tournament. The "
         "4 lowest-ranked teams played a knockout first round. 8 teams then entered a group stage in "
         "2 groups of 4. The top 2 from each group (4 teams) played knockout semifinals. New Zealand won "
         "the OFC tournament and qualified directly. New Caledonia, the runner-up, advanced to the "
         "inter-confederation playoff but was eliminated by Iraq.\n\n"

         "Section 8: Inter-confederation Playoff\n"
         "6 nations from 5 different confederations competed in Mexico in March 2026 for the final 2 "
         "World Cup spots. The participants were Bolivia (CONMEBOL), Iraq (AFC), DR Congo (CAF), "
         "New Caledonia (OFC), Jamaica (CONCACAF), and Suriname (CONCACAF). The 2 highest-ranked "
         "teams by FIFA ranking received byes directly to the semifinals, while the other 4 played "
         "single-match quarterfinals. Iraq defeated New Caledonia and DR Congo defeated Jamaica in the "
         "semifinals. In the two finals, Iraq and DR Congo each won their matches to claim the last "
         "2 spots and complete the historic field of 48 nations for the 2026 FIFA World Cup.\n\n",
         "Qualifications", LessonMode::General},

        // ── Lesson 2: Countries Represented ──
        {2, "Lesson 2: Countries Represented",
         "Which countries are competing in the 2026 World Cup, including first-timers and records.",

         "Section 1: Europe (16 teams)\n"
         "Europe is the most represented confederation at the 2026 World Cup with 16 spots — more than any "
         "other region. European nations have won 12 of the 22 World Cup editions held so far. Despite this "
         "dominance, Italy — a four-time champion — failed to qualify for a historic third consecutive "
         "tournament. Their last World Cup appearance was in 2014, where they were eliminated in the group "
         "stage. Italy lost in the UEFA playoff bracket, marking an unprecedented collapse for one of the "
         "sport's most historically successful nations.\n\n"
         "Notable returns: Norway qualifies for the first time since 1998, led by Erling Haaland, one of "
         "the world's best strikers. Scotland also returns after a long absence. Bosnia & Herzegovina "
         "qualifies for only their second World Cup ever.\n\n"
         "Qualified: Spain, France, England, Germany, Portugal, Netherlands, Belgium, Croatia, Switzerland, "
         "Austria, Norway, Scotland, Sweden, Turkiye, Bosnia & Herzegovina, Czechia.\n\n"
         "Key contenders: France (2018 champions), Spain (2010 champions), Germany (2014 champions), "
         "England (looking to end their 60-year wait for a second title), and Portugal (led by Cristiano "
         "Ronaldo, who will be 41 at the tournament).\n\n"

         "Section 2: South America (6 teams)\n"
         "South America is the second most successful continent in World Cup history. CONMEBOL nations have "
         "won 10 of the 22 World Cups, with Brazil (5 titles) and Argentina (3 titles) accounting for 8 "
         "of those wins alone. Despite having only 10 member nations, the South American qualifying campaign "
         "is considered one of the most difficult in world football — every match is competitive and played "
         "at high altitude or in extreme heat in some locations.\n\n"
         "Argentina enter as defending 2022 champions. Their captain Lionel Messi, widely regarded as the "
         "greatest player of all time, will be 38 during the tournament and this is expected to be his final "
         "World Cup. Brazil, the only nation to have qualified for every single World Cup since 1930, had "
         "a difficult qualifying campaign but secured their spot in 3rd place. Uruguay, winners of the very "
         "first World Cup in 1930, also qualified with a strong campaign.\n\n"
         "Notable absences: Chile (Copa América champions in 2015 and 2016), Peru, and Venezuela all failed "
         "to qualify despite having competitive squads.\n\n"
         "Qualified: Argentina, Brazil, Colombia, Uruguay, Ecuador, Paraguay.\n\n"

         "Section 3: Africa (10 teams)\n"
         "Africa has its biggest-ever World Cup presence in 2026, with 10 spots — up from 5 in 2022. This "
         "expansion acknowledges the rapid growth of African football, exemplified by Morocco becoming the "
         "first African nation ever to reach the World Cup semifinals at the 2022 tournament in Qatar. "
         "Morocco defeated Spain and Portugal in the knockout stages before being eliminated by France.\n\n"
         "Qualified: Morocco, Senegal, Ivory Coast, Egypt, Algeria, Ghana, South Africa, Tunisia, "
         "Cape Verde (debut), DR Congo (first since 1974).\n\n"
         "Historic stories: Cape Verde, a small island nation in the Atlantic Ocean with a population of "
         "around 600,000, secured their debut World Cup spot by beating Eswatini 3-0. DR Congo's return "
         "ends a 52-year absence — the longest gap between World Cup appearances in the tournament's "
         "history. A record 8 Arab nations qualified across Africa and Asia combined in 2026: Algeria, "
         "Egypt, Iraq, Jordan, Morocco, Qatar, Saudi Arabia, and Tunisia.\n\n"

         "Section 4: Asia (9 teams)\n"
         "Asia's allocation grew from 5 spots in 2022 to 8 direct spots plus a playoff berth for 2026, "
         "reflecting the growing quality of AFC football. Iraq won the inter-confederation playoff to "
         "bring the AFC total to 9 teams. Japan is the standout AFC nation — they topped their qualifying "
         "group, won their group at the 2022 World Cup (beating Germany and Spain), and reached the "
         "quarterfinals before being eliminated by Croatia on penalties.\n\n"
         "Qualified: Japan, South Korea, Australia, Iran, Saudi Arabia, Qatar, Jordan (debut), "
         "Uzbekistan (debut), Iraq (first since 1986).\n\n"
         "South Korea qualifies for their 11th consecutive World Cup — a remarkable streak of consistency. "
         "Jordan and Uzbekistan make their historic tournament debuts after strong qualifying campaigns. "
         "Qatar, the 2022 hosts who became the first host nation eliminated in the group stage of their "
         "own tournament, bounced back to qualify on merit for 2026 by winning their AFC group. Iraq "
         "returns for the first time since 1986, when they also qualified via an inter-confederation playoff.\n\n"

         "Section 5: North/Central America & Caribbean (6 teams)\n"
         "The host region receives 6 spots in total. USA, Mexico, and Canada were granted automatic "
         "qualification as co-hosts, leaving only 3 spots for the remaining 32 CONCACAF nations. This "
         "edition produced historic results for the Caribbean, a region that had been largely absent "
         "from recent World Cups.\n\n"
         "Qualified: USA (hosts), Mexico (hosts), Canada (hosts), Curacao, Haiti, Panama.\n\n"
         "Curacao — an island in the southern Caribbean Sea — became the smallest country by population "
         "(156,000 people) ever to qualify for a FIFA World Cup. Their squad, made up largely of players "
         "with Dutch heritage eligible through ancestry rules, went unbeaten in their qualifying group "
         "with 7 wins and 3 draws, finishing first. Haiti returns to the World Cup for the first time "
         "since 1974. Panama is making only their second World Cup appearance after debuting in 2018. "
         "Canada, as automatic hosts, would likely have qualified on merit — they topped their CONCACAF "
         "third-round group and feature rising stars playing in top European leagues.\n\n"

         "Section 6: Oceania (1 team)\n"
         "Oceania received its first-ever guaranteed World Cup spot for 2026. In every previous tournament, "
         "OFC nations had to win a two-legged inter-confederation playoff against a team from another "
         "region for even a chance of qualifying. This structural change by FIFA ensures the Pacific "
         "region always has representation at the world's biggest football event.\n\n"
         "Qualified: New Zealand.\n\n"
         "New Zealand won the OFC qualifying tournament to claim the guaranteed Oceania spot. This is "
         "their third World Cup appearance overall and first since 2010. At the 2010 World Cup in South "
         "Africa, New Zealand achieved something unique in tournament history: they were the only team "
         "to go unbeaten across all three group stage matches (drawing against Slovakia, Italy, and "
         "Paraguay) yet still failed to advance to the knockout stage. Their 2026 squad features players "
         "from the A-League and several European leagues, marking steady growth in the country's "
         "football development.",
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
