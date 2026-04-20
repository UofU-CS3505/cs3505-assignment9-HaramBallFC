#include "pages/JugglingGame.h"
#include "pages/JugglingGameCanvas.h"

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// JugglingGame = wrapper page that hosts `JugglingGameCanvas` plus a back button.

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

JugglingGame::JugglingGame(QVector<QString> facts, QWidget *parent)
    : QWidget(parent), world(nullptr), ball(nullptr), timer(new QTimer(this)), jugglesCount(0), facts(facts)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Top accent stripe ─────────────────────────────────────────────────
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #D4A843;");

    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(80, 0, 80, 0);
    bl->setSpacing(0);

    QLabel *eyebrow = new QLabel("JUGGLING GAME", body);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(
        "font-size: 11px; font-weight: 700; letter-spacing: 4px; color: #D4A843;"
    );

    QLabel *title = new QLabel("Juggling Game", body);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 42px; font-weight: 700; color: #FFFFFF;");

    // Gold divider
    QFrame *div = new QFrame(body);
    div->setFixedSize(50, 3);
    div->setStyleSheet("background-color: #D4A843; border: none;");

    QPushButton *backButton = new QPushButton("\u2190 Back to Lessons", body);
    backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent; color: #8FA3B8;"
        "  border: 1px solid #1E3A5F; border-radius: 8px;"
        "  padding: 9px 22px; font-size: 13px;"
        "}"
        "QPushButton:hover { color: #FFFFFF; border-color: #D4A843; }"
    );

    auto centered = [](QWidget *w) -> QHBoxLayout* {
        QHBoxLayout *h = new QHBoxLayout;
        h->setContentsMargins(0, 0, 0, 0);
        h->addStretch();
        h->addWidget(w);
        h->addStretch();
        return h;
    };

    JugglingGameCanvas* canvas = new JugglingGameCanvas(body);
    canvas -> setMinimumHeight(400);
    canvas -> setFocus();

    bl->addSpacing(0);
    bl->addWidget(eyebrow);
    bl->addSpacing(4);
    bl->addWidget(title);
    bl->addSpacing(14);
    bl->addWidget(canvas, 1);
    bl->addSpacing(16);
    bl->addLayout(centered(backButton));
    bl->addSpacing(16);


    root->addWidget(topBar);
    root->addWidget(body, 1);

    connect(backButton, &QPushButton::clicked, this, &JugglingGame::backRequested);
}

// FUNCTION IMPLEMENTATIONS

void JugglingGame::paintEvent(QPaintEvent*){}

void JugglingGame::keyPressEvent(QKeyEvent*){}

void JugglingGame::tick(){}
