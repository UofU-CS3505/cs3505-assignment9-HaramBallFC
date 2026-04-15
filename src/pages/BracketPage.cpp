#include "pages/BracketPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>

BracketPage::BracketPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #1a1a2e; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(60, 40, 60, 40);
    layout->setSpacing(16);

    QLabel *iconLabel = new QLabel("🏆", this);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("font-size: 72px;");

    QLabel *titleLabel = new QLabel("Bracket", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: 800; color: #f0c040;");

    QFrame *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("color: #3a3a5e;");

    QLabel *placeholderLabel = new QLabel(
        "Bracket coming soon.\n\n"
        "This feature is under development and will be\n"
        "available in a future update.",
        this);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setWordWrap(true);
    placeholderLabel->setStyleSheet(
        "font-size: 16px; color: #b0b0c0; "
        "background-color: #16213e; border: 1px solid #0f3460; "
        "border-radius: 10px; padding: 30px;");

    QString buttonStyle =
        "QPushButton {"
        "  background-color: #16213e; color: white; border: 2px solid #0f3460;"
        "  border-radius: 10px; padding: 12px 36px; font-size: 15px; font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "  background-color: #0f3460; border-color: #f0c040;"
        "}";

    QPushButton *backButton = new QPushButton("Back to Home", this);
    backButton->setStyleSheet(buttonStyle);

    layout->addStretch();
    layout->addWidget(iconLabel);
    layout->addWidget(titleLabel);
    layout->addWidget(divider);
    layout->addSpacing(10);
    layout->addWidget(placeholderLabel, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(backButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(backButton, &QPushButton::clicked,
            this, &BracketPage::backRequested);
}
