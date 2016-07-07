#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), net(0) {
    resize(qApp->desktop()->size() / 1.5);
    setMinimumSize(qApp->desktop()->size() / 4);

    qsrand(QTime::currentTime().msec());

    for (int i = 0; i < 20; i++) {
        QPointF p((double)qrand() / RAND_MAX, (double)qrand() / RAND_MAX);
        points << QPair<QPointF, int>(p, p.x() > p.y());
    }

    init();

    startTimer(10);
}

Widget::~Widget() {
    delete net;
}

void Widget::timerEvent(QTimerEvent *) {
    for (const QPair<QPointF, int> &point : points)
        qDebug() << net->learn(Network::Example({point.first.x(), point.first.y()}, {1.0 - point.second, (double)point.second}));

    repaint();
}

void Widget::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Escape:
        isFullScreen() ? showNormal() : qApp->quit();
        break;

    case Qt::Key_F11:
        isFullScreen() ? showNormal() : showFullScreen();
        break;
    }
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.fillRect(rect(), Qt::lightGray);

    p.translate(width() / 2, height() / 2);

    const QColor red = QColor(255, 0, 0, 150);
    const QColor green = QColor(0, 255, 0, 150);

    int radius = 5;
    double scale = 500;

    double step = 0.01;

    for (double x = 0; x < 1; x += step)
        for (double y = 0; y < 1; y += step) {
            std::vector<double> out = net->impulse({x, y});
            p.fillRect(QRectF((x - 0.5) * scale, (y - 0.5) * scale, step * scale, step * scale), out[0] > out[1] ? red : green);
        }

    p.setRenderHint(QPainter::Antialiasing);

    for (const QPair<QPointF, int> &point : points) {
        QPainterPath path;

        path.addEllipse((point.first - QPointF(0.5, 0.5)) * scale, radius, radius);

        p.fillPath(path, point.second == 0 ? red : green);
        p.strokePath(path, QPen(Qt::black));
    }
}

void Widget::init() {
    delete net;

    net = new Network({2, 8, 2});
    net->setVerbose(false);

    net->setEta(0.1);
}