#ifndef CHART_H
#define CHART_H

#include <QDialog>
#include <QChartView>
#include <QTableWidget>
#include <QLogValueAxis>

namespace Ui {
class Chart;
}

class Chart : public QDialog
{
    Q_OBJECT

public:
    void buildErrorTolChart(const QTableWidget * const view);
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();

private:
    Ui::Chart *ui;
    void resizeEvent(QResizeEvent *event);
    QChart *errorTolChart;
    QLogValueAxis *xAxis;
    QLogValueAxis *yAxis;
    QChartView *chartView;

    void initErrorTolChart();
};

#endif // CHART_H
