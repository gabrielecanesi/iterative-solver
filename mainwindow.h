#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <thread>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonLoad_clicked();

    void on_buttonRun_clicked();

private:
    Ui::MainWindow *ui;
    QPushButton *runButton;
    QLabel *loadingLabel;
    QMovie *loadingImage;
    QString matrixFile;
    std::thread *thread;

};
#endif // MAINWINDOW_H
