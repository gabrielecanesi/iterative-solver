#include "benchmarkresults.h"
#include "ui_benchmarkresults.h"

#include <Eigen/Sparse>
#include <util/Benchmark.h>
#include <string>
#include <QTableView>
#include <QModelIndex>
#include <fstream>
#include <QFileDialog>
#include <QApplication>
#include <QLineSeries>
#include <QChart>
#include <QScatterSeries>
#include <QValueAxis>
#include <QLogValueAxis>
#include <iostream>
#include <cmath>
BenchmarkResults::BenchmarkResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BenchmarkResults),
    table(nullptr)
{
    ui->setupUi(this);
    table = findChild<QTableWidget*>("tableResults");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    chart = new Chart(this);
}

BenchmarkResults::~BenchmarkResults()
{
    delete ui;
}

void BenchmarkResults::on_pushButton_clicked()
{
    exportCSV();
}

void BenchmarkResults::buildCharts() {

    QChart *c = new QChart();
    QLogValueAxis *xAxis = new QLogValueAxis();
    QValueAxis *yAxis = new QValueAxis();
    c->setTitle("Tolerance vs relative error");
    xAxis->setTitleText("Tolerance");
    xAxis->setBase(100);


    double maxError = 0.0;
    for (unsigned int i = 0; i < table->rowCount(); ++i) {
        if (table->model()->data(table->model()->index(i, 4)).toDouble() > maxError) {
            maxError = table->model()->data(table->model()->index(i, 4)).toDouble();
        }
    }

    yAxis->setTitleText("Relative error");
    yAxis->setRange(0, maxError);
    c->addAxis(xAxis, Qt::AlignBottom);
    c->addAxis(yAxis, Qt::AlignLeft);



    for (unsigned int i = 0; i < 4; ++i) {
        QString methodName = table->model()->data(table->model()->index(i, 0)).toString();
        QLineSeries *series = new QLineSeries();
        series->setName(methodName);
        for (unsigned int j = 0; j < 4; ++j) {
            double tolerance = table->model()->data(table->model()->index(4 * j + i, 1)).toDouble();
            double relativeError = table->model()->data(table->model()->index(4 * j + i, 4)).toDouble();
            *series << QPointF(tolerance, relativeError);
        }
        c->addSeries(series);
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
    }


    c->legend()->setVisible(true);
    c->legend()->setAlignment(Qt::AlignTop);

    QChartView *chartView = new QChartView(c);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(this->chart);
    chart->setChart(c);
    chartView->resize(chart->size());
    chart->setVisible(true);
}

void BenchmarkResults::exportCSV() {
    std::stringstream stream;
    for (unsigned int j = 0; j < table->columnCount() - 1; ++j) {
        stream << table->horizontalHeaderItem(j)->text().toStdString() << ",";
    }
    stream << table->horizontalHeaderItem(table->columnCount() - 1)->text().toStdString() << "\n";
    for (unsigned int i = 0; i  < table->rowCount(); ++ i) {
        for (unsigned int j = 0; j < table->columnCount() - 1; ++j) {
            stream << table->model()->data(table->model()->index(i, j)).toString().toStdString() << ",";
        }
        stream << table->model()->data(table->model()->index(table->rowCount() - 1, table->columnCount() - 1)).toString().toStdString() << "\n";
    }

    QString saveFileName = QFileDialog::getSaveFileName(this, "Save benchmark results", "Benchmark results", "Comma Separated Values (*.csv);; All Files (*)");
    std::ofstream outStream(saveFileName.toStdString());
    outStream << stream.str();

}
