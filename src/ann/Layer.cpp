#include "ann/Layer.h"

Layer::Layer() : is_training(true)
{
    name = "Layer_" + to_string(layer_idx++);
}

Layer::Layer(const Layer &orig) : is_training(orig.is_training), name(orig.name) {}

Layer::~Layer() {}

unsigned long long Layer::layer_idx = 0;
