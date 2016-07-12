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
    a.resize(sizes.size());
    g.resize(sizes.size() - 1);

    w.reserve(sizes.size() - 1);
    gsum.reserve(sizes.size() - 1);
    dw.reserve(sizes.size() - 1);

    for (uint i = 0; i < sizes.size() - 1; i++) {
        Matrix<double> m(sizes[i] + 1, sizes[i + 1]);

        w.push_back(m);
        gsum.push_back(m);
        dw.push_back(m);
    }

    learningRate = 0.01;
    momentum = 0.1;
    l2Decay = 0.001;
    maxLoss = 1e-3;

    batchSize = 1;
    maxEpochs = 1000;

    verbose = true;

    counter = 0;

    init();
}

void Network::init() {
    for (uint i = 0; i < w.size(); i++) {
        double scale = 1.0 / sqrt(w[i].height() * w[i].width());

        for (int j = 0; j < w[i].height(); j++)
            for (int k = 0; k < w[i].width(); k++) {
                w[i][j][k] = j < w[i].height() - 1 ? gaussRandom(0, scale) : 0;
                gsum[i][j][k] = 0;
                dw[i][j][k] = 0;
            }
    }
}

std::vector<double> Network::forward(const std::vector<double> &input) {
    a[0] = input;

    for (uint i = 0; i < w.size(); i++) {
        a[i].push_back(1);

        a[i + 1] = w[i].multiply(a[i]);

        if (i < w.size() - 1)
            tanh(a[i + 1]);
    }

    softmax(a.back());

    return a.back();
}

void Network::tanh(std::vector<double> &v) {
    for (uint j = 0; j < v.size(); j++)
        v[j] = ::tanh(v[j]);
}

void Network::softmax(std::vector<double> &v) {
    double max = v[0];
    for (uint j = 1; j < v.size(); j++)
        max = std::max(max, v[j]);

    double sum = 0;
    for (uint j = 0; j < v.size(); j++)
        sum += v[j] = exp(v[j] - max);

    for (uint j = 0; j < v.size(); j++)
        v[j] /= sum;
}

void Network::backward(uint classIndex) {
    g.back().resize(a.back().size());

    for (uint j = 0; j < a.back().size(); j++)
        g.back()[j] = -((j == classIndex ? 1 : 0) - a.back()[j]);

    for (int i = w.size() - 1; i >= 0; i--) {
        if (i < (int)w.size() - 1)
            for (uint j = 0; j < g[i].size(); j++)
                g[i][j] *= 1 - a[i + 1][j] * a[i + 1][j];

        if (i > 0) {
            g[i - 1] = w[i].multiplyTransposed(g[i]);
            g[i - 1].pop_back();
        }

        dw[i] += Matrix<double>::multiply(a[i], g[i]);
    }
}

void Network::train(const std::vector<Example> &examples) {
    std::vector<Example> ex(examples);

    for (int i = 0; i < maxEpochs; i++) {
        // std::random_shuffle(ex.begin(), ex.end());

        double loss = 0;

        for (const Example &e : ex)
            loss = std::max(loss, learn(e));

        if (verbose)
            std::cout << "\n" << i << ": loss = " << loss << "\n\n" << std::flush;

        if (loss <= maxLoss)
            break;
    }
}

double Network::learn(const Example &e) {
    forward(e.input());
    backward(e.classIndex());

    double loss = -log(a.back()[e.classIndex()]);

    if (verbose)
        std::cout << loss << "\n" << std::flush;

    if (++counter % batchSize == 0)
        for (uint i = 0; i < w.size(); i++)
            for (int j = 0; j < w[i].height(); j++)
                for (int k = 0; k < w[i].width(); k++) {
                    gsum[i][j][k] = momentum * gsum[i][j][k] - learningRate * ((j < w[i].height() - 1 ? l2Decay : 0) * w[i][j][k] + dw[i][j][k]) / batchSize;
                    w[i][j][k] += gsum[i][j][k];
                    dw[i][j][k] = 0;
                }

    return loss;
}

uint Network::predict(const std::vector<double> &input) {
    forward(input);

    double max = a.back()[0];
    uint iMax = 0;

    for (uint i = 1; i < a.back().size(); i++)
        if (max < a.back()[i]) {
            max = a.back()[i];
            iMax = i;
        }

    return iMax;
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

int Network::getBatchSize() const {
    return batchSize;
}

void Network::setBatchSize(int batchSize) {
    this->batchSize = batchSize;
}

int Network::getMaxEpochs() const {
    return maxEpochs;
}

void Network::setMaxEpochs(int maxEpochs) {
    this->maxEpochs = maxEpochs;
}

bool Network::isVerbose() const {
    return verbose;
}

void Network::setVerbose(bool verbose) {
    this->verbose = verbose;
}

double Network::gaussRandom() {
    static double v1, v2, s;
    static int phase = 0;
    double x;

    if (phase == 0) {
        do {
            double u1 = (double)rand() / RAND_MAX;
            double u2 = (double)rand() / RAND_MAX;

            v1 = 2 * u1 - 1;
            v2 = 2 * u2 - 1;
            s = v1 * v1 + v2 * v2;
        } while (s >= 1 || s == 0);

        x = v1 * sqrt(-2 * log(s) / s);
    } else
        x = v2 * sqrt(-2 * log(s) / s);

    phase = 1 - phase;

    return x;
}

double Network::gaussRandom(double mu, double std) {
    return mu + gaussRandom() * std;
}
