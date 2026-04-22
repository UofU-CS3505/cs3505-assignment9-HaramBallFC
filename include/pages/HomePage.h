#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// HomePage = first page user sees. Emits `startClicked()` to move forward.

#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWidget>

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);

signals:
    void startClicked();

//slot for image loading
private slots:
    void onImageLoaded(QNetworkReply *reply);

private:
    QLabel *m_emblemLabel;
    QLabel *m_mapLabel;
    QNetworkAccessManager *m_net;
};
