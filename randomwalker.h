#ifndef RANDOMWALKER_H
#define RANDOMWALKER_H

#include <eigen3/Eigen/Dense>

class RandomWalker {
    Eigen::VectorXcd state;
    Eigen::MatrixXd walkMatrix;
public:
    RandomWalker(const int length);
    decltype(state) step(const std::size_t n);
    Eigen::VectorXd getState() const;
};

#endif // RANDOMWALKER_H
