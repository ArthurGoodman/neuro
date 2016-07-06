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
    double alpha;
    double eta;
    double maxError;
    int maxEpochs;

    std::vector<Matrix<double>> w;
    std::vector<std::vector<double>> n;

public:
    Network(const std::vector<int> &sizes);

    std::vector<double> impulse(const std::vector<double> input);
    void learn(std::vector<Example> &examples);

    double getAlpha();
    void setAlpha(double alpha);

    double getEta();
    void setEta(double eta);

    double getMaxError();
    void setMaxError(double maxError);

    int getMaxEpochs();
    void setMaxEpochs(int maxEpochs);

private:
    double sigmoid(double x) const;
};
