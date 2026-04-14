#pragma once

#include <QWidget>

class PlayerModePage : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerModePage(QWidget *parent = nullptr);

signals:
    void openLessonsRequested();
    void backRequested();
};
