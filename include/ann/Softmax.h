#ifndef SOFTMAX_H
#define SOFTMAX_H

#include "ann/Layer.h"

class Softmax : public Layer
{
public:
    Softmax(int axis = -1);
    Softmax(const Softmax &orig);
    virtual ~Softmax();

    virtual xt::xarray<double> forward(xt::xarray<double> X);
    virtual Softmax *clone() const;

private:
    int axis;
    xt::xarray<double> cached_Y;
};

#endif /* SOFTMAX_H */
