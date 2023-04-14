#include "chart.h"
#include "ui_chart.h"


#include<QChart>
#include<QChartView>
#include<QLineSeries>

Chart::Chart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chart)
{
    ui->setupUi(this);
    setWindowTitle("Chart");
}

void Chart::setChart(QChart *chartView) {
    this->chartView = chartView;
    this->chartView->show();
}

Chart::~Chart()
{
    delete ui;
}
