#include "NumProcess.h"
#include <QString>
#include <cmath>

using namespace std;

//map<string, double> Calculator::exist_var;

/*
bool Calculator::judge_if_can_draw(QString equation)
{
     string str = equation.toStdString();
    // not an equation
    regex equation_mark("/ = /g"), x_co("/ x /g"), y_co("/ y /g");
    if(!regex_match(str, equation_mark)) throw "Error: No \"=\" in the equation";
    if((!regex_match(str, x_co))&&(!regex_match(str, y_co))) return false;
    return true;

}
*/

/*
double Calculator::RUN(QString equation, bool can_draw)
{
    exist_var["x"] = 0;
    exist_var["y"] = 0;
    can_draw = false;
    string str;
    str = equation.toStdString();
    // have been editted by Gabriel
    double ans(Input(can_draw, str));
    if(can_draw) return Output(ans);
    else return -1111;
}

auto Calculator::is_Var_exist(string name)
{
    auto it = exist_var.find(name);
    if (it == exist_var.end()) {
        throw "Error: Variable doesn't exist.";
    }
    else {
        return it;
    }
}

string process_Power(string front, string back) {

    //撠�敺�銝脣�銝璅��鈭� ������摮葡� -> front + "^ " + back
    istringstream fin(front), bin(back);
    string tmp;
    ostringstream ans;
    //ans << "( ";
    for (; fin >> tmp;) {
        if (tmp == "Power") {
            bool comma = false;
            int power = 0;
            ostringstream osfront, osback;
            int Lcount = 0;
            string temp;
            fin >> temp;
            do
            {
                if (temp == "Power") {
                    power++;
                }
                if (temp[0] == ',' && power != 0) power--;
                else if (temp[0] == ',' && comma == false) {
                    comma = true;
                    continue;
                }

                if (temp[0] == '(') Lcount++;
                else if (temp[0] == ')') Lcount--;

                if (comma) osback << temp << " ";
                else osfront << temp << " ";

            } while (Lcount != 0 && fin >> temp);
            ans << process_Power(osfront.str(), osback.str()) << " ";
        }
        else ans << tmp << " ";
    }
    if (back != "") ans << ") ";

    ostringstream ans2;
    ans2 << "( ";
    for (; bin >> tmp;) {
        if (tmp == "Power") {
            bool comma = false;
            int power = 0;
            ostringstream osfront, osback;
            int Lcount = 0;
            string temp;
            bin >> temp;
            do
            {
                if (temp == "Power") {
                    power++;
                }
                if (temp[0] == ',' && power != 0) power--;
                else if (temp[0] == ',' && comma == false) {
                    comma = true;
                    continue;
                }

                if (temp[0] == '(') Lcount++;
                else if (temp[0] == ')') Lcount--;

                if (comma) osback << temp << " ";
                else osfront << temp << " ";

            } while (Lcount != 0 && bin >> temp);
            ans2 << process_Power(osfront.str(), osback.str()) << " ";
        }
        else ans2 << tmp << " ";
    }

    if (back == "") return ans.str();
    return ans.str() + "^ " + ans2.str();
}

double Calculator::Input(bool& can_draw, string inputStr)
{
    stringstream input;
    regex NewVar("\w+");
    std::size_t found = inputStr.find('=');
    if (found != std::string::npos) {
        can_draw = true; // change into can_draw in order to makes it more meaningful in this case. to identify if it can be drawn
        input << inputStr;
        string temp;
        string returnSTR;
        vector<string> allstr;

        while (input >> temp) {
            allstr.push_back(temp);
        }

        /*
        constant equation ex. a=3
        regex x_co("/ x /g"), y_co("/ y /g");
        if((!regex_match(infix, x_co))&&(!regex_match(infix, y_co)))
        */

        //verify if the equation can draw , in other word, the variable(s) are (x,y) or const
        //need to add variable to map
