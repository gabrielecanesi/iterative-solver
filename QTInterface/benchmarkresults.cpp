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
    chart->close();
    delete chart;
    chart = new Chart(this);
    chart->buildErrorTolChart(table);
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
        stream << table->model()->data(table->model()->index(i, table->columnCount() - 1)).toString().toStdString() << "\n";
    }

    QString saveFileName = QFileDialog::getSaveFileName(this, "Save benchmark results", "Benchmark results", "Comma Separated Values (*.csv);; All Files (*)");
    std::ofstream outStream(saveFileName.toStdString());
    outStream << stream.str();

}
