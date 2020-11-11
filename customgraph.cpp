#include "customgraph.h"
#include <QValueAxis>

CustomGraph::CustomGraph(){
    series = new QtCharts::QLineSeries();

    chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Realtime background graph");

    chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    memset(graph_values, 0, sizeof(graph_values)*sizeof(unsigned int));
}

CustomGraph::~CustomGraph(){
    delete series;
    delete chart;
    delete chartView;
}

void CustomGraph::add_value(unsigned int value){
    graph_values[0] = value;
    for(uint8_t k=30-1; k>0; k--) graph_values[k]=graph_values[k-1];

    series->clear();
    for(unsigned i = 0; i < 30; i++) series->append(i, graph_values[i]);
    graph_values[0] = 0;
}

void CustomGraph::set_values_number(unsigned int number){

}

void CustomGraph::update(){
    chart->removeSeries(series);
    chart->addSeries(series);
}
