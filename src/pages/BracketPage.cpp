// AI was used to help with the designing in this file and keeping in line with the rest of the app
//
// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// BracketPage = group stage ranking → pick 8 third-place teams → knockout bracket.
#include "pages/BracketPage.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QVBoxLayout>

// Pixel-art style from other files.
QString BracketPage::pixelBtn(const QString &bg, const QString &hi,
                              const QString &sh, const QString &fg)
{
    return QString(
               "QPushButton {"
               "  background-color: %1; color: %4;"
               "  border-radius: 0px;"
               "  border-top:    4px solid %2; border-left:  4px solid %2;"
               "  border-bottom: 4px solid %3; border-right: 4px solid %3;"
               "  font-family: 'Press Start 2P'; font-size: 10px;"
               "  padding: 10px 14px; min-height: 34px;"
               "}"
               "QPushButton:hover  { background-color: %2; color: #FFFFFF; }"
               "QPushButton:pressed {"
               "  border-top:    4px solid %3; border-left:  4px solid %3;"
               "  border-bottom: 4px solid %2; border-right: 4px solid %2;"
               "}"
               "QPushButton:disabled { background-color: #141F2E; color: #3A4A5A;"
               "  border-color: #1A2A3A; }"
               ).arg(bg, hi, sh, fg);
}

// Rank badge colour: green=1st, gold=2nd, red=3rd, navy=4th/unranked
static QString rankColor(int rank)
{
    switch (rank) {
    case 1: return "#1A5A2A";
    case 2: return "#7A6020";
    case 3: return "#7A0018";
    default: return "#1E3A5F";
    }
}

// Style for a teams in group stage
static QString groupTeamBtnStyle(bool isAssigned)
{
    if (isAssigned)
        return "QPushButton { background-color:#1E3A5F; color:#FFFFFF;"
               "  border:2px solid #D4A843; padding:10px; text-align:left; font-size:13px; }"
               "QPushButton:hover { background-color:#27476F; }"; //highlighted once ranked

    return "QPushButton { background-color:#0F1E35; color:#8FA3B8;"
           "  border:2px solid #1E3A5F; padding:10px; text-align:left; font-size:13px; }"
           "QPushButton:hover { background-color:#162844; color:#FFFFFF; }";
}

// Style for the third-place pick buttons (green when selected)
static QString thirdBtnStyle(bool selected)
{
    if (selected)
        return "QPushButton { background-color:#1A5A2A; color:#FFFFFF;"
               "  border:2px solid #27AE60; padding:10px; text-align:left; font-size:13px; }"
               "QPushButton:hover { background-color:#216C33; }";

    return "QPushButton { background-color:#0F1E35; color:#8FA3B8;"
           "  border:2px solid #1E3A5F; padding:10px; text-align:left; font-size:13px; }"
           "QPushButton:hover { background-color:#162844; color:#FFFFFF; }";
}

// Constructor for the bracket

BracketPage::BracketPage(QWidget *parent)
    : QWidget(parent)
{
    initializeGroups();
    buildUi();
    refreshGroupStageUi();
}

// Group data (A-L) from the official Fifa World Cup 2026 groups
void BracketPage::initializeGroups()
{
    m_groups = {
                {"A", {"Mexico",        "South Africa", "South Korea",  "Czech Republic"}, QVector<int>(4,0), 1},
                {"B", {"Canada",        "Bosnia & Herz.", "Qatar",      "Switzerland"},    QVector<int>(4,0), 1},
                {"C", {"Brazil",        "Morocco",       "Haiti",       "Scotland"},        QVector<int>(4,0), 1},
                {"D", {"United States", "Paraguay",      "Australia",   "Turkey"},          QVector<int>(4,0), 1},
                {"E", {"Germany",       "Curacao",       "Ivory Coast", "Ecuador"},         QVector<int>(4,0), 1},
                {"F", {"Netherlands",   "Japan",         "Sweden",      "Tunisia"},         QVector<int>(4,0), 1},
                {"G", {"Belgium",       "Egypt",         "Iran",        "New Zealand"},     QVector<int>(4,0), 1},
                {"H", {"Spain",         "Cape Verde",    "Saudi Arabia","Uruguay"},         QVector<int>(4,0), 1},
                {"I", {"France",        "Senegal",       "Iraq",        "Norway"},          QVector<int>(4,0), 1},
                {"J", {"Argentina",     "Algeria",       "Austria",     "Jordan"},          QVector<int>(4,0), 1},
                {"K", {"Portugal",      "DR Congo",      "Uzbekistan",  "Colombia"},        QVector<int>(4,0), 1},
                {"L", {"England",       "Croatia",       "Ghana",       "Panama"},          QVector<int>(4,0), 1},
                };
}

