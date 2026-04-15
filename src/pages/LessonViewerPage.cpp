#include "pages/LessonViewerPage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QCoreApplication>
#include <QDir>

LessonViewerPage::LessonViewerPage(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(new QLabel(this))
    , m_slideTitle(new QLabel(this))
    , m_imageLabel(new QLabel(this))
    , m_slideText(new QLabel(this))
    , m_prevButton(new QPushButton("Previous", this))
    , m_nextButton(new QPushButton("Next", this))
    , m_slideCounter(new QLabel("Slide 1 of 1", this))
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentSlide(0)
    , m_currentLessonId(-1)
{
    setStyleSheet("background-color: #2f2f2f; color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 30, 40, 20);
    mainLayout->setSpacing(12);

    // ── Lesson title ──
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size: 28px; font-weight: 700;");

    // ── Slide section title ──
    m_slideTitle->setAlignment(Qt::AlignCenter);
    m_slideTitle->setStyleSheet("font-size: 18px; font-weight: 600; color: #d0d0d0;");

    // ── Image area ──
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setMinimumHeight(240);
    m_imageLabel->setMaximumHeight(280);
    m_imageLabel->setStyleSheet(
        "background-color: #3a3a3a; border-radius: 6px; color: #808080; font-size: 14px;");
    m_imageLabel->setText("Loading image...");
    m_imageLabel->setScaledContents(false);

    // ── Slide text in scroll area ──
    m_slideText->setWordWrap(true);
    m_slideText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_slideText->setStyleSheet("font-size: 15px; color: #d0d0d0; padding: 12px;");

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_slideText);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #3a3a3a; border: none; border-radius: 6px; }");

    // ── Slide navigation ──
    QHBoxLayout *slideNav = new QHBoxLayout();
    m_slideCounter->setAlignment(Qt::AlignCenter);
    m_slideCounter->setStyleSheet("font-size: 14px; color: #a0a0a0;");

    slideNav->addWidget(m_prevButton);
    slideNav->addStretch();
    slideNav->addWidget(m_slideCounter);
    slideNav->addStretch();
    slideNav->addWidget(m_nextButton);

    // ── Bottom buttons ──
    QHBoxLayout *buttonRow = new QHBoxLayout();
    QPushButton *backButton = new QPushButton("Back to Lesson Menu", this);
    QPushButton *homeButton = new QPushButton("Home", this);

    buttonRow->addWidget(backButton);
    buttonRow->addWidget(homeButton);
    buttonRow->addStretch();

    // ── Assemble layout ──
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_slideTitle);
    mainLayout->addWidget(m_imageLabel);
    mainLayout->addWidget(scrollArea, 1);
    mainLayout->addLayout(slideNav);
    mainLayout->addLayout(buttonRow);

    // ── Connections ──
    connect(m_prevButton, &QPushButton::clicked, this, &LessonViewerPage::prevSlide);
    connect(m_nextButton, &QPushButton::clicked, this, &LessonViewerPage::nextSlide);
    connect(backButton, &QPushButton::clicked, this, &LessonViewerPage::backRequested);
    connect(homeButton, &QPushButton::clicked, this, &LessonViewerPage::homeRequested);
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &LessonViewerPage::onImageDownloaded);
}

void LessonViewerPage::setLesson(const Lesson &lesson)
{
    m_titleLabel->setText(lesson.title);
    m_currentLessonId = lesson.id;
    m_currentSlide = 0;
    m_imageCache.clear();

    parseSlides(lesson.content);
    m_imageUrls = imageUrlsForLesson(lesson.id);

    showSlide(0);
}

void LessonViewerPage::parseSlides(const QString &content)
{
    m_slideTitles.clear();
    m_slideTexts.clear();

    QStringList lines = content.split('\n');
    QString currentTitle;
    QString currentText;

    for (const QString &line : lines) {
        if (line.startsWith("Section ")) {
            if (!currentTitle.isEmpty()) {
                m_slideTitles.append(currentTitle);
                m_slideTexts.append(currentText.trimmed());
            }
            currentTitle = line.trimmed();
            currentText.clear();
        } else {
            currentText += line + "\n";
        }
    }

    if (!currentTitle.isEmpty()) {
        m_slideTitles.append(currentTitle);
        m_slideTexts.append(currentText.trimmed());
    }

    if (m_slideTitles.isEmpty()) {
        m_slideTitles.append("Content");
        m_slideTexts.append(content);
    }
}

