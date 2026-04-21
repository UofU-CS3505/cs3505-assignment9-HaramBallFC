// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// JugglingGameCanvas = actual juggling mini-game physics + drawing.

#include "pages/JugglingGameCanvas.h"

#include <QPainter>
#include <QFontDatabase>
#include <QShowEvent>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

// Walk up from the executable directory until we find the asset file.
static QString findJuggleAsset(const QString &relativePath)
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


JugglingGameCanvas::JugglingGameCanvas(const QStringList &facts, QWidget* parent)
    : QWidget(parent)
    , world(nullptr)
    , ball(nullptr)
    , ground(nullptr)
    , timer(nullptr)
    , m_overlay(nullptr)
    , m_scoreLabel(nullptr)
    , m_reasonLabel(nullptr)
    , m_factLabel(nullptr)
    , m_facts(facts)
    , m_currentFactIndex(0)
    , jugglesCount(0)
    , gameOver(false)
    , highScore(0)
{

    QFontDatabase::addApplicationFont(":/fonts/PressStart2p.ttf");

    // Load pitch background (same image used by the PK game)
    const QString bgPath = findJuggleAsset("resources/images/newPitch.png");
    if (!bgPath.isEmpty())
        m_bgPixmap = QPixmap(bgPath);

    // Yamal player
    const QString playerPath = findJuggleAsset("resources/images/LaminePixel.png");
    if (!playerPath.isEmpty())
        m_playerPixmap = QPixmap(playerPath);

    // Reduced gravity so the ball moves slowly enough for the player to react
    b2Vec2 gravity(0.0f, 6.0f);
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
    m_overlay->setAttribute(Qt::WA_StyledBackground, true);
    m_overlay->setStyleSheet("background-color: rgba(10, 15, 30, 210);");
    m_overlay->hide();

    QVBoxLayout* overlayLayout = new QVBoxLayout(m_overlay);
    overlayLayout->setAlignment(Qt::AlignCenter);

    // Reason label: updated at runtime to "THE BALL FELL" or "HIT THE WALL!"
    m_reasonLabel = new QLabel("GAME OVER", m_overlay);
    m_reasonLabel->setAlignment(Qt::AlignCenter);
    m_reasonLabel->setStyleSheet("font-family: 'Press Start 2P'; font-size: 18px; color: #FFFFFF; background: transparent;");

    m_scoreLabel = new QLabel("", m_overlay);
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    m_scoreLabel->setStyleSheet("font-family: 'Press Start 2P'; font-size: 12px; color: #D4A843; background: transparent;");

    // Fact label shown during gameplay
    m_factLabel = new QLabel(this);
    m_factLabel->setAlignment(Qt::AlignCenter);
    m_factLabel->setWordWrap(true);
    m_factLabel->setStyleSheet(
        "font-family: 'Press Start 2P';"
        "font-size: 9px;"
        "color: #FFFFFF;"
        "background-color: rgba(11, 24, 41, 200);"
        "border: 2px solid #D4A843;"
        "padding: 12px;");
    m_factLabel->setFixedWidth(520);

    if (!m_facts.isEmpty()) {
        m_factLabel->setText(m_facts[0]);
    } else {
        m_factLabel->setText("Fun facts will appear here.");
    }

    m_factLabel->show();
    m_factLabel->raise();

    QPushButton* tryAgainBtn = new QPushButton("TRY AGAIN", m_overlay);
    tryAgainBtn->setStyleSheet(
        "QPushButton { font-family: 'Press Start 2P'; font-size: 12px; color: #000000;"
        "background-color: #D4A843; border-radius: 8px; padding: 12px 24px;}"
        "QPushButton:hover { background-color: #FFFFFF; }"
        );

    overlayLayout->addWidget(m_reasonLabel);
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

    // Draw pitch background (falls back to solid green if image not found)
    if (!m_bgPixmap.isNull())
        painter.drawPixmap(rect(), m_bgPixmap);
    else
        painter.fillRect(rect(), QColor("#008000"));

    // draw player
    if (!m_playerPixmap.isNull())
    {
        int playerW = 120;
        int playerH = 230;
        int playerX = width() / 4 - playerW / 2;   // player on the left
        int playerY = height() - playerH;
        painter.drawPixmap(playerX, playerY, playerW, playerH, m_playerPixmap);
    }

    b2Vec2 pos = ball -> GetPosition();

    float screenX = pos.x * scale;
    float screenY = pos.y * scale;
    float radius = 0.5f * scale;

    painter.setBrush(QColor("#D4A843"));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(screenX, screenY), radius, radius);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Press Start 2P", 14));
    painter.drawText(QRect(10, 10, 200, 30), Qt::AlignLeft, "JUGGLES: " + QString::number(jugglesCount)); //live score
    painter.drawText(QRect(width() - 210, 10, 200, 30), Qt::AlignRight, "BEST: " + QString::number(highScore)); // high score


}

