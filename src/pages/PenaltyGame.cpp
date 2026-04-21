// AI was used to assist with the Box2D physics simulation in this file.
//
// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// PenaltyGamePage = penalty kick mini-game + quick true/false questions.

#include "pages/PenaltyGame.h"

// Qt drawing / UI helper includes
#include <QRect>
#include <QPen>
#include <QFont>
#include <QBrush>
#include <QPushButton>
#include <QRandomGenerator>
#include <algorithm>
// Standard library includes
#include <cmath>
#include <algorithm>
#include <QDebug>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <numeric>
#include <QLabel>


//used a method similar to harry's to import the picture.
static QString findAsset(const QString &relativePath)
{
    QDir dir(QCoreApplication::applicationDirPath());
    for (int i = 0; i < 10; ++i) {
        const QString candidate = dir.absoluteFilePath(relativePath);
        if (QFileInfo::exists(candidate))
            return candidate;
        if (!dir.cdUp())
            break;
    }
    return {};
}

static void chooseRandomFive(const QStringList &allQuestions,
                             const QVector<bool> &allAnswers,
                             QStringList &pickedQuestions,
                             QVector<bool> &pickedAnswers)
{
    QVector<int> indices(allQuestions.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::shuffle(indices.begin(), indices.end(), *QRandomGenerator::global());

    pickedQuestions.clear();
    pickedAnswers.clear();

    int count = std::min(5, static_cast<int>(indices.size()));
    for (int i = 0; i < count; ++i) {
        int idx = indices[i];
        pickedQuestions << allQuestions[idx];
        pickedAnswers << allAnswers[idx];
    }
}


PenaltyGamePage::PenaltyGamePage(QWidget *parent)
    : QWidget(parent)
    , world(b2Vec2(0.0f, 0.0f))
    , ball(nullptr)
    , timer(this)
    , scored(false)
    , dragging(false)
    , ballVisible(true)
    , resetButton(new QPushButton("Restart", this))
    , backButton(new QPushButton("Quit", this))
    , shootButton(new QPushButton("Shoot", this))
    , angleSlider(new QSlider(Qt::Horizontal, this))
    , powerSlider(new QSlider(Qt::Vertical, this))
    , currentQuestionIndex(0)
    , score(0)
    , gameOver(false)
    , resultText("")
    , answerTarget("")

{
    setMinimumSize(1000, 700);
    setMouseTracking(true);

    backButton->setGeometry(20, 20, 140, 40);
    resetButton->setGeometry(820, 20, 140, 40);
    shootButton->setGeometry(650, 500, 100, 40);

    //new code to change ths size
    backButton->setStyleSheet("font-size: 12px; font-weight: bold; background-color: red; color: white;");
    resetButton->setStyleSheet("font-size: 8px; font-weight: bold;");
    shootButton->setStyleSheet("font-size: 8px; font-weight: bold;");

    angleSlider->setGeometry(width()/2-180, height() - 120, 360, 24);
    powerSlider->setGeometry(width()/2-290, height() - 290, 24, 170);


    //mapping for the images
    QString fieldPath = findAsset("resources/images/newPitch.png");
    QString playerPath = findAsset("resources/images/messiSprite.png");
    fieldPixmap = QPixmap(fieldPath);
    playerPixmap = QPixmap(playerPath);


    //LABELS FOR SLIDERS
    QLabel *powerLabel = new QLabel("POWER", this);
    powerLabel->setGeometry(width()/2 - 320, height() - 310, 70, 20);
    powerLabel->setStyleSheet("color: black; font-size: 18px;");

    QLabel *aimLabel = new QLabel("AIM", this);
    aimLabel->setGeometry(width()/2 - 20, height() - 98, 40, 20);
    aimLabel->setStyleSheet("color: black; font-size: 18px;");



    // -------------------------------------------------------------------------
    // STATIC GROUND BODY
    // -------------------------------------------------------------------------
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    // -------------------------------------------------------------------------
    // CREATE THE BALL
    // -------------------------------------------------------------------------
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    float centerX = (width() / 2.0f - 100.0f) / 60.0f;
    bodyDef.position.Set(centerX, 1.1f);

    ball = world.CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = 0.5f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.55f;

    ball->CreateFixture(&fixtureDef);

    // -------------------------------------------------------------------------
    // TIMER + RESET BUTTON CONNECTIONS
    // -------------------------------------------------------------------------
    connect(&timer, &QTimer::timeout, this, &PenaltyGamePage::updateWorld);
    connect(resetButton, &QPushButton::clicked, this, &PenaltyGamePage::resetBall);
    connect(backButton, &QPushButton::clicked, this, &PenaltyGamePage::backClicked);
    connect(shootButton, &QPushButton::clicked, this, &PenaltyGamePage::shootBall);


    //NEW


    // Angle slider
    angleSlider->setMinimum(-75);
    angleSlider->setMaximum(75);
    angleSlider->setValue(0);

    // Power slider
    powerSlider->setMinimum(4);
    powerSlider->setMaximum(30);
    powerSlider->setValue(12);

    //new code: I added some random questions instead of putting it in array (for now)
    questions << "The World Cup is held every 4 years."
              << "A soccer team has 12 players on the field."
              << "The 2026 World Cup will be hosted by 3 countries."
              << "A red card means the player can stay in the game."
              << "Argentina won the 2022 World Cup."
              << "A CONMEBOL team that finishes 7th goes to the inter-confederation playoff."
              << "CONCACAF had only 3 direct qualifying slots because the 3 host nations got automatic spots."
              << "The 2026 World Cup will have 16 nations";


    correctAnswers << true
                   << false
                   << true
                   << false
                   << true
                   << true
                   << true
                   <<false;


    chooseRandomFive(questions, correctAnswers, activeQuestions, activeAnswers);

    timer.start(10);
}

int PenaltyGamePage::worldToScreenX(float x)
{
    return static_cast<int>(x * 60.0f) + 100;
}

int PenaltyGamePage::worldToScreenY(float y)
{
    return height() - static_cast<int>(y * 60.0f) - 100;
}

//logic for resetting the ball
void PenaltyGamePage::resetBall()
{
    if (gameOver) {
        currentQuestionIndex = 0;
        score = 0;
        gameOver = false;
        resultText = "";
        answerTarget = "";
        chooseRandomFive(questions, correctAnswers, activeQuestions, activeAnswers);

    } else {
        resultText = "";
        answerTarget = "";
    }

    scored = false;
    dragging = false;
    ballVisible = true;

    if (ball != nullptr) {
        float centerX = (width() / 2.0f - 100.0f) / 60.0f;
        ball->SetTransform(b2Vec2(centerX, 1.1f), 0.0f);
        ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        ball->SetAngularVelocity(0.0f);
    }

    update();
}


//method for updating the world after each kick
void PenaltyGamePage::updateWorld()
{
    float timeStep = 1.0f / 60.0f;
    world.Step(timeStep, 6, 2);

    if (ball != nullptr && ballVisible) {
        b2Vec2 pos = ball->GetPosition();

        int ballX = worldToScreenX(pos.x);
        int ballY = worldToScreenY(pos.y);

        //GOAL DIMENSIONS
        int goalWidth = 750;
        int goalHeight = 90;
        int goalX = width()/2 - goalWidth/2;
        int goalY = 195;

        int boxWidth = 150;
        int boxHeight = 60;
        int squareMargin = 30;

        //Left Box True
        //QRect leftSquare(goalX + squareMargin, goalY + (goalHeight-boxHeight) /2 , boxWidth, boxHeight);
        QRect leftSquare(goalX, goalY, goalWidth/2, goalHeight);

        //Right Box False
        //QRect rightSquare(goalX + goalWidth - squareMargin - boxWidth, goalY + (goalHeight-boxHeight) /2 , boxWidth, boxHeight);
        QRect rightSquare(goalX + goalWidth/2, goalY, goalWidth/2, goalHeight);


        //Goal Box
        QRect goalRect(goalX, goalY, goalWidth, goalHeight);

        //add guard
        if(gameOver ==  false){

            if (leftSquare.contains(ballX, ballY)) {
                scored = true;
                answerTarget = "TRUE";
                ballVisible = false;
                ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                ball->SetAngularVelocity(0.0f);

                bool selectedAnswer = true;
                if (selectedAnswer == activeAnswers[currentQuestionIndex]) {
                    score++;
                    resultText = "CORRECT!";
                } else {
                    resultText = "WRONG!";
                }

                currentQuestionIndex++;
                if (currentQuestionIndex >= 5) {
                    gameOver = true;
                }
            }
            else if (rightSquare.contains(ballX, ballY)) {
                scored = true;
                answerTarget = "FALSE";
                ballVisible = false;
                ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                ball->SetAngularVelocity(0.0f);

                bool selectedAnswer = false;
                if (selectedAnswer == activeAnswers[currentQuestionIndex]) {
                    score++;
                    resultText = "CORRECT!";
                } else {
                    resultText = "WRONG!";
                }

                currentQuestionIndex++;
                if (currentQuestionIndex >= 5) {
                    gameOver = true;
                }
            }
        }

        // else if(goalRect.contains(ballX, ballY)) {
        //     scored = true;
        //     ballVisible = false;
        //     ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        //     ball->SetAngularVelocity(0.0f);
        // }
    }

    backButton->setGeometry(20, 20, 180, 40);
    resetButton->setGeometry(width() - 160, 20, 140, 40);
    shootButton->setGeometry(650, 500, 100, 40);


    update();
}


//NEW
void PenaltyGamePage::shootBall()
{
    if (!ball) return;

    scored = false;
    ballVisible = true;
    float centerX = (width() / 2.0f - 100.0f) / 60.0f;
    ball->SetTransform(b2Vec2(centerX, 1.1f), 0.0f);
    ball->SetLinearVelocity(b2Vec2(0, 0));
    ball->SetAngularVelocity(0);

    float power = static_cast<float>(powerSlider->value());
    float angleDeg = static_cast<float>(angleSlider->value());

    float rad = angleDeg * 3.14159265f / 180.0f;

    float vx = std::sin(rad) * power;
    float vy = std::cos(rad) * power;

    if (std::abs(vx) < 0.15f) vx = 0.0f;

    ball->SetLinearVelocity(b2Vec2(vx, vy));

    update();
}

void PenaltyGamePage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Draw field background image
    if (!fieldPixmap.isNull()) {
        p.drawPixmap(rect(), fieldPixmap);
    } else {
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(35, 130, 55));
        p.drawRect(rect());
    }

    // Goal
    int w = 750;
    int h = 90;
    int gx = width()/2 - w/2;
    int gy = 195;

    QPen pen(Qt::white);
    pen.setWidth(8);
    p.setPen(pen);

    //Draw Goal Rectangle
    p.drawLine(gx, gy, gx, gy+h);
    p.drawLine(gx+w, gy, gx+w, gy+h);
    p.drawLine(gx, gy, gx+w, gy);

    //Draw Two Square Boxes
    int boxWidth = 150;
    int boxHeight = 60;
    int squareMargin = 30;

    //QRect leftSquare(gx + squareMargin, gy + (h - boxHeight) / 2, boxWidth, boxHeight);
    QRect leftSquare(gx, gy, w/2, h);

    //QRect rightSquare(gx + w - squareMargin - boxWidth, gy + (h - boxHeight) / 2, boxWidth, boxHeight);
    QRect rightSquare(gx + w/2, gy, w/2, h);


    // Fill boxess with a gradient background
    QLinearGradient leftGradient(leftSquare.topLeft(), leftSquare.bottomLeft());
    leftGradient.setColorAt(0, QColor(255, 255, 255, 180));
    leftGradient.setColorAt(1, QColor(255, 255, 255, 180));


    QLinearGradient rightGradient(rightSquare.topLeft(), rightSquare.bottomLeft());
    rightGradient.setColorAt(0, QColor(255, 255, 255, 180));
    rightGradient.setColorAt(1, QColor(255, 255, 255, 180));

    p.setPen(QPen(Qt::white, 3));
    p.setBrush(leftGradient);
    p.drawRoundedRect(leftSquare, 8, 8);

    p.setBrush(rightGradient);
    p.drawRoundedRect(rightSquare, 8, 8);

    //DRAW T/F BOXES
    p.setPen(Qt::black);
    p.setFont(QFont("Press Start 2P", 14, QFont::Bold));
    p.drawText(leftSquare, Qt::AlignCenter, "TRUE");
    p.drawText(rightSquare, Qt::AlignCenter, "FALSE");



    //place holder for the question box
    QRect questionBox(width() / 2 - 280, 95, 560, 70);

    p.setPen(QPen(Qt::white, 3));
    p.setBrush(QColor(255, 255, 255, 210));
    p.drawRoundedRect(questionBox, 10, 10);

    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 14, QFont::Bold));

    if (!gameOver && currentQuestionIndex < questions.size()) {
        p.drawText(questionBox, Qt::AlignCenter | Qt::TextWordWrap,
                   activeQuestions[currentQuestionIndex]);
    }

    //Setup for the question box
    QRect statsBox(20, 85, 190, 95);

    QLinearGradient statsGradient(statsBox.topLeft(), statsBox.bottomLeft());
    statsGradient.setColorAt(0, QColor(255, 255, 255, 180));
    statsGradient.setColorAt(1, QColor(255, 255, 255, 180));

    p.setPen(QPen(Qt::white, 3));
    p.setBrush(statsGradient);
    p.drawRoundedRect(statsBox, 8, 8);

    p.setPen(Qt::black);
    p.setFont(QFont("Press Start 2P", 10, QFont::Bold));
    p.drawText(QRect(35, 105, 230, 25), Qt::AlignLeft | Qt::AlignVCenter,
               QString("Score: %1 / 5").arg(score));

    int shownQuestion = gameOver ? 5 : currentQuestionIndex + 1;
    p.drawText(QRect(35, 135, 230, 25), Qt::AlignLeft | Qt::AlignVCenter,
               QString("Question: %1 / 5").arg(shownQuestion));

    // Keep Messi fixed on the screen
    playerRect = QRect(width() / 2 - 120, height() - 270, 90, 140);

    if (!playerPixmap.isNull()) {
        p.drawPixmap(playerRect, playerPixmap);
    }

    // Ball
    if (ball && ballVisible) {
        b2Vec2 pos = ball->GetPosition();

        int x = worldToScreenX(pos.x);
        int y = worldToScreenY(pos.y);
        int r = 18;

        p.setPen(Qt::black);
        p.setBrush(Qt::white);
        p.drawEllipse(x-r, y-r, r*2, r*2);

        p.setBrush(Qt::black);
        p.drawEllipse(x-5, y-5, 10, 10);
    }

    // Logic to Print when Box was Hit
    //Logic similar to the True and False that tom had implemented
    if (!resultText.isEmpty() && !gameOver) {
        p.setPen(resultText == "CORRECT!" ? Qt::darkGreen : Qt::red);
        p.setFont(QFont("Press Start 2P", 16, QFont::Bold));
        p.drawText(QRect(0, 150, width(), 40), Qt::AlignCenter, resultText);
    }

    if (gameOver) {
        p.setPen(Qt::red);
        p.setFont(QFont("Press Start 2P", 18, QFont::Bold));
        p.drawText(QRect(0, 100, width(), 40), Qt::AlignCenter, "GAME OVER");

        p.setPen(Qt::blue);
        p.setFont(QFont("Press Start 2P", 14, QFont::Bold));
        p.drawText(QRect(0, 125, width(), 40), Qt::AlignCenter,
                   QString("FINAL SCORE: %1 / 5").arg(score));
    }
}



