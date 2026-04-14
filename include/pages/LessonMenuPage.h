#pragma once

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
    explicit LessonMenuPage(QWidget *parent = nullptr);

    void setPageTitle(const QString &title);
    void setLessons(const QVector<Lesson> &lessons);

signals:
    void lessonSelected(int lessonId);
    void backRequested();
    void homeRequested();

private:
    void updateLessonPreview(QListWidgetItem *current, QListWidgetItem *previous);

    QLabel *m_titleLabel;
    QLabel *m_previewLabel;
    QListWidget *m_lessonList;
    QVector<Lesson> m_lessons;
};
