// AI was used to assist with this file.
//
// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// QuizPage = runs a multiple-choice quiz and shows results screen.
#include "pages/QuizPage.h"
#include "SoundManager.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>
#include <QVBoxLayout>

#include <algorithm>
#include <random>

// ── Shared style helpers (World Cup theme) ──
static const QString kDark = "#0B1829";   // deep navy background
static const QString kCard = "#112035";   // card panels
static const QString kGreen = "#27AE60";   // correct answer
static const QString kRed = "#C8102E";   // wrong answer / FIFA red
static const QString kBlue = "#1E3A5F";   // default answer button
static const QString kBlueDark  = "#D4A843";   // next button (gold)
static const QString kTextLight = "#F0F4F8";
static const QString kTextMuted = "#5A7090";

static QString answerBtnStyle(const QString &bg, const QString &hover)
{
    return QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: #F0F4F8;"
        "  border: 1px solid #1E3A5F;"
        "  border-radius: 8px;"
        "  padding: 16px 14px;"
        "  font-size: 14px;"
        "  font-weight: 500;"
        "  text-align: left;"
        "}"
        "QPushButton:hover { background-color: %2; border-color: #D4A843; }"
        "QPushButton:disabled { color: #8FA3B8; }"
    ).arg(bg, hover);
}


QuizPage::QuizPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(QString("background-color: %1; color: #F0F4F8;").arg(kDark));

    // Master stacked layout: quiz view vs results view 
    QStackedLayout *stack = new QStackedLayout(this);
    stack->setContentsMargins(0, 0, 0, 0);

    
    //  QUIZ WIDGET

    m_quizWidget = new QWidget(this);
    QVBoxLayout *quizLayout = new QVBoxLayout(m_quizWidget);
    quizLayout->setContentsMargins(50, 30, 50, 24);
    quizLayout->setSpacing(14);

    // Top row: progress + score 
    QHBoxLayout *topRow = new QHBoxLayout();
    m_progressLabel = new QLabel(this);
    m_progressLabel->setStyleSheet(
        QString("font-family: 'Press Start 2P'; font-size: 10px; color: %1;").arg(kTextMuted));

    m_scoreLabel = new QLabel(this);
    m_scoreLabel->setAlignment(Qt::AlignRight);
    m_scoreLabel->setStyleSheet(
        QString("font-family: 'Press Start 2P'; font-size: 10px; color: %1;").arg(kTextMuted));

    topRow->addWidget(m_progressLabel);
    topRow->addStretch();
    topRow->addWidget(m_scoreLabel);

    //  Question label 
    m_questionLabel = new QLabel(this);
    m_questionLabel->setWordWrap(true);
    m_questionLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_questionLabel->setStyleSheet(
        "background-color: #112035;"
        "border-top: 4px solid #1E3A5F; border-left: 4px solid #1E3A5F;"
        "border-bottom: 4px solid #060D1A; border-right: 4px solid #060D1A;"
        "border-radius: 0px;"
        "padding: 20px;"
        "font-family: 'Press Start 2P';"
        "font-size: 11px;"
        "line-height: 200%;"
        "color: #FFFFFF;");
    m_questionLabel->setMinimumHeight(90);

    //  Answer buttons (2 × 2 grid) 
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(12);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);

    for (int i = 0; i < 4; ++i) {
        m_answerButtons[i] = new QPushButton(this);
        m_answerButtons[i]->setCursor(Qt::PointingHandCursor);
        m_answerButtons[i]->setStyleSheet(answerBtnStyle(kCard, "#1E3A5F"));
        m_answerButtons[i]->setMinimumHeight(68);
        m_answerButtons[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        grid->addWidget(m_answerButtons[i], i / 2, i % 2);

        connect(m_answerButtons[i], &QPushButton::clicked, this, [this, i]() {
            SoundManager::instance().playClick();
            onAnswerSelected(i);
        });
    }

    //  Feedback label 
    m_feedbackLabel = new QLabel(this);
    m_feedbackLabel->setWordWrap(true);
    m_feedbackLabel->setAlignment(Qt::AlignCenter);
    m_feedbackLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 11px; padding: 12px;");
    m_feedbackLabel->hide();

    //  Next button (gold) 
    m_nextButton = new QPushButton("Next Question \u2192", this);
    m_nextButton->setCursor(Qt::PointingHandCursor);
    m_nextButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #D4A843; color: #0B1829;"
        "  border: none; border-radius: 8px;"
        "  padding: 9px 22px; font-size: 14px; font-weight: 700;"
        "}"
        "QPushButton:hover { background-color: #F0C860; }"
        "QPushButton:pressed {"
        "  border-top: 4px solid #7A6020; border-left: 4px solid #7A6020;"
        "  border-bottom: 4px solid #F0C860; border-right: 4px solid #F0C860; }");
    m_nextButton->hide();
    connect(m_nextButton, &QPushButton::clicked, this, []() { SoundManager::instance().playClick(); });
    connect(m_nextButton, &QPushButton::clicked, this, &QuizPage::onNextClicked);

    // Back button (ghost) 
    QPushButton *backButton = new QPushButton("\u2190 Back to Lesson Menu", this);
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent; color: #8FA3B8;"
        "  border: 1px solid #1E3A5F; border-radius: 8px;"
        "  padding: 10px 18px; font-size: 13px;"
        "}"
        "QPushButton:hover { color: #FFFFFF; border-color: #D4A843; }");
    connect(backButton, &QPushButton::clicked, this, []() { SoundManager::instance().playClick(); });
    connect(backButton, &QPushButton::clicked, this, &QuizPage::backRequested);

    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->addWidget(backButton);
    bottomRow->addStretch();
    bottomRow->addWidget(m_nextButton);

    quizLayout->addLayout(topRow);
    quizLayout->addWidget(m_questionLabel);
    quizLayout->addLayout(grid);
    quizLayout->addWidget(m_feedbackLabel);
    quizLayout->addLayout(bottomRow);
    quizLayout->addStretch();

    
    //  RESULTS WIDGET
    
    m_resultsWidget = new QWidget(this);
    QVBoxLayout *resLayout = new QVBoxLayout(m_resultsWidget);
    resLayout->setContentsMargins(60, 60, 60, 40);
    resLayout->setSpacing(20);

    m_resultsTitleLabel = new QLabel("Quiz Complete!", m_resultsWidget);
    m_resultsTitleLabel->setAlignment(Qt::AlignCenter);
    m_resultsTitleLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 18px; color: #FFFFFF;");

    m_resultsScoreLabel = new QLabel(m_resultsWidget);
    m_resultsScoreLabel->setAlignment(Qt::AlignCenter);
    m_resultsScoreLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 36px; color: #D4A843;");

    m_resultsMessageLabel = new QLabel(m_resultsWidget);
    m_resultsMessageLabel->setAlignment(Qt::AlignCenter);
    m_resultsMessageLabel->setWordWrap(true);
    m_resultsMessageLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 9px; color: #8FA3B8; line-height: 250%;");

    m_resultsBackButton = new QPushButton("\u2190 BACK", m_resultsWidget);
    m_resultsBackButton->setCursor(Qt::PointingHandCursor);
    m_resultsBackButton->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent; color: #8FA3B8;"
        "  border-radius: 0px;"
        "  border-top: 2px solid #2E4E7A; border-left: 2px solid #2E4E7A;"
        "  border-bottom: 2px solid #060D1A; border-right: 2px solid #060D1A;"
        "  font-family: 'Press Start 2P'; font-size: 6px; letter-spacing: 1px;"
        "  padding: 12px 24px;"
        "}"
        "QPushButton:hover { color: #D4A843; border-color: #D4A843; }");
    connect(m_resultsBackButton, &QPushButton::clicked,
            this, &QuizPage::backRequested);

    m_resultsHomeButton = new QPushButton("HOME", m_resultsWidget);
    m_resultsHomeButton->setCursor(Qt::PointingHandCursor);
    m_resultsHomeButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #D4A843; color: #0B1829;"
        "  border-radius: 0px;"
        "  border-top: 4px solid #F0C860; border-left: 4px solid #F0C860;"
        "  border-bottom: 4px solid #7A6020; border-right: 4px solid #7A6020;"
        "  font-family: 'Press Start 2P'; font-size: 7px; letter-spacing: 1px;"
        "  padding: 12px 24px;"
        "}"
        "QPushButton:hover { background-color: #F0C860; }"
        "QPushButton:pressed {"
        "  border-top: 4px solid #7A6020; border-left: 4px solid #7A6020;"
        "  border-bottom: 4px solid #F0C860; border-right: 4px solid #F0C860; }");
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

    //  Add both views to master stack 
    stack->addWidget(m_quizWidget);    // index 0
    stack->addWidget(m_resultsWidget); // index 1
    stack->setCurrentIndex(0);
}


