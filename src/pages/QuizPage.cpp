#include "pages/QuizPage.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>
#include <QVBoxLayout>

#include <algorithm>
#include <random>

// ── Shared style helpers ──
static const QString kDark      = "#2f2f2f";
static const QString kCard      = "#3a3a3a";
static const QString kGreen     = "#27ae60";
static const QString kRed       = "#e74c3c";
static const QString kBlue      = "#2980b9";
static const QString kBlueDark  = "#1f6391";
static const QString kTextLight = "#d0d0d0";
static const QString kTextMuted = "#a0a0a0";

static QString answerBtnStyle(const QString &bg, const QString &hover)
{
    return QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 14px 10px;"
        "  font-size: 14px;"
        "  text-align: left;"
        "}"
        "QPushButton:hover { background-color: %2; }"
        "QPushButton:disabled { color: #c0c0c0; }"
    ).arg(bg, hover);
}

// ─────────────────────────────────────────────────────
QuizPage::QuizPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(QString("background-color: %1; color: white;").arg(kDark));

    // ── Master stacked layout: quiz view vs results view ──
    QStackedLayout *stack = new QStackedLayout(this);
    stack->setContentsMargins(0, 0, 0, 0);

    // ════════════════════════════════
    //  QUIZ WIDGET
    // ════════════════════════════════
    m_quizWidget = new QWidget(this);
    QVBoxLayout *quizLayout = new QVBoxLayout(m_quizWidget);
    quizLayout->setContentsMargins(50, 30, 50, 24);
    quizLayout->setSpacing(14);

    // ── Top row: progress + score ──
    QHBoxLayout *topRow = new QHBoxLayout();
    m_progressLabel = new QLabel(this);
    m_progressLabel->setStyleSheet(
        QString("font-size: 14px; color: %1;").arg(kTextMuted));

    m_scoreLabel = new QLabel(this);
    m_scoreLabel->setAlignment(Qt::AlignRight);
    m_scoreLabel->setStyleSheet(
        QString("font-size: 14px; color: %1;").arg(kTextMuted));

    topRow->addWidget(m_progressLabel);
    topRow->addStretch();
    topRow->addWidget(m_scoreLabel);

    // ── Question label ──
    m_questionLabel = new QLabel(this);
    m_questionLabel->setWordWrap(true);
    m_questionLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_questionLabel->setStyleSheet(
        QString(
            "background-color: %1;"
            "border-radius: 8px;"
            "padding: 18px;"
            "font-size: 17px;"
            "font-weight: 600;"
            "color: white;"
        ).arg(kCard));
    m_questionLabel->setMinimumHeight(90);

    // ── Answer buttons (2 × 2 grid) ──
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(10);

    for (int i = 0; i < 4; ++i) {
        m_answerButtons[i] = new QPushButton(this);
        m_answerButtons[i]->setCursor(Qt::PointingHandCursor);
        m_answerButtons[i]->setStyleSheet(answerBtnStyle(kCard, "#4a4a4a"));
        m_answerButtons[i]->setMinimumHeight(60);
        grid->addWidget(m_answerButtons[i], i / 2, i % 2);

        connect(m_answerButtons[i], &QPushButton::clicked, this, [this, i]() {
            onAnswerSelected(i);
        });
    }

    // ── Feedback label ──
    m_feedbackLabel = new QLabel(this);
    m_feedbackLabel->setWordWrap(true);
    m_feedbackLabel->setAlignment(Qt::AlignCenter);
    m_feedbackLabel->setStyleSheet("font-size: 15px; font-weight: 600; padding: 8px;");
    m_feedbackLabel->hide();

    // ── Next button ──
    m_nextButton = new QPushButton("Next Question →", this);
    m_nextButton->setCursor(Qt::PointingHandCursor);
    m_nextButton->setStyleSheet(
        QString(
            "QPushButton {"
            "  background-color: %1; color: white;"
            "  border: none; border-radius: 6px;"
            "  padding: 12px 24px; font-size: 15px; font-weight: 600;"
            "}"
            "QPushButton:hover { background-color: %2; }"
        ).arg(kBlue, kBlueDark));
    m_nextButton->hide();
    connect(m_nextButton, &QPushButton::clicked, this, &QuizPage::onNextClicked);

    QHBoxLayout *nextRow = new QHBoxLayout();
    nextRow->addStretch();
    nextRow->addWidget(m_nextButton);

    quizLayout->addLayout(topRow);
    quizLayout->addWidget(m_questionLabel);
    quizLayout->addLayout(grid);
    quizLayout->addWidget(m_feedbackLabel);
    quizLayout->addLayout(nextRow);
    quizLayout->addStretch();

    // ════════════════════════════════
    //  RESULTS WIDGET
    // ════════════════════════════════
    m_resultsWidget = new QWidget(this);
    QVBoxLayout *resLayout = new QVBoxLayout(m_resultsWidget);
    resLayout->setContentsMargins(60, 60, 60, 40);
    resLayout->setSpacing(20);

    m_resultsTitleLabel = new QLabel("Quiz Complete!", m_resultsWidget);
    m_resultsTitleLabel->setAlignment(Qt::AlignCenter);
    m_resultsTitleLabel->setStyleSheet("font-size: 32px; font-weight: 700;");

    m_resultsScoreLabel = new QLabel(m_resultsWidget);
    m_resultsScoreLabel->setAlignment(Qt::AlignCenter);
    m_resultsScoreLabel->setStyleSheet("font-size: 52px; font-weight: 700; color: #f0f0f0;");

    m_resultsMessageLabel = new QLabel(m_resultsWidget);
    m_resultsMessageLabel->setAlignment(Qt::AlignCenter);
    m_resultsMessageLabel->setWordWrap(true);
    m_resultsMessageLabel->setStyleSheet(
        QString("font-size: 16px; color: %1;").arg(kTextLight));

    m_resultsBackButton = new QPushButton("Back to Lesson Menu", m_resultsWidget);
    m_resultsBackButton->setCursor(Qt::PointingHandCursor);
    m_resultsBackButton->setStyleSheet(
        QString(
            "QPushButton { background-color: %1; color: white; border: none;"
            "  border-radius: 6px; padding: 12px 24px; font-size: 14px; }"
            "QPushButton:hover { background-color: %2; }"
        ).arg(kCard, "#4a4a4a"));
    connect(m_resultsBackButton, &QPushButton::clicked,
            this, &QuizPage::backRequested);

    m_resultsHomeButton = new QPushButton("Home", m_resultsWidget);
    m_resultsHomeButton->setCursor(Qt::PointingHandCursor);
    m_resultsHomeButton->setStyleSheet(
        QString(
            "QPushButton { background-color: %1; color: white; border: none;"
            "  border-radius: 6px; padding: 12px 24px; font-size: 14px; }"
            "QPushButton:hover { background-color: %2; }"
        ).arg(kBlue, kBlueDark));
    connect(m_resultsHomeButton, &QPushButton::clicked,
            this, &QuizPage::homeRequested);

    QHBoxLayout *resBtns = new QHBoxLayout();
    resBtns->addStretch();
    resBtns->addWidget(m_resultsBackButton);
    resBtns->addWidget(m_resultsHomeButton);
    resBtns->addStretch();

    resLayout->addStretch();
    resLayout->addWidget(m_resultsTitleLabel);
    resLayout->addWidget(m_resultsScoreLabel);
    resLayout->addWidget(m_resultsMessageLabel);
    resLayout->addSpacing(20);
    resLayout->addLayout(resBtns);
    resLayout->addStretch();

    // ── Add both views to master stack ──
    stack->addWidget(m_quizWidget);    // index 0
    stack->addWidget(m_resultsWidget); // index 1
    stack->setCurrentIndex(0);
}

