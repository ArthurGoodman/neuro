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

const double Network::alpha = 1.0;
const double Network::eta = 1.0;
const double Network::maxError = 1e-3;
const int Network::maxEpochs = 1000;

Network::Network(const std::vector<int> &sizes) {
    n = std::vector<std::vector<double>>(sizes.size());

    for (int i = 1; i < (int)sizes.size(); i++) {
        Matrix<double> m(sizes[i], sizes[i - 1]);

        for (int i = 0; i < m.height(); i++)
            for (int j = 0; j < m.width(); j++)
                m[i][j] = (double)rand() / RAND_MAX / 10;

        w.push_back(m);
    }
}

std::vector<double> Network::impulse(const std::vector<double> input) {
    n[0] = input;

    for (int i = 0; i < (int)w.size(); i++) {
        n[i + 1] = w[i].multiply(n[i]);

        for (int j = 0; j < (int)n[i + 1].size(); j++)
            n[i + 1][j] = sigmoid(n[i + 1][j]);
    }

    return n[n.size() - 1];
}

void Network::learn(std::vector<Example> &examples) {
    std::vector<Matrix<double>> wT(w.size());

    for (int i = 0; i < (int)w.size(); i++)
        wT[i] = w[i].transposed();

    for (int i = 0; i < maxEpochs; i++) {
        double error = 0;

        int c = 0;

        std::random_shuffle(examples.begin(), examples.end());

        for (const Example &e : examples) {
            impulse(e.input());

            std::vector<double> correctOutput = e.output();
            std::vector<double> output = n[n.size() - 1];

            std::vector<double> delta(correctOutput.size());

            double averageError = 0;

            for (int i = 0; i < (int)delta.size(); i++)
                averageError += fabs(delta[i] = correctOutput[i] - output[i]);

            averageError /= delta.size();

            std::cout << c++ << ": " << averageError << "\n";

            error = std::max(error, averageError);

            for (int i = w.size() - 1; i >= 0; i--) {
                for (int p = 0; p < (int)n[i].size(); p++)
                    for (int q = 0; q < (int)n[i + 1].size(); q++)
                        // wT[i][q][p] += eta * n[i][p] * (alpha * exp(alpha * n[i + 1][q]) / pow(exp(alpha * n[i + 1][q]) + 1, 2)) * delta[q];
                        wT[i][q][p] += eta * n[i][p] * n[i + 1][q] * (1 - n[i + 1][q]) * delta[q];

                if (i > 0)
                    delta = wT[i].multiply(delta);
            }

            for (int i = 0; i < (int)w.size(); i++)
                w[i] = wT[i].transposed();
        }

        std::cout << "\n" << i << ": error = " << error << "\n\n";

        if (error <= maxError)
            break;
    }
}

double Network::sigmoid(double x) {
    return 1.0 / (1.0 + exp(-alpha * x));
}
