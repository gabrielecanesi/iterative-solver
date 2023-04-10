#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "runner.h"
#include <QFileDialog>
#include <MatrixReader.h>
#include <IterativeSolver.h>
#include <eigen3/Eigen/Sparse>
#include <thread>
#include <QMovie>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    thread(nullptr)
{
    ui->setupUi(this);
    runButton = findChild<QPushButton*>("buttonRun");
    loadButton = findChild<QPushButton*>("buttonLoad");
    loadingLabel = findChild<QLabel*>("labelLoading");
    loadingImage = new QMovie(":assets/loading.gif");
    if (loadingImage->isValid()) {
        loadingImage->setScaledSize(this->size());
        loadingLabel->setMovie(loadingImage);
    }

    connect(this, SIGNAL(signal_finish()), this, SLOT(on_finishExecution()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete loadingLabel;
    delete loadingImage;
    delete loadButton;
}


void MainWindow::on_buttonLoad_clicked() {
    this->matrixFile = QFileDialog::getOpenFileName(this, "Select a matrix file", "", "Matrix Market Format (*.mtx) ;; All Files (*.*)");
    this->runButton->setEnabled(true);
}

void MainWindow::on_finishExecution() {
    loadingImage->stop();
    loadingLabel->hide();
    loadingLabel->setText("");
    this->runButton->setEnabled(true);
    this->loadButton->setEnabled(true);
}

void MainWindow::on_buttonRun_clicked(){
    if (this->runButton->isEnabled()){
        if (thread != nullptr) {
            thread->join();
        }
        delete thread;

        loadingLabel->show();
        loadingImage->start();
        this->runButton->setEnabled(false);
        this->loadButton->setEnabled(false);
        thread = new std::thread([&](){
            std::vector<IterativeBenchmark<double, Eigen::SparseMatrix<double>>> results = testMethods<double>(matrixFile.toStdString());
            for (auto &benchmark : results) {
                std::cout << benchmark.methodName() << std::endl;
                std::cout << benchmark.relativeError() << std::endl;
                std::cout << benchmark.elapsedMilliseconds() << std::endl;
                std::cout << benchmark.neededIterations() << std::endl << std::endl;
            }
            emit signal_finish();
        });
    }
}

