#ifndef CUSTOMGRAPH_H
#define CUSTOMGRAPH_H

#include <QLineSeries>
#include <QChart>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QChartView>

class CustomGraph{
public:
    CustomGraph();
    ~CustomGraph();

    void add_value(unsigned int value);
    void set_values_number(unsigned int number);
    QtCharts::QChartView *getGraphWidget() { return chartView; }
    void update();
private:
    QtCharts::QLineSeries *series;
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;

    unsigned int graph_values[30];
};

#endif // CUSTOMGRAPH_H
