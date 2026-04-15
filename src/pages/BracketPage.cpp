#include "pages/BracketPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

BracketPage::BracketPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Bracket", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *placeholderLabel = new QLabel(
        "Bracket coming soon.\n\n"
        "This feature is under development and will be available in a future update.",
        this);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setWordWrap(true);

    QPushButton *backButton = new QPushButton("Back to Home", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(placeholderLabel);
    layout->addSpacing(20);
    layout->addWidget(backButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(backButton, &QPushButton::clicked,
            this, &BracketPage::backRequested);
}
