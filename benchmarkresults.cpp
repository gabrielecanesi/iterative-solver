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
