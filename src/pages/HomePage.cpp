#include "pages/HomePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #2f2f2f; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("HaramBall FC", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700;");

    QLabel *subtitleLabel = new QLabel(
        "Welcome to our educational app about the World Cup 2026. We wanted to help users learn about the upcoming FIFA World Cup hosted in North America through lessons, quizzes, and interactive features designed for both fans.",
        this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setWordWrap(true);
    subtitleLabel->setStyleSheet("font-size: 16px; color: #d0d0d0;");

    QLabel *teamLabel = new QLabel(
        "Created by: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan, Henish Patel, Reman Adhikari, Ethan Cobler",
        this);
    teamLabel->setAlignment(Qt::AlignCenter);
    teamLabel->setWordWrap(true);
    teamLabel->setStyleSheet("font-size: 14px; color: #d0d0d0;");

    QPushButton *startButton = new QPushButton("Start your journey!", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addWidget(teamLabel);
    layout->addSpacing(20);
    layout->addWidget(startButton);
    layout->addStretch();

    connect(startButton, &QPushButton::clicked,
            this, &HomePage::startClicked);
}
