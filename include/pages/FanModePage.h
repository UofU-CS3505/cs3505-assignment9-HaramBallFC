#pragma once

#include <QWidget>

class FanModePage : public QWidget
{
    Q_OBJECT

public:
    explicit FanModePage(QWidget *parent = nullptr);

signals:
    void openLessonsRequested();
    void backRequested();
};
