#include "pages/FanModePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

FanModePage::FanModePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #2f2f2f; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Fan Mode", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700;");

    QLabel *infoLabel = new QLabel(
        "Explore lessons on soccer rules, World Cup history, and tournament formats so you can understand and enjoy every match.",
        this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("font-size: 16px; color: #d0d0d0;");

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
