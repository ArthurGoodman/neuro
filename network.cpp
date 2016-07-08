#include "network.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>

Network::Example::Example(const std::vector<double> &input, const std::vector<double> &output)
    : in(input), out(output) {
}

const std::vector<double> &Network::Example::input() const {
    return in;
}

const std::vector<double> &Network::Example::output() const {
    return out;
}

Network::Network(const std::vector<int> &sizes) {
    learningRate = 1.0;
    momentum = 0.9;
    maxError = 1e-3;
    maxEpochs = 1000;

    verbose = true;

    n = std::vector<std::vector<double>>(sizes.size());

    w.reserve(sizes.size() - 1);
    dw.reserve(sizes.size() - 1);

    for (int i = 1; i < (int)sizes.size(); i++) {
        w.push_back(Matrix<double>(sizes[i - 1] + 1, sizes[i]));
        dw.push_back(Matrix<double>(sizes[i - 1] + 1, sizes[i]));
    }

    init();
}

void Network::init() {
    for (int m = 0; m < (int)w.size(); m++) {
        double scale = 1.0 / sqrt(w[m].height() * w[m].width());

        for (int i = 0; i < w[m].height(); i++)
            for (int j = 0; j < w[m].width(); j++) {
                w[m][i][j] = (double)rand() / RAND_MAX * scale;
                dw[m][i][j] = 0;
            }
    }
}

std::vector<double> Network::impulse(const std::vector<double> &input) {
    n[0] = input;

    for (int i = 0; i < (int)w.size(); i++) {
        n[i].push_back(1);
        n[i + 1] = w[i].multiply(n[i]);

        for (int j = 0; j < (int)n[i + 1].size(); j++)
            n[i + 1][j] = tanh(n[i + 1][j]);
    }

    return n[n.size() - 1];
}

void Network::train(const std::vector<Example> &examples) {
    std::vector<Example> ex(examples);

    for (int i = 0; i < maxEpochs; i++) {
        double error = 0;

        std::random_shuffle(ex.begin(), ex.end());

        for (const Example &e : ex)
            error = std::max(error, learn(e));

        if (verbose)
            std::cout << "\n" << i << ": error = " << error << "\n\n";

        if (error <= maxError)
            break;
    }
}

double Network::learn(const Example &e) {
    impulse(e.input());

    const std::vector<double> &correctOutput = e.output();
    const std::vector<double> &output = n[n.size() - 1];

    std::vector<double> delta(output.size());

    double averageError = 0;

    for (int i = 0; i < (int)output.size(); i++) {
        averageError += fabs(correctOutput[i] - output[i]);
        delta[i] = (1 - output[i] * output[i]) * (correctOutput[i] - output[i]);
    }

    averageError /= delta.size();

    if (verbose)
        std::cout << averageError << "\n";

    for (int i = w.size() - 1; i >= 0; i--) {
        for (int p = 0; p < w[i].height(); p++)
            for (int q = 0; q < w[i].width(); q++) {
                dw[i][p][q] = momentum * dw[i][p][q] + (1 - momentum) * learningRate * n[i][p] * delta[q];
                w[i][p][q] += dw[i][p][q];
            }

        if (i > 0) {
            delta = w[i].multiplyTransposed(delta);

            for (int p = 0; p < w[i].height() - 1; p++)
                delta[p] *= 1 - n[i][p] * n[i][p];
        }
    }

    return averageError;
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

double Network::getMaxError() const {
    return maxError;
}

void Network::setMaxError(double maxError) {
    this->maxError = maxError;
}

int Network::getMaxEpochs() const {
    return maxEpochs;
}

void Network::setMaxEpochs(int maxEpochs) {
    this->maxEpochs = maxEpochs;
}
