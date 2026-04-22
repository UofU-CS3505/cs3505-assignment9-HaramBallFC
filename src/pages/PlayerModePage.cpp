// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
// AI was used to make this file
// PlayerModePage = hub page for Player mode (lessons + penalty game).
#include "pages/PlayerModePage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PlayerModePage::PlayerModePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    //  Top accent stripe (gold for player) 
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #D4A843;");

    // Content
    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(80, 0, 80, 0);
    bl->setSpacing(0);

    QLabel *eyebrow = new QLabel("PLAYER MODE", body);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 9px; letter-spacing: 4px; color: #D4A843;"
    );

    QLabel *titleLabel = new QLabel("Player Mode", body);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 22px; color: #FFFFFF;");

    QLabel *infoLabel = new QLabel(
        "Go through lessons, increase and test your ball knowledge\n"
        "with quizzes, and play the penalty kick game.",
        body);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 12px; color: #8FA3B8; line-height: 250%;");

    // Gold divider
    QFrame *div = new QFrame(body);
    div->setFixedSize(50, 3);
    div->setStyleSheet("background-color: #D4A843; border: none;");

    QPushButton *lessonsButton = new QPushButton("BROWSE LESSONS  \u2192", body);
    lessonsButton->setFixedHeight(50);
    lessonsButton->setMinimumWidth(280);
    lessonsButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #C8102E; color: #FFFFFF;"
        "  border-radius: 0px;"
        "  border-top: 4px solid #E83A58; border-left: 4px solid #E83A58;"
        "  border-bottom: 4px solid #7A0018; border-right: 4px solid #7A0018;"
        "  font-family: 'Press Start 2P'; font-size: 10px; letter-spacing: 1px;"
        "  padding: 10px 24px;"
        "}"
        "QPushButton:hover { background-color: #E83A58; }"
        "QPushButton:pressed {"
        "  border-top: 4px solid #7A0018; border-left: 4px solid #7A0018;"
        "  border-bottom: 4px solid #E83A58; border-right: 4px solid #E83A58; }"
    );

    QPushButton *playGameButton = new QPushButton("PLAY THE GAME", body);
    playGameButton->setFixedHeight(50);
    playGameButton->setMinimumWidth(280);
    playGameButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #D4A843; color: #0B1829;"
        "  border-radius: 0px;"
        "  border-top: 4px solid #F0C860; border-left: 4px solid #F0C860;"
        "  border-bottom: 4px solid #7A6020; border-right: 4px solid #7A6020;"
        "  font-family: 'Press Start 2P'; font-size: 10px; letter-spacing: 1px;"
        "  padding: 10px 24px;"
        "}"
        "QPushButton:hover { background-color: #F0C860; }"
        "QPushButton:pressed {"
        "  border-top: 4px solid #7A6020; border-left: 4px solid #7A6020;"
        "  border-bottom: 4px solid #F0C860; border-right: 4px solid #F0C860; }"
    );

    QPushButton *backButton = new QPushButton("\u2190 BACK", body);
    backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent; color: #8FA3B8;"
        "  border-radius: 0px;"
        "  border-top: 2px solid #2E4E7A; border-left: 2px solid #2E4E7A;"
        "  border-bottom: 2px solid #060D1A; border-right: 2px solid #060D1A;"
        "  font-family: 'Press Start 2P'; font-size: 10px; letter-spacing: 1px;"
        "  padding: 9px 22px;"
        "}"
        "QPushButton:hover { color: #D4A843; border-color: #D4A843; }"
    );

    auto centered = [](QWidget *w) -> QHBoxLayout* {
        QHBoxLayout *h = new QHBoxLayout;
        h->setContentsMargins(0, 0, 0, 0);
        h->addStretch();
        h->addWidget(w);
        h->addStretch();
        return h;
    };

    bl->addStretch(2);
    bl->addWidget(eyebrow);
    bl->addSpacing(8);
    bl->addWidget(titleLabel);
    bl->addSpacing(18);
    bl->addWidget(infoLabel);
    bl->addSpacing(28);
    bl->addLayout(centered(div));
    bl->addSpacing(24);
    bl->addLayout(centered(lessonsButton));
    bl->addSpacing(12);
    bl->addLayout(centered(playGameButton));
    bl->addSpacing(16);
    bl->addLayout(centered(backButton));
    bl->addStretch(2);

    root->addWidget(topBar);
    root->addWidget(body, 1);

    connect(lessonsButton,  &QPushButton::clicked, this, &PlayerModePage::openLessonsRequested);
    connect(backButton,     &QPushButton::clicked, this, &PlayerModePage::backRequested);
    connect(playGameButton, &QPushButton::clicked, this, &PlayerModePage::playGameClicked);
}
