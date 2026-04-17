// AI was used to assist with this file.
#include "pages/BracketPage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

BracketPage::BracketPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Top accent stripe ─────────────────────────────────────────────────
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #27AE60;");

    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(80, 0, 80, 0);
    bl->setSpacing(0);

    QLabel *eyebrow = new QLabel("BRACKET", body);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(
        "font-size: 11px; font-weight: 700; letter-spacing: 4px; color: #D4A843;"
    );

    QLabel *titleLabel = new QLabel("Tournament Bracket", body);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 42px; font-weight: 700; color: #FFFFFF;");

    QLabel *placeholderLabel = new QLabel(
        "The full 2026 FIFA World Cup bracket is coming soon.\n\n"
        "This feature is currently under development and will show\n"
        "the complete tournament structure once it is ready.",
        body);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setWordWrap(true);
    placeholderLabel->setStyleSheet("font-size: 15px; color: #8FA3B8;");

    // Gold divider
    QFrame *div = new QFrame(body);
    div->setFixedSize(50, 3);
    div->setStyleSheet("background-color: #D4A843; border: none;");

    QPushButton *backButton = new QPushButton("\u2190 Back to Home", body);
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
    bl->addSpacing(24);
    bl->addWidget(placeholderLabel);
    bl->addSpacing(28);
    bl->addLayout(centered(div));
    bl->addSpacing(28);
    bl->addLayout(centered(backButton));
    bl->addStretch(2);

    root->addWidget(topBar);
    root->addWidget(body, 1);

    connect(backButton, &QPushButton::clicked, this, &BracketPage::backRequested);
}
