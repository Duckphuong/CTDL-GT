#include "ann/BaseModel.h"
#include "ann/xtensor_lib.h"

BaseModel::BaseModel() {}

BaseModel::BaseModel(const BaseModel &orig)
{
    for (auto it = orig.layers.begin(); it != orig.layers.end(); ++it)
    {
        layers.add((*it)->clone());
    }
}

BaseModel::~BaseModel()
{
    for (auto ptr_layer : layers)
    {
        delete ptr_layer;
    }
}

void BaseModel::add(Layer *layer)
{
    layers.add(layer);
}

xt::xarray<double> BaseModel::predict(xt::xarray<double> X)
{
    for (auto ptr_layer : layers)
    {
        X = ptr_layer->forward(X);
    }
    return X;
}

BaseModel *BaseModel::clone() const
{
    return new BaseModel(*this);
}
