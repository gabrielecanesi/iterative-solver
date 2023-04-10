#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <thread>
#include <QLabel>

#include "benchmarkresults.h"

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

    void on_finishExecution();

signals:
    void signal_finish();

private:
    Ui::MainWindow *ui;
    QPushButton *runButton;
    QPushButton *loadButton;
    QLabel *loadingLabel;
    QMovie *loadingImage;
    QString matrixFile;
    BenchmarkResults *resultsDialog;
    std::thread *thread;

};
#endif // MAINWINDOW_H
