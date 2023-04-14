#ifndef CHART_H
#define CHART_H

#include <QDialog>
#include <QChartView>

namespace Ui {
class Chart;
}

class Chart : public QDialog
{
    Q_OBJECT

public:
    void setChart(QChart *chartView);
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();

private:
    Ui::Chart *ui;
    QChart *chartView;
};

#endif // CHART_H
