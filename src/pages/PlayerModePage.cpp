#include "pages/PlayerModePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PlayerModePage::PlayerModePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Player Mode", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *infoLabel = new QLabel(
        "This section can later focus on roles, positioning, strategy, and player decision-making.",
        this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);

    QPushButton *lessonsButton = new QPushButton("Open Player Lessons", this);
    QPushButton *backButton = new QPushButton("Back to Mode Selection", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);
    layout->addSpacing(20);
    layout->addWidget(lessonsButton);
    layout->addWidget(backButton);
    layout->addStretch();

    connect(lessonsButton, &QPushButton::clicked,
            this, &PlayerModePage::openLessonsRequested);
    connect(backButton, &QPushButton::clicked,
            this, &PlayerModePage::backRequested);
}