// function to set up the UI for the flow of Groups -> Third Place Selection -> KOs
void BracketPage::buildUi()
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Top green accent stripe
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #27AE60;");

    m_internalStack = new QStackedWidget(this);
    m_internalStack->addWidget(buildGroupStageView());   // index 0
    m_internalStack->addWidget(buildThirdPlaceView());   // index 1
    m_internalStack->addWidget(buildKnockoutView());     // index 2

    root->addWidget(topBar);
    root->addWidget(m_internalStack, 1);
}

// ***************************************************************************
//  Group Stage phase
//  Click teams in order to assign ranks 1 to 3, 4th is set automatically
//  "Reset Group" button clears the group

QWidget *BracketPage::buildGroupStageView()
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(60, 34, 60, 28);
    root->setSpacing(18);

    QLabel *eyebrow = new QLabel("BRACKET", page);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(
        "font-family:'Press Start 2P'; font-size:10px; letter-spacing:3px; color:#D4A843;");

    QLabel *title = new QLabel("Group Stage", page);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:34px; font-weight:700; color:#FFFFFF;");

    QLabel *subtitle = new QLabel(
        "Click teams in order of where you think they will place!\n"
        "1st place, 2nd place, 3rd place and 4th. Use Reset group if you change your mind!.", page);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet("font-size:14px; color:#8FA3B8;");

    // 4 teams in each group (in it's own container)
    QWidget *groupsContainer = new QWidget(page);
    groupsContainer->setStyleSheet("background:#0B1829;");
    QGridLayout *grid = new QGridLayout(groupsContainer);
    grid->setHorizontalSpacing(18);
    grid->setVerticalSpacing(18);
    grid->setContentsMargins(8, 8, 8, 8);

    m_groupTeamButtons.clear();
    m_groupResetButtons.clear();

    for (int i = 0; i < m_groups.size(); ++i) {
        QFrame *card = new QFrame(groupsContainer);
        card->setStyleSheet("QFrame { background-color:#0F1E35; border:none; }");

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(16, 16, 16, 16);
        cardLayout->setSpacing(10);

        // Group header
        QLabel *groupTitle = new QLabel(QString("GROUP %1").arg(m_groups[i].name), card);
        groupTitle->setAlignment(Qt::AlignCenter);
        groupTitle->setStyleSheet(
            "font-family:'Press Start 2P'; font-size:11px; color:#FFFFFF;");

        QFrame *divider = new QFrame(card);
        divider->setFixedHeight(3);
        divider->setStyleSheet("background-color:#27AE60; border:none;");

        cardLayout->addWidget(groupTitle);
        cardLayout->addWidget(divider);

        // team buttons
        QVector<QPushButton *> teamButtons;
        for (int j = 0; j < m_groups[i].teams.size(); ++j) {
            QPushButton *btn = new QPushButton(card);
            btn->setMinimumHeight(38);
            cardLayout->addWidget(btn);
            teamButtons.push_back(btn);

            connect(btn, &QPushButton::clicked, this, [this, i, j]() {
                handleGroupTeamClick(i, j);
            });
        }

        // Reset button
        QPushButton *resetBtn = new QPushButton("RESET GROUP", card);
        resetBtn->setStyleSheet(pixelBtn("#1E3A5F", "#3B5F8A", "#10243D", "#FFFFFF"));
        resetBtn->setMinimumHeight(34);
        cardLayout->addSpacing(4);
        cardLayout->addWidget(resetBtn);

        connect(resetBtn, &QPushButton::clicked, this, [this, i]() {
            resetGroup(i);
        });

        m_groupTeamButtons.push_back(teamButtons);
        m_groupResetButtons.push_back(resetBtn);

        grid->addWidget(card, i / 4, i % 4);
    }

    // function to help with scroll since there are 12 teams
    QScrollArea *scrollArea = new QScrollArea(page);
    scrollArea->setWidget(groupsContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background:transparent; border:none; }"
        "QScrollBar:vertical { width:6px; background:#0B1829; }"
        "QScrollBar::handle:vertical { background:#1E3A5F; border-radius:3px; }");

    QPushButton *backBtn = new QPushButton("← BACK", page);
    backBtn->setFixedWidth(150);
    backBtn->setStyleSheet(pixelBtn("#1E3A5F", "#3B5F8A", "#10243D", "#FFFFFF"));

    m_toThirdPlaceButton = new QPushButton("CONTINUE TO THIRD PLACE →", page);
    m_toThirdPlaceButton->setStyleSheet(pixelBtn("#27AE60", "#38C172", "#145A32", "#FFFFFF"));
    m_toThirdPlaceButton->setEnabled(false);   // enabled only when all groups are ranked

    connect(backBtn, &QPushButton::clicked, this, &BracketPage::backRequested);
    connect(m_toThirdPlaceButton, &QPushButton::clicked, this, [this]() {
        collectThirdPlaceTeams();   // gather the 3rd-place team from every group
        refreshThirdPlaceUi();
        m_internalStack->setCurrentIndex(1);
    });

    QHBoxLayout *bottomRow = new QHBoxLayout;
    bottomRow->addWidget(backBtn);
    bottomRow->addStretch();
    bottomRow->addWidget(m_toThirdPlaceButton);

    root->addWidget(eyebrow);
    root->addWidget(title);
    root->addWidget(subtitle);
    root->addWidget(scrollArea, 1);
    root->addLayout(bottomRow);
    return page;
}

