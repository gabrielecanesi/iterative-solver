#include "benchmarkresults.h"
#include "ui_benchmarkresults.h"

#include <eigen3/Eigen/Sparse>
#include <Benchmark.h>
#include <string>

BenchmarkResults::BenchmarkResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BenchmarkResults),
    table(nullptr)
{
    ui->setupUi(this);
    table = findChild<QTableWidget*>("tableResults");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

BenchmarkResults::~BenchmarkResults()
{
    delete ui;
}

QString formatToScientific(double value) {
    std::ostringstream stream;
    stream << value;
    return QString::fromStdString(stream.str());
}

void BenchmarkResults::buildTable(const std::vector<IterativeBenchmark<double, Eigen::SparseMatrix<double>>> &benchmarks) {
    table->clearContents();
    for (auto &benchmark : benchmarks) {
        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(benchmark.methodName()), 1));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(formatToScientific(benchmark.tolerance())));
        table->setItem(table->rowCount() - 1, 2, new QTableWidgetItem(formatToScientific(benchmark.elapsedMilliseconds())));
        table->setItem(table->rowCount() - 1, 3, new QTableWidgetItem(formatToScientific(benchmark.neededIterations())));
        table->setItem(table->rowCount() - 1, 4, new QTableWidgetItem(formatToScientific(benchmark.relativeError())));
    }
}
