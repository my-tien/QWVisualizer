#ifndef WALKER_H
#define WALKER_H

#include <complex>
#include <eigen3/Eigen/Dense>
#include <vector>

class Walker {
    bool isQuantum{false};
    Eigen::VectorXcd state;
    Eigen::MatrixXd walkMatrix;

    Eigen::PermutationMatrix<Eigen::Dynamic> permutationMatrix() const;
public:
    explicit Walker(const decltype(state) & initState, const Eigen::Matrix2d & coin);
    decltype(state) step(const std::size_t n);
    virtual Eigen::VectorXd getProbabilities() const;
    static auto hadamardCoin() {
        static Eigen::Matrix2d hadamard;
        hadamard << 1, 1,
                    1, -1;
        hadamard /= std::sqrt(2);
        return hadamard;
    }
    static auto classicCoin() {
        static Eigen::Matrix2d coin;
        coin << 1, 1,
                1, 1;
        coin /= 2;
        return coin;
    }
};

#endif // WALKER_H
