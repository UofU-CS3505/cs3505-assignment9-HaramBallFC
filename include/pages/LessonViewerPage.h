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

    // Loads lesson data and initializes slides.
    void setLesson(const Lesson &lesson, LessonMode mode);  

//emits when the user click a certain button
signals:
    void backRequested();               
    void homeRequested();               
    void quizRequested(int lessonId);   

//slots for the lesson viewing logic (slides)
private slots:
    void nextSlide();                              
    void prevSlide();                              
    void onImageDownloaded(QNetworkReply *reply);  

private:
    void parseSlides(const QString &content);     // Parses raw lesson content into slide title/text data.
    void showSlide(int index);                    // Renders a slide by index.
    void downloadImage(int slideIndex);           // Starts downloading image for the given slide.
    QStringList imageUrlsForLesson(int lessonId); // Returns image URLs associated with a lesson ID.

    // Displays the lesson/page title at the top.
    QLabel *m_titleLabel;  

    // Slide content
    QLabel *m_slideTitle;  // Displays the current slide title.
    QLabel *m_imageLabel;  // Displays the current slide image.
    QLabel *m_slideText;   // Displays the current slide body text.

    // Navigation
    QPushButton *m_prevButton;   // Navigates to previous slide.
    QPushButton *m_nextButton;   // Navigates to next slide.
    QLabel *m_slideCounter;      // Shows "current slide / total slides".
    QPushButton *m_quizButton;   // Starts quiz for the current lesson.


    // Manages async HTTP requests for slide images.
    QNetworkAccessManager *m_networkManager;  

    // Slide data
    QStringList m_slideTitles;      // Stores parsed slide titles.
    QStringList m_slideTexts;       // Stores parsed slide text content.
    QStringList m_imageUrls;        // Stores image URLs per slide.
    QMap<int, QPixmap> m_imageCache; // Caches downloaded images by slide index.
    int m_currentSlide;             // Tracks currently displayed slide index.
    int m_currentLessonId;          // Tracks current lesson ID for navigation/quiz.
};
