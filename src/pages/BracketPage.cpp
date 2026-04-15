#include "pages/BracketPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

BracketPage::BracketPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #2f2f2f; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Bracket", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700;");

    QLabel *placeholderLabel = new QLabel(
        "Bracket coming soon.\n\n"
        "This feature is under development and will be available in a future update.",
        this);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setWordWrap(true);
    placeholderLabel->setStyleSheet("font-size: 16px; color: #d0d0d0;");

    QPushButton *backButton = new QPushButton("Back to Home", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(placeholderLabel);
    layout->addSpacing(20);
    layout->addWidget(backButton);
    layout->addStretch();

    connect(backButton, &QPushButton::clicked,
            this, &BracketPage::backRequested);
}
