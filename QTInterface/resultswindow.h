#ifndef CHART_H
#define CHART_H

#include <QDialog>
#include <QChartView>
#include <QTableWidget>
#include <QLogValueAxis>
#include "uiutils.h"
#include <util/Benchmark.h>

namespace Ui {
class Chart;
}

class ResultsWindow : public QDialog
{
    Q_OBJECT

public:
    void buildErrorTolChart(const QTableWidget * const view);
    void buildTimeChart(const QTableWidget * const table);
    explicit ResultsWindow(QWidget *parent = nullptr);
    void showEvent(QShowEvent *event);
    void exportCsv();

    template <typename Precision, typename MatrixType>
    void buildTable(const std::vector<IterativeBenchmark<Precision, MatrixType>> &benchmarks) {
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
    void buildCharts();
    ~ResultsWindow();
private slots:
    void on_tabWidget_currentChanged(int index);

    void on_exportCsvButton_clicked();

private:
    Ui::Chart *ui;
    QChart *errorTolChart;
    QChart *timeChart;
    QLogValueAxis *xAxis;
    QLogValueAxis *yAxis;
    QLogValueAxis *xTimeAxis;
    QLogValueAxis *yTimeAxis;
    QChartView *chartView;
    QChartView *timeChartView;
    QTableWidget *table;
    void initErrorTolChart();
    void initTimeChart();
    void resizeEvent(QResizeEvent *event);
    void init(QChart **chart, QLogValueAxis **xAxis, QLogValueAxis **yAxis, QChartView **chartView, const QString &frameName);
    void buildChart(const QTableWidget * const table, int yCol, const QString &yLabel, QLogValueAxis *xAxis, QLogValueAxis *yAxis, QChart *chart, QChartView *chartView, const QString &title, const QString &frameName);
};

#endif // CHART_H
