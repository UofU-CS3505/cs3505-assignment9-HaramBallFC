#include "pages/FanModePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

FanModePage::FanModePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Fan Mode", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *infoLabel = new QLabel(
        "This section can later focus on tournament history, famous matches, fan knowledge, and match viewing skills.",
        this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);

    QPushButton *lessonsButton = new QPushButton("Open Fan Lessons", this);
    QPushButton *backButton = new QPushButton("Back to Mode Selection", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);
    layout->addSpacing(20);
    layout->addWidget(lessonsButton);
    layout->addWidget(backButton);
    layout->addStretch();

    connect(lessonsButton, &QPushButton::clicked,
            this, &FanModePage::openLessonsRequested);
    connect(backButton, &QPushButton::clicked,
            this, &FanModePage::backRequested);
}
