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
    double learningRate;
    double maxError;
    int maxEpochs;

    bool verbose;

    std::vector<Matrix<double>> w;
    std::vector<std::vector<double>> n;

public:
    Network(const std::vector<int> &sizes);

    void init();

    std::vector<double> impulse(const std::vector<double> &input);
    void train(const std::vector<Example> &examples);
    double learn(const Example &e);

    bool isVerbose() const;
    void setVerbose(bool verbose);

    double getLearningRate() const;
    void setLearningRate(double learningRate);

    double getMaxError() const;
    void setMaxError(double maxError);

    int getMaxEpochs() const;
    void setMaxEpochs(int maxEpochs);
};
