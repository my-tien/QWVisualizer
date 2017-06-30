#ifndef CLASSICALWALKER_H
#define CLASSICALWALKER_H

#include "randomwalker.h"

class ClassicalWalker : public RandomWalker {
public:
    explicit ClassicalWalker(const int length);
    virtual Eigen::VectorXd getProbabilities() const override;
};

#endif // CLASSICALWALKER_H
