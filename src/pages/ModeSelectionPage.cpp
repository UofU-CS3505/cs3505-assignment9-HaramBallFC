#include "pages/ModeSelectionPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ModeSelectionPage::ModeSelectionPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Choose Your Learning Path", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *descriptionLabel = new QLabel(
        "Fan Mode can focus on watching, tournament history, and match understanding.\n"
        "Player Mode can focus on positions, movement, and strategy.",
        this);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setWordWrap(true);

    QPushButton *fanButton = new QPushButton("Fan Mode", this);
    QPushButton *playerButton = new QPushButton("Player Mode", this);
    QPushButton *backButton = new QPushButton("Back", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(descriptionLabel);
    layout->addSpacing(20);
    layout->addWidget(fanButton);
    layout->addWidget(playerButton);
    layout->addWidget(backButton);
    layout->addStretch();

    connect(fanButton, &QPushButton::clicked,
            this, &ModeSelectionPage::fanModeSelected);
    connect(playerButton, &QPushButton::clicked,
            this, &ModeSelectionPage::playerModeSelected);
    connect(backButton, &QPushButton::clicked,
            this, &ModeSelectionPage::backRequested);
}
