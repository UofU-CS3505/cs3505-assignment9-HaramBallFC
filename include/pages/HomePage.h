#pragma once

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

private slots:
    void onImageLoaded(QNetworkReply *reply);

private:
    QLabel                *m_emblemLabel;
    QLabel                *m_mapLabel;
    QNetworkAccessManager *m_net;
};
