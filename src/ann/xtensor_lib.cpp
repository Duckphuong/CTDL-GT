#include "ann/xtensor_lib.h"

std::string shape2str(xt::svector<unsigned long> vec)
{
    std::stringstream ss;
    ss << "(";
    for (size_t idx = 0; idx < vec.size(); idx++)
    {
        ss << vec[idx] << (idx < vec.size() - 1 ? ", " : "");
    }
    return ss.str() + ")";
}

int positive_index(int idx, int size)
{
    return (idx < 0) ? (size + idx) : idx;
}

xt::xarray<double> outer_stack(xt::xarray<double> X, xt::xarray<double> Y)
{
    size_t rows = X.shape()[0];
    size_t cols = Y.shape()[0];
    xt::xarray<double> result = xt::zeros<double>({rows, cols});

    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            result(i, j) = X(i) * Y(j);
        }
    }
    return result;
}

xt::xarray<double> diag_stack(xt::xarray<double> X)
{
    return xt::diag(X); // Sử dụng xt::diag để tạo ma trận chéo
}

xt::xarray<double> matmul_on_stack(xt::xarray<double> X, xt::xarray<double> Y)
{
    return xt::linalg::dot(X, Y); // Sử dụng dot product cho phép nhân ma trận
}

ulong_array confusion_matrix(ulong_array y_true, ulong_array y_pred)
{
    size_t num_classes = xt::amax(y_true)() + 1; // Số lượng lớp
    ulong_array conf_matrix = xt::zeros<ulong_array>({num_classes, num_classes});

    for (size_t i = 0; i < y_true.shape()[0]; ++i)
    {
        conf_matrix(y_true(i), y_pred(i))++;
    }
    return conf_matrix;
}

xt::xarray<ulong> class_count(xt::xarray<ulong> confusion)
{
    return xt::sum(confusion, -1); // Đếm số lượng lớp
}

double_array calc_metrics(ulong_array y_true, ulong_array y_pred)
{
    ulong_array conf = confusion_matrix(y_true, y_pred);
    ulong_array counts = class_count(conf);

    // Tạo một mảng cho các chỉ số
    double_array metrics = xt::zeros<double_array>({7}); // 7 metrics

    // Tính độ chính xác
    ulong correct = xt::sum(xt::diag(conf))(); // Tổng số dự đoán chính xác
    ulong total = xt::sum(counts)();           // Tổng số mẫu
    metrics[static_cast<int>(class_metrics::ACCURACY)] = static_cast<double>(correct) / static_cast<double>(total);

    // Tính các chỉ số khác như Precision, Recall, F1-Score (có thể thêm vào ở đây)

    return metrics;
}
