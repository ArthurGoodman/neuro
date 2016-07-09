#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), net(0) {
    resize(qApp->desktop()->size() / 1.5);
    setMinimumSize(qApp->desktop()->size() / 4);

    qsrand(QTime::currentTime().msec());

    scale = 500;

    net = new Network({2, 6, 2});
    net->setVerbose(false);

    net->setLearningRate(0.01);
    net->setMomentum(0.1);
    net->setL2Decay(0);

    net->setBatchSize(1);

    init();

    startTimer(0);
}

Widget::~Widget() {
    delete net;
}

void Widget::timerEvent(QTimerEvent *) {
    const int n = 20;

    double averageLoss = 0;

    for (int i = 0; i < n; i++)
        for (const QPair<QPointF, int> &point : points)
            averageLoss += net->learn(Network::Example({point.first.x(), point.first.y()}, point.second));

    averageLoss /= n * points.size();

    qDebug() << averageLoss;

    update();
}

void Widget::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Escape:
        isFullScreen() ? showNormal() : qApp->quit();
        break;

    case Qt::Key_F11:
        isFullScreen() ? showNormal() : showFullScreen();
        break;

    case Qt::Key_R:
        init();
        break;
    }
}

void Widget::wheelEvent(QWheelEvent *e) {
    if (e->delta() > 0)
        scale *= 1.1;
    else
        scale /= 1.1;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.fillRect(rect(), Qt::lightGray);

    p.translate(width() / 2, height() / 2);

    const QColor bgRed(228, 82, 82);
    const QColor bgGreen(82, 228, 82);

    const QColor red(200, 55, 55);
    const QColor green(55, 200, 55);

    int radius = 5;

    double step = 0.01;

    for (double x = 0; x < 1; x += step)
        for (double y = 0; y < 1; y += step) {
            uint out = net->predict({x, y});
            p.fillRect(QRectF((x - 0.5) * scale, (0.5 - y - step) * scale, step * scale, step * scale), out == 0 ? bgRed : bgGreen);
        }

    for (const QPair<QPointF, int> &point : points) {
        QPainterPath path;

        path.addEllipse((QPointF(point.first.x() - 0.5, 0.5 - point.first.y())) * scale, radius, radius);

        p.fillPath(path, point.second == 0 ? red : green);
        p.strokePath(path, QPen(Qt::black));
    }
}

void Widget::init() {
    points.clear();

    for (int i = 0; i < 100; i++) {
        QPointF p((double)qrand() / RAND_MAX, (double)qrand() / RAND_MAX);

        points << QPair<QPointF, int>(p, p.y() < cos((p.x() - 0.5) * M_PI) * 0.75 * (cos(5 * M_PI * p.x()) + 1.75) / 2);

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
