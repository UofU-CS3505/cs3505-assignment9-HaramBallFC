#pragma once

#include <QWidget>

class ModeSelectionPage : public QWidget
{
    Q_OBJECT

public:
    explicit ModeSelectionPage(QWidget *parent = nullptr);

signals:
    void fanModeSelected();
    void playerModeSelected();
    void backRequested();
};
