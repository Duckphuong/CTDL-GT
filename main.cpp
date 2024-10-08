#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "include/list/listheader.h"

#include "include/list/XArrayListDemo.h"
#include "include/list/DLinkedListDemo.h"
#include "include/ann/demo.h"

#include "ann/funtions.h"
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"
#include "ann/dataloader.h"

using namespace std;

int main(int argc, char **argv)
{
    cout << "Assignment-1a" << endl;

    dlistDemo1();
    dlistDemo2();
    dlistDemo3();
    dlistDemo4();
    dlistDemo5();
    dlistDemo6();
    cout << endl;
    xlistDemo1();
    xlistDemo2();
    xlistDemo3();
    xlistDemo4();

    cout << endl;
    cout << "END" << endl;

    cout << "Assignment-1b" << endl;
    cout << "demo 1" << endl;
    dataloadertc1();
    cout << "demo 2" << endl;
    dataloadertc2();
    cout << "demo 3" << endl;
    dataloadertc3();
    cout << "demo 4" << endl;
    dataloadertc4();
    cout << "demo 5" << endl;
    dataloadertc5();
    // cout << "demo 6" << endl;
    // dataloadertc6();
    cout << "demo 7" << endl;
    dataloadertc7();
    cout << "END" << endl;

    return 0;
}

/*
g++ -I include -I src -std=c++17 main.cpp
.\a.exe
*/