void LessonViewerPage::showSlide(int index)
{
    if (index < 0 || index >= m_slideTitles.size())
        return;

    m_currentSlide = index;
    m_slideTitle->setText(m_slideTitles.at(index));
    m_slideText->setText(m_slideTexts.at(index));

    m_slideCounter->setText(
        QString("Slide %1 of %2").arg(index + 1).arg(m_slideTitles.size()));

    m_prevButton->setEnabled(index > 0);
    m_nextButton->setEnabled(index < m_slideTitles.size() - 1);

    if (m_imageCache.contains(index)) {
        m_imageLabel->setPixmap(
            m_imageCache[index].scaled(m_imageLabel->size(),
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else if (index < m_imageUrls.size() && !m_imageUrls.at(index).isEmpty()) {
        QString path = m_imageUrls.at(index);
        if (!path.startsWith("http")) {
            // Local file - resolve relative to app directory
            QString filePath = path;
            if (!QDir::isAbsolutePath(filePath)) {
                filePath = QCoreApplication::applicationDirPath() + "/../../../" + filePath;
            }
            QPixmap pixmap(filePath);
            if (!pixmap.isNull()) {
                m_imageCache[index] = pixmap;
                m_imageLabel->setPixmap(
                    pixmap.scaled(m_imageLabel->size(),
                                  Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                m_imageLabel->setText("Image not found");
            }
        } else {
            // Web URL
            m_imageLabel->setText("Loading image...");
            downloadImage(index);
        }
    } else {
        m_imageLabel->setText("No image available");
    }
}

void LessonViewerPage::nextSlide()
{
    if (m_currentSlide < m_slideTitles.size() - 1)
        showSlide(m_currentSlide + 1);
}

void LessonViewerPage::prevSlide()
{
    if (m_currentSlide > 0)
        showSlide(m_currentSlide - 1);
}

void LessonViewerPage::downloadImage(int slideIndex)
{
    if (slideIndex < 0 || slideIndex >= m_imageUrls.size())
        return;

    QNetworkRequest request(QUrl(m_imageUrls.at(slideIndex)));
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("slideIndex", slideIndex);
}

void LessonViewerPage::onImageDownloaded(QNetworkReply *reply)
{
    int slideIndex = reply->property("slideIndex").toInt();

    if (reply->error() == QNetworkReply::NoError) {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());

        if (!pixmap.isNull()) {
            m_imageCache[slideIndex] = pixmap;

            if (m_currentSlide == slideIndex) {
                m_imageLabel->setPixmap(
                    pixmap.scaled(m_imageLabel->size(),
                                  Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    } else {
        if (m_currentSlide == slideIndex) {
            m_imageLabel->setText("Image could not be loaded");
        }
    }

    reply->deleteLater();
}

QStringList LessonViewerPage::imageUrlsForLesson(int lessonId)
{
    QStringList urls;

    if (lessonId == 3) {
        // Lesson 3: Rules and Regulations
        // Slide 1: Offside Rule
        urls << "https://cdn.sanity.io/images/8dhz9iqq/production/c5ac4b9adc456f3a94bcdd512e11496f1917661f-1236x810.png"
        // Slide 2: Fouls and Penalties
             << "/Users/henishpatel/Documents/untitled folder/resources/images/fouls.jpeg"
        // Slide 3: Extra Time and Penalty Shootouts
             << "/Users/henishpatel/Documents/untitled folder/resources/images/penalty.jpeg";
    } else if (lessonId == 4) {
        // Lesson 4: History of the World Cup
        // Slide 1: Beginning of the World Cup
        urls << "/Users/henishpatel/Documents/untitled folder/resources/images/worldcup_trophy.webp"
        // Slide 2: Growth of the Tournament
             << "/Users/henishpatel/Documents/untitled folder/resources/images/worldcup_bracket.jpg"
        // Slide 3: Successful Countries
             << "/Users/henishpatel/Documents/untitled folder/resources/images/worldcup_slide3.png"
        // Slide 4: 2026 World Cup Changes
             << "/Users/henishpatel/Documents/untitled folder/resources/images/worldcup_slide4.png";
    }

    return urls;
}