// ***************************************************************************
//  Third-Place Selection
//  User selects 8 teams from 12 for the KO according the WC rules

QWidget *BracketPage::buildThirdPlaceView()
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(60, 34, 60, 28);
    root->setSpacing(18);

    QLabel *eyebrow = new QLabel("BRACKET", page);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(
        "font-family:'Press Start 2P'; font-size:10px; letter-spacing:3px; color:#D4A843;");

    QLabel *title = new QLabel("Select Your Third-Place Teams", page);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:34px; font-weight:700; color:#FFFFFF;");

    QLabel *subtitle = new QLabel(
        "Pick the 8 third-place teams that you think will make it to the Round of 32.", page);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet("font-size:14px; color:#8FA3B8;");

    m_thirdPlaceCountLabel = new QLabel("0 / 8 selected", page);
    m_thirdPlaceCountLabel->setAlignment(Qt::AlignCenter);
    m_thirdPlaceCountLabel->setStyleSheet(
        "font-family:'Press Start 2P'; font-size:10px; color:#FFFFFF;");

    // 1 button for the 3rd place of that group
    QWidget *selectionContainer = new QWidget(page);
    QGridLayout *grid = new QGridLayout(selectionContainer);
    grid->setHorizontalSpacing(18);
    grid->setVerticalSpacing(18);

    m_thirdPlaceButtons.clear();

    for (int i = 0; i < 12; ++i) {
        QString groupLetter = QString(QChar('A' + i));

        QPushButton *btn = new QPushButton(selectionContainer);
        btn->setMinimumHeight(52);
        btn->setStyleSheet(thirdBtnStyle(false));

        connect(btn, &QPushButton::clicked, this, [this, groupLetter]() {
            toggleThirdPlaceSelection(groupLetter);
        });

        m_thirdPlaceButtons[groupLetter] = btn;
        grid->addWidget(btn, i / 4, i % 4);
    }

    QPushButton *backBtn = new QPushButton("← BACK", page);
    backBtn->setFixedWidth(150);
    backBtn->setStyleSheet(pixelBtn("#1E3A5F", "#3B5F8A", "#10243D", "#FFFFFF"));

    m_toKnockoutButton = new QPushButton("KNOCKOUT BRACKET →", page);
    m_toKnockoutButton->setStyleSheet(pixelBtn("#27AE60", "#38C172", "#145A32", "#FFFFFF"));
    m_toKnockoutButton->setEnabled(false);   // enabled once exactly 8 are chosen

    connect(backBtn, &QPushButton::clicked, this, [this]() {
        m_internalStack->setCurrentIndex(0);
    });
    connect(m_toKnockoutButton, &QPushButton::clicked, this, [this]() {
        seedBracket();        // build the R32 matchups from ranked groups + 8 chosen thirds
        rebuildKnockoutUi();  // render the visual bracket
        m_internalStack->setCurrentIndex(2);
    });

    QHBoxLayout *bottomRow = new QHBoxLayout;
    bottomRow->addWidget(backBtn);
    bottomRow->addStretch();
    bottomRow->addWidget(m_toKnockoutButton);

    root->addWidget(eyebrow);
    root->addWidget(title);
    root->addWidget(subtitle);
    root->addWidget(m_thirdPlaceCountLabel);
    root->addWidget(selectionContainer, 1);
    root->addLayout(bottomRow);
    return page;
}

