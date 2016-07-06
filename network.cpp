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
    alpha = 1.0;
    eta = 1.0;
    maxError = 1e-3;
    maxEpochs = 1000;

    n = std::vector<std::vector<double>>(sizes.size());

    for (int i = 1; i < (int)sizes.size(); i++) {
        Matrix<double> m(sizes[i - 1] + 1, sizes[i]);

        for (int i = 0; i < m.height(); i++)
            for (int j = 0; j < m.width(); j++)
                m[i][j] = (double)rand() / RAND_MAX * 1e-1;

        w.push_back(m);
    }
}

std::vector<double> Network::impulse(const std::vector<double> &input) {
    n[0] = input;

    for (int i = 0; i < (int)w.size(); i++) {
        n[i].push_back(1);

        n[i + 1] = w[i].multiply(n[i]);

        for (int j = 0; j < (int)n[i + 1].size(); j++)
            n[i + 1][j] = sigmoid(n[i + 1][j]);

        n[i].pop_back();
    }

    return n[n.size() - 1];
}

void Network::learn(const std::vector<Example> &examples) {
    std::vector<Example> ex(examples);

    for (int i = 0; i < maxEpochs; i++) {
        double error = 0;

        int c = 0;

        std::random_shuffle(ex.begin(), ex.end());

        for (const Example &e : ex) {
            impulse(e.input());

            const std::vector<double> &correctOutput = e.output();
            const std::vector<double> &output = n[n.size() - 1];

            std::vector<double> delta(output.size());

            double averageError = 0;

            for (int i = 0; i < (int)output.size(); i++) {
                averageError += fabs(correctOutput[i] - output[i]);
                delta[i] = output[i] * (1 - output[i]) * (correctOutput[i] - output[i]);
            }

            averageError /= delta.size();

            std::cout << c++ << ": " << averageError << "\n";

            error = std::max(error, averageError);

            for (int i = w.size() - 1; i >= 0; i--) {
                for (int p = 0; p < (int)n[i].size() - 1; p++)
                    for (int q = 0; q < (int)n[i + 1].size(); q++)
                        w[i][p][q] += eta * n[i][p] * delta[q];

                for (int q = 0; q < (int)n[i + 1].size(); q++)
                    w[i][n[i].size() - 1][q] += delta[q];

                if (i > 0) {
                    delta = w[i].multiplyTransposed(delta);

                    for (int p = 0; p < (int)n[i].size() - 1; p++)
                        delta[p] *= n[i][p] * (1 - n[i][p]);
                }
            }
        }

        std::cout << "\n" << i << ": error = " << error << "\n\n";

        if (error <= maxError)
            break;
    }
}

double Network::getAlpha() {
    return alpha;
}

void Network::setAlpha(double alpha) {
    this->alpha = alpha;
}

double Network::getEta() {
    return eta;
}

void Network::setEta(double eta) {
    this->eta = eta;
}

double Network::getMaxError() {
    return maxError;
}

void Network::setMaxError(double maxError) {
    this->maxError = maxError;
}

int Network::getMaxEpochs() {
    return maxEpochs;
}

void Network::setMaxEpochs(int maxEpochs) {
    this->maxEpochs = maxEpochs;
}

double Network::sigmoid(double x) const {
    return 1.0 / (1.0 + exp(-alpha * x));
}
