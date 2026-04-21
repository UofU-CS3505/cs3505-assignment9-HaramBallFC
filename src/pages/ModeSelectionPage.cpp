// AI was used to assist with this file.
//
// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// ModeSelectionPage = choose Fan / Player / Bracket paths.
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

// ── Pixel button style builder ────────────────────────────────────────────
// bg: main color   hi: lighter highlight (top/left border)   sh: shadow (bottom/right)
static QString pixelBtn(const QString &bg, const QString &hi,
                         const QString &sh, const QString &fg)
{
    return QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: %4;"
        "  border-radius: 0px;"
        "  border-top:    4px solid %2;"
        "  border-left:   4px solid %2;"
        "  border-bottom: 4px solid %3;"
        "  border-right:  4px solid %3;"
        "  font-family: 'Press Start 2P';"
        "  font-size: 10px;"
        "  letter-spacing: 1px;"
        "  padding: 10px 16px;"
        "  min-height: 38px;"
        "}"
        "QPushButton:hover {"
        "  background-color: %2;"
        "  color: #FFFFFF;"
        "}"
        "QPushButton:pressed {"
        "  border-top:    4px solid %3;"
        "  border-left:   4px solid %3;"
        "  border-bottom: 4px solid %2;"
        "  border-right:  4px solid %2;"
        "  padding: 12px 14px 8px 18px;"
        "}"
    ).arg(bg, hi, sh, fg);
}

// ── Helper: build one mode card ───────────────────────────────────────────
static QFrame *makeCard(const QString &cardTitle,
                         const QString &desc,
                         const QString &btnText,
                         const QString &btnStyle,
                         const QString &accentColor,
                         const QString &imagePath,
                         QWidget       *parent,
                         QPushButton  **outBtn)
{
    QFrame *card = new QFrame(parent);
    // Pixel panel: sharp corners, 3D inset border in accent color
    card->setStyleSheet(QString(
        "QFrame {"
        "  background-color: #0F1E35;"
        // "  border-top:    4px solid %1;"
        // "  border-left:   4px solid %1;"
        // "  border-bottom: 4px solid %1;"
        // "  border-right:  4px solid %1;"
        "  border: none;"
        "  border-radius: 0px;"
        "}"
        ));


    card->setFixedSize(300, 460);

    QVBoxLayout *l = new QVBoxLayout(card);
    l->setContentsMargins(24, 26, 24, 24);
    l->setSpacing(10);

    // Pixel accent line at top
    //Not needed- Reman

    // QFrame *accentBar = new QFrame(card);
    // accentBar->setFixedHeight(4);
    // accentBar->setStyleSheet(
    //    QString("background-color: %1; border: none;").arg(accentColor));

    // Title in pixel font
    QLabel *ttl = new QLabel(cardTitle, card);
    ttl->setAlignment(Qt::AlignCenter);
    ttl->setWordWrap(true);
    ttl->setStyleSheet(
        "font-family: 'Press Start 2P';"
        "font-size: 16px;"
        "color: #FFFFFF;"
        "background: transparent;"
        "letter-spacing: 1px;"
    );

    //code for accent bar- Reman
    QFrame *titleDivider = new QFrame(card);
    titleDivider->setFixedHeight(3);
    titleDivider->setStyleSheet(
        QString("background-color: %1; border: none;").arg(accentColor));

    // Description in smaller regular font
    QLabel *dsc = new QLabel(desc, card);
    dsc->setWordWrap(true);
    dsc->setAlignment(Qt::AlignCenter);
    dsc->setStyleSheet(
        "font-size: 16px; color: #8FA3B8; background: transparent; line-height: 150%;"
    );

    // Card illustration
    QLabel *imgLabel = new QLabel(card);
    imgLabel->setAlignment(Qt::AlignCenter);
    imgLabel->setStyleSheet("background: transparent;");
    imgLabel->setFixedSize(110, 110);
    if (!imagePath.isEmpty()) {
        const QString fullPath = findAsset(imagePath);
        if (!fullPath.isEmpty()) {
            QPixmap px(fullPath);
            if (!px.isNull())
                imgLabel->setPixmap(
                    px.scaled(110, 110, Qt::KeepAspectRatio,
                              Qt::FastTransformation));  // pixelated, no smoothing
        }
    }

    QPushButton *btn = new QPushButton(btnText, card);
    btn->setStyleSheet(btnStyle);
    btn->setMinimumHeight(44);
    *outBtn = btn;

    //l->addWidget(accentBar);
    l->addSpacing(8);
    l->addWidget(ttl);
    l->addSpacing(6);
    l->addWidget(titleDivider);
    l->addSpacing(10);
    l->addWidget(dsc);
    l->addStretch();
    l->addWidget(imgLabel, 0, Qt::AlignCenter);
    l->addSpacing(10);
    l->addWidget(btn);

    return card;
}