// ***************************************************************************
//  Knockout stages view

QWidget *BracketPage::buildKnockoutView()
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Header row which has back btn, the title, and reset btn
    QWidget *topRow = new QWidget(page);
    topRow->setStyleSheet("background:#0B1829;");
    QHBoxLayout *tr = new QHBoxLayout(topRow);
    tr->setContentsMargins(20, 12, 20, 12);
    tr->setSpacing(12);

    QPushButton *backBtn = new QPushButton("← BACK", topRow);
    backBtn->setFixedWidth(130);
    backBtn->setStyleSheet(pixelBtn("#1E3A5F", "#3B5F8A", "#10243D", "#FFFFFF"));
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        m_internalStack->setCurrentIndex(1);
    });

    QLabel *ttl = new QLabel("2026 FIFA WORLD CUP — KNOCKOUT BRACKET", topRow);
    ttl->setAlignment(Qt::AlignCenter);
    ttl->setStyleSheet(
        "font-family:'Press Start 2P'; font-size:9px; color:#D4A843; letter-spacing:1px;");

    // Reset button
    QPushButton *resetBtn = new QPushButton("↺ RESET", topRow);
    resetBtn->setFixedWidth(130);
    resetBtn->setStyleSheet(pixelBtn("#7A0018", "#C8102E", "#400010", "#FFFFFF"));
    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        seedBracket();
        rebuildKnockoutUi();
    });

    tr->addWidget(backBtn);
    tr->addStretch();
    tr->addWidget(ttl);
    tr->addStretch();
    tr->addWidget(resetBtn);

    // Scrollable canvas, built in the rebuild function
    m_bracketScroll = new QScrollArea(page);
    m_bracketScroll->setWidgetResizable(true);
    m_bracketScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_bracketScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_bracketScroll->setStyleSheet(
        "QScrollArea { border:none; background:#0B1829; }"
        "QScrollBar:horizontal { height:7px; background:#0B1829; }"
        "QScrollBar::handle:horizontal { background:#1E3A5F; border-radius:3px; }"
        "QScrollBar:vertical { width:7px; background:#0B1829; }"
        "QScrollBar::handle:vertical { background:#1E3A5F; border-radius:3px; }");

    root->addWidget(topRow);
    root->addWidget(m_bracketScroll, 1);
    return page;
}

// ***************************************************************************
// Logic for all the stages:
// ***************************************************************************

// ***************************************************************************
// Group Stages

// Assigning order (rank) based on click, sets last team to 4th itself
void BracketPage::handleGroupTeamClick(int groupIndex, int teamIndex)
{
    GroupState &group = m_groups[groupIndex];
    if (group.nextRank > 4) return;            // group already complete
    if (group.ranks[teamIndex] != 0) return;   // team already ranked

    group.ranks[teamIndex] = group.nextRank;
    ++group.nextRank;

    // When 3 teams are ranked, the last one becomes 4th automatically
    if (group.nextRank == 4) {
        for (int i = 0; i < group.ranks.size(); ++i) {
            if (group.ranks[i] == 0) {
                group.ranks[i] = 4;
                group.nextRank = 5;
                break;
            }
        }
    }

    refreshGroupStageUi();
}

