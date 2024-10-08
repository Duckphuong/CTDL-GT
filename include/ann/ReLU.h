#ifndef RELU_H
#define RELU_H
#include "ann/Layer.h"
#include <xtensor/xarray.hpp>

class ReLU : public Layer
{
public:
    ReLU();
    ReLU(const ReLU &orig);
    virtual ~ReLU();

    xt::xarray<double> forward(xt::xarray<double> X) override; // Override keyword added for clarity

private:
    xt::xarray<bool> mask;
};

#endif /* RELU_H */
