#include "ann/ReLU.h"

ReLU::ReLU()
{
    name = "ReLU_" + to_string(++layer_idx);
}

ReLU::ReLU(const ReLU &orig) : Layer(orig)
{
    name = "ReLU_" + to_string(++layer_idx);
}

ReLU::~ReLU()
{
}

xt::xarray<double> ReLU::forward(xt::xarray<double> X)
{
    mask = X > 0;                 // Tạo mask cho các giá trị dương
    return xt::where(mask, X, 0); // Trả về mảng với các giá trị âm được thay bằng 0
}