// Functionality for the "reset group" btn
void BracketPage::resetGroup(int groupIndex)
{
    m_groups[groupIndex].ranks    = QVector<int>(4, 0);
    m_groups[groupIndex].nextRank = 1;
    refreshGroupStageUi();
}

// allows the "Continue to 3rd place" btn to work
bool BracketPage::allGroupsComplete() const
{
    for (const GroupState &g : m_groups)
        for (int r : g.ranks)
            if (r == 0) return false;
    return true;
}

// Button label format
QString BracketPage::teamLabelForDisplay(int groupIndex, int teamIndex) const
{
    int rank = m_groups[groupIndex].ranks[teamIndex];
    QString rankText = (rank == 0) ? "[ ]" : QString("[%1]").arg(rank); // ex: [1]  Mexico, [2]  South Africa, [ ]  Haiti
    return QString("%1  %2").arg(rankText, m_groups[groupIndex].teams[teamIndex]);
}

// Return the team name for KOs
QString BracketPage::teamAtRank(const GroupState &group, int rank) const
{
    for (int i = 0; i < group.ranks.size(); ++i)
        if (group.ranks[i] == rank) return group.teams[i];
    return {};
}

// ***************************************************************************
// Third Place

// Build map from each group's ranked 3rd-place entry
void BracketPage::collectThirdPlaceTeams()
{
    m_thirdPlaceGroups.clear();
    m_thirdPlaceTeams.clear();
    m_selectedThirdGroups.clear();

    for (const GroupState &group : m_groups) {
        m_thirdPlaceGroups.push_back(group.name);
        m_thirdPlaceTeams[group.name] = teamAtRank(group, 3);
    }
}

void BracketPage::toggleThirdPlaceSelection(const QString &groupName)
{
    if (m_selectedThirdGroups.contains(groupName))
        m_selectedThirdGroups.remove(groupName);
    else if (m_selectedThirdGroups.size() < 8)
        m_selectedThirdGroups.insert(groupName);

    refreshThirdPlaceUi();
}

bool BracketPage::thirdPlaceSelectionComplete() const
{
    return m_selectedThirdGroups.size() == 8;
}

// ***************************************************************************
// Knockout Rounds

void BracketPage::seedBracket()
{
    auto winner = [&](int gi) -> BracketSlot { // Helper to collect winners
        return { teamAtRank(m_groups[gi], 1), m_groups[gi].name, 1, true };
    };
    auto runnerUp = [&](int gi) -> BracketSlot { // Helper to collect 2nd place
        return { teamAtRank(m_groups[gi], 2), m_groups[gi].name, 2, true };
    };

    // Build an ordered list of the 8 selected third-place teams
    QVector<BracketSlot> thirds;
    for (const QString &letter : m_thirdPlaceGroups) {
        if (m_selectedThirdGroups.contains(letter))
            thirds.append({ m_thirdPlaceTeams[letter], letter, 3, true });
    }
    auto third = [&](int i) -> BracketSlot {
        return (i < thirds.size()) ? thirds[i] : BracketSlot{"TBD","",0,false};
    };

    // Round of 32 match-ups based on previous WC formats
    // 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, 7=H, 8=I, 9=J, 10=K, 11=L
    // HARDCODE!!: Winners of group A-H, get the 8 3rd place teams
    using P = QPair<BracketSlot, BracketSlot>;
    QVector<P> r32 = {
        {winner(0),  third(0)},    // A1 vs 3rd place
        {winner(2),  third(1)},    // C1 vs 3rd place
        {winner(1),  third(2)},    // B1 vs 3rd place
        {winner(3),  third(3)},    // D1 vs 3rd place
        {runnerUp(0),runnerUp(1)}, // A2 vs B2
        {runnerUp(2),runnerUp(3)}, // C2 vs D2
        {winner(4),  third(4)},    // E1 vs 3rd place
        {winner(5),  third(5)},    // F1 vs 3rd place
        {winner(6),  third(6)},    // G1 vs 3rd place
        {winner(7),  third(7)},    // H1 vs 3rd place
        {runnerUp(4),runnerUp(5)}, // E2 vs F2
        {runnerUp(6),runnerUp(7)}, // G2 vs H2
        {winner(8),  runnerUp(9)}, // I1 vs J2
        {winner(10), runnerUp(11)}, // K1 vs L2
        {winner(9),  runnerUp(10)}, // J1 vs K2
        {winner(11), runnerUp(8)}, // L1 vs I2
    };

    // initializing rounds ro16 onwrds
    m_bracket.clear();
    m_bracket.resize(NUM_ROUNDS);
    m_bracket[0] = r32;   // 16

    int matchCounts[] = { 8, 4, 2, 1, 1 };  // R16 -> QF -> SF -> Final -> Champion placeholder
    for (int r = 1; r < NUM_ROUNDS; ++r) {
        m_bracket[r].resize(matchCounts[r - 1]);
        for (auto &pair : m_bracket[r]) {
            pair.first.decided  = false;
            pair.second.decided = false;
        }
    }
}

