#include "walker.h"

#include <unsupported/Eigen/KroneckerProduct>
#include <unsupported/Eigen/MatrixFunctions>

Walker::Walker(const decltype(state) & initState, const Eigen::Matrix2d & coin) {
    isQuantum = (coin == hadamardCoin());
    state = initState;
    const auto length = state.rows()/2;
    auto outcomesMatrix = Eigen::kroneckerProduct(Eigen::MatrixXd::Identity(length, length), coin);
    walkMatrix = permutationMatrix() * outcomesMatrix;
}

decltype(Walker::state) Walker::step(const std::size_t n) {
    state = walkMatrix.pow(n) * state;
    return  state;
}

Eigen::PermutationMatrix<Eigen::Dynamic> Walker::permutationMatrix() const {
    Eigen::VectorXi permutation(state.rows());
    for (int idx = 0; idx < state.rows() - 1; idx+=2) {
        permutation[idx]= (idx - 2)%(state.rows());
        permutation[idx+1] = (idx+1 + 2)%(state.rows());
    }
    permutation[0] = state.rows() - 2;
    Eigen::PermutationMatrix<Eigen::Dynamic> permMatrix(permutation);
    return permMatrix;
}

Eigen::VectorXd Walker::getProbabilities() const {
    Eigen::VectorXd probabilities(state.rows()/2);
    if (isQuantum) {
        for (std::size_t idx = 0; idx < probabilities.rows(); ++idx) {
            probabilities[idx] = std::abs(std::pow(state[idx*2], 2)) + std::abs(std::pow(state[idx*2 + 1], 2));
        }
    } else {
        for (std::size_t idx = 0; idx < probabilities.rows(); ++idx) {
            probabilities[idx] = std::abs(state[idx*2] + state[idx*2 + 1]);
        }
    }
    return  probabilities;
}
