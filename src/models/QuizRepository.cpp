// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// QuizRepository = holds quiz questions in memory (hard-coded for assignment).

#include "models/QuizRepository.h"

// Constructs the quiz repository and loads both questions and juggling facts.
QuizRepository::QuizRepository()
{
    loadQuestions();
    loadFacts();
}

// Returns quiz questions for a specific lesson (lesson 5 returns a combined review of lessons 1–4).
QVector<QuizQuestion> QuizRepository::questionsForLesson(int lessonId) const
{
    QVector<QuizQuestion> result;

    for (const QuizQuestion &q : m_questions) {
        // Lesson 5 is the Fan mode overall review, so include questions from lessons 1 through 4.
        if (lessonId == 5) {
            if (q.lessonId >= 1 && q.lessonId <= 4) {
                result.append(q);
            }
        } else {
            // For non-review lessons, include only questions matching that lesson ID.
            if (q.lessonId == lessonId) {
                result.append(q);
            }
        }
    }

    return result;
}

// Returns every quiz question currently stored.
QVector<QuizQuestion> QuizRepository::allQuestions() const
{
    return m_questions;
}

// Returns all fact strings used by the juggling/game review feature.
QStringList QuizRepository::jugglingFacts() const
{
    return m_facts;
}

// Loads all hard-coded quiz questions into memory.
void QuizRepository::loadQuestions()
{
    m_questions = {

        // -- Lesson 1: Qualifications --
        {1,
         "A CONMEBOL team finishes 7th in the final league table. What happens to them?",
         {"They're eliminated",
          "They qualify directly",
          "They go to the inter-confederation playoff",
          "They enter a European playoff"},
        2},

        {1,
         "Why does CONCACAF have only 3 direct qualifying slots available despite getting 6 total spots?",
         {"Two slots are reserved for Caribbean nations",
          "Three host nations received automatic spots, using up the rest",
          "FIFA capped direct slots at 3 per region",
          "Two slots went to OFC instead"},
        1},

        {1,
         "Which statement about the inter-confederation playoff is correct?",
         {"All 6 teams played a single knockout round",
          "The two highest ranked teams got byes to the finals",
          "Only CONCACAF teams were eligible",
          "It was held in San Francisco"},
        1},

        {1,
         "Which confederation got a guaranteed spot for the first time in 2026?",
         {"Africa (CAF)",
          "Asia (AFC)",
          "Oceania (OFC)",
          "South America (CONMEBOL)"},
        2},

        {1,
         "UEFA group winners qualify automatically. How are the remaining 4 European spots decided?",
         {"By FIFA ranking",
          "A knockout playoff between second place teams",
          "A second group stage",
          "Random draw"},
        1},

        {1,
         "How many nations will compete in the 2026 World Cup?",
         {"32",
          "48",
          "64",
          "40"},
        1},

        {1,
         "Which three countries are co-hosting the 2026 World Cup?",
         {"USA, England, Mexico",
          "Canada, Mexico, USA",
          "Brazil, Argentina, USA",
          "France, Germany, Spain"},
        1},

        {1,
         "When did qualification for the 2026 World Cup begin?",
         {"March 2022",
          "September 2023",
          "January 2024",
          "June 2025"},
        1},

        {1,
         "How many teams does CONMEBOL (South America) have in its entire confederation?",
         {"10",
          "16",
          "32",
          "54"},
        0},

        {1,
         "How do South American teams decide who qualifies?",
         {"A knockout tournament",
          "A single round-robin league table where everyone plays everyone",
          "Regional group stages",
          "A random FIFA draw"},
        1},

        {1,
         "Which South American team finished 7th and went to the inter-confederation playoff?",
         {"Chile",
          "Venezuela",
          "Bolivia",
          "Peru"},
        2},

        {1,
         "How many direct qualifying spots does Asia (AFC) receive?",
         {"4",
          "6",
          "8",
          "12"},
        2},

        {1,
         "How many rounds did Asia's (AFC) qualification process involve?",
         {"2",
          "3",
          "5",
          "7"},
        2},

        {1,
         "In CONCACAF's first round, which teams played each other?",
         {"The top 4 ranked teams",
          "The 4 weakest teams ranked 29-32",
          "All 32 teams in the confederation",
          "Only Caribbean island nations"},
        1},

        {1,
         "In CONCACAF's second round, how did the format differ from most other qualifying stages?",
         {"Teams played each other only once instead of home and away",
          "Only Caribbean teams participated",
          "Games were decided by penalty shootouts",
          "Groups had 3 teams instead of 5"},
        0},

        {1,
         "How many teams advanced from CONCACAF's second round to the third round?",
         {"6",
          "8",
          "12",
          "16"},
        2},

        {1,
         "Where was the inter-confederation playoff tournament held in March 2026?",
         {"San Francisco",
          "Mexico",
          "Toronto",
          "Washington, D.C."},
        1},

        {1,
         "How many total teams participated in the inter-confederation playoff?",
         {"4",
          "6",
          "8",
          "10"},
        1},

        {1,
         "Which two teams won the inter-confederation playoff and claimed the final World Cup spots?",
         {"Jamaica and Bolivia",
          "New Caledonia and Suriname",
          "DR Congo and Iraq",
          "Nigeria and Cameroon"},
        2},

        {1,
         "In Oceania's qualification, how many teams entered the tournament in total?",
         {"6",
          "9",
          "11",
          "14"},
        2},

        // -- Lesson 2: Countries Represented --
        {2,
         "Which country has qualified for every World Cup since 1930?",
         {"Germany",
          "Argentina",
          "Brazil",
          "Italy"},
        2},
        
        {2,
         "Four countries are making their first World Cup appearance in 2026. Which of these is one of them?",
         {"Cape Verde",
          "Haiti",
          "DR Congo",
          "Iraq"},
        0},
        
        {2,
         "A 4-time World Cup winner failed to qualify for the third tournament in a row. Who?",
         {"England",
          "Germany",
          "France",
          "Italy"},
        3},
        
        {2,
         "How many Arab nations qualified for 2026, a record number?",
         {"4",
          "6",
          "8",
          "10"},
        2},
        
        {2,
         "Which of these countries did not qualify for the 2026 World Cup?",
         {"Uzbekistan",
          "Curaçao",
          "Lebanon",
          "Cape Verde"},
        2},
        
        {2,
         "Which continent has won more World Cups than all other continents combined?",
         {"South America",
          "Europe",
          "Asia",
          "Africa"},
        1},
        
        {2,
         "How many spots did Africa receive for 2026, up from 5 in 2022?",
         {"7",
          "8",
          "10",
          "12"},
        2},
        
        {2,
         "DR Congo returned to the World Cup after an absence dating back to which year?",
         {"1986",
          "1990",
          "1974",
          "1998"},
        2},
        
        {2,
         "Which country is the defending World Cup champion heading into 2026?",
         {"France",
          "Brazil",
          "Argentina",
          "Spain"},
        2},
        
        {2,
         "How many World Cups has Brazil won?",
         {"3",
          "4",
          "5",
          "6"},
        2},
        
        {2,
         "Which country won the very first World Cup in 1930?",
         {"Brazil",
          "Argentina",
          "Uruguay",
          "Italy"},
        2},
        
        {2,
         "Curaçao is the smallest country ever to qualify for a World Cup. Approximately what is their population?",
         {"1.5 million",
          "500,000",
          "156,000",
          "50,000"},
        2},
        
        {2,
         "How did Curaçao qualify for the World Cup?",
         {"They won the inter-confederation playoff",
          "They received an automatic host spot",
          "They finished first in their CONCACAF qualifying group",
          "They were selected by FIFA ranking"},
        2},
        
        {2,
         "When was Haiti's only previous World Cup appearance before 2026?",
         {"1966",
          "1974",
          "1982",
          "1990"},
        1},
        
        {2,
         "When was New Zealand's last World Cup appearance before 2026?",
         {"2002",
          "2006",
          "2010",
          "2014"},
        2},
        
        {2,
         "What happened when New Zealand last appeared at the World Cup in 2010?",
         {"They were eliminated in the first round on goal difference",
          "They went undefeated in the group stage but still didn't advance",
          "They lost all three group matches",
          "They reached the quarterfinals"},
        1},
        
        {2,
         "How did Cape Verde secure their World Cup debut?",
         {"They won the inter-confederation playoff",
          "They beat Eswatini 3-0",
          "They were given an automatic spot by FIFA",
          "They won a penalty shootout against Nigeria"},
        1},
        
        {2,
         "Which of the following is NOT one of the four first-time World Cup qualifiers in 2026?",
         {"Jordan",
          "Uzbekistan",
          "Cape Verde",
          "Panama"},
        3},
        
        {2,
         "Iraq is returning to the World Cup for the first time since which year?",
         {"1974",
          "1986",
          "1994",
          "1998"},
        1},
        
        {2,
         "Which of these is NOT one of the 8 Arab nations that qualified for 2026?",
         {"Jordan",
          "Lebanon",
          "Tunisia",
          "Algeria"},
        1},

        // ── Lesson 3: Rules and Regulations ──

        {3,
         "At which World Cup did the Video Assistant Referee (VAR) make its debut?",
         {"2014 Brazil",
          "2018 Russia",
          "2022 Qatar",
          "2010 South Africa"},
         1},

        {3,
         "How many camera-tracking units does Semi-Automated Offside Technology (SAOT) use under the stadium roof?",
         {"6",
          "8",
          "10",
          "12"},
         3},

        {3,
         "Approximately what percentage of penalty kicks are traditionally scored?",
         {"55–60%",
          "65–70%",
          "75–80%",
          "85–90%"},
         2},

        {3,
         "Who performed the notorious foul on Xabi Alonso in the 2010 World Cup final?",
         {"Arjen Robben",
          "Wesley Sneijder",
          "Nigel de Jong",
          "Robin van Persie"},
         2},

        {3,
         "How many yellow cards did referee Howard Webb show in the 2010 World Cup final — a record?",
         {"8",
          "10",
          "12",
          "14"},
         3},

        {3,
         "What was the final score of Argentina's 2022 World Cup penalty shootout win over France?",
         {"3–2",
          "4–2",
          "5–3",
          "4–3"},
         1},

        {3,
         "A player is in an offside position but receives the ball directly from which of these — and is NOT penalized?",
         {"A forward pass from a teammate",
          "A throw-in by their own team",
          "A rebound off the goalkeeper",
          "A header from a teammate in the opponent's half"},
         1},

        {3,
         "Which organization develops and governs the official Laws of the Game?",
         {"FIFA",
          "UEFA",
          "IFAB (International Football Association Board)",
          "IOC (International Olympic Committee)"},
         2},

        {3,
         "In the 2006 World Cup final, what did Zinedine Zidane do that earned him a red card?",
         {"He deliberately handled the ball in the penalty area",
          "He headbutted Marco Materazzi in the chest",
          "He tackled the goalkeeper outside the box",
          "He argued with the referee and refused to leave the pitch"},
         1},

        {3,
         "Roberto Baggio's famous missed penalty decided which World Cup final?",
         {"1990 Italy vs. Argentina",
          "1998 France vs. Brazil",
          "1994 Brazil vs. Italy",
          "2006 Italy vs. France"},
         2},

        {3,
         "How many total minutes does extra time last in a World Cup knockout match?",
         {"20 minutes",
          "25 minutes",
          "30 minutes",
          "45 minutes"},
         2},

        {3,
         "A player in an offside position receives the ball after it rebounds off the goalkeeper. Are they penalized?",
         {"No — rebounds from the goalkeeper are always exempt",
          "Only if the referee raises the flag first",
          "Yes — gaining an advantage from an offside position is an infraction",
          "No — only deliberate passes trigger the offside rule"},
         2},

        {3,
         "During a penalty kick, which two people are the only ones allowed inside the penalty area?",
         {"The captain and the goalkeeper",
          "The kicker and the goalkeeper",
          "The kicker and one defender of their choice",
          "Any two players chosen by each team"},
         1},

        {3,
         "What does receiving a red card mean for the offending player's team?",
         {"They must substitute the player immediately",
          "They continue with 10 players for the rest of the match",
          "They are awarded a penalty kick",
          "The match is replayed from the point of the foul"},
         1},

        {3,
         "What is 'goal hanging' and why does the offside rule exist to prevent it?",
         {"Holding the crossbar to block shots — banned for safety reasons",
          "Wasting time near your own goal — punished with a yellow card",
          "Positioning a player near the opponent's goal to intercept easy passes without defending",
          "Deliberately kicking the ball out of bounds near the goal line"},
         2},

        {3,
         "At which World Cup did Semi-Automated Offside Technology (SAOT) make its debut?",
         {"2018 Russia",
          "2021 Euro",
          "2022 Qatar",
          "2026 USA"},
         2},

        // ── Lesson 4: History of the World Cup ──

        {4,
         "Jules Rimet served as president of FIFA from 1921 until which year?",
         {"1938",
          "1946",
          "1950",
          "1954"},
         3},

        {4,
         "How many European nations made the boat journey to the inaugural 1930 World Cup in Uruguay?",
         {"2",
          "4",
          "6",
          "8"},
         1},

        {4,
         "What was the estimated attendance at the 1950 World Cup final — still a world record for any football match?",
         {"150,000",
          "175,000",
          "199,854",
          "220,000"},
         2},

        {4,
         "In what year did the World Cup expand to 24 teams for the first time?",
         {"1974",
          "1978",
          "1982",
          "1986"},
         2},

        {4,
         "Kylian Mbappe scored a hat trick in the 2022 final. Who was the only other player to score a hat trick in a World Cup final?",
         {"Pele (1958)",
          "Geoff Hurst (1966)",
          "Ronaldo (2002)",
          "Zinedine Zidane (2006)"},
         1},

        {4,
         "The Estadio Azteca will host matches in 2026. How many different World Cups will it have hosted by then?",
         {"1",
          "2",
          "3",
          "4"},
         2},

        {4,
         "Which was the first FIFA World Cup ever held on the African continent?",
         {"2006 Germany",
          "2010 South Africa",
          "2014 Brazil",
          "2022 Qatar"},
         1},

        {4,
         "How many total matches will be played at the 2026 World Cup, up from 64 in previous tournaments?",
         {"80",
          "88",
          "96",
          "104"},
         3},

        {4,
         "Which two teams played in the first ever World Cup final in 1930?",
         {"Brazil vs. Argentina",
          "Uruguay vs. Argentina",
          "Uruguay vs. Brazil",
          "France vs. Yugoslavia"},
         1},

        {4,
         "What was the score of the first World Cup final in 1930?",
         {"2–1",
          "3–0",
          "4–2",
          "5–1"},
         2},

        {4,
         "How many World Cup titles has Germany won in total?",
         {"3",
          "4",
          "5",
          "6"},
         1},

        {4,
         "What is the 'Maracanazo'?",
         {"Brazil's 7-1 loss to Germany in the 2014 semi-final",
          "Maradona's Hand of God goal against England in 1986",
          "Uruguay's shock victory over Brazil in the 1950 World Cup final",
          "Argentina's penalty shootout win over France in 2022"},
         2},

        {4,
         "The 1994 World Cup, still the record holder for total tournament attendance, was held where?",
         {"Germany",
          "France",
          "United States",
          "Japan"},
         2},

        {4,
         "The 2022 World Cup was the first ever held in which region?",
         {"Asia",
          "Africa",
          "The Middle East",
          "Eastern Europe"},
         2},

        {4,
         "How many World Cup titles has Argentina won?",
         {"1",
          "2",
          "3",
          "4"},
         2},

        {4,
         "Why was the 2022 World Cup in Qatar held in November–December instead of the usual June–July?",
         {"To avoid clashing with the UEFA Champions League",
          "Due to extreme summer heat in Qatar",
          "FIFA required it as part of the hosting agreement",
          "To maximize global TV viewership in the US time zone"},
         1},
    };
}

