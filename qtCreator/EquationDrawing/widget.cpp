#include "widget.h"
#include "ui_widget.h"

using std::ostringstream;
using std::stringstream;
using std::istringstream;

QString NumberProcess::addSpace(QString input)
{
    int mark = -1;
    // num = 1
    // char = 2
    // else = 3

    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] <= '9' && input[i] >= '0')
        {
            if (mark != 1)
            {
                input.insert(i, " ");
                mark = 1;
                i++;
            }
        }
        else if (((input[i] <= 'z' && input[i] >= 'a') || (input[i] <= 'Z' && input[i] >= 'A')))
        {
            if (mark != 2)
            {
                input.insert(i, " ");
                mark = 2;
                i++;
            }
        }
        else
        {
            input.insert(i, " ");
            mark = 3;
            i++;
        }
    }

    input.remove(0,1);
    return input;
}

double NumberProcess::RUN(QString equation)
{
    string str;
    str = equation.toStdString();
    // have been editted by Gabriel
    double ans(Input(str));
    return Output(ans);
}

auto NumberProcess::is_Var_exist(string name)
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

double NumberProcess::Input(string inputStr)
{
    stringstream input;
    regex NewVar("[a-zA-Z]");
    std::size_t found = inputStr.find('=');

    // want no "=" in the inputStr
    if (found == std::string::npos) {
        input << inputStr;
        string temp;
        string returnSTR;
        vector<string> allstr;

        while (input >> temp) {
            allstr.push_back(temp);
        }

        returnSTR = judgeFormat(returnSTR);
        return calculate(InfixtoPosfix(returnSTR));
    }
    else throw "Error: more then one \"=\" sign in the frame";
}

string NumberProcess::judgeFormat(string infix)
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

bool NumberProcess::isVariable(string str) {
    for (auto i : exist_var) {
        if (i.first == str) {
            return true;
        }
    }
    return false;
}

double NumberProcess::calculate(string posfix)
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

int NumberProcess::weight(char op)
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

string NumberProcess::InfixtoPosfix(string infix)
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

double NumberProcess::Output(double ans)
{
    stringstream ss;
    ss << ans;
    cout << ans << endl;
    return stod(ss.str());
}

void NumberProcess::test()
{
    /*
    Number A("-123.16543");
    Number C("abc");
    string temp;
    getline(std::cin,temp);
    judgeFormat(temp);
    //std::cout << A.getNum() << ' ' << A.getDecimal() << ' ' << A.Integer << ' ' << A.negative << '\n';
    */
    while (true)
    {
        bool equal = false;
        string str;
        getline(cin, str);
        // have been edited by Gabriel
        double ans(Input(str));
        if (!equal) Output(ans);
    }
}

//===============================================================================================================

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    Widget::makeplot();
    row_count = 0;
    graph_count = 0;
}

Widget::~Widget()
{
    ui->customPlot->clearGraphs();
    delete ui;
}

void Widget::on_CreateFunction_clicked()
{
    createFrame();
    row_count++;
    graph_count++;
}

void Widget::makeplot(){
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(-1, 1);
    ui->customPlot->yAxis->setRange(-1, 1);
    ui->customPlot->setInteraction(QCP::iRangeDrag,true);
    ui->customPlot->setInteraction(QCP::iRangeZoom,true);
    ui->customPlot->replot();
}

void Widget::createFrame(){
    ui->customPlot->addGraph();
    MyFrame* frame = new MyFrame(this);
    frame->setParent(this);
    frame->resize(460,30);
    frame->move(500,80+row_count*30);
    frame->show();
    ui->customPlot->graph(frame->graph_index)->setPen(QPen(QColor(frame->r,frame->g,frame->b)));

    FrameList.append(frame);

    connect(frame->text, SIGNAL(cursorPositionChanged(int,int)), frame, SLOT(showOption()));
    connect(frame->text, SIGNAL(returnPressed()), frame, SLOT(hideOption()));
    connect(frame->showGraph_btn, SIGNAL(clicked()), frame, SLOT(changeIcon()));
    connect(frame->deleteFrame_btn, SIGNAL(clicked()), frame, SLOT(deleteFrame()));
    connect(frame->deleteFrame_btn, SIGNAL(clicked()), frame, SLOT(deleteLater()));
    connect(frame, SIGNAL(drawGraph(QString, int)), this, SLOT(drawing(QString, int)));
    connect(frame, SIGNAL(showGraph(int)), this, SLOT(showing(int)));
    connect(frame, SIGNAL(hideGraph(int)), this, SLOT(hiding(int)));
    connect(frame, SIGNAL(deleteGraph(int)), this, SLOT(deleting(int)));

    frame->emitDrawing();
}

