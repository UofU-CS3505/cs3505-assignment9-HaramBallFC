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
{
    setMinimumSize(1000, 700);
    setMouseTracking(true);

    backButton->setGeometry(20, 20, 180, 40);
    resetButton->setGeometry(820, 20, 140, 40);

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
    bodyDef.position.Set(5.5f, 1.5f);

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
        ball->SetTransform(b2Vec2(5.5f, 1.5f), 0.0f);
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

    update();
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

    ball->SetTransform(b2Vec2(5.5f, 1.5f), 0.0f);
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

void PenaltyGamePage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw field background image
    if (!fieldPixmap.isNull()) {
        painter.drawPixmap(rect(), fieldPixmap);
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(35, 130, 55));
        painter.drawRect(rect());
    }



    int goalWidth = 280;
    int goalHeight = 90;
    int goalX = width() / 2 - goalWidth / 2;
    int goalY = 40;

    QPen goalPen(Qt::white);
    goalPen.setWidth(6);
    goalPen.setCapStyle(Qt::RoundCap);
    painter.setPen(goalPen);
    painter.setBrush(Qt::NoBrush);

    painter.drawLine(goalX, goalY, goalX, goalY + goalHeight);
    painter.drawLine(goalX + goalWidth, goalY,
                     goalX + goalWidth, goalY + goalHeight);
    painter.drawLine(goalX, goalY, goalX + goalWidth, goalY);

    // Keep Messi fixed on the screen
    playerRect = QRect(width() / 2 - 170, height() - 310, 90, 140);

    if (!playerPixmap.isNull()) {
        painter.drawPixmap(playerRect, playerPixmap);
    }


    // Draw ball on top
    if (ball != nullptr && ballVisible) {
        b2Vec2 pos = ball->GetPosition();

        int x = worldToScreenX(pos.x);
        int y = worldToScreenY(pos.y);
        int r = 18;

        painter.setPen(Qt::black);
        painter.setBrush(Qt::white);
        painter.drawEllipse(x - r, y - r, r * 2, r * 2);

        painter.setBrush(Qt::black);
        painter.drawEllipse(x - 5, y - 5, 10, 10);

        if (dragging) {
            int dx = dragCurrent.x() - dragStart.x();
            int dy = dragCurrent.y() - dragStart.y();

            int length = static_cast<int>(std::sqrt(dx * dx + dy * dy));
            int thickness = std::min(20, 4 + length / 12);

            QPen dragPen(Qt::red);
            dragPen.setWidth(thickness);
            dragPen.setCapStyle(Qt::RoundCap);

            painter.setPen(dragPen);
            painter.drawLine(dragStart, dragCurrent);
        }
    }

    if (scored) {
        painter.setPen(Qt::yellow);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(width() / 2 - 45, 155, "GOAL!");
    }
}
