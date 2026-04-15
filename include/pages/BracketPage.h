#pragma once

#include <QWidget>

class BracketPage : public QWidget
{
    Q_OBJECT

public:
    explicit BracketPage(QWidget *parent = nullptr);

signals:
    void backRequested();
};