// ─────────────────────────────────────────────────────
void QuizPage::startQuiz(int lessonId)
{
    m_lessonId     = lessonId;
    m_currentIndex = 0;
    m_score        = 0;
    m_questions    = m_repo.questionsForLesson(lessonId);

    // Shuffle and pick only 8 questions from the full pool
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(m_questions.begin(), m_questions.end(), rng);
    if (m_questions.size() > 8)
        m_questions = m_questions.mid(0, 8);

    // Switch to quiz view
    static_cast<QStackedLayout *>(layout())->setCurrentIndex(0);

    if (m_questions.isEmpty()) {
        m_questionLabel->setText("No questions available for this lesson.");
        lockAnswerButtons(true);
        return;
    }

    showQuestion(0);
}

// ─────────────────────────────────────────────────────
void QuizPage::showQuestion(int index)
{
    const QuizQuestion &q = m_questions.at(index);

    int total = m_questions.size();
    m_progressLabel->setText(
        QString("Question %1 of %2").arg(index + 1).arg(total));
    m_scoreLabel->setText(
        QString("Score: %1 / %2").arg(m_score).arg(total));

    m_questionLabel->setText(q.question);

    for (int i = 0; i < 4; ++i) {
        m_answerButtons[i]->setText(
            QString("%1.  %2").arg(QChar('A' + i)).arg(q.choices.at(i)));
    }

    resetAnswerButtons();
    m_feedbackLabel->hide();
    m_nextButton->hide();
}