// Loads hard-coded soccer/world-cup facts used for the juggling review/game mode.
void QuizRepository::loadFacts()
{
    m_facts = {
        "The 2026 FIFA World Cup will be the first World Cup with 48 teams.",
        "Canada, Mexico, and the United States are co-hosting the 2026 World Cup.",
        "Brazil is the only nation to have appeared in every World Cup.",
        "The first FIFA World Cup was held in Uruguay in 1930.",
        "The 2026 World Cup will feature 104 matches.",
        "VAR was first used in a men's World Cup in 2018.",
        "Semi-Automated Offside Technology was introduced at the 2022 World Cup.",
        "A standard football match lasts 90 minutes plus stoppage time.",
        "Penalty shootouts are used if a knockout match stays tied after extra time.",
        "The offside rule exists to prevent goal-hanging.",
        "Argentina won the 2022 FIFA World Cup.",
        "The Estadio Azteca will become the first stadium to host matches in three World Cups.",
        "Oceania received a guaranteed World Cup spot for the first time in 2026.",
        "Curaçao is one of the smallest nations ever to qualify for a World Cup.",
        "Cape Verde qualified for its first World Cup in 2026.",
        "New Zealand went undefeated in the 2010 World Cup group stage but still did not advance.",
        "Kylian Mbappe scored a hat trick in the 2022 World Cup final.",
        "Ebrahim's favorite fact: I love Toni Kroos :) (Greatest Midfielder ever).",
        "Reman's favorite fact: Brazil in the only country to have played in every World Cup.",
        "Tom's favorite fact: A World Cup final can be decided by penalties after 120 minutes.",
        "Harry's favorite fact: Zidane was never caught offside in his entire career.",
        "Diego's favorite fact: Mexico holds the record for the most losses in world cup history. 28 losses in 60 matches",
        "Ethan's favorite fact: The fastest World Cup goal was in 11 seconds. Scored by Hakan Şükür of Turkey."
    };
}
