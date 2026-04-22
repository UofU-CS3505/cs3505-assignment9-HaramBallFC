#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// LessonViewerPage displays lesson content as slides
// and lets the user move through them.

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
    void setLesson(const Lesson &lesson, LessonMode mode);

signals:
    void backRequested();
    void homeRequested();
    void quizRequested(int lessonId);

private slots:
    void nextSlide();
    void prevSlide();
    void onImageDownloaded(QNetworkReply *reply);

private:
    // Breaks lesson content into slide data.
    void parseSlides(const QString &content);

    // Shows the selected slide.
    void showSlide(int index);

    // Downloads the image for a slide.
    void downloadImage(int slideIndex);

    // Returns image URLs for a lesson.
    QStringList imageUrlsForLesson(int lessonId);

    // Main lesson title.
    QLabel *m_titleLabel;

    // Slide content widgets.
    QLabel *m_slideTitle;
    QLabel *m_imageLabel;
    QLabel *m_slideText;

    // Navigation controls.
    QPushButton *m_prevButton;
    QPushButton *m_nextButton;
    QLabel *m_slideCounter;
    QPushButton *m_quizButton;

    // Handles image downloads.
    QNetworkAccessManager *m_networkManager;

    // Stored slide data.
    QStringList m_slideTitles;
    QStringList m_slideTexts;
    QStringList m_imageUrls;
    QMap<int, QPixmap> m_imageCache;
    int m_currentSlide;
    int m_currentLessonId;
};