void JugglingGameCanvas::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
    {
        const float scale = 50.0f;

        // Ball screen position
        b2Vec2 pos = ball->GetPosition();
        float ballX = pos.x * scale;
        float ballY = pos.y * scale;
        float radius = 0.5f * scale;

        // Player draw position (same values used in paintEvent)
        int playerW = 120;
        int playerH = 230;
        int playerX = width() / 4 - playerW / 2;
        int playerY = height() - playerH;

        // Simple hit box around the upper body / head area
        int hitLeft   = playerX - 10;
        int hitRight  = playerX + playerW + 10;
        int hitTop    = playerY - 40;
        int hitBottom = playerY + 110;

        bool ballInHitBox =
            (ballX + radius >= hitLeft) &&
            (ballX - radius <= hitRight) &&
            (ballY + radius >= hitTop) &&
            (ballY - radius <= hitBottom);

        if (ballInHitBox)
        {
            b2Vec2 impulse(0.0f, -5.2f);
            ball->ApplyLinearImpulse(impulse, ball->GetWorldCenter(), true);
            jugglesCount++;

            // Change fact every 4 juggles
            if (!m_facts.isEmpty() && jugglesCount % 4 == 0) {
                m_currentFactIndex = (m_currentFactIndex + 1) % m_facts.size();
                m_factLabel->setText(m_facts[m_currentFactIndex]);
            }
        }
    }
}

void JugglingGameCanvas::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    m_overlay->setGeometry(rect());
    repositionGround();

    // fact box on the right of the canvas
    if (m_factLabel != nullptr) {
        int labelX = width() - m_factLabel->width() - 20;
        int labelY = 70;
        m_factLabel->move(labelX, labelY);
        m_factLabel->raise();
    }
}

void JugglingGameCanvas::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    repositionGround();
    resetGame();
    m_overlay->hide();
}

// Moves the Box2D ground body so its top surface sits exactly at the canvas bottom.
void JugglingGameCanvas::repositionGround()
{
    if (!ground || height() <= 0)
        return;

    const float scale = 50.0f;
    // Ground box half-height is 0.5, so place body 0.5 units below canvas bottom
    // so its top surface == height() pixels.
    float groundWorldY = height() / scale + 0.5f;
    ground->SetTransform(b2Vec2(ground->GetPosition().x, groundWorldY), 0.0f);
}

void JugglingGameCanvas::tick()
{
    if (gameOver)
        return;

    world -> Step(1.0f / 60.0f, 6, 2); //1/60 is one frame at 60fps, the 6 is velocity of how Box2D handles forces, 2 is positon, how Box2D handles positions

    const float scale = 50.0f;
    float ballY   = ball->GetPosition().y * scale;
    float radius  = 0.5f * scale;        // 25 px
    float groundY = (float)height();     // actual canvas bottom in pixels

    // 8px tolerance accounts for Box2D's internal skin gap that keeps
    // the ball slightly above the physical ground surface.
    bool hitGround  = (ballY + radius >= groundY - 8.0f);
    bool hitCeiling = (ballY - radius <= 0);

    if (hitGround || hitCeiling)
    {
        gameOver = true;

        // Update high score
        if (jugglesCount > highScore)
        {
            highScore = jugglesCount;
            QSettings settings("HaramBallFC", "JugglingGame");
            settings.setValue("highscore", highScore);
        }

        m_reasonLabel->setText(hitCeiling ? "HIT THE CEILING!" : "THE BALL FELL");
        m_scoreLabel->setText("Score: " + QString::number(jugglesCount) + "\nBest: " + QString::number(highScore));
        m_overlay->raise();
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

    // Reset displayed fact
    m_currentFactIndex = 0;
    if (m_factLabel != nullptr) {
        if (!m_facts.isEmpty()) {
            m_factLabel->setText(m_facts[0]);
        } else {
            m_factLabel->setText("Fun facts will appear here.");
        }
        m_factLabel->raise();
    }

    // Re-center ball horizontally, start 35% down the canvas
    const float scale = 50.0f;
    float centerX = (width() > 0) ? width() / (3.5f * scale) : 3.0f;
    float startY  = (height() > 0) ? height() * 0.35f / scale : 3.0f;
    ball->SetTransform(b2Vec2(centerX, startY), 0.0f);
    ball->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
}
