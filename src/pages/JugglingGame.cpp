#include "pages/JugglingGame.h"
#include "pages/JugglingGameCanvas.h"
#include "SoundManager.h"

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// JugglingGame = wrapper page that hosts `JugglingGameCanvas` plus a back button.

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

/**
 * @brief Constructs the JugglingGame wrapper page. Builds the full page layout
 *        including the gold top accent stripe, title labels, and the JugglingGameCanvas
 *        which owns all physics and game logic. Facts are passed directly into the
 *        canvas at construction. The back button emits backRequested() to signal
 *        the main window to return to the lesson menu.
 * @param facts - list of fact strings injected from the lesson repository
 * @param parent - parent widget passed to QWidget
 */
JugglingGame::JugglingGame(const QStringList &facts, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Top accent stripe 
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #D4A843;");

    QLabel *eyebrow = new QLabel("JUGGLING GAME", this);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(
        "font-size: 11px; font-weight: 700; letter-spacing: 4px; color: #D4A843;"
    );

    QLabel *title = new QLabel("Juggling Game", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 42px; font-weight: 700; color: #FFFFFF;");

    // Gold divider
    QFrame *div = new QFrame(this);
    div->setFixedSize(50, 3);
    div->setStyleSheet("background-color: #D4A843; border: none;");

    QPushButton *backButton = new QPushButton("\u2190 Back to Lessons", this);
    backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent; color: #8FA3B8;"
        "  border: 1px solid #1E3A5F; border-radius: 8px;"
        "  padding: 9px 22px; font-size: 13px;"
        "}"
        "QPushButton:hover { color: #FFFFFF; border-color: #D4A843; }"
    );

    auto centered = [](QWidget *w) -> QHBoxLayout* {
        QHBoxLayout *h = new QHBoxLayout;
        h->setContentsMargins(0, 0, 0, 0);
        h->addStretch();
        h->addWidget(w);
        h->addStretch();
        return h;
    };

    JugglingGameCanvas* canvas = new JugglingGameCanvas(facts, this);
    canvas -> setMinimumHeight(400);
    canvas -> setFocus();

    root->addWidget(topBar);
    root->addSpacing(2);
    root->addWidget(eyebrow);
    root->addSpacing(4);
    root->addWidget(title);
    root->addSpacing(14);
    root->addWidget(canvas, 1);
    root->addSpacing(16);
    root->addLayout(centered(backButton));
    root->addSpacing(16);

    connect(backButton, &QPushButton::clicked, this, []() { SoundManager::instance().playClick(); });
    connect(backButton, &QPushButton::clicked, this, &JugglingGame::backRequested);
}
