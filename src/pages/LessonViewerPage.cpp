// AI was used to assist with this file.
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
#include <QFileInfo>

LessonViewerPage::LessonViewerPage(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(new QLabel(this))
    , m_slideTitle(new QLabel(this))
    , m_imageLabel(new QLabel(this))
    , m_slideText(new QLabel(this))
    , m_prevButton(new QPushButton("Previous", this))
    , m_nextButton(new QPushButton("Next", this))
    , m_slideCounter(new QLabel("Slide 1 of 1", this))
    , m_quizButton(new QPushButton("Take Quiz", this))
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentSlide(0)
    , m_currentLessonId(-1)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 30, 40, 20);
    mainLayout->setSpacing(12);

    // ── Lesson title ──
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 13px; color: #FFFFFF;");

    // ── Slide section title ──
    m_slideTitle->setAlignment(Qt::AlignCenter);
    m_slideTitle->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 9px; color: #D4A843; line-height: 200%;");

    // ── Image area ──
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setMinimumHeight(240);
    m_imageLabel->setMaximumHeight(280);
    m_imageLabel->setStyleSheet(
        "background-color: #112035;"
        "border: 1px solid #1E3A5F;"
        "border-radius: 10px;"
        "color: #3A5070; font-size: 14px;");
    m_imageLabel->setText("Loading image...");
    m_imageLabel->setScaledContents(false);

    // ── Slide text in scroll area ──
    m_slideText->setWordWrap(true);
    m_slideText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_slideText->setStyleSheet(
        "font-size: 15px; color: #B0C4D8; padding: 30px 36px; line-height: 200%;");

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_slideText);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "  background-color: #0F1E35;"
        "  border-top: 3px solid #060D1A; border-left: 3px solid #060D1A;"
        "  border-bottom: 3px solid #1E3A5F; border-right: 3px solid #1E3A5F;"
        "  border-radius: 0px;"
        "}");

    // ── Slide navigation ──
    QHBoxLayout *slideNav = new QHBoxLayout();
    m_slideCounter->setAlignment(Qt::AlignCenter);
    m_slideCounter->setStyleSheet(
        "font-family: 'Press Start 2P'; font-size: 10px; color: #5A7090;");

    const QString navBtnStyle =
        "QPushButton {"
        "  font-family: 'Press Start 2P'; font-size: 11px;"
        "  padding: 10px 22px;"
        "}";
    m_prevButton->setStyleSheet(navBtnStyle);
    m_nextButton->setStyleSheet(navBtnStyle);

    slideNav->addWidget(m_prevButton);
    slideNav->addStretch();
    slideNav->addWidget(m_slideCounter);
    slideNav->addStretch();
    slideNav->addWidget(m_nextButton);

    // ── Bottom buttons ──
    QHBoxLayout *buttonRow = new QHBoxLayout();
    QPushButton *backButton = new QPushButton("\u2190 Back", this);
    QPushButton *homeButton = new QPushButton("Home", this);

    const QString ghostBtn =
        "QPushButton {"
        "  background-color: transparent; color: #8FA3B8;"
        "  border: 1px solid #1E3A5F; border-radius: 8px;"
        "  padding: 8px 18px; font-size: 13px;"
        "}"
        "QPushButton:hover { color: #FFFFFF; border-color: #D4A843; }";
    backButton->setStyleSheet(ghostBtn);
    homeButton->setStyleSheet(ghostBtn);

    m_quizButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #D4A843; color: #0B1829;"
        "  border: none; border-radius: 8px;"
        "  padding: 9px 22px; font-size: 14px; font-weight: 700;"
        "}"
        "QPushButton:hover   { background-color: #B8922E; color: #FFFFFF; }"
        "QPushButton:pressed { background-color: #9A7A24; }");

    buttonRow->addWidget(backButton);
    buttonRow->addWidget(homeButton);
    buttonRow->addStretch();
    buttonRow->addWidget(m_quizButton);

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
    connect(m_quizButton, &QPushButton::clicked, this, [this]() {
        emit quizRequested(m_currentLessonId);
    });
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &LessonViewerPage::onImageDownloaded);
}

