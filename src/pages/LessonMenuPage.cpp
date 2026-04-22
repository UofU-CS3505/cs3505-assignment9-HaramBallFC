// AI was used to assist with this file.
//
// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// LessonMenuPage = list of lessons on left, preview on right.
#include "pages/LessonMenuPage.h"
#include "SoundManager.h"

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

    // Top header bar
    QWidget *header = new QWidget(this);
    header->setFixedHeight(68);
    header->setStyleSheet(
        "background-color: #0F1E35;"
        "border-bottom: 4px solid #C8102E;"
    );
    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(24, 0, 24, 0);

    m_titleLabel->setParent(header);
    m_titleLabel->setStyleSheet(
        "font-family: 'Press Start 2P';"
        "font-size: 12px; color: #FFFFFF; background: transparent;"
    );

    QPushButton *homeButton = new QPushButton("HOME", header);
    homeButton->setFixedWidth(110);
    homeButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #1A3050; color: #F0F4F8;"
        "  border-radius: 0px;"
        "  border-top: 3px solid #2E4E7A; border-left: 3px solid #2E4E7A;"
        "  border-bottom: 3px solid #060D1A; border-right: 3px solid #060D1A;"
        "  font-family: 'Press Start 2P'; font-size: 9px; letter-spacing: 1px;"
        "  padding: 7px 12px;"
        "}"
        "QPushButton:hover { background-color: #1E3A5F; color: #D4A843; }"
        "QPushButton:pressed {"
        "  border-top: 3px solid #060D1A; border-left: 3px solid #060D1A;"
        "  border-bottom: 3px solid #2E4E7A; border-right: 3px solid #2E4E7A; }"
    );

    hl->addWidget(m_titleLabel);
    hl->addStretch();
    hl->addWidget(homeButton);

    // Body: left panel + right panel
    QWidget *body = new QWidget(this);
    QHBoxLayout *bodyLayout = new QHBoxLayout(body);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    // Left panel: lesson list 
    QWidget *leftPanel = new QWidget(body);
    leftPanel->setFixedWidth(480);
    leftPanel->setStyleSheet(
        "background-color: #0F1E35;"
        "border-right: 4px solid #1E3A5F;"
    );
    QVBoxLayout *ll = new QVBoxLayout(leftPanel);
    ll->setContentsMargins(16, 20, 16, 16);
    ll->setSpacing(10);

    QLabel *listHeader = new QLabel("LESSONS", leftPanel);
    listHeader->setStyleSheet(
        "font-family: 'Press Start 2P';"
        "font-size: 8px; letter-spacing: 3px;"
        "color: #D4A843; background: transparent;"
    );

    // Pixel-style list widget
    m_lessonList->setParent(leftPanel);
    m_lessonList->setStyleSheet(
        "QListWidget {"
        "  background-color: #0A1628;"
        "  border-top: 3px solid #060D1A; border-left: 3px solid #060D1A;"
        "  border-bottom: 3px solid #1E3A5F; border-right: 3px solid #1E3A5F;"
        "  border-radius: 0px; padding: 4px; outline: none;"
        "  font-family: 'Press Start 2P'; font-size: 10px;"
        "}"
        "QListWidget::item {"
        "  padding: 12px 12px; border-radius: 0px;"
        "  color: #8FA3B8; margin: 3px 0px;"
        "  border-left: 3px solid transparent;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #1E3A5F; color: #F0F4F8;"
        "  border-left: 3px solid #D4A843;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #C8102E; color: #FFFFFF;"
        "  border-left: 3px solid #E83A58;"
        "}"
    );

    ll->addWidget(listHeader);
    ll->addWidget(m_lessonList, 1);

    // Right panel: preview + open button 
    QWidget *rightPanel = new QWidget(body);
    rightPanel->setStyleSheet("background-color: #0B1829;");
    QVBoxLayout *rl = new QVBoxLayout(rightPanel);
    rl->setContentsMargins(36, 32, 36, 24);
    rl->setSpacing(16);

    m_previewLabel->setParent(rightPanel);
    m_previewLabel->setWordWrap(true);
    m_previewLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_previewLabel->setStyleSheet(
        "font-family: 'Press Start 2P';"
        "font-size: 11px; color: #8FA3B8; background: transparent; line-height: 220%;"
    );

    QPushButton *openButton = new QPushButton("OPEN LESSON  \u2192", rightPanel);
    openButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #C8102E; color: #FFFFFF;"
        "  border-radius: 0px;"
        "  border-top: 4px solid #E83A58; border-left: 4px solid #E83A58;"
        "  border-bottom: 4px solid #7A0018; border-right: 4px solid #7A0018;"
        "  font-family: 'Press Start 2P'; font-size: 10px; letter-spacing: 1px;"
        "  padding: 12px 32px;"
        "}"
        "QPushButton:hover   { background-color: #E83A58; }"
        "QPushButton:pressed {"
        "  border-top: 4px solid #7A0018; border-left: 4px solid #7A0018;"
        "  border-bottom: 4px solid #E83A58; border-right: 4px solid #E83A58; }"
        "QPushButton:disabled { background-color: #3A1520; color: #6A3040;"
        "  border-color: #3A1520; }"
    );
    openButton->setFixedHeight(50);

    QHBoxLayout *openRow = new QHBoxLayout;
    openRow->addStretch();
    openRow->addWidget(openButton);

    rl->addWidget(m_previewLabel, 1);
    rl->addLayout(openRow);

    bodyLayout->addWidget(leftPanel);
    bodyLayout->addWidget(rightPanel, 1);

    //Footer bar 
    QWidget *footer = new QWidget(this);
    footer->setFixedHeight(62);
    footer->setStyleSheet(
        "background-color: #0F1E35;"
        "border-top: 4px solid #1E3A5F;"
    );
    QHBoxLayout *fl = new QHBoxLayout(footer);
    fl->setContentsMargins(20, 0, 20, 0);

    QPushButton *backButton = new QPushButton("\u2190 BACK", footer);
    backButton->setFixedWidth(120);
    backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent; color: #8FA3B8;"
        "  border-radius: 0px;"
        "  border-top: 2px solid #2E4E7A; border-left: 2px solid #2E4E7A;"
        "  border-bottom: 2px solid #060D1A; border-right: 2px solid #060D1A;"
        "  font-family: 'Press Start 2P'; font-size: 9px; letter-spacing: 1px;"
        "  padding: 8px 14px;"
        "}"
        "QPushButton:hover { color: #D4A843; border-color: #D4A843; }"
        "QPushButton:pressed {"
        "  border-top: 2px solid #060D1A; border-left: 2px solid #060D1A;"
        "  border-bottom: 2px solid #2E4E7A; border-right: 2px solid #2E4E7A; }"
    );

    fl->addWidget(backButton);
    fl->addStretch();

    root->addWidget(header);
    root->addWidget(body, 1);
    root->addWidget(footer);

    //Connections 
    connect(m_lessonList, &QListWidget::currentItemChanged,
            this, &LessonMenuPage::updateLessonPreview);

    connect(openButton, &QPushButton::clicked, this, []() { SoundManager::instance().playClick(); });
    connect(openButton, &QPushButton::clicked, this, [this]() {
        int row = m_lessonList->currentRow();
        if (row >= 0 && row < m_lessons.size())
            emit lessonSelected(m_lessons.at(row).id);
    });

    connect(backButton, &QPushButton::clicked, this, []() { SoundManager::instance().playClick(); });
    connect(backButton, &QPushButton::clicked, this, &LessonMenuPage::backRequested);
    connect(homeButton, &QPushButton::clicked, this, []() { SoundManager::instance().playClick(); });
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
