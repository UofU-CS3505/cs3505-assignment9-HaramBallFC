#include "pages/LessonMenuPage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>

LessonMenuPage::LessonMenuPage(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(new QLabel("Lessons", this))
    , m_previewLabel(new QLabel("Select a lesson to preview it.", this))
    , m_lessonList(new QListWidget(this))
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *contentLayout = new QHBoxLayout();
    QHBoxLayout *buttonRow = new QHBoxLayout();

    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setWordWrap(true);
    m_previewLabel->setMinimumWidth(260);

    QPushButton *openButton = new QPushButton("Open Selected Lesson", this);
    QPushButton *backButton = new QPushButton("Back", this);
    QPushButton *homeButton = new QPushButton("Home", this);

    contentLayout->addWidget(m_lessonList, 2);
    contentLayout->addWidget(m_previewLabel, 1);

    buttonRow->addWidget(backButton);
    buttonRow->addWidget(homeButton);
    buttonRow->addStretch();
    buttonRow->addWidget(openButton);

    mainLayout->addWidget(m_titleLabel);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(buttonRow);

    connect(m_lessonList, &QListWidget::currentItemChanged,
            this, &LessonMenuPage::updateLessonPreview);

    connect(openButton, &QPushButton::clicked, this, [this]() {
        int row = m_lessonList->currentRow();
        if (row >= 0 && row < m_lessons.size()) {
            emit lessonSelected(m_lessons.at(row).id);
        }
    });

    connect(backButton, &QPushButton::clicked,
            this, &LessonMenuPage::backRequested);
    connect(homeButton, &QPushButton::clicked,
            this, &LessonMenuPage::homeRequested);
}

void LessonMenuPage::setPageTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void LessonMenuPage::setLessons(const QVector<Lesson> &lessons)
{
    m_lessons = lessons;
    m_lessonList->clear();

    for (const Lesson &lesson : m_lessons) {
        m_lessonList->addItem(lesson.title);
    }

    if (!m_lessons.isEmpty()) {
        m_lessonList->setCurrentRow(0);
    } else {
        m_previewLabel->setText("No lessons are available yet for this section.");
    }
}

void LessonMenuPage::updateLessonPreview(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);

    if (current == nullptr) {
        m_previewLabel->setText("Select a lesson to preview it.");
        return;
    }

    int row = m_lessonList->row(current);
    if (row < 0 || row >= m_lessons.size()) {
        m_previewLabel->setText("Lesson preview unavailable.");
        return;
    }

    const Lesson &lesson = m_lessons.at(row);

    m_previewLabel->setText(
        QString("Category: %1\n\n%2")
            .arg(lesson.category, lesson.description)
    );
}
