#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// LessonViewerPage turns `Lesson.content` into slides and lets user navigate.

#include "models/Lesson.h"

#include <QWidget>
#include <QStringList>
#include <QMap>
#include <QPixmap>

class QLabel;                  // Forward declaration for label widgets.
class QTextEdit;               // Forward declaration for text edit widget (if needed by implementation).
class QPushButton;             // Forward declaration for button widgets.
class QNetworkAccessManager;   // Forward declaration for image download manager.
class QNetworkReply;           // Forward declaration for async network replies.

class LessonViewerPage : public QWidget
{
    Q_OBJECT  // Enables Qt meta-object features (signals/slots).

public:
    explicit LessonViewerPage(QWidget *parent = nullptr);  // Constructs the lesson viewer page.

    void setLesson(const Lesson &lesson, LessonMode mode);  // Loads lesson data and initializes slides.

signals:
    void backRequested();               // Emitted when user wants to go back.
    void homeRequested();               // Emitted when user wants to return home.
    void quizRequested(int lessonId);   // Emitted when user wants to start the quiz for a lesson.

private slots:
    void nextSlide();                              // Moves to the next slide.
    void prevSlide();                              // Moves to the previous slide.
    void onImageDownloaded(QNetworkReply *reply);  // Handles downloaded slide image replies.

private:
    void parseSlides(const QString &content);     // Parses raw lesson content into slide title/text data.
    void showSlide(int index);                    // Renders a slide by index.
    void downloadImage(int slideIndex);           // Starts downloading image for the given slide.
    QStringList imageUrlsForLesson(int lessonId); // Returns image URLs associated with a lesson ID.

    // Header
    QLabel *m_titleLabel;  // Displays the lesson/page title at the top.

    // Slide content
    QLabel *m_slideTitle;  // Displays the current slide title.
    QLabel *m_imageLabel;  // Displays the current slide image.
    QLabel *m_slideText;   // Displays the current slide body text.

    // Navigation
    QPushButton *m_prevButton;   // Navigates to previous slide.
    QPushButton *m_nextButton;   // Navigates to next slide.
    QLabel *m_slideCounter;      // Shows "current slide / total slides".
    QPushButton *m_quizButton;   // Starts quiz for the current lesson.

    // Network
    QNetworkAccessManager *m_networkManager;  // Manages async HTTP requests for slide images.

    // Slide data
    QStringList m_slideTitles;      // Stores parsed slide titles.
    QStringList m_slideTexts;       // Stores parsed slide text content.
    QStringList m_imageUrls;        // Stores image URLs per slide.
    QMap<int, QPixmap> m_imageCache; // Caches downloaded images by slide index.
    int m_currentSlide;             // Tracks currently displayed slide index.
    int m_currentLessonId;          // Tracks current lesson ID for navigation/quiz.
};
