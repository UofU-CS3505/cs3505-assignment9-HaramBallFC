#include "pages/LessonViewerPage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFrame>

LessonViewerPage::LessonViewerPage(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(new QLabel("Lesson Title", this))
    , m_categoryLabel(new QLabel("Category", this))
    , m_contentView(new QTextEdit(this))
{
    setStyleSheet("background-color: #1a1a2e; color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 24, 40, 20);
    mainLayout->setSpacing(10);

    QHBoxLayout *buttonRow = new QHBoxLayout();

    // Title
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size: 26px; font-weight: 800; color: #f0c040;");

    // Category badge
    m_categoryLabel->setAlignment(Qt::AlignCenter);
    m_categoryLabel->setStyleSheet(
        "font-size: 13px; color: #b0b0c0; "
        "background-color: #0f3460; border-radius: 6px; padding: 4px 16px;");
    m_categoryLabel->setMaximumWidth(300);

    // Divider
    QFrame *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("color: #3a3a5e;");

    // Content area
    m_contentView->setReadOnly(true);
    m_contentView->setPlaceholderText("Lesson content will appear here.");
    m_contentView->setStyleSheet(
        "QTextEdit {"
        "  background-color: #16213e; color: #e0e0f0; "
        "  border: 2px solid #0f3460; border-radius: 8px; "
        "  padding: 20px; font-size: 15px; line-height: 1.6;"
        "}"
        "QScrollBar:vertical {"
        "  background: #16213e; width: 10px; border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #0f3460; border-radius: 5px; min-height: 30px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height: 0px;"
        "}");

    // Buttons
    QString buttonStyle =
        "QPushButton {"
        "  background-color: #16213e; color: white; border: 2px solid #0f3460;"
        "  border-radius: 8px; padding: 10px 24px; font-size: 14px; font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "  background-color: #0f3460; border-color: #f0c040;"
        "}";

    QPushButton *backButton = new QPushButton("Back to Lesson Menu", this);
    QPushButton *homeButton = new QPushButton("Home", this);

    backButton->setStyleSheet(buttonStyle);
    homeButton->setStyleSheet(buttonStyle);

    buttonRow->addWidget(backButton);
    buttonRow->addWidget(homeButton);
    buttonRow->addStretch();

    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_categoryLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(divider);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(m_contentView);
    mainLayout->addLayout(buttonRow);

    connect(backButton, &QPushButton::clicked,
            this, &LessonViewerPage::backRequested);
    connect(homeButton, &QPushButton::clicked,
            this, &LessonViewerPage::homeRequested);
}

void LessonViewerPage::setLesson(const Lesson &lesson)
{
    m_titleLabel->setText(lesson.title);
    m_categoryLabel->setText(QString("Category: %1").arg(lesson.category));

    QString fullText = QString("%1\n\n%2")
                           .arg(lesson.description, lesson.content);

    m_contentView->setPlainText(fullText);
}