// advance the team that user clicks on to the next round
void BracketPage::advanceWinner(int round, int matchIdx, int winnerSlot)
{
    BracketSlot winSlot = (winnerSlot == 0) ? m_bracket[round][matchIdx].first : m_bracket[round][matchIdx].second;

    int nextRound = round + 1;
    if (nextRound >= NUM_ROUNDS) return;

    if (nextRound == NUM_ROUNDS - 1) {
        // write into the single champion placeholder
        m_bracket[nextRound][0].first = winSlot;
        m_bracket[nextRound][0].first.decided = true;
        return;
    }

    int nextMatch = matchIdx / 2;
    int nextSlot  = matchIdx % 2;
    auto &target = m_bracket[nextRound][nextMatch];
    if (nextSlot == 0) { target.first  = winSlot; target.first.decided  = true; }
    else               { target.second = winSlot; target.second.decided = true; }
}

// *********************************************************************************
// Knockout Rounds UI

// Building/rebuilding bracket set up
void BracketPage::rebuildKnockoutUi()
{
    if (m_bracketScroll->widget())
        delete m_bracketScroll->takeWidget();

    QWidget *canvas = new QWidget;
    canvas->setStyleSheet("background:#0B1829;");
    QVBoxLayout *cvl = new QVBoxLayout(canvas);
    cvl->setContentsMargins(16, 12, 16, 12);
    cvl->setSpacing(8);

    QWidget *legend = new QWidget(canvas);
    QHBoxLayout *ll = new QHBoxLayout(legend);
    ll->setContentsMargins(0,0,0,0);
    ll->setSpacing(14);
    ll->addStretch();
    auto addLegendItem = [&](const QString &color, const QString &label) {
        QLabel *dot = new QLabel(legend);
        dot->setFixedSize(12, 12);
        dot->setStyleSheet(QString("background:%1;").arg(color));
        QLabel *lbl = new QLabel(label, legend);
        lbl->setStyleSheet("font-family:'Press Start 2P'; font-size:6px; color:#8FA3B8;");
        ll->addWidget(dot); ll->addWidget(lbl);
    };
    addLegendItem("#1A5A2A", "Group Winner"); //green
    addLegendItem("#7A6020", "Runner-up"); //yellow
    addLegendItem("#7A0018", "3rd Place"); // red
    ll->addStretch();
    cvl->addWidget(legend);

    QLabel *hint = new QLabel("Click on the team you think makes it to the next round", canvas);
    hint->setAlignment(Qt::AlignCenter);
    hint->setStyleSheet("font-family:'Press Start 2P'; font-size:6px; color:#5A7090;");
    cvl->addWidget(hint);
    cvl->addSpacing(4);


    const QStringList roundNames = {
        "ROUND OF 32", "ROUND OF 16", "QUARTER-FINALS", "SEMI-FINALS",  "FINAL",       "CHAMPION"
    };
    QHBoxLayout *bracketH = new QHBoxLayout;
    bracketH->setSpacing(6);
    bracketH->setContentsMargins(0,0,0,0);
    for (int r = 0; r < NUM_ROUNDS; ++r)
        bracketH->addWidget(makeRoundColumn(r, roundNames[r]));

    cvl->addLayout(bracketH, 1);
    m_bracketScroll->setWidget(canvas);
}

