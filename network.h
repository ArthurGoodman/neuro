#pragma once

#include <vector>
#include <string>

#include "matrix.h"

typedef unsigned int uint;

class Network {
public:
    class Example {
        std::vector<double> in;
        uint ci;

    public:
        Example(const std::vector<double> &in, uint classIndex);

        const std::vector<double> &input() const;
        uint classIndex() const;
    };

private:
    std::vector<Matrix<double>> w, gsum, dw;
    std::vector<std::vector<double>> a, g;

    double learningRate;
    double momentum;
    double l2Decay;
    double maxLoss;

    int batchSize;
    int maxEpochs;

    bool verbose;

    int counter;

public:
    static Network loadFromFile(const std::string &fileName);

    Network();
    Network(const std::vector<int> &sizes);
    Network(const Network &net);
    Network(Network &&net);

    Network &operator=(const Network &net);
    Network &operator=(Network &&net);

    void defaults();
    void init();

    std::vector<double> forward(const std::vector<double> &input);

private:
    void tanh(std::vector<double> &v);
    void softmax(std::vector<double> &v);
    void backward(uint classIndex);

public:
    void train(const std::vector<Example> &examples);
    double learn(const Example &e);

    uint predict(const std::vector<double> &input);

    void saveToFile(const std::string &fileName);

    double getLearningRate() const;
    void setLearningRate(double learningRate);

    double getMomentum() const;
    void setMomentum(double momentum);

    double getL2Decay() const;
    void setL2Decay(double l2Decay);

    double getMaxLoss() const;
    void setMaxLoss(double maxLoss);

    int getBatchSize() const;
    void setBatchSize(int batchSize);

    int getMaxEpochs() const;
    void setMaxEpochs(int maxEpochs);

    bool isVerbose() const;
    void setVerbose(bool verbose);

private:
    double gaussRandom();
    double gaussRandom(double mu, double std);
};
