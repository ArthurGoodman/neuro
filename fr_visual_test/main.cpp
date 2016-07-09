#include <fr.h>

#include "mainwindow.h"

int main(int argc, const char **argv) {
    Application app(argc, argv);

    MainWindow w;
    w.show();

    return app.run();
}
