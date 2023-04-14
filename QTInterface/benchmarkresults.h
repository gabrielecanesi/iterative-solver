#ifndef BENCHMARKRESULTS_H
#define BENCHMARKRESULTS_H

#include <QDialog>
#include <QTableWidget>
#include <util/Benchmark.h>
#include "uiutils.h"

namespace Ui {
class BenchmarkResults;
}

class BenchmarkResults : public QDialog
{
    Q_OBJECT

public:
    template<typename Precision, typename MatrixType>
    void buildTable(const std::vector<Precision, MatrixType> &benchmarks) {
        table->setRowCount(0);

        for (auto &benchmark : benchmarks) {
            table->insertRow(table->rowCount());
            table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(benchmark.methodName()), 1));
            table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(UIUtils::formatToScientific(benchmark.tolerance())));
            table->setItem(table->rowCount() - 1, 2, new QTableWidgetItem(UIUtils::formatToScientific(benchmark.elapsedMilliseconds())));
            table->setItem(table->rowCount() - 1, 3, new QTableWidgetItem(UIUtils::formatToScientific(benchmark.neededIterations())));
            table->setItem(table->rowCount() - 1, 4, new QTableWidgetItem(UIUtils::formatToScientific(benchmark.relativeError())));
        }
    }

    explicit BenchmarkResults(QWidget *parent = nullptr);
    ~BenchmarkResults();

private slots:
    void on_pushButton_clicked();

private:
    Ui::BenchmarkResults *ui;
    QTableWidget *table;

    void exportCSV();
};

#endif // BENCHMARKRESULTS_H