// Building column (space) for each round
QWidget *BracketPage::makeRoundColumn(int round, const QString &roundName)
{
    QWidget *col = new QWidget;
    col->setStyleSheet("background:transparent;");
    col->setMinimumWidth(158);

    QVBoxLayout *vl = new QVBoxLayout(col);
    vl->setContentsMargins(0,0,0,0);
    vl->setSpacing(0);

    // Round label, and green underline following the app
    QLabel *hdr = new QLabel(roundName, col);
    hdr->setAlignment(Qt::AlignCenter);
    hdr->setFixedHeight(22);
    hdr->setStyleSheet(
        "font-family:'Press Start 2P'; font-size:8px; color:#27AE60; letter-spacing:1px;");
    vl->addWidget(hdr);

    QFrame *hdrLine = new QFrame(col);
    hdrLine->setFixedHeight(2);
    hdrLine->setStyleSheet("background:#27AE60; border:none;");
    vl->addWidget(hdrLine);
    vl->addSpacing(4);

    // Champions box
    if (round == NUM_ROUNDS - 1) {
        vl->addStretch();
        QWidget *champBox = new QWidget(col);
        champBox->setFixedSize(152, 72);
        champBox->setStyleSheet("background:#1A2A10; border:2px solid #D4A843;");
        QVBoxLayout *cl = new QVBoxLayout(champBox);
        cl->setContentsMargins(6,6,6,6);
        cl->setSpacing(2);

        QLabel *trophy = new QLabel("🏆", champBox);
        trophy->setAlignment(Qt::AlignCenter);
        trophy->setStyleSheet("font-size:22px; background:transparent;");

        BracketSlot &s = m_bracket[round][0].first;
        QLabel *champName = new QLabel(s.decided ? s.teamName : "?", champBox);
        champName->setAlignment(Qt::AlignCenter);
        champName->setWordWrap(true);
        champName->setStyleSheet(
            s.decided
                ? "font-family:'Press Start 2P'; font-size:8px; color:#D4A843; background:transparent;"
                : "font-family:'Press Start 2P'; font-size:8px; color:#3A5070; background:transparent;");
        cl->addWidget(trophy);
        cl->addWidget(champName);

        vl->addWidget(champBox, 0, Qt::AlignCenter);
        vl->addStretch();
        return col;
    }

    // Normal round
    int matchCount = m_bracket[round].size();
    vl->addStretch(1);
    for (int m = 0; m < matchCount; ++m) {
        vl->addWidget(makeMatchWidget(round, m));
        if (m < matchCount - 1)
            vl->addStretch(2);
    }
    vl->addStretch(1);
    return col;
}

