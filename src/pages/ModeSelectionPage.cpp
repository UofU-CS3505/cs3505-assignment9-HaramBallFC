// AI was used to assist with this file.
#include "pages/ModeSelectionPage.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

// Walk up from the executable directory until we find the asset file.
static QString findAsset(const QString &relativePath)
{
    QDir dir(QCoreApplication::applicationDirPath());
    for (int i = 0; i < 10; ++i) {
        const QString candidate = dir.absoluteFilePath(relativePath);
        if (QFileInfo::exists(candidate))
            return candidate;
        if (!dir.cdUp())
            break;
    }
    return {};
}

// ── Helper: build one mode card ───────────────────────────────────────────
static QFrame *makeCard(const QString &icon,
                         const QString &cardTitle,
                         const QString &desc,
                         const QString &btnText,
                         const QString &btnStyle,
                         const QString &imagePath,
                         QWidget       *parent,
                         QPushButton  **outBtn)
{
    QFrame *card = new QFrame(parent);
    card->setStyleSheet(
        "QFrame {"
        "  background-color: #112035;"
        "  border: 1px solid #1E3A5F;"
        "  border-radius: 14px;"
        "}"
    );
    card->setMinimumSize(240, 280);

    QVBoxLayout *l = new QVBoxLayout(card);
    l->setContentsMargins(28, 30, 28, 28);
    l->setSpacing(10);

    QLabel *ico = new QLabel(icon, card);
    ico->setAlignment(Qt::AlignCenter);
    ico->setStyleSheet("font-size: 38px; background: transparent;");
    ico->setVisible(!icon.isEmpty());

    QLabel *ttl = new QLabel(cardTitle, card);
    ttl->setAlignment(Qt::AlignCenter);
    ttl->setStyleSheet(
        "font-size: 20px; font-weight: 700; color: #FFFFFF; background: transparent;"
    );

    QLabel *dsc = new QLabel(desc, card);
    dsc->setWordWrap(true);
    dsc->setAlignment(Qt::AlignCenter);
    dsc->setStyleSheet(
        "font-size: 13px; color: #8FA3B8; background: transparent;"
    );

    // Card illustration — loaded from resources/images/
    QLabel *imgLabel = new QLabel(card);
    imgLabel->setAlignment(Qt::AlignCenter);
    imgLabel->setStyleSheet("background: transparent;");
    imgLabel->setFixedSize(120, 120);
    if (!imagePath.isEmpty()) {
        const QString fullPath = findAsset(imagePath);
        if (!fullPath.isEmpty()) {
            QPixmap px(fullPath);
            if (!px.isNull())
                imgLabel->setPixmap(
                    px.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }

    QPushButton *btn = new QPushButton(btnText, card);
    btn->setStyleSheet(btnStyle);
    btn->setMinimumHeight(42);
    *outBtn = btn;

    l->addWidget(ico);
    l->addSpacing(4);
    l->addWidget(ttl);
    l->addWidget(dsc);
    l->addStretch();
    l->addWidget(imgLabel, 0, Qt::AlignCenter);
    l->addSpacing(12);
    l->addWidget(btn);

    return card;
}

ModeSelectionPage::ModeSelectionPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Top accent bar ────────────────────────────────────────────────────
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #C8102E;");

    // ── Content body ──────────────────────────────────────────────────────
    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(60, 50, 60, 40);
    bl->setSpacing(0);

    QLabel *heading = new QLabel("Choose Your Path", body);
    heading->setAlignment(Qt::AlignCenter);
    heading->setStyleSheet("font-size: 34px; font-weight: 700; color: #FFFFFF;");

    QLabel *sub = new QLabel("How do you want to experience the World Cup?", body);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("font-size: 15px; color: #8FA3B8;");

    // ── Button styles ─────────────────────────────────────────────────────
    const QString redBtn =
        "QPushButton { background-color: #C8102E; color: #FFFFFF; border: none;"
        "  border-radius: 8px; font-size: 14px; font-weight: 700; }"
        "QPushButton:hover   { background-color: #A50D26; }"
        "QPushButton:pressed { background-color: #8A0A1F; }";

    const QString goldBtn =
        "QPushButton { background-color: #D4A843; color: #0B1829; border: none;"
        "  border-radius: 8px; font-size: 14px; font-weight: 700; }"
        "QPushButton:hover   { background-color: #B8922E; color: #FFFFFF; }"
        "QPushButton:pressed { background-color: #9A7A24; }";

    const QString greenBtn =
        "QPushButton { background-color: #1A4A2A; color: #FFFFFF;"
        "  border: 1px solid #27AE60; border-radius: 8px;"
        "  font-size: 14px; font-weight: 700; }"
        "QPushButton:hover   { background-color: #27AE60; }"
        "QPushButton:pressed { background-color: #1E8449; }";

    // ── Cards ─────────────────────────────────────────────────────────────
    QPushButton *fanBtn     = nullptr;
    QPushButton *playerBtn  = nullptr;
    QPushButton *bracketBtn = nullptr;

    QFrame *fanCard = makeCard(
        "", "Fan Mode",
        "Learn the rules, history, and key moments\n"
        "of the World Cup so you can follow\n"
        "every match with confidence.",
        "Enter Fan Mode  \u2192", redBtn,
        "resources/images/card_fan.png",
        body, &fanBtn);

    QFrame *playerCard = makeCard(
        "", "Player Mode",
        "Go through lessons on rules and strategy,\n"
        "test your knowledge with quizzes,\n"
        "and play the penalty kick game.",
        "Enter Player Mode  \u2192", goldBtn,
        "resources/images/card_player.png",
        body, &playerBtn);

    QFrame *bracketCard = makeCard(
        "", "Bracket",
        "View and explore the 2026 FIFA World Cup\n"
        "tournament bracket.\n"
        "(Coming soon)",
        "View Bracket  \u2192", greenBtn,
        "resources/images/card_bracket.png",
        body, &bracketBtn);

    QHBoxLayout *cards = new QHBoxLayout;
    cards->setSpacing(20);
    cards->addWidget(fanCard);
    cards->addWidget(playerCard);
    cards->addWidget(bracketCard);

    // ── Back button row ───────────────────────────────────────────────────
    QHBoxLayout *bottomRow = new QHBoxLayout;
    QPushButton *backBtn = new QPushButton("\u2190 Back", body);
    backBtn->setFixedWidth(110);
    bottomRow->addWidget(backBtn);
    bottomRow->addStretch();

    bl->addWidget(heading);
    bl->addSpacing(8);
    bl->addWidget(sub);
    bl->addSpacing(36);
    bl->addLayout(cards, 1);
    bl->addSpacing(28);
    bl->addLayout(bottomRow);

    root->addWidget(topBar);
    root->addWidget(body, 1);

    connect(fanBtn,     &QPushButton::clicked, this, &ModeSelectionPage::fanModeSelected);
    connect(playerBtn,  &QPushButton::clicked, this, &ModeSelectionPage::playerModeSelected);
    connect(bracketBtn, &QPushButton::clicked, this, &ModeSelectionPage::bracketSelected);
    connect(backBtn,    &QPushButton::clicked, this, &ModeSelectionPage::backRequested);
}
