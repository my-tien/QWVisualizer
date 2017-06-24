#include "randomwalker.h"

#include <cmath>
#include <complex>
#include <iostream>
#include <unsupported/Eigen/KroneckerProduct>
#include <unsupported/Eigen/MatrixFunctions>

using namespace std::complex_literals;

RandomWalker::RandomWalker(const int length) {
    const int dim = length * 2;
    state.setZero(dim);
    state[length - 1] = 1;
    state[length] = 1i;
    state /= std::sqrt(2);
    Eigen::Matrix2d H;
    H << 1, 1,
         1, -1;
    H /= std::sqrt(2);
    auto outcomesMatrix = Eigen::kroneckerProduct(Eigen::MatrixXd::Identity(length, length), H);

    auto permute = [](const int dim) {
        Eigen::VectorXi permutation(dim);
        for (int idx = 0; idx < dim - 1; idx+=2) {
            permutation[idx]= (idx - 2)%(dim);
            permutation[idx+1] = (idx+1 + 2)%(dim);
        }
        permutation[0] = dim - 2;
        return permutation;
    };

    Eigen::PermutationMatrix<Eigen::Dynamic> permutationMatrix(permute(dim));
    walkMatrix = permutationMatrix * outcomesMatrix;
}

decltype(RandomWalker::state) RandomWalker::step(const std::size_t n) {
    state = walkMatrix.pow(n) * state;
    return  state;
}

Eigen::VectorXd RandomWalker::getState() const {
    Eigen::VectorXd probabilities(state.rows()/2);
    for (std::size_t idx = 0; idx < probabilities.rows(); ++idx) {
        probabilities[idx] = std::abs(std::pow(state[idx*2], 2)) + std::abs(std::pow(state[idx*2 + 1], 2));
    }
    return  probabilities;
}