// ─────────────────────────────────────────────────────
void QuizPage::onAnswerSelected(int choiceIndex)
{
    const QuizQuestion &q = m_questions.at(m_currentIndex);
    bool correct = (choiceIndex == q.correctIndex);

    if (correct) {
        ++m_score;
        m_feedbackLabel->setStyleSheet(
            QString("font-size: 15px; font-weight: 600; color: %1; padding: 8px;").arg(kGreen));
        m_feedbackLabel->setText("✓  Correct!");
        m_answerButtons[choiceIndex]->setStyleSheet(
            answerBtnStyle(kGreen, kGreen));
    } else {
        m_feedbackLabel->setStyleSheet(
            QString("font-size: 15px; font-weight: 600; color: %1; padding: 8px;").arg(kRed));
        m_feedbackLabel->setText(
            QString("✗  Wrong!  The correct answer was:  %1")
                .arg(q.choices.at(q.correctIndex)));
        m_answerButtons[choiceIndex]->setStyleSheet(
            answerBtnStyle(kRed, kRed));
        m_answerButtons[q.correctIndex]->setStyleSheet(
            answerBtnStyle(kGreen, kGreen));
    }

    lockAnswerButtons(true);
    m_feedbackLabel->show();

    bool isLast = (m_currentIndex == m_questions.size() - 1);
    m_nextButton->setText(isLast ? "See Results" : "Next Question →");
    m_nextButton->show();

    m_scoreLabel->setText(
        QString("Score: %1 / %2").arg(m_score).arg(m_questions.size()));
}

// ─────────────────────────────────────────────────────
void QuizPage::onNextClicked()
{
    ++m_currentIndex;
    if (m_currentIndex >= m_questions.size()) {
        showResults();
    } else {
        showQuestion(m_currentIndex);
    }
}

// ─────────────────────────────────────────────────────
void QuizPage::showResults()
{
    int total = m_questions.size();
    m_resultsScoreLabel->setText(
        QString("%1 / %2").arg(m_score).arg(total));

    QString msg;
    double pct = total > 0 ? (double)m_score / total : 0.0;
    if (pct == 1.0)
        msg = "Perfect score! You nailed every question.";
    else if (pct >= 0.75)
        msg = "Great work! You have a solid understanding of this lesson.";
    else if (pct >= 0.5)
        msg = "Not bad! Review the lesson to strengthen what you missed.";
    else
        msg = "Keep practicing. Read through the lesson again and try once more.";

    m_resultsMessageLabel->setText(msg);

    static_cast<QStackedLayout *>(layout())->setCurrentIndex(1);
}

// ─────────────────────────────────────────────────────
void QuizPage::lockAnswerButtons(bool locked)
{
    for (int i = 0; i < 4; ++i)
        m_answerButtons[i]->setEnabled(!locked);
}

void QuizPage::resetAnswerButtons()
{
    lockAnswerButtons(false);
    for (int i = 0; i < 4; ++i)
        m_answerButtons[i]->setStyleSheet(answerBtnStyle(kCard, "#4a4a4a"));
}
