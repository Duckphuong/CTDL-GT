#ifndef DEMO_H
#define DEMO_H

#include <iostream>
#include <iomanip>
#include "dataloader.h"
#include "dataset.h"

using namespace std;

void dataloadertc1()
{
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    cout << ds.len() << endl;
    DataLabel<double, int> item = ds.getitem(0);
    cout << item.getData() << endl;
    cout << item.getLabel() << endl;
}

void dataloadertc2()
{
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    cout << xt::adapt(ds.get_data_shape()) << endl;
    cout << xt::adapt(ds.get_label_shape()) << endl;
}

void dataloadertc3()
{
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false);
    auto it = loader.begin();
    it++;
    cout << xt::adapt((*it).getData().shape()) << endl;
    cout << xt::adapt((*it).getLabel().shape()) << endl;
    cout << xt::adapt((*(++it)).getData().shape()) << endl;
    cout << xt::adapt((*(++it)).getLabel().shape()) << endl;
    // cout << ((*it).getData().shape()[0]) << endl;
    // cout << ((*it).getData().shape()[1]) << endl;
    // cout << ((*it).getData().shape()[2]) << endl;
}

void dataloadertc4()
{
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false);
    for (auto it = loader.begin(); it != loader.end(); it++)
    {
        cout << ((*it).getData().shape()[0]) << endl;
        cout << ((*it).getLabel().shape()[0]) << endl;
    }
}

void dataloadertc5()
{
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false);
    for (auto batch : loader)
    {
        cout << (xt::adapt(batch.getData().shape())) << endl;
        cout << (xt::adapt(batch.getLabel().shape())) << endl;
    }
}

void dataloadertc6()
{
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({100, 3, 3});
    xt::xarray<int> t = xt::ones<int>({100});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false, true);
    int i = 1;
    for (auto batch : loader)
    {
        cout << "I " << i << endl;
        cout << batch.getData() << endl;
        cout << batch.getLabel() << endl;
        ++i;
    }
}

void dataloadertc7()
{
    xt::random::seed(10);
    xt::xarray<double> X = xt::random::randn<double>({105, 10, 10});
    xt::xarray<int> t = xt::ones<int>({105});
    TensorDataset<double, int> ds(X, t);
    DataLoader<double, int> loader(&ds, 10, false, true);
    for (auto batch : loader)
    {
        cout << (xt::adapt(batch.getData().shape())) << endl;
        cout << (xt::adapt(batch.getLabel().shape())) << endl;
    }
}

#endif /* DEMO_H */