void LessonViewerPage::setLesson(const Lesson &lesson, LessonMode mode)
{
    m_titleLabel->setText(lesson.title);
    m_currentLessonId = lesson.id;
    m_currentSlide = 0;
    m_imageCache.clear();

    m_quizButton->setVisible(mode == LessonMode::Player);

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

    // Add a blank line between every content line for pixel-font readability
    QString spaced = m_slideTexts.at(index);
    spaced.replace("\n", "\n\n");
    m_slideText->setText(spaced);

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
            auto findByWalkingUp = [](const QString &relativePath) -> QString {
                QDir dir(QCoreApplication::applicationDirPath());
                for (int i = 0; i < 10; ++i) {
                    const QString candidate = dir.absoluteFilePath(relativePath);
                    if (QFileInfo::exists(candidate)) {
                        return candidate;
                    }
                    if (!dir.cdUp()) {
                        break;
                    }
                }
                return QString();
            };

            // Local file or Qt resource (:/images/...)
            QString filePath = path;
            if (!path.startsWith(":/") && !QDir::isAbsolutePath(path)) {
                const QString resolved = findByWalkingUp(path);
                if (!resolved.isEmpty()) {
                    filePath = resolved;
                }
            }

            QPixmap pixmap(filePath);
            // Fallback: if Qt resources are not packaged in this build, load from repo files.
            if (pixmap.isNull() && path.startsWith(":/images/")) {
                const QString imageName = path.mid(QString(":/images/").size());
                const QString repoImagePath = findByWalkingUp("resources/images/" + imageName);
                if (!repoImagePath.isEmpty()) {
                    pixmap.load(repoImagePath);
                }
            }
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

    if (lessonId == 1) {
        // Lesson 1: Qualifications
        // Slide 1: Qualifications Overview - 2026 World Cup qualification logo
        urls << "https://upload.wikimedia.org/wikipedia/en/thumb/b/bc/2026_FIFA_World_Cup_qualification_logo.png/330px-2026_FIFA_World_Cup_qualification_logo.png"
        // Slide 2: Asian Football Confederation (AFC)
             << "https://upload.wikimedia.org/wikipedia/en/thumb/2/28/Asian_Football_Confederation_logo_2025.svg/500px-Asian_Football_Confederation_logo_2025.svg.png"
        // Slide 3: Union of European Football Associations (UEFA)
             << "https://upload.wikimedia.org/wikipedia/en/thumb/9/9d/UEFA_full_logo.svg/500px-UEFA_full_logo.svg.png"
        // Slide 4: Confederation of African Football (CAF)
             << "https://upload.wikimedia.org/wikipedia/en/thumb/7/72/Confederation_of_African_Football_logo.svg/500px-Confederation_of_African_Football_logo.svg.png"
        // Slide 5: South American Football Confederation (CONMEBOL)
             << "https://upload.wikimedia.org/wikipedia/en/thumb/a/a8/CONMEBOL_logo_%282017%29.svg/500px-CONMEBOL_logo_%282017%29.svg.png"
        // Slide 6: Confederation of North, Central America and Caribbean (CONCACAF)
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/8/87/Concacaf_logo.svg/500px-Concacaf_logo.svg.png"
        // Slide 7: Oceania Football Confederation (OFC)
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/9/93/Oceania_Football_Confederation_logo.svg/500px-Oceania_Football_Confederation_logo.svg.png"
        // Slide 8: Inter-confederation Playoff - Estadio Azteca, 2026 host
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/4/4b/Estadio_Azteca_y_sus_alrededores_46.jpg/500px-Estadio_Azteca_y_sus_alrededores_46.jpg";
    } else if (lessonId == 2) {
        // Lesson 2: Countries Represented
        // Slide 1: Europe (16 teams) - Spain celebrating 2010 World Cup win
        urls << "https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/2010_FIFA_World_Cup_Spain_with_cup.JPG/500px-2010_FIFA_World_Cup_Spain_with_cup.JPG"
        // Slide 2: South America (6 teams) - Brazil 1970 World Cup squad
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/6/6c/Brazil_1970.JPG/500px-Brazil_1970.JPG"
        // Slide 3: Africa (10 teams) - Morocco vs Iran, 2018 World Cup
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/0/0c/Iran-Morocco_by_soccer.ru_14.jpg/500px-Iran-Morocco_by_soccer.ru_14.jpg"
        // Slide 4: Asia (9 teams) - Japan lineup vs Germany, 2022 World Cup
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/d/dd/2022_FIFA_World_Cup_Germany_1%E2%80%932_Japan_-_%285%29.jpg/500px-2022_FIFA_World_Cup_Germany_1%E2%80%932_Japan_-_%285%29.jpg"
        // Slide 5: North/Central America & Caribbean - Landon Donovan vs Algeria 2010
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/1/17/Landon_Donovan_vs_Algeria.jpg/500px-Landon_Donovan_vs_Algeria.jpg"
        // Slide 6: Oceania (1 team) - New Zealand vs Bahrain 2010 World Cup qualification
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/5/56/Westpac_Stadium_14112009.jpg/500px-Westpac_Stadium_14112009.jpg";
    } else if (lessonId == 3) {
        // Lesson 3: Rules and Regulations
        // Slide 1: Offside Rule - diagram
        urls << "https://cdn.sanity.io/images/8dhz9iqq/production/c5ac4b9adc456f3a94bcdd512e11496f1917661f-1236x810.png"
        // Slide 2: Fouls and Penalties - tackle in a match
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/c/c8/ASV_Dra%C3%9Fburg_vs._SC_Ritzing_20190601_%2878%29.jpg/500px-ASV_Dra%C3%9Fburg_vs._SC_Ritzing_20190601_%2878%29.jpg"
        // Slide 3: Extra Time and Penalty Shootouts - penalty kick in action
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/d/d3/Ivory_Coast_penalty.jpg/500px-Ivory_Coast_penalty.jpg";
    } else if (lessonId == 4) {
        // Lesson 4: History of the World Cup
        // Slide 1: Beginning of the World Cup
        urls << ":/images/lesson4_section1_hero.jpg"
        // Slide 2: Growth of the Tournament
             << ":/images/lesson4_section2_growth_collage.jpg"
        // Slide 3: Successful Countries - Maradona's Goal of the Century 1986
             << "https://upload.wikimedia.org/wikipedia/commons/thumb/7/7a/Maradona_gol_a_inglaterra.jpg/500px-Maradona_gol_a_inglaterra.jpg"
        // Slide 4: 2026 World Cup Changes - Official 2026 emblem
             << "https://upload.wikimedia.org/wikipedia/en/thumb/1/17/2026_FIFA_World_Cup_emblem.svg/500px-2026_FIFA_World_Cup_emblem.svg.png";
    }

    return urls;
}