// void PenaltyGamePage::mousePressEvent(QMouseEvent *event)
// {
//     if (ball == nullptr || !ballVisible) return;

//     int ballX = worldToScreenX(ball->GetPosition().x);
//     int ballY = worldToScreenY(ball->GetPosition().y);

//     int dx = event->pos().x() - ballX;
//     int dy = event->pos().y() - ballY;

//     if (dx * dx + dy * dy <= 30 * 30) {
//         dragging = true;
//         dragStart = event->pos();
//         dragCurrent = event->pos();
//     }
// }

// void PenaltyGamePage::mouseMoveEvent(QMouseEvent *event)
// {
//     if (!dragging) return;

//     dragCurrent = event->pos();
//     update();
// }

// void PenaltyGamePage::mouseReleaseEvent(QMouseEvent *event)
// {
//     if (!dragging || ball == nullptr || !ballVisible) return;

//     dragging = false;
//     dragCurrent = event->pos();
//     scored = false;

//     float centerX = (width() / 2.0f - 100.0f) / 60.0f;
//     ball->SetTransform(b2Vec2(centerX, 1.1f), 0.0f);
//     ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
//     ball->SetAngularVelocity(0.0f);

//     int dx = dragCurrent.x() - dragStart.x();
//     int dy = dragCurrent.y() - dragStart.y();

//     float length = std::sqrt(static_cast<float>(dx * dx + dy * dy));

//     if (length < 1.0f) {
//         update();
//         return;
//     }

//     float dirX = dx / length;
//     float dirY = dy / length;
//     dirY = -dirY;

//     float power = length / 4.0f;

//     float vx = -dirX * power;
//     float vy = dirY * power;

//     if (std::abs(dx) < 4) {
//         vx = 0.0f;
//     }

//     if (vy < 4.0f) {
//         vy = 4.0f;
//     }

//     ball->SetLinearVelocity(b2Vec2(vx, vy));

//     update();
// }



