#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
    resize(qApp->desktop()->size() / 1.5);
    setMinimumSize(qApp->desktop()->size() / 4);
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
}
