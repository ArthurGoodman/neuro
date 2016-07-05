#include <iostream>
#include <cstdlib>
#include <ctime>

#include "network.h"

#include "fr/image.h"

void print(const std::vector<double> &v) {
    for (int i = 0; i < (int)v.size(); i++)
        std::cout << v[i] << " ";

    std::cout << "\n";
}

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

    fr::Image _1("data/1.bmp");
    fr::Image _2("data/2.bmp");
    fr::Image _3("data/3.bmp");
    fr::Image _4("data/4.bmp");
    fr::Image _5("data/5.bmp");
    fr::Image _6("data/6.bmp");
    fr::Image _7("data/7.bmp");
    fr::Image _8("data/8.bmp");
    //    fr::Image _o("data/o.bmp");
    //    fr::Image _f("data/f.bmp");
    //    fr::Image _cd("data/cd.bmp");
    //    fr::Image _cm("data/cm.bmp");
    //    fr::Image _cl("data/cl.bmp");

    std::vector<double> _1_data = processImage(_1);
    std::vector<double> _2_data = processImage(_2);
    std::vector<double> _3_data = processImage(_3);
    std::vector<double> _4_data = processImage(_4);
    std::vector<double> _5_data = processImage(_5);
    std::vector<double> _6_data = processImage(_6);
    std::vector<double> _7_data = processImage(_7);
    std::vector<double> _8_data = processImage(_8);
    //    std::vector<double> _o_data = processImage(_o);
    //    std::vector<double> _f_data = processImage(_f);
    //    std::vector<double> _cd_data = processImage(_cd);
    //    std::vector<double> _cm_data = processImage(_cm);
    //    std::vector<double> _cl_data = processImage(_cl);

    std::vector<Network::Example> examples;

    examples.push_back(Network::Example(_1_data, {1, 0, 0, 0, 0, 0, 0, 0 /*, 0, 0, 0*/}));
    examples.push_back(Network::Example(_2_data, {0, 1, 0, 0, 0, 0, 0, 0 /*, 0, 0, 0*/}));
    examples.push_back(Network::Example(_3_data, {0, 0, 1, 0, 0, 0, 0, 0 /*, 0, 0, 0*/}));
    examples.push_back(Network::Example(_4_data, {0, 0, 0, 1, 0, 0, 0, 0 /*, 0, 0, 0*/}));
    examples.push_back(Network::Example(_5_data, {0, 0, 0, 0, 1, 0, 0, 0 /*, 0, 0, 0*/}));
    examples.push_back(Network::Example(_6_data, {0, 0, 0, 0, 0, 1, 0, 0 /*, 0, 0, 0*/}));
    examples.push_back(Network::Example(_7_data, {0, 0, 0, 0, 0, 0, 1, 0 /*, 0, 0, 0*/}));
    examples.push_back(Network::Example(_8_data, {0, 0, 0, 0, 0, 0, 0, 1 /*, 0, 0, 0*/}));
    //    examples.push_back(Network::Example(_o_data, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}));
    //    examples.push_back(Network::Example(_f_data, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}));
    //    examples.push_back(Network::Example(_cd_data, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}));
    //    examples.push_back(Network::Example(_cm_data, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}));
    //    examples.push_back(Network::Example(_cl_data, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}));

    Network net({6912, 300, 8 /*11*/});

    net.learn(examples);

    print(net.impulse(_1_data));
    print(net.impulse(_2_data));
    print(net.impulse(_3_data));
    print(net.impulse(_4_data));
    print(net.impulse(_5_data));
    print(net.impulse(_6_data));
    print(net.impulse(_7_data));
    print(net.impulse(_8_data));
    //    print(net.impulse(_o_data));
    //    print(net.impulse(_f_data));
    //    print(net.impulse(_cd_data));
    //    print(net.impulse(_cm_data));
    //    print(net.impulse(_cl_data));

    return 0;
}
