#ifndef QUANTUMWALKER_H
#define QUANTUMWALKER_H

#include "randomwalker.h"

#include <eigen3/Eigen/Dense>

class QuantumWalker : public RandomWalker {
public:
    QuantumWalker(const int length);
    virtual Eigen::VectorXd getProbabilities() const override;
};

#endif // QUANTUMWALKER_H
