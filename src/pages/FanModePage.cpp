// AI was used to assist with this file.
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

    // ── Top accent stripe ─────────────────────────────────────────────────
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #C8102E;");

    // ── Content ───────────────────────────────────────────────────────────
    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(80, 0, 80, 0);
    bl->setSpacing(0);

    QLabel *eyebrow = new QLabel("FAN MODE", body);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(
        "font-size: 11px; font-weight: 700; letter-spacing: 4px; color: #D4A843;"
    );

    QLabel *titleLabel = new QLabel("Fan Mode", body);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 42px; font-weight: 700; color: #FFFFFF;");

    QLabel *infoLabel = new QLabel(
        "Explore lessons on soccer rules, World Cup history, and tournament\n"
        "formats so you can understand and enjoy every match.",
        body);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("font-size: 16px; color: #8FA3B8;");

    // Gold divider
    QFrame *div = new QFrame(body);
    div->setFixedSize(50, 3);
    div->setStyleSheet("background-color: #D4A843; border: none;");

    QPushButton *lessonsButton = new QPushButton("Browse Lessons  \u2192", body);
    lessonsButton->setFixedSize(240, 50);
    lessonsButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #C8102E; color: #FFFFFF;"
        "  border: none; border-radius: 25px;"
        "  font-size: 15px; font-weight: 700;"
        "}"
        "QPushButton:hover   { background-color: #A50D26; }"
        "QPushButton:pressed { background-color: #8A0A1F; }"
    );

    QPushButton *backButton = new QPushButton("\u2190 Back to Mode Selection", body);
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