void QuizPage::startQuiz(int lessonId)
{
    m_lessonId = lessonId;
    m_currentIndex = 0;
    m_score = 0;
    m_questions = m_repo.questionsForLesson(lessonId);

    // Shuffle and pick questions — 10 for the Fan Mode combined quiz, 8 for all others
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(m_questions.begin(), m_questions.end(), rng);
    int cap = (lessonId == 5) ? 10 : 8;
    if (m_questions.size() > cap)
        m_questions = m_questions.mid(0, cap);

    // Switch to quiz view
    static_cast<QStackedLayout *>(layout())->setCurrentIndex(0);

    if (m_questions.isEmpty()) {
        m_questionLabel->setText("No questions available for this lesson.");
        lockAnswerButtons(true);
        return;
    }

    showQuestion(0);
}


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


void QuizPage::onAnswerSelected(int choiceIndex)
{
    const QuizQuestion &q = m_questions.at(m_currentIndex);
    bool correct = (choiceIndex == q.correctIndex);

    if (correct) {
        ++m_score;
        SoundManager::instance().playCorrect();
        m_feedbackLabel->setStyleSheet(
            QString("font-family: 'Press Start 2P'; font-size: 14px; color: %1; padding: 12px;").arg(kGreen));
        m_feedbackLabel->setText("✓  Correct!");
        m_answerButtons[choiceIndex]->setStyleSheet(
            answerBtnStyle(kGreen, kGreen));
    } else {
        SoundManager::instance().playWrong();
        m_feedbackLabel->setStyleSheet(
            QString("font-family: 'Press Start 2P'; font-size: 14px; color: %1; padding: 12px; line-height: 200%;").arg(kRed));
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


void QuizPage::onNextClicked()
{
    ++m_currentIndex;
    if (m_currentIndex >= m_questions.size()) {
        showResults();
    } else {
        showQuestion(m_currentIndex);
    }
}

void QuizPage::showResults()
{
    int total = m_questions.size();
    m_resultsScoreLabel->setText(
        QString("%1 / %2").arg(m_score).arg(total));

    QString msg;
    double pct = total > 0 ? (double)m_score / total : 0.0;
    if (pct >= 0.5)
        SoundManager::instance().playPass();
    else
        SoundManager::instance().playFail();

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
