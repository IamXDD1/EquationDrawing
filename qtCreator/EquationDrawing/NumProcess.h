#ifndef NUMPROCESS_H
#define NUMPROCESS_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <stack>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <sstream>

using std::cin;
using std::cout;
using std::getline;
using std::endl;
using std::map;
using std::string;
using std::vector;
using std::regex;
using std::stack;

class Number {
    string num;
    string decimal;
    string deNum;
    string deDecimal;
public:
    string name;
    bool Integer;
    bool negative;
    Number();
    Number(string a);
    Number(const Number& a);

    inline string getNum() { return num; }
    inline string getDecimal() { return decimal; }
    inline string getDeNum() { return deNum; }
    inline string getDeDecimal() { return deDecimal; }

    Number& operator = (Number a);
    Number operator + (Number a);
    Number operator - (Number a);
    Number operator * (Number a);
    Number operator / (Number a);
    Number operator ^ (Number a);
    Number operator % (Number a);
    friend std::ostream& operator << (std::ostream& out, Number a);


};


class Calculator {
    static int weight(char op);
public:
    static map<string, Number> exist_var;
    string show_exist;
    void RUN();
    static Number Input(bool& equal, string inputStr);
    static string judgeFormat(string infix); //判斷名字 若有變數則直接替換

    static Number calculate(string posfix);
    static bool isVariable(string str);
    static auto is_Var_exist(string name);

    static string InfixtoPosfix(string infix);
    static string Output(Number ans);
    void test();
};

#endif // NUMPROCESS_H
