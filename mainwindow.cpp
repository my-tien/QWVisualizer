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
    int row = 0;
    auto addNumberControl = [&row, this](QSpinBox & spin, QSlider & slider, const int value, const int min, const int max, const QString & prefix, const QString & suffix) {
        QObject::connect(&spin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&slider, this](const int value) {
            if (value != slider.value()) {
                slider.setValue(value);
            }
            simulate();
        });
        QObject::connect(&slider, &QSlider::valueChanged, [&spin, this](const int value) {
            if (value != spin.value()) {
                spin.setValue(value);
            }
        });

        spin.setPrefix(prefix);
        spin.setSuffix(suffix);
        spin.setRange(min, max);
        slider.setRange(spin.minimum(), spin.maximum());
        spin.setValue(value);
        slider.setOrientation(Qt::Horizontal);
        controlsLayout.addWidget(&spin, row, 0);
        controlsLayout.addWidget(&slider, row++, 1);
    };
    addNumberControl(pathLenSpin, pathLenSlider, 7, 1, 100, "length: ", "");
    addNumberControl(stepsSpin, stepsSlider, 3, 1, 1000, "", " steps");
    controlsLayout.setAlignment(Qt::AlignTop);
    graphDummyWidget.setLayout(&graphLayout);
    controlsDummyWidget.setLayout(&controlsLayout);
    split.addWidget(&graphDummyWidget);
    split.addWidget(&controlsDummyWidget);
    mainLayout.addWidget(&split);
    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(&mainLayout);
    simulate();
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
        double max = 0;
        for (std::size_t i = 0; i < state.rows(); ++i) {
            max = std::max(max, state[i]);
            *data << state[i];
        }
        graph.valueAxis()->setRange(0, max);
        series->dataProxy()->addRow(data);
        graph.addSeries(series);
    };
    sim(cWalker, cGraph);
    sim(qWalker, qGraph);
}
