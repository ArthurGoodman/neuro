#pragma once

#include <QtWidgets>

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);
};
