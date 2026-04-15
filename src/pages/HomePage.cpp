#include "pages/HomePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #1a1a2e; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(60, 40, 60, 40);
    layout->setSpacing(12);

    // Trophy icon
    QLabel *iconLabel = new QLabel("⚽", this);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("font-size: 64px;");

    QLabel *titleLabel = new QLabel("HaramBall FC", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 36px; font-weight: 800; color: #f0c040;");

    QLabel *subtitleLabel = new QLabel(
        "A World Cup inspired soccer learning app built with Qt Widgets.",
        this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setWordWrap(true);
    subtitleLabel->setStyleSheet("font-size: 15px; color: #b0b0c0;");

    // Divider line
    QFrame *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("color: #3a3a5e;");

    // Styled buttons
    QString buttonStyle =
        "QPushButton {"
        "  background-color: #16213e; color: white; border: 2px solid #0f3460;"
        "  border-radius: 10px; padding: 14px 40px; font-size: 16px; font-weight: 600;"
        "  min-width: 260px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #0f3460; border-color: #f0c040;"
        "}";

    QPushButton *fanButton = new QPushButton("🏟  Fan Mode", this);
    QPushButton *playerButton = new QPushButton("⚽  Player Mode", this);
    QPushButton *bracketButton = new QPushButton("🏆  Bracket", this);

    fanButton->setStyleSheet(buttonStyle);
    playerButton->setStyleSheet(buttonStyle);
    bracketButton->setStyleSheet(buttonStyle);

    layout->addStretch();
    layout->addWidget(iconLabel);
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(10);
    layout->addWidget(divider);
    layout->addSpacing(20);
    layout->addWidget(fanButton, 0, Qt::AlignCenter);
    layout->addWidget(playerButton, 0, Qt::AlignCenter);
    layout->addWidget(bracketButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(fanButton, &QPushButton::clicked,
            this, &HomePage::fanModeClicked);
    connect(playerButton, &QPushButton::clicked,
            this, &HomePage::playerModeClicked);
    connect(bracketButton, &QPushButton::clicked,
            this, &HomePage::bracketClicked);
}
