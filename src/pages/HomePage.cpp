// AI was used to assist with this file.
#include "pages/HomePage.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QNetworkRequest>
#include <QPixmap>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

// Walk up from the executable directory until we find the file.
// Same approach used in LessonViewerPage for lesson images.
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

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
    , m_emblemLabel(new QLabel(this))
    , m_mapLabel(new QLabel(this))
    , m_net(new QNetworkAccessManager(this))
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Top accent stripe ─────────────────────────────────────────────────
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(5);
    topBar->setStyleSheet("background-color: #C8102E;");

    // ── Main content ──────────────────────────────────────────────────────
    QWidget *content = new QWidget(this);
    QHBoxLayout *outerH = new QHBoxLayout(content);
    outerH->setContentsMargins(0, 0, 0, 0);

    QWidget *center = new QWidget(content);
    center->setFixedWidth(740);
    QVBoxLayout *cl = new QVBoxLayout(center);
    cl->setContentsMargins(0, 0, 0, 0);
    cl->setSpacing(0);

    // ── Emblem — centered ─────────────────────────────────────────────────
    m_emblemLabel->setAlignment(Qt::AlignCenter);
    m_emblemLabel->setStyleSheet("background: transparent;");
    m_emblemLabel->setFixedSize(160, 247);

    QString emblemPath = findAsset("resources/images/wc2026_emblem_200.png");
    if (!emblemPath.isEmpty()) {
        QPixmap px(emblemPath);
        if (!px.isNull())
            m_emblemLabel->setPixmap(
                px.scaled(160, 247, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Host-country map — fetched from network, shown below emblem when loaded
    m_mapLabel->setAlignment(Qt::AlignCenter);
    m_mapLabel->setStyleSheet("background: transparent;");
    m_mapLabel->setFixedSize(260, 0);   // hidden until map loads

    // ── Text ──────────────────────────────────────────────────────────────
    QLabel *fifaLabel = new QLabel("FIFA WORLD CUP 2026", center);
    fifaLabel->setAlignment(Qt::AlignCenter);
    fifaLabel->setStyleSheet(
        "font-family: 'Press Start 2P';"
        "font-size: 15px;"
        "letter-spacing: 3px;"
        "color: #D4A843;"
    );

    QFrame *div = new QFrame(center);
    div->setFixedSize(80, 4);
    div->setStyleSheet("background-color: #D4A843; border: none;");

    QLabel *title = new QLabel("HaramBall FC", center);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-family: 'Press Start 2P';"
        "font-size: 9px;"
        "color: #8FA3B8;"
        "letter-spacing: 2px;"
    );

    QLabel *subtitle = new QLabel(
        "Your guide to the 2026 FIFA World Cup.\n"
        "Lessons, quizzes, and games — all in one place.",
        center);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 8px; color: #5A7090; line-height: 220%;");

    QPushButton *startBtn = new QPushButton("PRESS START  \u2192", center);
    startBtn->setFixedSize(220, 44);
    startBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #C8102E;"
        "  color: #FFFFFF;"
        "  border-radius: 0px;"
        "  border-top:    4px solid #E83A58;"
        "  border-left:   4px solid #E83A58;"
        "  border-bottom: 4px solid #7A0018;"
        "  border-right:  4px solid #7A0018;"
        "  font-family: 'Press Start 2P';"
        "  font-size: 7px;"
        "  letter-spacing: 2px;"
        "}"
        "QPushButton:hover { background-color: #E83A58; }"
        "QPushButton:pressed {"
        "  border-top:    4px solid #7A0018;"
        "  border-left:   4px solid #7A0018;"
        "  border-bottom: 4px solid #E83A58;"
        "  border-right:  4px solid #E83A58;"
        "}"
    );

    QLabel *team = new QLabel(
        "Ebrahim Himayoun  \u00B7  Tom Rasmussen  \u00B7  Diego Galavan\n"
        "Henish Patel  \u00B7  Reman Adhikari  \u00B7  Ethan Cobler",
        center);
    team->setAlignment(Qt::AlignCenter);
    team->setWordWrap(true);
    team->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 7px; color: #3A5070; line-height: 220%;");

    auto centered = [](QWidget *w) -> QHBoxLayout* {
        QHBoxLayout *h = new QHBoxLayout;
        h->setContentsMargins(0, 0, 0, 0);
        h->addStretch(); h->addWidget(w); h->addStretch();
        return h;
    };

    cl->addStretch(2);
    cl->addLayout(centered(m_emblemLabel));
    cl->addSpacing(20);
    cl->addWidget(fifaLabel);
    cl->addSpacing(8);
    cl->addLayout(centered(div));
    cl->addSpacing(10);
    cl->addWidget(title);
    cl->addSpacing(14);
    cl->addWidget(subtitle);
    cl->addSpacing(28);
    cl->addLayout(centered(startBtn));
    cl->addStretch(1);
    cl->addWidget(team);
    cl->addSpacing(20);

    outerH->addStretch();
    outerH->addWidget(center);
    outerH->addStretch();

    // ── Bottom accent stripe ──────────────────────────────────────────────
    QWidget *bottomBar = new QWidget(this);
    bottomBar->setFixedHeight(5);
    bottomBar->setStyleSheet("background-color: #D4A843;");

    root->addWidget(topBar);
    root->addWidget(content, 1);
    root->addWidget(bottomBar);

    // ── Network: fetch host-country map ───────────────────────────────────
    QNetworkRequest req(QUrl(
        "https://upload.wikimedia.org/wikipedia/commons/thumb/8/8e/"
        "2026_FIFA_World_Cup_host_cities.svg/400px-2026_FIFA_World_Cup_host_cities.svg.png"));
    req.setRawHeader("User-Agent",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36");
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);
    m_net->get(req);
    connect(m_net, &QNetworkAccessManager::finished,
            this,  &HomePage::onImageLoaded);

    connect(startBtn, &QPushButton::clicked, this, &HomePage::startClicked);
}

void HomePage::onImageLoaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QPixmap px;
        px.loadFromData(reply->readAll());
        if (!px.isNull()) {
            m_mapLabel->setFixedSize(260, 160);
            m_mapLabel->setPixmap(
                px.scaled(260, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    reply->deleteLater();
}
