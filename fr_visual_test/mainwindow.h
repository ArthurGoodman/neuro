#pragma once

#include "fr.h"
#include "network.h"

class MainWindow : public Window {
    Network *net;
    std::vector<std::pair<Vector2, int>> points;

    double scale;

public:
    explicit MainWindow();
    ~MainWindow();

protected:
    void closeEvent();
    void timerEvent(TimerEvent *e);
    void keyPressEvent(KeyEvent *e);
    void mouseWheelEvent(MouseWheelEvent *e);
    void paintEvent();

private:
    void createNet();
    void learn();
    void init();
    void defaults();
};