ModeSelectionPage::ModeSelectionPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Top pixel stripe ──────────────────────────────────────────────────
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(6);
    topBar->setStyleSheet("background-color: #C8102E;");

    // ── Content body ──────────────────────────────────────────────────────
    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(60, 44, 60, 36);
    bl->setSpacing(0);

    // Pixel-font heading
    QLabel *heading = new QLabel("CHOOSE YOUR PATH", body);
    heading->setAlignment(Qt::AlignCenter);
    heading->setStyleSheet(
        "font-family: 'Press Start 2P';"
        "font-size: 16px;"
        "color: #D4A843;"
        "letter-spacing: 2px;"
    );

    QLabel *sub = new QLabel("How do you want to experience the World Cup?", body);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("font-size: 12px; color: #8FA3B8;" "font-family: 'Press Start 2P';");

    // ── Pixel button styles ───────────────────────────────────────────────
    // Red: #C8102E  hi=#E83A58  sh=#7A0018
    const QString redBtn  = pixelBtn("#C8102E", "#E83A58", "#7A0018", "#FFFFFF");
    // Gold: #D4A843  hi=#F0C860  sh=#7A6020
    const QString goldBtn = pixelBtn("#D4A843", "#F0C860", "#7A6020", "#0B1829");
    // Green: #1A5A2A  hi=#27AE60  sh=#0A2010
    const QString greenBtn= pixelBtn("#1A5A2A", "#27AE60", "#0A2010", "#FFFFFF");

    // ── Cards ─────────────────────────────────────────────────────────────
    QPushButton *fanBtn     = nullptr;
    QPushButton *playerBtn  = nullptr;
    QPushButton *bracketBtn = nullptr;

    QFrame *fanCard = makeCard(
        "FAN MODE",
        "Go over the lessons on\n"
        "Qualifications, Teams, Rules and History\n"
        "of the WC at your own pace.",
        "ENTER  \u2192", redBtn, "#C8102E",
        "resources/images/card_fan.png",
        body, &fanBtn);

    QFrame *playerCard = makeCard(
        "PLAYER MODE",
        "Put your ball knowledge to the test,\n"
        "play the penalty game and become\n"
        "a World Cup expert.",
        "ENTER  \u2192", goldBtn, "#D4A843",
        "resources/images/card_player.png",
        body, &playerBtn);

    QFrame *bracketCard = makeCard(
        "BRACKET",
        "Predict your own 2026 FIFA\n"
        "World Cup group stage and\n"
        "full knockout bracket.",
        "VIEW  \u2192", greenBtn, "#27AE60",
        "resources/images/card_bracket.png",
        body, &bracketBtn);

    QHBoxLayout *cards = new QHBoxLayout;
    cards->setSpacing(24);
    cards->addStretch();
    cards->addWidget(fanCard);
    cards->addWidget(playerCard);
    cards->addWidget(bracketCard);
    cards->addStretch();

    // ── Back button row ───────────────────────────────────────────────────
    QHBoxLayout *bottomRow = new QHBoxLayout;
    QPushButton *backBtn = new QPushButton("\u2190 BACK", body);
    backBtn->setFixedWidth(130);
    bottomRow->addWidget(backBtn);
    bottomRow->addStretch();

    bl->addWidget(heading);
    bl->addSpacing(10);
    bl->addWidget(sub);
    bl->addSpacing(32);
    bl->addStretch();
    bl->addLayout(cards);
    bl->addStretch();
    bl->addSpacing(24);
    bl->addLayout(bottomRow);

    root->addWidget(topBar);
    root->addWidget(body, 1);

    connect(fanBtn,     &QPushButton::clicked, this, &ModeSelectionPage::fanModeSelected);
    connect(playerBtn,  &QPushButton::clicked, this, &ModeSelectionPage::playerModeSelected);
    connect(bracketBtn, &QPushButton::clicked, this, &ModeSelectionPage::bracketSelected);
    connect(backBtn,    &QPushButton::clicked, this, &ModeSelectionPage::backRequested);
}
