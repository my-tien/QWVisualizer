#include "classicalwalker.h"

#include <iostream>
#include <unsupported/Eigen/KroneckerProduct>

ClassicalWalker::ClassicalWalker(const int length) : RandomWalker(length) {
    state[length - 1] = 1;
    Eigen::Matrix2d coin;
    coin << 1, 1,
            1, 1;
    coin /= 2;
    auto outcomesMatrix = Eigen::kroneckerProduct(Eigen::MatrixXd::Identity(length, length), coin);
    walkMatrix = permutationMatrix() * outcomesMatrix;
}

Eigen::VectorXd ClassicalWalker::getProbabilities() const {
    Eigen::VectorXd probabilities(state.rows()/2);
    for (std::size_t idx = 0; idx < probabilities.rows(); ++idx) {
        probabilities[idx] = std::abs(state[idx*2] + state[idx*2 + 1]);
    }
    return  probabilities;
}
