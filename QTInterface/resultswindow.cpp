#include "resultswindow.h"
#include "ui_resultswindow.h"


#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QLogValueAxis>
#include <QTableView>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <QScatterSeries>

ResultsWindow::ResultsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chart),
    errorTolChart(nullptr) {
    ui->setupUi(this);
    setWindowTitle("Results");
    table = findChild<QTableWidget*>("tableResults");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    initErrorTolChart();
    initTimeChart();
}


void ResultsWindow::initErrorTolChart() {
    init(&errorTolChart, &xAxis, &yAxis, &chartView, "frameError");
}

void ResultsWindow::initTimeChart() {
    init(&timeChart, &xTimeAxis, &yTimeAxis, &timeChartView, "frameTime");
}


void ResultsWindow::init(QChart **chart, QLogValueAxis **xAxis, QLogValueAxis **yAxis, QChartView **chartView, const QString &frameName) {
    *chart = new QChart();
    *xAxis = new QLogValueAxis();
    *yAxis = new QLogValueAxis();
    *chartView = new QChartView(*chart);
    (*xAxis)->setTitleText("Tolerance");
    (*xAxis)->setBase(10);
    (*xAxis)->setMin(1e-10);
    (*xAxis)->setLabelFormat("%g");
    (*xAxis)->setMax(1e-4);
    (*yAxis)->setBase(10);
    (*xAxis)->setMinorTickCount(-1);
    (*yAxis)->setMinorTickCount(-1);
    (*yAxis)->setLabelFormat("%g");
    (*chart)->addAxis(*xAxis, Qt::AlignBottom);
    (*chart)->addAxis(*yAxis, Qt::AlignLeft);
}

void ResultsWindow::buildCharts() {
    buildErrorTolChart(table);
    buildTimeChart(table);
}

void ResultsWindow::buildErrorTolChart(const QTableWidget * const table) {
    buildChart(table, 4, "Relative error", xAxis, yAxis, errorTolChart, chartView, "Tolerance vs relative error", "frameError");
}

void ResultsWindow::buildTimeChart(const QTableWidget * const table) {
    buildChart(table, 2, "Time (ms)", xTimeAxis, yTimeAxis, timeChart, timeChartView, "Tolerance vs time", "frameTime");
}


void ResultsWindow::buildChart(const QTableWidget * const table, int yCol,const QString &yLabel, QLogValueAxis *xAxis, QLogValueAxis *yAxis, QChart *chart, QChartView *chartView, const QString &title, const QString &frameName) {
    yAxis->setTitleText(yLabel);
    chart->setTitle(title);

    double maxValue = 0.0;
    double minValue = 10000000000;
    for (unsigned int i = 0; i < table->rowCount(); ++i) {
        if (table->model()->data(table->model()->index(i, yCol)).toDouble() > maxValue) {
            maxValue = table->model()->data(table->model()->index(i, yCol)).toDouble();
        }

        if (table->model()->data(table->model()->index(i, yCol)).toDouble() < minValue) {
            minValue = table->model()->data(table->model()->index(i, yCol)).toDouble();
        }
    }

    yAxis->setRange(minValue, maxValue);

    for (unsigned int i = 0; i < 4; ++i) {
        QString methodName = table->model()->data(table->model()->index(i, 0)).toString();
        QLineSeries *series = new QLineSeries();
        series->setName(methodName);
        for (unsigned int j = 0; j < 4; ++j) {
            double tolerance = table->model()->data(table->model()->index(4 * j + i, 1)).toDouble();
            double metricValue = table->model()->data(table->model()->index(4 * j + i, yCol)).toDouble();
            *series << QPointF(tolerance, metricValue);
        }
        chart->addSeries(series);
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
    }

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(findChild<QFrame*>(frameName));
}

void ResultsWindow::resizeEvent(QResizeEvent *event) {
    chartView->resize((findChild<QFrame*>("frameError"))->size());
    timeChartView->resize((findChild<QFrame*>("frameTime"))->size());
}

ResultsWindow::~ResultsWindow()
{
    delete ui;
}

void ResultsWindow::on_tabWidget_currentChanged(int index)
{
    chartView->resize((findChild<QFrame*>("frameError"))->size());
    timeChartView->resize((findChild<QFrame*>("frameTime"))->size());
}

void ResultsWindow::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    chartView->resize((findChild<QFrame*>("frameError"))->size());
    timeChartView->resize((findChild<QFrame*>("frameTime"))->size());
}


void ResultsWindow::exportCsv() {
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

void ResultsWindow::on_exportCsvButton_clicked()
{
    exportCsv();
}

