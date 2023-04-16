#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../runner.h"
#include <QFileDialog>
#include <util/MatrixReader.h>
#include <solver/IterativeSolver.h>
#include <exceptions/NonSymmetricAndPositiveDefiniteException.h>
#include <Eigen/Sparse>
#include <thread>
#include <QMovie>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    errorDialog(new QMessageBox()),
    thread(nullptr),
    checkMatrix(false)
{
    ui->setupUi(this);
    runButton = findChild<QPushButton*>("buttonRun");
    loadButton = findChild<QPushButton*>("buttonLoad");
    loadingLabel = findChild<QLabel*>("labelLoading");
    selectedLabel = findChild<QLabel*>("labelSelected");
    loadingImage = new QMovie(":assets/loading.gif");
    if (loadingImage->isValid()) {
        loadingImage->setScaledSize(this->size());
        loadingLabel->setMovie(loadingImage);
    }

    connect(this, SIGNAL(signal_finish()), this, SLOT(on_finishExecution()));
    connect(this, SIGNAL(signal_error(const std::string&)), this, SLOT(on_error(const std::string&)));
}

MainWindow::~MainWindow() {
    delete ui;
    deleteThread();
}


void MainWindow::on_buttonLoad_clicked() {
    QString select = QFileDialog::getOpenFileName(this, "Select a matrix file", "", "Matrix Market Format (*.mtx) ;; All Files (*.*)");
    if (!select.isEmpty()) {
        matrixFile = select;
        selectedLabel->setText("Selected " + matrixFile.split("/").last());
        runButton->setEnabled(true);
    }
}

void MainWindow::stopAnimation() {
    loadingImage->stop();
    loadingLabel->hide();
    loadingLabel->setText("");
    runButton->setEnabled(true);
    loadButton->setEnabled(true);
}

void MainWindow::deleteThread() {
    if (thread != nullptr && thread->joinable()) {
        thread->join();
    }
    delete thread;

}

void MainWindow::on_finishExecution() {
    stopAnimation();
    ResultsWindow *resultsDialog = new ResultsWindow(this);
    resultsDialog->setWindowTitle("Results - " + matrixFile.split("/").last());
    resultsDialog->buildTable<precision, Eigen::SparseMatrix<precision>>(results);
    resultsDialog->show();
    resultsDialog->buildCharts();
}

void MainWindow::on_error(const std::string &error) {
    stopAnimation();
    errorDialog->critical(this, "Error!!!!!", error.c_str());
    errorDialog->setFixedSize(500, 200);
}

void MainWindow::on_buttonRun_clicked(){
    deleteThread();
    loadingLabel->show();
    loadingImage->start();
    this->runButton->setEnabled(false);
    this->loadButton->setEnabled(false);
    thread = new std::thread([&](){
        try {
            results = testMethods<precision>(matrixFile.toStdString(), checkMatrix);
            emit signal_finish();
        } catch (const NonSymmetricAndPositiveDefiniteException &e) {
            emit signal_error("Error! the provided matrix is not positive definite and symmetric.");
        }
    });

}


void MainWindow::on_checkBox_stateChanged(int arg1) {
    checkMatrix = arg1 != 0;
}

