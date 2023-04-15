#include "chart.h"
#include "ui_chart.h"


#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QLineSeries>
#include <QLogValueAxis>
#include <QTableView>

Chart::Chart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chart),
    errorTolChart(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Chart");
    initErrorTolChart();
}


void Chart::initErrorTolChart() {
    QRect screenrect = QApplication::primaryScreen()->geometry();
    move(screenrect.top(), screenrect.left());
    errorTolChart = new QChart();
    xAxis = new QLogValueAxis();
    yAxis = new QLogValueAxis();
    errorTolChart->setTitle("Tolerance vs relative error - Log scale axes");
    xAxis->setTitleText("Tolerance");
    xAxis->setBase(10);
    xAxis->setMin(10e-10);
    xAxis->setLabelFormat("%g");
    xAxis->setMax(10e-4);
    yAxis->setBase(0);
    xAxis->setMinorTickCount(-1);
    yAxis->setMinorTickCount(-1);
    yAxis->setLabelFormat("%g");
    yAxis->setBase(10);
    errorTolChart->addAxis(xAxis, Qt::AlignBottom);
    errorTolChart->addAxis(yAxis, Qt::AlignLeft);
    chartView = new QChartView(errorTolChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(this);
    chartView->resize(size());
}

void Chart::buildErrorTolChart(const QTableWidget * const table) {
    double maxError = 0.0;
    double minError = 10000000000;
    for (unsigned int i = 0; i < table->rowCount(); ++i) {
        if (table->model()->data(table->model()->index(i, 4)).toDouble() > maxError) {
            maxError = table->model()->data(table->model()->index(i, 4)).toDouble();
        }

        if (table->model()->data(table->model()->index(i, 4)).toDouble() < minError) {
            minError = table->model()->data(table->model()->index(i, 4)).toDouble();
        }
    }

    yAxis->setTitleText("Relative error");
    yAxis->setRange(minError, maxError);

    for (unsigned int i = 0; i < 4; ++i) {
        QString methodName = table->model()->data(table->model()->index(i, 0)).toString();
        QLineSeries *series = new QLineSeries();
        series->setName(methodName);
        for (unsigned int j = 0; j < 4; ++j) {
            double tolerance = table->model()->data(table->model()->index(4 * j + i, 1)).toDouble();
            double relativeError = table->model()->data(table->model()->index(4 * j + i, 4)).toDouble();
            *series << QPointF(tolerance, relativeError);
        }
        errorTolChart->addSeries(series);
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
    }


    errorTolChart->legend()->setVisible(true);
    errorTolChart->legend()->setAlignment(Qt::AlignTop);
}

void Chart::resizeEvent(QResizeEvent *event) {
    chartView->resize(size());
}

Chart::~Chart()
{
    delete ui;
}