// Matchup box/widget for the KOs
QWidget *BracketPage::makeMatchWidget(int round, int matchIdx)
{
    auto &p  = m_bracket[round][matchIdx];
    BracketSlot &s0 = p.first;
    BracketSlot &s1 = p.second;

    QWidget *w = new QWidget;
    w->setFixedWidth(154);
    w->setStyleSheet("background:transparent;");

    QVBoxLayout *vl = new QVBoxLayout(w);
    vl->setContentsMargins(0,0,0,0);
    vl->setSpacing(0);

    // Small match label above the card
    QLabel *matchLbl = new QLabel(QString("M%1").arg(matchIdx + 1), w);
    matchLbl->setStyleSheet(
        "font-family:'Press Start 2P'; font-size:6px; color:#2A4060; padding:1px 2px;");
    vl->addWidget(matchLbl);

    // Build one slot row, where user can click
    auto makeSlotRow = [&](BracketSlot &slot, int playerIdx) -> QWidget* {
        QWidget *sw = new QWidget(w);
        sw->setFixedHeight(28);
        QHBoxLayout *hl = new QHBoxLayout(sw);
        hl->setContentsMargins(3,1,3,1);
        hl->setSpacing(4);

        // Coloured group badge (green/yellow/red by rank)
        QLabel *badge = new QLabel(sw);
        badge->setFixedSize(16, 16);
        badge->setAlignment(Qt::AlignCenter);
        badge->setText(slot.decided ? slot.groupLetter : "?");
        badge->setStyleSheet(
            QString("background:%1; color:#FFFFFF; font-family:'Press Start 2P'; font-size:4px;")
                .arg(slot.decided ? rankColor(slot.groupRank) : "#1E3A5F"));

        // Clicking the name button advances this team to the next round
        QPushButton *nameBtn = new QPushButton(slot.decided ? slot.teamName : "TBD", sw);
        nameBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        nameBtn->setFixedHeight(22);
        nameBtn->setStyleSheet(
            "QPushButton { background:#0F1E35; color:" +
            QString(slot.decided ? "#FFFFFF" : "#2A4060") + ";"
                                                            "  border:1px solid #1E3A5F; border-radius:0px;"
                                                            "  font-family:'Press Start 2P'; font-size:8px;"
                                                            "  text-align:left; padding:1px 4px; }"
                                                            "QPushButton:hover { background:#1A5A2A; color:#FFFFFF; border-color:#27AE60; }"
                                                            "QPushButton:disabled { background:#0A1420; color:#1E2E40; border-color:#0F1E35; }");

        // Only enable clicking when both teams in the match are known
        bool canClick = s0.decided && s1.decided && round < NUM_ROUNDS - 1;
        if (canClick) {
            int r = round, mi = matchIdx, pi = playerIdx;
            connect(nameBtn, &QPushButton::clicked, this, [this, r, mi, pi]() {
                advanceWinner(r, mi, pi);
                rebuildKnockoutUi();   // redraw so the winner appears in the next column
            });
        } else {
            nameBtn->setEnabled(slot.decided);
        }

        hl->addWidget(badge);
        hl->addWidget(nameBtn);
        return sw;
    };

    // Outer card frame with both slot rows
    QFrame *outer = new QFrame(w);
    outer->setStyleSheet("QFrame { background:#0F1E35; border:1px solid #1E3A5F; }");
    QVBoxLayout *ol = new QVBoxLayout(outer);
    ol->setContentsMargins(0,0,0,0);
    ol->setSpacing(0);
    ol->addWidget(makeSlotRow(s0, 0));

    QFrame *sep = new QFrame(outer);
    sep->setFixedHeight(1);
    sep->setStyleSheet("background:#1E3A5F; border:none;");
    ol->addWidget(sep);

    ol->addWidget(makeSlotRow(s1, 1));
    vl->addWidget(outer);
    return w;
}

// ***************************************************************************
//  Helper methods for refresh/reset; update existing widgets in-place

void BracketPage::refreshGroupStageUi()
{
    for (int i = 0; i < m_groupTeamButtons.size(); ++i)
        for (int j = 0; j < m_groupTeamButtons[i].size(); ++j) {
            bool assigned = (m_groups[i].ranks[j] != 0);
            m_groupTeamButtons[i][j]->setText(teamLabelForDisplay(i, j));
            m_groupTeamButtons[i][j]->setStyleSheet(groupTeamBtnStyle(assigned));
        }

    if (m_toThirdPlaceButton)
        m_toThirdPlaceButton->setEnabled(allGroupsComplete());
}

void BracketPage::refreshThirdPlaceUi()
{
    for (const QString &g : m_thirdPlaceButtons.keys()) {
        bool selected = m_selectedThirdGroups.contains(g);
        QString label = QString("Group %1  —  %2").arg(g, m_thirdPlaceTeams.value(g));
        m_thirdPlaceButtons[g]->setText(label);
        m_thirdPlaceButtons[g]->setStyleSheet(thirdBtnStyle(selected));
    }

    if (m_thirdPlaceCountLabel)
        m_thirdPlaceCountLabel->setText(
            QString("%1 / 8 selected").arg(m_selectedThirdGroups.size()));

    if (m_toKnockoutButton)
        m_toKnockoutButton->setEnabled(thirdPlaceSelectionComplete());
}
