#include "mainwindow.h"

#include <iostream>
#include <ctime>
#include <cmath>

MainWindow::MainWindow() {
    srand(time(0));

    resize(Application::desktop()->size() / 1.5);
    setMinimumSize(Application::desktop()->size() / 4);
    move(Application::desktop()->rect().center() - rect().center());

    createNet();
    init();
    defaults();

    setTimer(0);
}

MainWindow::~MainWindow() {
    delete net;
}

void MainWindow::closeEvent() {
    Application::instance()->quit();
}

void MainWindow::timerEvent(TimerEvent *) {
    learn();
    update();
}

void MainWindow::keyPressEvent(KeyEvent *e) {
    switch (e->key()) {
    case VK_ESCAPE:
        close();
        break;

    case 'R':
        init();
        break;

    case VK_BACK:
        defaults();
        break;
    }
}

void MainWindow::mouseWheelEvent(MouseWheelEvent *e) {
    if (e->delta() > 0)
        scale *= 1.1;
    else
        scale /= 1.1;
}

void MainWindow::paintEvent() {
    Painter p(this);
    p.fillRect(rect(), Color(211, 211, 211));

    p.setAntialiasing(true);

    Color red(255, 0, 0, 150);
    Color green(0, 255, 0, 150);

    int radius = 5;

    double step = 0.01;

    for (double x = 0; x < 1; x += step)
        for (double y = 0; y < 1; y += step) {
            uint out = net->predict({x, y});
            p.fillRect(fr::Rectangle((x - 0.5) * scale + width() / 2, (0.5 - y - step) * scale + height() / 2, (x - 0.5 + step) * scale + width() / 2, (0.5 - y) * scale + height() / 2), out == 0 ? red : green);
        }

    for (const std::pair<Vector2, int> &point : points) {
        p.setColor(point.second == 0 ? red : green);
        p.fillEllipse((Point(scale * (point.first.x() - 0.5), scale * (0.5 - point.first.y()))) + rect().center(), radius, radius);
        p.setColor(Color());
        p.drawEllipse((Point(scale * (point.first.x() - 0.5), scale * (0.5 - point.first.y()))) + rect().center(), radius, radius);
    }
}

void MainWindow::createNet() {
    net = new Network({2, 6, 2});
    net->setVerbose(false);

    net->setLearningRate(0.01);
    net->setMomentum(0.1);
    net->setL2Decay(0);

    net->setBatchSize(1);
}

void MainWindow::learn() {
    const int n = 20;

    double averageLoss = 0;

    for (int i = 0; i < n; i++)
        for (const std::pair<Vector2, int> &point : points)
            averageLoss += net->learn(Network::Example({point.first.x(), point.first.y()}, point.second));

    averageLoss /= n * points.size();

    std::cout << averageLoss << "\n" << std::flush;
}

void MainWindow::init() {
    points.clear();

    for (int i = 0; i < 100; i++) {
        Vector2 p((double)rand() / RAND_MAX, (double)rand() / RAND_MAX);

        points.push_back(std::pair<Vector2, int>(p, p.y() < cos((p.x() - 0.5) * M_PI) * 0.75 * (cos(5 * M_PI * p.x()) + 1.75) / 2));

        // points << QPair<QPointF, int>(p, sqrt((p.x() - 0.5) * (p.x() - 0.5) + (p.y() - 0.5) * (p.y() - 0.5)) < 0.3);

        // bool green = p.y() < cos((p.x() - 0.5) * M_PI) * 0.75 * (cos(5 * M_PI * p.x()) + 1.75) / 2;
        // if (green)
        //     p.ry() *= 0.8;
        // else
        //     p.ry() = 1.0 - (1.0 - p.ry()) * 0.8;
        // points << QPair<QPointF, int>(p, green);
    }

    net->init();
}

void MainWindow::defaults() {
    scale = 500;
}
