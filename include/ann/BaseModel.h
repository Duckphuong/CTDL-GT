#ifndef BASEMODEL_H
#define BASEMODEL_H

#include "ann/xtensor_lib.h"
#include "list/DLinkedList.h"
#include "ann/Layer.h"

class BaseModel
{
public:
    BaseModel();
    BaseModel(const BaseModel &orig);
    virtual ~BaseModel();

    virtual void add(Layer *layer);
    virtual xt::xarray<double> predict(xt::xarray<double> X);
    virtual BaseModel *clone() const;

protected:
    DLinkedList<Layer *> layers;
};

#endif /* BASEMODEL_H */
