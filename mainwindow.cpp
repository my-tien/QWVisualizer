#include "mainwindow.h"

#include "classicalwalker.h"
#include "quantumwalker.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    for (auto & graph : {&cGraph, &qGraph}) {
        graph->setFlags(graph->flags() ^ Qt::FramelessWindowHint);
        graph->columnAxis()->setRange(0,6);
        graph->valueAxis()->setRange(0, 1);
        auto graphWidget = QWidget::createWindowContainer(graph, this);
        graphWidget->setMinimumSize(500, 500);
        graphLayout.addWidget(graphWidget);
    }
    mainLayout.addLayout(&graphLayout);

    pathLenSpin.setValue(7);
    stepsSpin.setValue(2);
    pathLenSpin.setPrefix("length: ");
    stepsSpin.setSuffix(" steps");

    int row = 0;
    controlsLayout.setAlignment(Qt::AlignTop);
    controlsLayout.addWidget(&pathLenSpin, row++, 0);
    controlsLayout.addWidget(&stepsSpin, row++, 0);
    mainLayout.addLayout(&controlsLayout);
    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(&mainLayout);
    simulate();

    QObject::connect(&pathLenSpin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](const int &) { simulate(); });
    QObject::connect(&stepsSpin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](const int &) { simulate(); });
}

void MainWindow::simulate() {
    QuantumWalker qWalker{pathLenSpin.value()};
    ClassicalWalker cWalker{pathLenSpin.value()};
    auto sim = [this](RandomWalker & walker, QtDataVisualization::Q3DBars & graph) {
        for (auto * series : graph.seriesList()) {
            graph.removeSeries(series);
        }
        graph.columnAxis()->setRange(0, pathLenSpin.value() - 1);
        QtDataVisualization::QBar3DSeries * series = new QtDataVisualization::QBar3DSeries();
        QtDataVisualization::QBarDataRow * data = new QtDataVisualization::QBarDataRow();

        walker.step(stepsSpin.value());
        const auto state = walker.getProbabilities();
        for (std::size_t i = 0; i < state.rows(); ++i) {
            *data << state[i];
        }
        series->dataProxy()->addRow(data);
        graph.addSeries(series);
    };
    sim(cWalker, cGraph);
    sim(qWalker, qGraph);
}
