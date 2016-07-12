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

    case VK_F11:
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

    const Color bgRed(228, 82, 82);
    const Color bgGreen(82, 228, 82);

    const Color red(200, 55, 55);
    const Color green(55, 200, 55);

    int radius = 5;

    double step = 0.1;

    for (double x = -5; x < 5; x += step)
        for (double y = -5; y < 5; y += step) {
            uint out = net->predict({x, y});
            p.fillRect(fr::Rectangle((x - 0.5) * scale + width() / 2, (0.5 - y - step) * scale + height() / 2, (x - 0.5 + step) * scale + width() / 2, (0.5 - y) * scale + height() / 2), out == 0 ? bgRed : bgGreen);
        }

    for (const std::pair<Vector2, int> &point : points) {
        p.setColor(point.second == 0 ? red : green);
        p.fillEllipse((Point(scale * (point.first.x() - 0.5), scale * (0.5 - point.first.y()))) + rect().center(), radius, radius);
        p.setColor(Color());
        p.drawEllipse((Point(scale * (point.first.x() - 0.5), scale * (0.5 - point.first.y()))) + rect().center(), radius, radius);
    }
}

void MainWindow::createNet() {
    net = new Network({2, 6, 2, 2});
    net->setVerbose(false);

    net->setLearningRate(0.01);
    net->setMomentum(0.1);
    net->setL2Decay(0.001);

    net->setBatchSize(10);
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
    defaultData();
    net->init();
}

void MainWindow::defaultData() {
    int n = 200;

    for (int i = 0; i < n; i++) {
        Vector2 p((double)rand() / RAND_MAX, (double)rand() / RAND_MAX);

        // points.push_back(std::make_pair((p - Vector2(0.5, 0.5)) * 10, p.y() < cos((p.x() - 0.5) * M_PI) * 0.75 * (cos(5 * M_PI * p.x()) + 1.75) / 2));

        // points.push_back(std::make_pair((p - Vector2(0.5, 0.5)) * 10, sqrt((p.x() - 0.5) * (p.x() - 0.5) + (p.y() - 0.5) * (p.y() - 0.5)) < 0.3));

        bool green = p.y() < cos((p.x() - 0.5) * M_PI) * 0.75 * (cos(5 * M_PI * p.x()) + 1.75) / 2;
        green ? p.ry() *= 0.8 : p.ry() = 1.0 - (1.0 - p.ry()) * 0.8;
        points.push_back(std::make_pair((p - Vector2(0.5, 0.5)) * 10, green));
    }
}

void MainWindow::circleData() {
    int n = 50;

    for (int i = 0; i < n; i++) {
        double r = (double)rand() / RAND_MAX * 2;
        double t = (double)rand() / RAND_MAX * 2 * M_PI;
        points.push_back(std::make_pair(Vector2(r * sin(t), -r * cos(t)), 1));
    }

    for (int i = 0; i < n; i++) {
        double r = 3.0 + (double)rand() / RAND_MAX * 2;
        double t = 2.0 * M_PI * i / 50;
        points.push_back(std::make_pair(Vector2(r * sin(t), -r * cos(t)), 0));
    }
}

void MainWindow::spiralData() {
    int n = 100;

    for (int i = 0; i < n; i++) {
        double r = (double)i / n * 5 + (double)rand() / RAND_MAX * 0.2 - 0.1;
        double t = 1.25 * i / n * 2 * M_PI + (double)rand() / RAND_MAX * 0.2 - 0.1;
        points.push_back(std::make_pair(Vector2(r * sin(t), -r * cos(t)), 1));
    }

    for (int i = 0; i < n; i++) {
        double r = (double)i / n * 5 + (double)rand() / RAND_MAX * 0.2 - 0.1;
        double t = 1.25 * i / n * 2 * M_PI + M_PI + (double)rand() / RAND_MAX * 0.2 - 0.1;
        points.push_back(std::make_pair(Vector2(r * sin(t), -r * cos(t)), 0));
    }
}

void MainWindow::defaults() {
    scale = 50.001;
}
