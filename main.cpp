// AI was used to assist with this file.
#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //  Load pixel font 
    {
        QDir dir(QCoreApplication::applicationDirPath());
        for (int i = 0; i < 10; ++i) {
            QString candidate = dir.absoluteFilePath(
                "resources/fonts/PressStart2P-Regular.ttf");
            if (QFileInfo::exists(candidate)) {
                QFontDatabase::addApplicationFont(candidate);
                break;
            }
            if (!dir.cdUp()) break;
        }
    }

    //  Global Pixel / World Cup Theme 
    app.setStyleSheet(R"(

        /* ── Base ── */
        QMainWindow, QWidget {
            background-color: #0B1829;
            color: #F0F4F8;
            font-family: "Segoe UI", Arial, Helvetica, sans-serif;
            font-size: 13px;
        }

        /* ── Default pixel button ── */
        QPushButton {
            background-color: #1A3050;
            color: #F0F4F8;
            border-radius: 0px;
            border-top:    3px solid #2E4E7A;
            border-left:   3px solid #2E4E7A;
            border-bottom: 3px solid #060D1A;
            border-right:  3px solid #060D1A;
            padding: 10px 22px;
            font-family: "Press Start 2P";
            font-size: 6px;
            min-height: 36px;
            letter-spacing: 1px;
        }
        QPushButton:hover {
            background-color: #1E3A5F;
            color: #D4A843;
        }
        QPushButton:pressed {
            border-top:    3px solid #060D1A;
            border-left:   3px solid #060D1A;
            border-bottom: 3px solid #2E4E7A;
            border-right:  3px solid #2E4E7A;
            padding: 12px 20px 8px 24px;
        }
        QPushButton:disabled {
            background-color: #0f1a2a;
            color: #2e4a66;
            border-color: #162032;
        }

        /* ── Labels ── */
        QLabel {
            background-color: transparent;
            color: #F0F4F8;
        }

        /* ── List Widget ── */
        QListWidget {
            background-color: #0F1E35;
            border-top:    3px solid #1A3A5F;
            border-left:   3px solid #1A3A5F;
            border-bottom: 3px solid #060D1A;
            border-right:  3px solid #060D1A;
            border-radius: 0px;
            padding: 6px;
            outline: none;
            font-size: 12px;
        }
        QListWidget::item {
            padding: 10px 14px;
            border-radius: 0px;
            color: #8FA3B8;
            margin: 2px 0px;
        }
        QListWidget::item:hover {
            background-color: #1E3A5F;
            color: #F0F4F8;
        }
        QListWidget::item:selected {
            background-color: #C8102E;
            color: #FFFFFF;
            font-family: "Press Start 2P";
            font-size: 8px;
        }

        /* ── Scroll Bars ── */
        QScrollBar:vertical {
            background-color: #0B1829;
            width: 6px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background-color: #1E3A5F;
            min-height: 24px;
        }
        QScrollBar::handle:vertical:hover { background-color: #D4A843; }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical     { height: 0px; }

        QScrollBar:horizontal {
            background-color: #0B1829;
            height: 6px;
        }
        QScrollBar::handle:horizontal {
            background-color: #1E3A5F;
            min-width: 24px;
        }
        QScrollBar::handle:horizontal:hover { background-color: #D4A843; }
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal    { width: 0px; }

        /* ── Scroll Area ── */
        QScrollArea {
            background-color: transparent;
            border: none;
        }
        QScrollArea > QWidget > QWidget { background-color: transparent; }

        /* ── Stacked Widget ── */
        QStackedWidget { background-color: #0B1829; }

    )");
    

    MainWindow window;
    window.show();

    return app.exec();
}
