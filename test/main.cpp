#include <iostream>
#include <cstdlib>
#include <ctime>

#include "network.h"

#include "fr/image.h"

std::vector<double> processImage(const fr::Image &image) {
    std::vector<double> result;

    for (int x = 0; x < image.width(); x++)
        for (int y = 0; y < image.height(); y++) {
            int color = image.getPixel(x, y);

            result.push_back((double)fr::Color::red(color) / 255);
            result.push_back((double)fr::Color::green(color) / 255);
            result.push_back((double)fr::Color::blue(color) / 255);
        }

    return result;
}

int main(int, const char **) {
    srand(time(0));

    std::vector<double> _1 = processImage(fr::Image("data/1.bmp"));
    std::vector<double> _2 = processImage(fr::Image("data/2.bmp"));
    std::vector<double> _3 = processImage(fr::Image("data/3.bmp"));
    std::vector<double> _4 = processImage(fr::Image("data/4.bmp"));
    std::vector<double> _5 = processImage(fr::Image("data/5.bmp"));
    std::vector<double> _6 = processImage(fr::Image("data/6.bmp"));
    std::vector<double> _7 = processImage(fr::Image("data/7.bmp"));
    std::vector<double> _8 = processImage(fr::Image("data/8.bmp"));
    std::vector<double> _o = processImage(fr::Image("data/o.bmp"));
    std::vector<double> _f = processImage(fr::Image("data/f.bmp"));
    std::vector<double> _cd = processImage(fr::Image("data/cd.bmp"));
    std::vector<double> _cm = processImage(fr::Image("data/cm.bmp"));
    std::vector<double> _cl = processImage(fr::Image("data/cl.bmp"));

    std::vector<Network::Example> examples;

    examples.push_back(Network::Example(_1, 0));
    examples.push_back(Network::Example(_2, 1));
    examples.push_back(Network::Example(_3, 2));
    examples.push_back(Network::Example(_4, 3));
    examples.push_back(Network::Example(_5, 4));
    examples.push_back(Network::Example(_6, 5));
    examples.push_back(Network::Example(_7, 6));
    examples.push_back(Network::Example(_8, 7));
    examples.push_back(Network::Example(_o, 8));
    examples.push_back(Network::Example(_f, 9));
    examples.push_back(Network::Example(_cd, 10));
    examples.push_back(Network::Example(_cm, 10));
    examples.push_back(Network::Example(_cl, 10));

    int size = 48 * 48 * 3;

    Network net({size, 11});

    net.setLearningRate(0.01);
    net.setMomentum(0.1);
    net.setL2Decay(0.001);

    net.setMaxEpochs(1000);
    net.setMaxLoss(1e-4);

    net.train(examples);

    std::cout << net.predict(_1) << "\n";
    std::cout << net.predict(_2) << "\n";
    std::cout << net.predict(_3) << "\n";
    std::cout << net.predict(_4) << "\n";
    std::cout << net.predict(_5) << "\n";
    std::cout << net.predict(_6) << "\n";
    std::cout << net.predict(_7) << "\n";
    std::cout << net.predict(_8) << "\n";
    std::cout << net.predict(_o) << "\n";
    std::cout << net.predict(_f) << "\n";
    std::cout << net.predict(_cd) << "\n";
    std::cout << net.predict(_cm) << "\n";
    std::cout << net.predict(_cl) << "\n";

    return 0;
}