/*
        if (regex_match(allstr[0], NewVar) || allstr[1] == "=") { // [var] = formula
            auto it = is_Var_exist(allstr[0]);
            for (int i = 2; i < allstr.size(); i++) {
                returnSTR += allstr[i];
                if (i != allstr.size() - 1) {
                    returnSTR += " ";
                }
            }
            returnSTR = process_Power(returnSTR, "");
            returnSTR = judgeFormat(returnSTR);
            it->second = calculate(InfixtoPosfix(returnSTR));
            return stod(allstr[0]);
        }
        else throw "Input Error!";

        return stod(allstr[2]);
    }
    else throw "Error: not an equation";
}

string Calculator::judgeFormat(string infix)
{
    istringstream in(infix);
    ostringstream toReturn;
    int countLParentheses = 0;
    int countRParentheses = 0;
    string part;
    bool divide = false;  // judging divide 0 or not
    bool sign = true;     // judging two mathmatical symbols connect or not. ex: 2 * * 2 �� 2 + + 2 (x)
    bool number = false;  // judging two numbers connect or not. ex: 2 2 + 3 1 2 (x) -> should be 22 + 312 (o)
    bool minus = false;  // -> - <- (-5)
    double var_temp;
    for (; in >> part;) {
        if (isdigit(part[0]) || (isdigit(part[1]) && part[0] == '-')) {
            double sub = stod(part);
            var_temp = sub;
            if (divide && var_temp == 0) throw "Error: Can't divide zero.";
            if (number) throw "Error: Two numbers connect.";
            if (minus) {
                if (part[0] == '-') part.erase(part.begin());
                else part.insert(part.begin(), '-');
                minus = false;
            }
            divide = false;
            sign = false;
            number = true;
        }
        else if (part[0] == '(' || part[0] == ')') {
            if (part[0] == '(') countLParentheses++;
            else countRParentheses++;
        }
        else if (part[0] == '!') {
            if (sign) {
                throw "Error: Two mathmatical symbols connect or begin with mathmatical symbol.";
            }
            divide = false;
            sign = false;
            number = false;
        }
        else if (part[0] == '-') {
            if (minus) {
                minus = false;
                sign = true;
                part = "";
            }
            else {
                if (sign) {
                    minus = true;
                    sign = false;
                    part = "";
                }
                if (number) {
                    minus = false;
                    sign = true;
                }
            }

            divide = false;
            number = false;
        }
        else if (part[0] == '+' || part[0] == '*' || part[0] == '/' || part[0] == '^') {

            if (sign) {
                throw "Error: Two mathmatical symbols connect or begin with mathmatical symbol.";
            }
            switch (part[0])
            {
            case '/':
                divide = true; sign = true; number = false; break;
            default:
                divide = false; sign = true; number = false; break;
            }
        }
        else { //if it's not digit or symbol, judging whether it is variable.
            if (!isVariable(part)) throw "Error: Variable doesn't exist.";
            else {
                if (minus)  exist_var[part] *= -1;
                divide = false;
                sign = false;
                number = true;
                minus = false;
            }
        }
        toReturn << part << " ";
    }

    if (countLParentheses != countRParentheses) throw "Incomplete parentheses.";
    return toReturn.str();
}

bool Calculator::isVariable(string str) {
    for (auto i : exist_var) {
        if (i.first == str) {
            return true;
        }
    }
    return false;
}

double Calculator::calculate(string posfix)
{
    istringstream istr(posfix);
    string str;
    stack<double> temp;
    for (; istr >> str;)
    {
        //cout << str << '\n';
        if (isdigit(str[0]) || (isdigit(str[1]) && str[0] == '-') || isVariable(str)) {
            double toPush = stod(str);
            temp.push(toPush);
        }
        else {
            switch (str[0])
            {
            case '+':
                if (temp.size() >= 2) {
                    double a(temp.top());
                    temp.pop();
                    double b(temp.top());
                    temp.pop();

                    temp.push(b + a);
                }
                else throw "Can't a + b, for stack has only one number!\n";
                break;
            case '-':
                if (temp.size() >= 2) {
                    double a(temp.top());
                    temp.pop();
                    double b(temp.top());
                    temp.pop();

                    temp.push(b - a);
                }
                else throw "Can't a - b, for stack has only one number!\n";
                break;
            case '*':
                if (temp.size() >= 2) {
                    double a(temp.top());
                    temp.pop();
                    double b(temp.top());
                    temp.pop();

                    temp.push(b * a);
                }
                else throw "Can't a * b, for stack has only one number!\n";
                break;
            case '/':
                if (temp.size() >= 2) {
                    double a(temp.top());
                    temp.pop();
                    double b(temp.top());
                    temp.pop();

                    temp.push(b / a);
                }
                else throw "Can't a / b, for stack has only one number!\n";
                break;
            case '^':
                if (temp.size() >= 2) {
                    double a(temp.top());
                    temp.pop();
                    double b(temp.top());
                    temp.pop();

                    temp.push(pow(b, a));
                }
                else throw "Can't a ^ b, for stack has only one number!\n";
                break;
            case '!':
                if (temp.size() >= 1) {
                    double a(temp.top());
                    temp.pop();
                    if (a < 0 || floor(a) != a)
                        throw "Error: Wrong factorial type.";
                    double cal = 1;
                    for(int i = 1; i < a; i++)
                    {
                        cal *= i;
                    }
                    temp.push(cal);
                }
                else throw "Can't a !, for stack has no number!\n";
                break;
            }
        }

    }
    return temp.top();
}

int Calculator::weight(char op)
{
    switch (op)
    {
    case '(': return -1;
    case '!':  return 3; //push into queue directly
    case '^': return 2;
    case '*': case '/': case '%': return 1;
    case '+': case '-': return 0;
    }
    return -2;
}

string Calculator::InfixtoPosfix(string infix)
{
    istringstream in(infix);
    ostringstream posfix;
    stack<char> saveOperator;

    string temp;
    for (; in >> temp;) {
        if (isdigit(temp[0]) || (isdigit(temp[1]) && temp[0] == '-') || isVariable(temp)) {
            posfix << temp << " ";
        }
        else {
            switch (temp[0])
            {
            case '(': saveOperator.push('('); break;
            case ')':
                for (; saveOperator.top() != '(';) {
                    if (!saveOperator.empty()) {
                        posfix << saveOperator.top() << " ";
                        saveOperator.pop();
                    }
                }
                saveOperator.pop();
                break;
            case '+': case '-': case '*': case '/': case '^': case '!': case '%':
                for (; !saveOperator.empty() && weight(temp[0]) <= weight(saveOperator.top());) {
                    if (!saveOperator.empty()) {
                        posfix << saveOperator.top() << " ";
                        saveOperator.pop();
                    }
                }
                saveOperator.push(temp[0]);
                break;
            }
        }
    }

    for (; !saveOperator.empty();) {
        posfix << saveOperator.top() << " ";
        saveOperator.pop();
    }
    if (posfix.str().empty()) throw "Error: Empty calculation.";
    return posfix.str();
}

double Calculator::Output(double ans)
{
    stringstream ss;
    ss << ans;
    cout << ans << endl;
    return stod(ss.str());
}

void Calculator::test()
{
    /*
    Number A("-123.16543");
    Number C("abc");
    string temp;
    getline(std::cin,temp);
    judgeFormat(temp);
    //std::cout << A.getNum() << ' ' << A.getDecimal() << ' ' << A.Integer << ' ' << A.negative << '\n';
    */
/*
    while (true)
    {
        bool equal = false;
        string str;
        getline(cin, str);
        // have been edited by Gabriel
        double ans(Input(equal, str));
        if (!equal) Output(ans);
    }
}
*/