void Widget::drawing(QString equation, int graph_idx)
{
    // need to tell if it can be drawn? (if the variable doesn't include x or y)
    // --> a new function
    QString temp = "" + equation[equation.size()-1];
    int num = temp.toInt();
    // QMessageBox::information(NULL,equation,equation);
    /*
     * bool can_draw = judge_if_can_draw(equation);
     * if(can_draw)
     * {
     *      QVector<double> x(20001), y(20001); // initialize with entries 0..100
     *      for (int i=0; i<20001; ++i)
     *      {
     *          x[i] = i; // x goes from -1 to 1
     *          //sometig convert x to i
     *          y[i] = RUN(equation, can_draw); // let's plot a quadratic function
     *      }
     * }
     * else
     * {
     *      RUN(equation, can_draw);
     *      return;
     * }
     */

    QVector<double> x(20001), y(20001); // initialize with entries 0..100
    for (int i=0; i<20001; ++i)
    {
      x[i] = i; // x goes from -1 to 1
      y[i] = num; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->customPlot->graph(graph_idx)->setData(x, y);
    ui->customPlot->replot();
}

void Widget::showing(int graph_idx)
{
    ui->customPlot->graph(graph_idx)->setVisible(true);
    ui->customPlot->replot();
}

void Widget::hiding(int graph_idx)
{
    ui->customPlot->graph(graph_idx)->setVisible(false);
    ui->customPlot->replot();
}

void Widget::deleting(int graph_idx)
{
    ui->customPlot->graph(graph_idx)->data()->clear();
    ui->customPlot->replot();
}


//===============================================================================================================

void MyFrame::initialize()
{
    graph_index = graph_count;
    isEquationShow = true;
    int btn_size = 28;

    color_btn = new QPushButton;
    color_btn->setParent(this);
    r = rand()%256;
    g = rand()%256;
    b = rand()%256;
    color_btn->setStyleSheet("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
    color_btn->resize(btn_size,btn_size);
    color_btn->move(0,1);
    color_btn->show();

    text = new QLineEdit;
    text->setParent(this);
    text->setText("y=1");
    text->setCursorPosition(0);
    text->resize(370,btn_size);
    text->move(30,1);
    text->show();

    showGraph_btn = new QPushButton;
    showGraph_btn->setParent(this);
    showGraph_btn->resize(btn_size,btn_size);
    showGraph_btn->move(400,1);
    showGraph_btn->hide();
    showGraph_btn->setIcon(QIcon(image_path + "show equation.png"));
    showGraph_btn->setIconSize(QSize(btn_size, btn_size));
    showGraph_btn->setFixedSize(QSize(btn_size, btn_size));

    deleteFrame_btn = new QPushButton;
    deleteFrame_btn->setParent(this);
    deleteFrame_btn->resize(btn_size,btn_size);
    deleteFrame_btn->move(431,1);
    deleteFrame_btn->hide();
    deleteFrame_btn->setIcon(QIcon(image_path + "delete equation.png"));
    deleteFrame_btn->setIconSize(QSize(btn_size, btn_size));
    deleteFrame_btn->setFixedSize(QSize(btn_size, btn_size));

    splitEqualSign(text->text());
}

void MyFrame::splitEqualSign(QString temp)
{
    bool findEqualSign = false;
    for(int i=0; i<temp.size(); i++){
        if(temp[i] == '='){
            var.name = temp.mid(0,i);
            var.equation = temp.mid(i+1,temp.size()-i-1);
            findEqualSign = true;
            break;
        }
    }
    if(!findEqualSign){
        color_btn->setText("E");
        return;
    }
    else{
        color_btn->setText(" ");
    }
    var.equation = addSpace(var.equation);
    judgeError();
}

bool MyFrame::isSaveWord(QString name)
{
    if(name != "y" && name != "x" && name != "x^2+y^2") return false;
    else return true;
}

void MyFrame::replaceVar()
{
    var.temp = "( ";

    QTextStream Qss(&var.equation);
    QString str;
    for(;!Qss.atEnd();)
    {
        Qss >> str;
        QString toReplace = str;
        for(int i=0; i<FrameList.count(); i++){
            if(FrameList[i]->color_btn->text() == "E") continue; // if error -> skip
            if(FrameList[i] == this){
                if(this->var.name == str)
                this->color_btn->setText(" ");
                break;
            }
            else{
                // var redefine
                if(this->var.name == FrameList[i]->var.name && !isSaveWord(this->var.name)) {
                    //toReplace = str;
                    this->color_btn->setText("E");
                    break;
                }
                else{
                    this->color_btn->setText(" ");
                }
            }
            if(FrameList[i]->var.name == str){
                toReplace = FrameList[i]->var.temp;
            }
        }
        var.temp += toReplace + ' ';
    }
    var.temp += ')';
}

void MyFrame::judgeError()
{
    emitdeleting(); //enter new equation
    //L&R of equal sign has no content
    if(this->var.equation == "" || this->var.name == "") {
        this->color_btn->setText("E");
        return;
    }
    //variable name start without english
    if(!(this->var.name[0].toLower() >= 'a' && this->var.name[0].toLower() <= 'z')){
        color_btn->setText("E");
        return;
    }
    //if equation has variable, replace it to variable's equation
    replaceVar();

    QMessageBox::information(NULL,"ori:"+var.equation,"re:"+var.temp);
    // if error
    //color_btn->setText("E");

    // else
    //color_btn->setText(" ");
    //emitDrawing();
}

void MyFrame::deleteFrame(){
    row_count--;
    bool find = false;
    for(int i=0; i<FrameList.count(); i++){
        if(FrameList[i] == this){
            FrameList.erase(FrameList.begin()+i);
            find = true;
        }
        if(find && i != FrameList.count()){
            FrameList[i]->move(500,80+i*30);
        }
    }
    emitdeleting();
}

void MyFrame::showOption()
{
    showGraph_btn->show();
    deleteFrame_btn->show();
}

void MyFrame::hideOption() // press enter
{
    text->setCursorPosition(0);
    showGraph_btn->hide();
    deleteFrame_btn->hide();

    splitEqualSign(text->text());

    //QMessageBox::information(NULL,equation,equation);
}

void MyFrame::changeIcon()
{
    if(isEquationShow){
        isEquationShow = false;
        showGraph_btn->setIcon(QIcon(image_path + "hide equation.png"));
        color_btn->setText("X");
        emitHiding();
    }
    else{
        isEquationShow = true;
        showGraph_btn->setIcon(QIcon(image_path + "show equation.png"));
        color_btn->setText(" ");
        emitShowing();
    }
}
