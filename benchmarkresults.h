#ifndef BENCHMARKRESULTS_H
#define BENCHMARKRESULTS_H

#include <QDialog>
#include <QTableWidget>
#include <eigen3/Eigen/Sparse>
#include <Benchmark.h>
namespace Ui {
class BenchmarkResults;
}

class BenchmarkResults : public QDialog
{
    Q_OBJECT

public:
    void buildTable(const std::vector<IterativeBenchmark<double, Eigen::SparseMatrix<double>>> &benchmarks);
    explicit BenchmarkResults(QWidget *parent = nullptr);
    ~BenchmarkResults();

private:
    Ui::BenchmarkResults *ui;
    QTableWidget *table;
};

#endif // BENCHMARKRESULTS_H
