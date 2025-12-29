#include "iostring.hpp"
#include <generator>
#include <print>
#include <ranges>
#include <string>
#include <vector>

namespace Myios {}

void demo_iostr() {
    // + how to output
    std::string str0 = "ab"
                       "c";   // 拼接
    std::println("{}", str0); // out: abc
    // std::string str1 = "ab" + "c"; //* const char * 没有+operator
    std::string str1 = "ab\nc";
    std::println("{}", str1); // out: ab <换行> c
    std::string str2 = "ab c";
    std::println("{}", str2); // out: ab c
    std::string str3 = "\\/";
    std::println("{}", str3); // out: \/
    // + raw string
    // std::string str4 = R"This is RaW string!";//* invalid
    std::string str4 = R"(This is RaW string!)";   //* have to include brackets
    std::println("{}", str4);                      // out: This is RaW string!
    std::string str5 = R"+(This is RaW string!)+"; //* ++(...)++的匹配很重要
    std::println("{}", str5);                      // out: This is RaW string!
    std::string str6 = R"+("If U wanta )")+";
    // * 如果你想输出"
    std::println("{}", str6); // out: If U wanta )"
    // string to arithmatic
    std::string str7 = "123";
    int num = std::stoi(str7);           //* std::stoi(str) 将字符串转换为整数
    std::println("{}", num);             // out: 123
    std::string str8 = "123 456";        //* end=空格
    std::println("{}", std::stoi(str8)); // out: 123
    std::println("{}", std::stoi(str7, nullptr,
                                 16)); // out:291 =3+16*2+1*16**2//* 16进制转换
    std::string str9 = "123 456\0";
    std::println("C Style string: \n size: {}, content: {}", str9.size(),
                 str9); // size: 7 content: 123 456 // *\0不计入size
    std::string str;
    std::println("Uninitialized string size: {}", str.size());
    // retrival
} // namespace void demo_iostr()