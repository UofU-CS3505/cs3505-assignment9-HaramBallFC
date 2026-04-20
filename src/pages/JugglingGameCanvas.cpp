#include "pages/JugglingGameCanvas.h"

#include <QPainter>
#include <QFontDatabase>


JugglingGameCanvas::JugglingGameCanvas(QWidget* parent) : QWidget(parent)
    , world(nullptr), ball(nullptr), ground(nullptr), timer(nullptr), jugglesCount(0), gameOver(false), highScore(0)
{

    QFontDatabase::addApplicationFont(":/fonts/PressStart2p.ttf");

    // Box2D has a 2D vector with an x and y value, here y is positive 9.8 which means downward force
    b2Vec2 gravity(0.0f, 9.8f);
    world = new b2World(gravity);

    b2BodyDef groundDef;
    groundDef.position.Set(5.0f, 10.0f);
    ground = world -> CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(10.0f, 0.5f);
    ground -> CreateFixture(&groundShape, 0.0f);

    b2BodyDef ballDef;
    ballDef.type = b2_dynamicBody; //the dynamic body allows for forces and collisions to occur
    ballDef.position.Set(5.0f, 1.0f);
    ball = world -> CreateBody(&ballDef);

    b2CircleShape ballShape;
    ballShape.m_radius = 0.5f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &ballShape;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.4f; //restitution refers to the "bounciness of the ball". 0.0 is no bounce, 1.0 is elastic so 0.6 is 60% of bounciness
    ball -> CreateFixture(&fixtureDef);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &JugglingGameCanvas::tick);
    timer->start(16);

    setFocusPolicy(Qt::StrongFocus);

    // WIDGET FOR GAME OVER SCREEN

    m_overlay = new QWidget(this);
    m_overlay -> hide();

    QVBoxLayout* overlayLayout = new QVBoxLayout(m_overlay);
    overlayLayout->setAlignment(Qt::AlignCenter);

    QLabel* ballFellLabel = new QLabel("THE BALL FELL", m_overlay);
    ballFellLabel->setAlignment(Qt::AlignCenter);
    ballFellLabel->setStyleSheet("font-family: 'Press Start 2P'; font-size: 18px; color: #FFFFFF; background: transparent;");

    m_scoreLabel = new QLabel("", m_overlay);
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    m_scoreLabel->setStyleSheet("font-family: 'Press Start 2P'; font-size: 12px; color: #D4A843; background: transparent;");

    QPushButton* tryAgainBtn = new QPushButton("TRY AGAIN", m_overlay);
    tryAgainBtn->setStyleSheet(
        "QPushButton { font-family: 'Press Start 2P'; font-size: 12px; color: #000000;"
        "background-color: #D4A843; border-radius: 8px; padding: 12px 24px;}"
        "QPushButton:hover { background-color: #FFFFFF; }"
        );

    overlayLayout->addWidget(ballFellLabel);
    overlayLayout->addSpacing(16);
    overlayLayout->addWidget(m_scoreLabel);
    overlayLayout->addSpacing(24);
    overlayLayout->addWidget(tryAgainBtn);

    connect(tryAgainBtn, &QPushButton::clicked, this, [this](){
        resetGame();
        m_overlay->hide();
        setFocus();
    });

    QSettings settings("HaramBallFC", "JugglingGame");
    highScore = settings.value("highscore", 0).toInt();
}

// FUNCTION IMPLEMENTATION

void JugglingGameCanvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const float scale = 50.0f; // In box2D one unit is 50 pixels

    painter.fillRect(rect(), QColor("#008000"));

    b2Vec2 pos = ball -> GetPosition();

    float screenX = pos.x * scale;
    float screenY = pos.y * scale;
    float radius = 0.5f * scale;

    painter.setBrush(QColor("#D4A843"));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(screenX, screenY), radius, radius);
}

void JugglingGameCanvas::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space && !event -> isAutoRepeat())
    {
        b2Vec2 impulse(0.0f, -5.0f);
        ball -> ApplyLinearImpulse(impulse, ball->GetWorldCenter(), true);
        jugglesCount++;
    }
}

void JugglingGameCanvas::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    m_overlay->setGeometry(rect());
}

void JugglingGameCanvas::tick()
{
    if (gameOver)
        return;

    world -> Step(1.0f / 60.0f, 6, 2); //1/60 is one frame at 60fps, the 6 is velocity of how Box2D handles forces, 2 is positon, how Box2D handles positions

    const float scale = 50.0f;
    float ballY = ball->GetPosition().y * scale;
    float groundY = 10.0f * scale;

    if (ballY >= groundY)
    {
        gameOver = true;
        m_scoreLabel->setText("Score: " + QString::number(jugglesCount) + "\nBest: " + QString::number(highScore));

        m_overlay->show();
    }

    update();
}

int JugglingGameCanvas::getJugglesCount() const {
    return jugglesCount;
}

void JugglingGameCanvas::resetGame()
{
    jugglesCount = 0;
    gameOver = false;
    ball -> SetTransform(b2Vec2(5.0f, 1.0f), 0.0f); // Set Transform resets the postion. position then angle
    ball -> SetLinearVelocity(b2Vec2(0.0f, 0.0f));

}
