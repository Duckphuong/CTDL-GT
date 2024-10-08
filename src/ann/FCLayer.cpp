#include "ann/FCLayer.h"
#include "ann/funtions.h"

FCLayer::FCLayer(int in_features, int out_features, bool use_bias)
    : m_nIn_Features(in_features), m_nOut_Features(out_features), m_bUse_Bias(use_bias)
{
    name = "FC_" + to_string(++layer_idx);
    m_unSample_Counter = 0;

    init_weights();
}

void FCLayer::init_weights()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);

    // Khởi tạo trọng số với phân phối chuẩn
    m_aWeights = xt::random::randn<double>({m_nOut_Features, m_nIn_Features}, gen);

    // Khởi tạo bias (nếu có)
    if (m_bUse_Bias)
    {
        m_aBias = xt::zeros<double>({m_nOut_Features});
    }
}

FCLayer::FCLayer(const FCLayer &orig) : Layer(orig)
{
    m_nIn_Features = orig.m_nIn_Features;
    m_nOut_Features = orig.m_nOut_Features;
    m_bUse_Bias = orig.m_bUse_Bias;
    m_aWeights = orig.m_aWeights;
    m_aBias = orig.m_aBias;
    m_unSample_Counter = orig.m_unSample_Counter;
}

FCLayer::~FCLayer()
{
    // Destructor
}

xt::xarray<double> FCLayer::forward(xt::xarray<double> X)
{
    m_aCached_X = X; // Lưu lại input để sử dụng cho việc tính gradient sau này

    xt::xarray<double> output = xt::linalg::dot(X, xt::transpose(m_aWeights));
    if (m_bUse_Bias)
    {
        output += m_aBias;
    }

    return output;
}

FCLayer *FCLayer::fromPretrained(string filename, bool use_bias)
{
    // Tạo một layer mới với trọng số từ file pretrained
    FCLayer *layer = new FCLayer();
    layer->m_bUse_Bias = use_bias;

    std::ifstream file(filename);
    if (!file)
    {
        cerr << "Error: File not found!" << endl;
        return nullptr;
    }

    // Đọc trọng số từ file (giả định file chứa trọng số dưới dạng text)
    file >> layer->m_aWeights;
    if (use_bias)
    {
        file >> layer->m_aBias;
    }

    file.close();
    return layer;
}

FCLayer *FCLayer::clone() const
{
    return new FCLayer(*this);
}
