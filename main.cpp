// AI was used to assist with this file.
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ── Global World Cup Theme ──────────────────────────────────────────────
    app.setStyleSheet(R"(

        /* ── Base ── */
        QMainWindow, QWidget {
            background-color: #0B1829;
            color: #F0F4F8;
            font-family: "Segoe UI", Arial, Helvetica, sans-serif;
            font-size: 14px;
        }

        /* ── Default Button ── */
        QPushButton {
            background-color: #16284A;
            color: #F0F4F8;
            border: 1px solid #1E3A5F;
            border-radius: 8px;
            padding: 9px 22px;
            font-size: 13px;
            font-weight: 600;
            min-height: 36px;
        }
        QPushButton:hover {
            background-color: #1E3A5F;
            border-color: #D4A843;
            color: #FFFFFF;
        }
        QPushButton:pressed {
            background-color: #0d1e38;
            border-color: #C8102E;
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
            border: 1px solid #1E3A5F;
            border-radius: 8px;
            padding: 6px;
            outline: none;
            font-size: 14px;
        }
        QListWidget::item {
            padding: 10px 14px;
            border-radius: 6px;
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
            font-weight: 600;
        }

        /* ── Scroll Bars ── */
        QScrollBar:vertical {
            background-color: #0B1829;
            width: 6px;
            margin: 0;
            border-radius: 3px;
        }
        QScrollBar::handle:vertical {
            background-color: #1E3A5F;
            border-radius: 3px;
            min-height: 24px;
        }
        QScrollBar::handle:vertical:hover { background-color: #D4A843; }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical     { height: 0px; }

        QScrollBar:horizontal {
            background-color: #0B1829;
            height: 6px;
            border-radius: 3px;
        }
        QScrollBar::handle:horizontal {
            background-color: #1E3A5F;
            border-radius: 3px;
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
    // ───────────────────────────────────────────────────────────────────────

    MainWindow window;
    window.show();

    return app.exec();
}
