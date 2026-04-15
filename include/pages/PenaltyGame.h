#ifndef PENALTYGAMEPAGE_H
#define PENALTYGAMEPAGE_H

#include <QWidget>

class PenaltyGamePage : public QWidget
{
    Q_OBJECT

public:
    explicit PenaltyGamePage(QWidget *parent = nullptr);

signals:
    void backClicked();
};

#endif
