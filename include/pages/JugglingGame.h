#pragma once

#include <QWidget>

class JugglingGame : public QWidget
{
    Q_OBJECT

public:
    explicit JugglingGame(QWidget *parent = nullptr);

signals:
    void backRequested();
};
