#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// QuizPage displays questions from `QuizRepository` and tracks score.

#include "models/QuizRepository.h"

#include <QWidget>

class QLabel;
class QPushButton;
class QGridLayout;
class QVBoxLayout;

class QuizPage : public QWidget
{
    Q_OBJECT

public:
    explicit QuizPage(QWidget *parent = nullptr);

    void startQuiz(int lessonId);

signals:
    void backRequested();
    void homeRequested();

private slots:
    void onAnswerSelected(int choiceIndex);
    void onNextClicked();

private:
    void showQuestion(int index);
    void showResults();
    void lockAnswerButtons(bool locked);
    void resetAnswerButtons();

    // ── Widgets ──
    QLabel      *m_progressLabel;
    QLabel      *m_scoreLabel;
    QLabel      *m_questionLabel;
    QPushButton *m_answerButtons[4];
    QLabel      *m_feedbackLabel;
    QPushButton *m_nextButton;

    // Results screen widgets
    QWidget     *m_quizWidget;
    QWidget     *m_resultsWidget;
    QLabel      *m_resultsTitleLabel;
    QLabel      *m_resultsScoreLabel;
    QLabel      *m_resultsMessageLabel;
    QPushButton *m_resultsBackButton;
    QPushButton *m_resultsHomeButton;

    // ── State ──
    QuizRepository      m_repo;
    QVector<QuizQuestion> m_questions;
    int m_currentIndex = 0;
    int m_score        = 0;
    int m_lessonId     = -1;
};
