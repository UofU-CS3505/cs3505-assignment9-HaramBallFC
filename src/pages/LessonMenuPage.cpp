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
    setStyleSheet("background-color: #1a1a2e; color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 20);
    mainLayout->setSpacing(14);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    QHBoxLayout *buttonRow = new QHBoxLayout();

    // Title
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size: 28px; font-weight: 800; color: #f0c040;");

    // Lesson list styling
    m_lessonList->setStyleSheet(
        "QListWidget {"
        "  background-color: #16213e; color: white; border: 2px solid #0f3460;"
        "  border-radius: 8px; font-size: 15px; padding: 6px;"
        "}"
        "QListWidget::item {"
        "  padding: 12px; border-bottom: 1px solid #0f3460;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #0f3460; color: #f0c040;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #1c2a4a;"
        "}");

    // Preview panel
    m_previewLabel->setWordWrap(true);
    m_previewLabel->setMinimumWidth(280);
    m_previewLabel->setStyleSheet(
        "font-size: 14px; color: #c0c0d0; "
        "background-color: #16213e; border: 2px solid #0f3460; "
        "border-radius: 8px; padding: 20px;");
    m_previewLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // Button styling
    QString buttonStyle =
        "QPushButton {"
        "  background-color: #16213e; color: white; border: 2px solid #0f3460;"
        "  border-radius: 8px; padding: 10px 24px; font-size: 14px; font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "  background-color: #0f3460; border-color: #f0c040;"
        "}";

    QString openStyle =
        "QPushButton {"
        "  background-color: #0f3460; color: #f0c040; border: 2px solid #f0c040;"
        "  border-radius: 8px; padding: 10px 24px; font-size: 14px; font-weight: 700;"
        "}"
        "QPushButton:hover {"
        "  background-color: #f0c040; color: #1a1a2e;"
        "}";

    QPushButton *openButton = new QPushButton("Open Selected Lesson", this);
    QPushButton *backButton = new QPushButton("Back", this);
    QPushButton *homeButton = new QPushButton("Home", this);

    openButton->setStyleSheet(openStyle);
    backButton->setStyleSheet(buttonStyle);
    homeButton->setStyleSheet(buttonStyle);

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
