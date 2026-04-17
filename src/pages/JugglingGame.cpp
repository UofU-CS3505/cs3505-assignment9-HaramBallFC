#include "pages/JugglingGame.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

JugglingGame::JugglingGame(QWidget *parent)
    : QWidget(parent)
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

    QLabel *comingSoon = new QLabel("Coming Soon", body);
    comingSoon->setAlignment(Qt::AlignCenter);
    comingSoon->setStyleSheet(
        "font-size: 13px; font-weight: 700; letter-spacing: 2px;"
        "color: #D4A843; background-color: #1A2A42;"
        "border: 1px solid #D4A843; border-radius: 12px;"
        "padding: 4px 16px;"
    );

    QLabel *desc = new QLabel(
        "Keep the ball in the air using Box2D physics while reviewing\n"
        "key ideas from the Fan Mode lessons.",
        body);
    desc->setAlignment(Qt::AlignCenter);
    desc->setWordWrap(true);
    desc->setStyleSheet("font-size: 15px; color: #8FA3B8;");

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

    bl->addStretch(2);
    bl->addWidget(eyebrow);
    bl->addSpacing(8);
    bl->addWidget(title);
    bl->addSpacing(14);
    bl->addLayout(centered(comingSoon));
    bl->addSpacing(20);
    bl->addWidget(desc);
    bl->addSpacing(28);
    bl->addLayout(centered(div));
    bl->addSpacing(28);
    bl->addLayout(centered(backButton));
    bl->addStretch(2);

    root->addWidget(topBar);
    root->addWidget(body, 1);

    connect(backButton, &QPushButton::clicked, this, &JugglingGame::backRequested);
}
