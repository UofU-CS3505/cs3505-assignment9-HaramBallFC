#include "pages/JugglingGame.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

JugglingGame::JugglingGame(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Juggling Game (Coming Soon)", this);
    title->setAlignment(Qt::AlignCenter);

    QLabel *desc = new QLabel(
        "This is where the Box2D juggling game will go.\n"
        "Users will keep the ball in the air while reviewing key ideas.",
        this);
    desc->setAlignment(Qt::AlignCenter);
    desc->setWordWrap(true);

    QPushButton *backButton = new QPushButton("Back to Lessons", this);

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(desc);
    layout->addWidget(backButton);
    layout->addStretch();

    connect(backButton, &QPushButton::clicked,
            this, &JugglingGame::backRequested);
}
