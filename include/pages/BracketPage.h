#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// BracketPage lets user predict group ranks then play through knockouts.

#include <QMap>
#include <QSet>
#include <QStringList>
#include <QVector>
#include <QWidget>

class QLabel;
class QPushButton;
class QScrollArea;
class QStackedWidget;

// Bracket flow:
// Group Stage: user clicks teams to rank them 1st-4th
// Third-Place Selection: user selects 8 of the 12 third-place teams
// Knockout Rounds: Ro32 → Ro16 → QF → SF → Final → Champion

class BracketPage : public QWidget
{
    Q_OBJECT

public:
    explicit BracketPage(QWidget *parent = nullptr);

signals:
    void backRequested();

private:
    // Group Stage
    struct GroupState {
        QString     name;
        QStringList teams;
        QVector<int> ranks;
        int         nextRank = 1;
    };

    // Bracket Sots: for team in the KOs
    struct BracketSlot {
        QString teamName;
        QString groupLetter;
        int groupRank = 0;
        bool decided   = false;
    };

    // Build UI
    void buildUi();
    QWidget *buildGroupStageView();
    QWidget *buildThirdPlaceView();
    QWidget *buildKnockoutView();

    // Group Stage logic
    void    initializeGroups();
    void    handleGroupTeamClick(int groupIndex, int teamIndex);
    void    resetGroup(int groupIndex);
    bool    allGroupsComplete() const;
    QString teamLabelForDisplay(int groupIndex, int teamIndex) const;
    QString teamAtRank(const GroupState &group, int rank) const;

    // Third-place selection logic
    void collectThirdPlaceTeams();
    void toggleThirdPlaceSelection(const QString &groupName);
    bool thirdPlaceSelectionComplete() const;

    // Knockout round logic
    void    seedBracket();
    void    advanceWinner(int round, int matchIdx, int winnerSlot);
    QWidget *makeRoundColumn(int round, const QString &roundName);
    QWidget *makeMatchWidget(int round, int matchIdx);

    // Helpers for refresh
    void refreshGroupStageUi();
    void refreshThirdPlaceUi();
    void rebuildKnockoutUi();

    // style helper
    static QString pixelBtn(const QString &bg, const QString &hi, const QString &sh, const QString &fg);

    // Navigation
    QStackedWidget *m_internalStack = nullptr;

    // Group Stage data and UI
    QVector<GroupState> m_groups;
    QVector<QVector<QPushButton *>> m_groupTeamButtons;
    QVector<QPushButton *> m_groupResetButtons;
    QPushButton *m_toThirdPlaceButton = nullptr;

    // Third-place data and UI
    QStringList m_thirdPlaceGroups;
    QMap<QString, QString>   m_thirdPlaceTeams;
    QSet<QString> m_selectedThirdGroups;
    QMap<QString, QPushButton *> m_thirdPlaceButtons;
    QLabel *m_thirdPlaceCountLabel = nullptr;
    QPushButton *m_toKnockoutButton = nullptr;

    // Knockout bracket data and UI
    static const int NUM_ROUNDS = 6;
    QVector<QVector<QPair<BracketSlot, BracketSlot>>> m_bracket;
    QScrollArea *m_bracketScroll = nullptr;
};
