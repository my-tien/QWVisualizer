#ifndef RANDOMWALKER_H
#define RANDOMWALKER_H

#include <eigen3/Eigen/Dense>

class RandomWalker {
protected:
    Eigen::VectorXcd state;
    Eigen::MatrixXd walkMatrix;

    Eigen::PermutationMatrix<Eigen::Dynamic> permutationMatrix() const;
public:
    RandomWalker(const int length);
    decltype(state) step(const std::size_t n);
    virtual Eigen::VectorXd getProbabilities() const = 0;
};

#endif // RANDOMWALKER_H
