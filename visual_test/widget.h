#pragma once

#include <QtWidgets>

#include "network.h"

class Widget : public QWidget {
    Q_OBJECT

    Network *net;
    QVector<QPair<QPointF, int>> points;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void timerEvent(QTimerEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    void init();
};
