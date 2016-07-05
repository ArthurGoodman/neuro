#pragma once

#include <vector>

#include "matrix.h"

class Network {
public:
    class Example {
        std::vector<double> in;
        std::vector<double> out;

    public:
        Example(const std::vector<double> &in, const std::vector<double> &out);

        const std::vector<double> &input() const;
        const std::vector<double> &output() const;
    };

private:
    static const double alpha;
    static const double eta;
    static const double maxError;
    static const int maxEpochs;

    std::vector<Matrix<double>> w;
    std::vector<std::vector<double>> n;

public:
    Network(const std::vector<int> &sizes);

    std::vector<double> impulse(const std::vector<double> input);
    void learn(std::vector<Example> &examples);

private:
    static double sigmoid(double x);
};
