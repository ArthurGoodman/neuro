#include "network.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>

Network::Example::Example(const std::vector<double> &input, uint classIndex)
    : in(input), ci(classIndex) {
}

const std::vector<double> &Network::Example::input() const {
    return in;
}

uint Network::Example::classIndex() const {
    return ci;
}

Network::Network(const std::vector<int> &sizes) {
    a.resize(sizes.size() + 1);
    g.resize(sizes.size() - 1);

    w.reserve(sizes.size() - 1);
    gsum.reserve(sizes.size() - 1);

    for (uint i = 0; i < sizes.size() - 1; i++) {
        Matrix<double> m(sizes[i] + 1, sizes[i + 1]);

        w.push_back(m);
        gsum.push_back(m);
    }

    dw.resize(sizes.size() - 1);

    learningRate = 0.1;
    momentum = 0.9;
    l2Decay = 0.001;
    maxLoss = 1e-3;

    maxEpochs = 1000;

    verbose = true;

    init();
}

void Network::init() {
    for (uint i = 0; i < w.size(); i++) {
        double scale = 1.0 / sqrt(w[i].height() * w[i].width());

        for (int j = 0; j < w[i].height(); j++)
            for (int k = 0; k < w[i].width(); k++) {
                w[i][j][k] = (double)rand() / RAND_MAX * scale;
                gsum[i][j][k] = 0;
            }
    }
}

std::vector<double> Network::forward(const std::vector<double> &input) {
    a[0] = input;

    for (uint i = 0; i < w.size(); i++) {
        a[i].push_back(1);

        if (i > 0)
            g[i - 1].resize(a[i].size());

        a[i + 1] = w[i].multiply(a[i]);

        if (i < w.size() - 1)
            tanh(a[i + 1]);
    }

    a.back() = softmax(a[a.size() - 2]);

    g[w.size() - 1].resize(a.back().size());

    return a.back();
}

void Network::tanh(std::vector<double> &v) {
    for (uint j = 0; j < v.size(); j++)
        v[j] = ::tanh(v[j]);
}

std::vector<double> Network::softmax(const std::vector<double> &v) {
    std::vector<double> r(v.size());

    double max = v[0];
    for (uint j = 1; j < v.size(); j++)
        max = std::max(max, v[j]);

    double sum = 0;
    for (uint j = 0; j < v.size(); j++)
        sum += r[j] = exp(v[j] - max);

    for (uint j = 0; j < v.size(); j++)
        r[j] /= sum;

    return r;
}

void Network::backward(uint classIndex) {
    for (uint j = 0; j < a.back().size(); j++)
        g.back()[j] = -((j == classIndex ? 1 : 0) - a.back()[j]);

    for (int i = w.size() - 1; i >= 0; i--) {
        if (i < (int)w.size() - 1)
            for (uint j = 0; j < g[i].size() - 1; j++)
                g[i][j] *= 1 - a[i + 1][j] * a[i + 1][j];

        if (i > 0)
            g[i - 1] = w[i].multiplyTransposed(g[i]);

        dw[i] = Matrix<double>::multiply(a[i], g[i]);
    }
}

void Network::train(const std::vector<Example> &examples) {
    std::vector<Example> ex(examples);

    for (int i = 0; i < maxEpochs; i++) {
        std::random_shuffle(ex.begin(), ex.end());

        double loss = 0;

        for (const Example &e : ex)
            loss = std::max(loss, learn(e));

        if (verbose)
            std::cout << "\n" << i << ": loss = " << loss << "\n\n";

        if (loss <= maxLoss)
            break;
    }
}

double Network::learn(const Example &e) {
    forward(e.input());
    backward(e.classIndex());

    for (uint i = 0; i < w.size(); i++)
        for (int j = 0; j < w[i].height(); j++)
            for (int k = 0; k < w[i].width(); k++) {
                gsum[i][j][k] = momentum * gsum[i][j][k] - learningRate * (l2Decay * w[i][j][k] + dw[i][j][k]);
                w[i][j][k] += gsum[i][j][k];
            }

    double loss = -log(a.back()[e.classIndex()]);

    if (verbose)
        std::cout << loss << "\n";

    return loss;
}

bool Network::isVerbose() const {
    return verbose;
}

void Network::setVerbose(bool verbose) {
    this->verbose = verbose;
}

double Network::getLearningRate() const {
    return learningRate;
}

void Network::setLearningRate(double learningRate) {
    this->learningRate = learningRate;
}

double Network::getMomentum() const {
    return momentum;
}

void Network::setMomentum(double momentum) {
    this->momentum = momentum;
}

double Network::getL2Decay() const {
    return l2Decay;
}

void Network::setL2Decay(double l2Decay) {
    this->l2Decay = l2Decay;
}

double Network::getMaxLoss() const {
    return maxLoss;
}

void Network::setMaxLoss(double maxLoss) {
    this->maxLoss = maxLoss;
}

int Network::getMaxEpochs() const {
    return maxEpochs;
}

void Network::setMaxEpochs(int maxEpochs) {
    this->maxEpochs = maxEpochs;
}
