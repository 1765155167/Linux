#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <ctime>
//实际上是对pthread的封装

double caculate(double num)
{
    if(num < 0)
    {
        return 0;
    }
    //暂停20ms
    //std::this_thread::sleep_for(std::chrono::milliseconds(20));
    int i = 1000000;
    while(i--);
    return std::sqrt(num*num + std::sqrt((num - 5) * (num + 2.5)) / 2.0) / num;
}

template<typename Iter, typename Fun>
double visitRange(Iter iterBegin, Iter iterEnd, Fun func) {
    double v = 0;
    for(auto iter = iterBegin; iter != iterEnd; ++iter)
    {
        v += func(*iter);
    }
    return v;
}

//线性方式计算和
void test01(std::vector<double>& v)
{
    double value = 0;
    for(auto& info: v)
    {
        value += caculate(info);
    }
    std::cout << "value:" << value << std::endl;
    return;
}

//多线程方式计算
void test02(std::vector<double>& v)
{
    double anotherv = 0.0;
    auto iter = v.begin() + (v.size() / 2);
    auto iterEnd = v.end();
    std::thread s([&anotherv, iter, iterEnd](){
        anotherv = visitRange(iter, iterEnd, caculate);
    });
    //计算的后半部分
    auto halfv = visitRange(v.begin(), iter, caculate);
    //清理工作
    s.join();
    std::cout << "value:" << (halfv + anotherv) << std::endl;
    return;
}

int main()
{
    auto mainThreadId = std::this_thread::get_id();
    std::cout << "id:" << mainThreadId << std::endl;
    std::vector<double> v;
    for(int i = 0; i < 10000; i++) {
        v.push_back(rand());
    } 
    std::cout << "size:" << v.size() << std::endl;
    auto nowc = clock();
    test01(v);
    auto finishc = clock();
    std::cout << "线性方式计算:" << finishc - nowc << std::endl;
    nowc = clock();
    test02(v);
    finishc = clock();
    std::cout << "多线程方式计算:" << finishc - nowc << std::endl;
    return 0;
}

