#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QRect>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

private slots:
    void on_actionOpen_ROM_triggered();

    void on_action_Re_Start_Game_triggered();

    void on_actionPause_Game_triggered();

    void on_actionInsert_Coin_triggered();

private:
    Ui::MainWindow *ui;

    // Space invaders pixel screen resolution.
    size_t BIT_MAP_REL_HEIGHT = 256;
    size_t BIT_MAP_REL_WIDTH = 244;

    // Leaving space for the menu bar.
    QPoint UPPER_CORNER = QPoint(0, 22);

};
#endif // MAINWINDOW_H
