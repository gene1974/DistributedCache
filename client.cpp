#include <iostream>
#include <map> 
#include <string>  
#include <random>

using namespace std;
using std::string;
using std::random_device;
using std::default_random_engine;

string strRand(int length = 8) {			// length: 产生字符串的长度
    char tmp;							// tmp: 暂存一个随机数
    string buffer;						// buffer: 保存返回值

   
    random_device rd;					// 产生一个 std::random_device 对象 rd
    default_random_engine random(rd());	// 用 rd 初始化一个随机数发生器 random

    for (int i = 0; i < length; i++) {
        tmp = random() % 36;	// 随机一个小于 36 的整数，0-9、A-Z 共 36 种字符
        if (tmp < 10) {			// 如果随机数小于 10，变换成一个阿拉伯数字的 ASCII
            tmp += '0';
        }
        else {				// 否则，变换成一个大写字母的 ASCII
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}

int keyRand() {

    constexpr int MIN = 10000000;
    constexpr int MAX = 99999999;
    random_device rd;
    std::default_random_engine eng(rd());
    uniform_int_distribution<int> distr(MIN, MAX);
    return distr(eng);
}



int main() {

    cout << keyRand();


         return 0;
 }


    













