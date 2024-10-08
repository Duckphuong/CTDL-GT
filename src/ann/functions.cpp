#include "ann/funtions.h"
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <limits>

xt::xarray<double> softmax(xt::xarray<double> X, int axis)
{
    // Đảm bảo rằng axis là hợp lệ
    if (axis < -X.dimension() || axis >= static_cast<int>(X.dimension()))
    {
        throw std::out_of_range("Axis is out of range.");
    }

    // Bước 1: Tính giá trị max trên trục đã cho
    auto max_X = xt::amax(X, axis);

    // Bước 2: Trừ max_X khỏi X để tránh overflow
    auto exp_X = xt::exp(X - xt::broadcast(max_X, X.shape()));

    // Bước 3: Tính tổng của exp_X trên trục đã cho
    auto sum_exp_X = xt::sum(exp_X, axis);

    // Bước 4: Chia exp_X cho sum_exp_X để nhận giá trị softmax
    return exp_X / xt::broadcast(sum_exp_X, X.shape());
}
