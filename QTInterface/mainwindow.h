#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <thread>
#include <QLabel>
#include "errordialog.h"

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

    void on_error(const std::string &error);

    void on_checkBox_stateChanged(int arg1);

signals:
    void signal_finish();
    void signal_error(const std::string &error);

private:
    Ui::MainWindow *ui;
    QPushButton *runButton;
    QPushButton *loadButton;
    QLabel *loadingLabel;
    QMovie *loadingImage;
    QString matrixFile;
    BenchmarkResults *resultsDialog;
    ErrorDialog *errorDialog;
    bool checkMatrix;
    std::thread *thread;


    void stopAnimation();

};
#endif // MAINWINDOW_H
