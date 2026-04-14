#pragma once

#include "models/Lesson.h"

#include <QWidget>

class QLabel;
class QTextEdit;

class LessonViewerPage : public QWidget
{
    Q_OBJECT

public:
    explicit LessonViewerPage(QWidget *parent = nullptr);

    void setLesson(const Lesson &lesson);

signals:
    void backRequested();
    void homeRequested();

private:
    QLabel *m_titleLabel;
    QLabel *m_categoryLabel;
    QTextEdit *m_contentView;
};
