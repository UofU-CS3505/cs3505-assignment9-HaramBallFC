// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
// AI was used to make this file
// FanModePage = hub page for Fan mode (go to lessons, back nav).
#include "pages/FanModePage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

FanModePage::FanModePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    //Top accent stripe
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #C8102E;");

    //Content 
    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(80, 0, 80, 0);
    bl->setSpacing(0);

    QLabel *eyebrow = new QLabel("FAN MODE", body);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 9px; letter-spacing: 4px; color: #D4A843;"
    );

    QLabel *titleLabel = new QLabel("Fan Mode", body);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 22px; color: #FFFFFF;");

    QLabel *infoLabel = new QLabel(
        "Explore lessons on the World cup qualifications, countries represented, rules, \n"
        " and history so you can understand and enjoy every match.",
        body);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 16px; color: #8FA3B8; line-height: 250%;");

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
    bl->addSpacing(30);
    bl->addLayout(centered(lessonsButton));
    bl->addSpacing(16);
    bl->addLayout(centered(backButton));
    bl->addStretch(2);

    root->addWidget(topBar);
    root->addWidget(body, 1);

    connect(lessonsButton, &QPushButton::clicked, this, &FanModePage::openLessonsRequested);
    connect(backButton,    &QPushButton::clicked, this, &FanModePage::backRequested);
}
