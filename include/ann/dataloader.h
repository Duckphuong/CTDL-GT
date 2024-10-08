/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"

using namespace std;

template <typename DType, typename LType>
class DataLoader
{
private:
    Dataset<DType, LType> *dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    vector<int> batch_vector;
    int cur_idx;
    int batch_end;

public:
    int get_cur_idx() const
    {
        return cur_idx;
    }

    int get_batch_end() const
    {
        return batch_end;
    }

    // Hàm khởi tạo DataLoader
    DataLoader(Dataset<DType, LType> *dataset, int batch_size, bool shuffle = true, bool drop_last = false)
        : dataset(dataset), batch_size(batch_size), shuffle(shuffle), drop_last(drop_last), cur_idx(0)
    {
        // Khởi tạo mảng chỉ số cho dataset
        int dataset_size = dataset->len();
        batch_vector.resize(dataset_size);
        std::iota(batch_vector.begin(), batch_vector.end(), 0);

        // Xáo trộn dữ liệu nếu shuffle = true
        if (shuffle)
        {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(batch_vector.begin(), batch_vector.end(), g);
        }
    }

    virtual ~DataLoader() {}

    Batch<DType, LType> next_batch()
    {
        int dataset_size = dataset->len();
        // if (batch_end == dataset_size)
        // {
        //     cout << "Yss" << endl;
        //     return Batch<DType, LType>(xt::xarray<DType>(), xt::xarray<LType>());
        // }

        // cout << "cur_idx1 " << cur_idx << endl;
        // Kiểm tra số lượng mẫu còn lại
        int remaining_samples = dataset_size - cur_idx;
        // cout << "remaining_samples" << remaining_samples << endl;
        // cout << "min" << min(batch_size, remaining_samples) << endl;
        batch_end = cur_idx + std::min(batch_size, remaining_samples);

        // Nếu batch cuối nhỏ hơn batch_size và drop_last = false, gộp các phần tử còn lại vào batch cuối
        if (!drop_last && (dataset_size - batch_end < batch_size))
        {
            batch_end = dataset_size; // Lấy tất cả phần tử còn lại
        }
        // cout << "batch end" << batch_end << " " << "dataset size " << dataset_size << endl;
        // Lấy các chỉ số của batch hiện tại
        std::vector<int> batch_batch_vector(batch_vector.begin() + cur_idx, batch_vector.begin() + batch_end);
        // for (auto it : batch_batch_vector)
        // {
        //     cout << it << " ";
        // }
        // cout << endl;

        // Khởi tạo batch_data và batch_labels với kích thước thích hợp
        xt::xarray<DType> batch_data = xt::empty<DType>(std::vector<size_t>{0, dataset->get_data_shape()[1], dataset->get_data_shape()[2]});
        xt::xarray<LType> batch_labels = xt::empty<LType>(std::vector<size_t>{0});

        for (int idx : batch_batch_vector)
        {
            DataLabel<DType, LType> data_label = dataset->getitem(idx);
            batch_data = xt::concatenate(xt::xtuple(batch_data, xt::expand_dims(data_label.getData(), 0)));
            batch_labels = xt::concatenate(xt::xtuple(batch_labels, xt::expand_dims(data_label.getLabel(), 0)));
        }

        // Cập nhật chỉ số cho batch tiếp theo

        // if (batch_end == dataset_size)
        // {
        //     cout<<"YEs"<<endl;
        //     cur_idx = batch_end - batch_size;
        // }

        // cout << "cur_idx2 " << cur_idx << endl;

        return Batch<DType, LType>(batch_data, batch_labels);
    }

    class Iterator
    {
    private:
        DataLoader<DType, LType> *loader;

    public:
        Iterator(DataLoader<DType, LType> *loader) : loader(loader) {}
        // Hàm truy cập giá trị hiện tại
        Batch<DType, LType> operator*()
        {
            return loader->next_batch();
        }

        // Tiến tới phần tử tiếp theo
        Iterator &operator++()
        {
            if (loader->cur_idx < loader->dataset->len())
            {
                loader->cur_idx += loader->batch_size;
            }
            return *this;
        }

        // Tiến tới phần tử tiếp theo (postfix)
        Iterator operator++(int)
        {
            Iterator temp = *this; // Lưu bản sao của iterator hiện tại
            ++(*this);             // Gọi toán tử ++ trước
            return temp;           // Trả về bản sao cũ
        }

        // Kiểm tra có còn phần tử không
        bool operator!=(const Iterator &other) const
        {
            return loader->cur_idx < loader->dataset->len() - loader->batch_size; // Sửa lại điều kiện dừng
        }
    };

    Iterator begin()
    {
        cur_idx = 0; // Reset chỉ số
        return Iterator(this);
    }

    Iterator end()
    {
        return Iterator(this);
    }
};

#endif /* DATALOADER_H */
