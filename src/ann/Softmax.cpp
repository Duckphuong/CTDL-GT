#include "ann/Softmax.h"
#include "ann/funtions.h"

Softmax::Softmax(int axis) : axis(axis)
{
    name = "Softmax_" + to_string(++layer_idx);
}

Softmax::Softmax(const Softmax &orig) : axis(orig.axis), cached_Y(orig.cached_Y) {}

Softmax::~Softmax() {}

xt::xarray<double> Softmax::forward(xt::xarray<double> X)
{
    cached_Y = softmax(X, axis);
    return cached_Y;
}

Softmax *Softmax::clone() const 
{
    return new Softmax(*this);
}
