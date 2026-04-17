// AI was used to assist with the Box2D physics simulation in this file.

#include "pages/PenaltyGame.h"

// Qt drawing / UI helper includes
#include <QRect>
#include <QPen>
#include <QFont>
#include <QBrush>
#include <QPushButton>

// Standard library includes
#include <cmath>
#include <algorithm>
#include <QDebug>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>


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

PenaltyGamePage::PenaltyGamePage(QWidget *parent)
    : QWidget(parent)
    , world(b2Vec2(0.0f, 0.0f))
    , ball(nullptr)
    , timer(this)
    , scored(false)
    , dragging(false)
    , ballVisible(true)
    , resetButton(new QPushButton("Reset Shot", this))
    , backButton(new QPushButton("Back to Player Mode", this))
    , shootButton(new QPushButton("Shoot", this))
    , angleSlider(new QSlider(Qt::Horizontal, this))
    , powerSlider(new QSlider(Qt::Vertical, this))

{
    setMinimumSize(1000, 700);
    setMouseTracking(true);

    backButton->setGeometry(20, 20, 180, 40);
    resetButton->setGeometry(820, 20, 140, 40);
    shootButton->setGeometry(650, 500, 100, 40);

    angleSlider->setGeometry(width()/2-180, height() - 120, 360, 24);
    powerSlider->setGeometry(width()/2-290, height() - 290, 24, 170);



    QString fieldPath = findAsset("resources/images/field.png");
    QString playerPath = findAsset("resources/images/messiSprite.png");


    fieldPixmap = QPixmap(fieldPath);
    playerPixmap = QPixmap(playerPath);



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
    bodyDef.position.Set(centerX, 1.5f);

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

void PenaltyGamePage::resetBall()
{
    scored = false;
    dragging = false;
    ballVisible = true;

    if (ball != nullptr) {
        float centerX = (width() / 2.0f - 100.0f) / 60.0f;
        ball->SetTransform(b2Vec2(centerX, 1.5f), 0.0f);
        ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        ball->SetAngularVelocity(0.0f);
    }

    update();
}

void PenaltyGamePage::updateWorld()
{
    float timeStep = 1.0f / 60.0f;
    world.Step(timeStep, 6, 2);

    if (ball != nullptr && ballVisible) {
        b2Vec2 pos = ball->GetPosition();

        int ballX = worldToScreenX(pos.x);
        int ballY = worldToScreenY(pos.y);

        QRect goalRect(width() / 2 - 140, 40, 280, 90);

        if (goalRect.contains(ballX, ballY)) {
            scored = true;
            ballVisible = false;
            ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            ball->SetAngularVelocity(0.0f);
        }
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
    ball->SetTransform(b2Vec2(centerX, 1.5f), 0.0f);
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
    int w = 280;
    int h = 90;
    int gx = width()/2 - w/2;
    int gy = 40;

    QPen pen(Qt::white);
    pen.setWidth(6);
    p.setPen(pen);

    p.drawLine(gx, gy, gx, gy+h);
    p.drawLine(gx+w, gy, gx+w, gy+h);
    p.drawLine(gx, gy, gx+w, gy);


    // Keep Messi fixed on the screen
    playerRect = QRect(width() / 2 - 120, height() - 310, 90, 140);

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

    // Goal text
    if (scored) {
        p.setPen(Qt::yellow);
        p.setFont(QFont("Arial", 24, QFont::Bold));
        p.drawText(width()/2 - 45, 155, "GOAL!");
    }
}



void PenaltyGamePage::mousePressEvent(QMouseEvent *event)
{
    if (ball == nullptr || !ballVisible) return;

    int ballX = worldToScreenX(ball->GetPosition().x);
    int ballY = worldToScreenY(ball->GetPosition().y);

    int dx = event->pos().x() - ballX;
    int dy = event->pos().y() - ballY;

    if (dx * dx + dy * dy <= 30 * 30) {
        dragging = true;
        dragStart = event->pos();
        dragCurrent = event->pos();
    }
}

void PenaltyGamePage::mouseMoveEvent(QMouseEvent *event)
{
    if (!dragging) return;

    dragCurrent = event->pos();
    update();
}

void PenaltyGamePage::mouseReleaseEvent(QMouseEvent *event)
{
    if (!dragging || ball == nullptr || !ballVisible) return;

    dragging = false;
    dragCurrent = event->pos();
    scored = false;

    float centerX = (width() / 2.0f - 100.0f) / 60.0f;
    ball->SetTransform(b2Vec2(centerX, 1.5f), 0.0f);
    ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    ball->SetAngularVelocity(0.0f);

    int dx = dragCurrent.x() - dragStart.x();
    int dy = dragCurrent.y() - dragStart.y();

    float length = std::sqrt(static_cast<float>(dx * dx + dy * dy));

    if (length < 1.0f) {
        update();
        return;
    }

    float dirX = dx / length;
    float dirY = dy / length;
    dirY = -dirY;

    float power = length / 4.0f;

    float vx = -dirX * power;
    float vy = dirY * power;

    if (std::abs(dx) < 4) {
        vx = 0.0f;
    }

    if (vy < 4.0f) {
        vy = 4.0f;
    }

    ball->SetLinearVelocity(b2Vec2(vx, vy));

    update();
}

