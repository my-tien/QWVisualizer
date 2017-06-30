#include "randomwalker.h"

#include <unsupported/Eigen/MatrixFunctions>

RandomWalker::RandomWalker(const int length) {
    const int dim = length * 2;
    state.setZero(dim);
}

decltype(RandomWalker::state) RandomWalker::step(const std::size_t n) {
    state = walkMatrix.pow(n) * state;
    return  state;
}

Eigen::PermutationMatrix<Eigen::Dynamic> RandomWalker::permutationMatrix() const {
    Eigen::VectorXi permutation(state.rows());
    for (int idx = 0; idx < state.rows() - 1; idx+=2) {
        permutation[idx]= (idx - 2)%(state.rows());
        permutation[idx+1] = (idx+1 + 2)%(state.rows());
    }
    permutation[0] = state.rows() - 2;
    Eigen::PermutationMatrix<Eigen::Dynamic> permMatrix(permutation);
    return permMatrix;
}
