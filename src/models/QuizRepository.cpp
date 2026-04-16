#include "models/QuizRepository.h"

QuizRepository::QuizRepository()
{
    loadQuestions();
}

QVector<QuizQuestion> QuizRepository::questionsForLesson(int lessonId) const
{
    QVector<QuizQuestion> result;

    for (const QuizQuestion &q : m_questions) {
        if (lessonId == 5) { //this is for Fan mode as it is "lesson" #4, it makes the quiz review a COMBINED OVERALL one
            if (q.lessonId >= 1 && q.lessonId <= 4) {
                result.append(q);
            }
        } else {
            if (q.lessonId == lessonId) {
                result.append(q); // for the rest, logic remains the same
            }
        }
    }

    return result;
}

QVector<QuizQuestion> QuizRepository::allQuestions() const
{
    return m_questions;
}

void QuizRepository::loadQuestions()
{
    m_questions = {

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
