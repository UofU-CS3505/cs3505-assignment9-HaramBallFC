#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// LessonMenuPage shows lesson list + small preview.

#include "models/Lesson.h"

#include <QWidget>
#include <QVector>

class QLabel;
class QListWidget;
class QListWidgetItem;

class LessonMenuPage : public QWidget
{
    Q_OBJECT

public:
    // Constructs the lesson menu page and optionally sets a parent widget.
    explicit LessonMenuPage(QWidget *parent = nullptr);

    // Sets the visible title text shown at the top of the page.
    void setPageTitle(const QString &title);

    // Replaces the current lesson collection and refreshes the lesson list UI.
    void setLessons(const QVector<Lesson> &lessons);

signals:
    // Emitted when a lesson is selected; passes the selected lesson's ID.
    void lessonSelected(int lessonId);

    // Emitted when the user requests navigation to the previous/back page.
    void backRequested();

    // Emitted when the user requests navigation to the home page.
    void homeRequested();

private:
    // Updates the preview panel when the current lesson list selection changes.
    void updateLessonPreview(QListWidgetItem *current, QListWidgetItem *previous);

    QLabel *m_titleLabel;
    QLabel *m_previewLabel;
    QListWidget *m_lessonList;
    QVector<Lesson> m_lessons;
};
