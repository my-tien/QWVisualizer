#include "quantumwalker.h"

#include <cmath>
#include <complex>
#include <iostream>
#include <unsupported/Eigen/KroneckerProduct>
#include <unsupported/Eigen/MatrixFunctions>

using namespace std::complex_literals;

QuantumWalker::QuantumWalker(const int length) : RandomWalker(length) {
    state[length - 1] = 1;
    state[length] = 1i;
    state /= std::sqrt(2);

    Eigen::Matrix2d Hadamard;
    Hadamard << 1, 1,
                1, -1;
    Hadamard /= std::sqrt(2);
    auto outcomesMatrix = Eigen::kroneckerProduct(Eigen::MatrixXd::Identity(length, length), Hadamard);

    walkMatrix = permutationMatrix() * outcomesMatrix;
}

Eigen::VectorXd QuantumWalker::getProbabilities() const {
    Eigen::VectorXd probabilities(state.rows()/2);
    for (std::size_t idx = 0; idx < probabilities.rows(); ++idx) {
        probabilities[idx] = std::abs(std::pow(state[idx*2], 2)) + std::abs(std::pow(state[idx*2 + 1], 2));
    }
    return  probabilities;
}

