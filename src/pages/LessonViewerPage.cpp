#include "pages/LessonViewerPage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

LessonViewerPage::LessonViewerPage(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(new QLabel("Lesson Title", this))
    , m_categoryLabel(new QLabel("Category", this))
    , m_contentView(new QTextEdit(this))
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonRow = new QHBoxLayout();

    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_categoryLabel->setAlignment(Qt::AlignCenter);

    m_contentView->setReadOnly(true);
    m_contentView->setPlaceholderText("Lesson content will appear here.");

    QPushButton *backButton = new QPushButton("Back to Lesson Menu", this);
    QPushButton *homeButton = new QPushButton("Home", this);

    buttonRow->addWidget(backButton);
    buttonRow->addWidget(homeButton);
    buttonRow->addStretch();

    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_categoryLabel);
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
