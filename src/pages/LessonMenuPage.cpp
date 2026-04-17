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
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Top header bar ────────────────────────────────────────────────────
    QWidget *header = new QWidget(this);
    header->setFixedHeight(64);
    header->setStyleSheet(
        "background-color: #0F1E35;"
        "border-bottom: 1px solid #1E3A5F;"
    );
    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(24, 0, 24, 0);

    m_titleLabel->setParent(header);
    m_titleLabel->setStyleSheet(
        "font-size: 20px; font-weight: 700; color: #FFFFFF; background: transparent;"
    );

    QPushButton *homeButton = new QPushButton("Home", header);
    homeButton->setFixedWidth(100);

    hl->addWidget(m_titleLabel);
    hl->addStretch();
    hl->addWidget(homeButton);

    // ── Body: left panel + right panel ───────────────────────────────────
    QWidget *body = new QWidget(this);
    QHBoxLayout *bodyLayout = new QHBoxLayout(body);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    // ── Left panel: lesson list ───────────────────────────────────────────
    QWidget *leftPanel = new QWidget(body);
    leftPanel->setFixedWidth(290);
    leftPanel->setStyleSheet(
        "background-color: #0F1E35;"
        "border-right: 1px solid #1E3A5F;"
    );
    QVBoxLayout *ll = new QVBoxLayout(leftPanel);
    ll->setContentsMargins(16, 20, 16, 16);
    ll->setSpacing(10);

    QLabel *listHeader = new QLabel("LESSONS", leftPanel);
    listHeader->setStyleSheet(
        "font-size: 10px; font-weight: 700; letter-spacing: 3px;"
        "color: #D4A843; background: transparent;"
    );

    m_lessonList->setParent(leftPanel);

    ll->addWidget(listHeader);
    ll->addWidget(m_lessonList, 1);

    // ── Right panel: preview + open button ───────────────────────────────
    QWidget *rightPanel = new QWidget(body);
    rightPanel->setStyleSheet("background-color: #0B1829;");
    QVBoxLayout *rl = new QVBoxLayout(rightPanel);
    rl->setContentsMargins(36, 32, 36, 24);
    rl->setSpacing(16);

    m_previewLabel->setParent(rightPanel);
    m_previewLabel->setWordWrap(true);
    m_previewLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_previewLabel->setStyleSheet(
        "font-size: 15px; color: #8FA3B8; background: transparent;"
    );

    QPushButton *openButton = new QPushButton("Open Lesson  \u2192", rightPanel);
    openButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #C8102E; color: #FFFFFF;"
        "  border: none; border-radius: 8px;"
        "  padding: 12px 32px; font-size: 14px; font-weight: 700;"
        "}"
        "QPushButton:hover   { background-color: #A50D26; }"
        "QPushButton:pressed { background-color: #8A0A1F; }"
        "QPushButton:disabled { background-color: #3A1520; color: #6A3040; }"
    );
    openButton->setFixedHeight(46);

    QHBoxLayout *openRow = new QHBoxLayout;
    openRow->addStretch();
    openRow->addWidget(openButton);

    rl->addWidget(m_previewLabel, 1);
    rl->addLayout(openRow);

    bodyLayout->addWidget(leftPanel);
    bodyLayout->addWidget(rightPanel, 1);

    // ── Footer bar ────────────────────────────────────────────────────────
    QWidget *footer = new QWidget(this);
    footer->setFixedHeight(58);
    footer->setStyleSheet(
        "background-color: #0F1E35;"
        "border-top: 1px solid #1E3A5F;"
    );
    QHBoxLayout *fl = new QHBoxLayout(footer);
    fl->setContentsMargins(20, 0, 20, 0);

    QPushButton *backButton = new QPushButton("\u2190 Back", footer);
    backButton->setFixedWidth(110);

    fl->addWidget(backButton);
    fl->addStretch();

    root->addWidget(header);
    root->addWidget(body, 1);
    root->addWidget(footer);

    // ── Connections ───────────────────────────────────────────────────────
    connect(m_lessonList, &QListWidget::currentItemChanged,
            this, &LessonMenuPage::updateLessonPreview);

    connect(openButton, &QPushButton::clicked, this, [this]() {
        int row = m_lessonList->currentRow();
        if (row >= 0 && row < m_lessons.size())
            emit lessonSelected(m_lessons.at(row).id);
    });

    connect(backButton, &QPushButton::clicked, this, &LessonMenuPage::backRequested);
    connect(homeButton, &QPushButton::clicked, this, &LessonMenuPage::homeRequested);
}

void LessonMenuPage::setPageTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void LessonMenuPage::setLessons(const QVector<Lesson> &lessons)
{
    m_lessons = lessons;
    m_lessonList->clear();

    for (const Lesson &lesson : m_lessons)
        m_lessonList->addItem(lesson.title);

    if (!m_lessons.isEmpty())
        m_lessonList->setCurrentRow(0);
    else
        m_previewLabel->setText("No lessons are available yet for this section.");
}

void LessonMenuPage::updateLessonPreview(QListWidgetItem *current,
                                          QListWidgetItem *previous)
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
