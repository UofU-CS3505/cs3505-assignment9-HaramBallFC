#pragma once

#include "models/Lesson.h"

#include <QWidget>
#include <QStringList>
#include <QMap>
#include <QPixmap>

class QLabel;
class QTextEdit;
class QPushButton;
class QNetworkAccessManager;
class QNetworkReply;

class LessonViewerPage : public QWidget
{
    Q_OBJECT

public:
    explicit LessonViewerPage(QWidget *parent = nullptr);

    void setLesson(const Lesson &lesson);

signals:
    void backRequested();
    void homeRequested();
    void quizRequested(int lessonId);

private slots:
    void nextSlide();
    void prevSlide();
    void onImageDownloaded(QNetworkReply *reply);

private:
    void parseSlides(const QString &content);
    void showSlide(int index);
    void downloadImage(int slideIndex);
    QStringList imageUrlsForLesson(int lessonId);

    // Header
    QLabel *m_titleLabel;

    // Slide content
    QLabel *m_slideTitle;
    QLabel *m_imageLabel;
    QLabel *m_slideText;

    // Navigation
    QPushButton *m_prevButton;
    QPushButton *m_nextButton;
    QLabel *m_slideCounter;

    // Network
    QNetworkAccessManager *m_networkManager;

    // Slide data
    QStringList m_slideTitles;
    QStringList m_slideTexts;
    QStringList m_imageUrls;
    QMap<int, QPixmap> m_imageCache;
    int m_currentSlide;
    int m_currentLessonId;
};
