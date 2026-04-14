#include "pages/HomePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("HaramBall FC", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *subtitleLabel = new QLabel(
        "A World Cup inspired soccer learning app built with Qt Widgets.",
        this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setWordWrap(true);

    QPushButton *startButton = new QPushButton("Start Learning", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(20);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(startButton, &QPushButton::clicked,
            this, &HomePage::startClicked);
}
