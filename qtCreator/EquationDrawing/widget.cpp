#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
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
        if ((input[i] <= '9' && input[i] >= '0') || input[i] == '.')
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

QString NumberProcess::RUN(QString equation)
{
    string str = equation.toStdString();
    return QString::fromStdString(Input(str));
}

string NumberProcess::Input(string inputStr)
{
    int doubleM = 0;
    for(int i = 0 ; i < inputStr.size() ; i++)
    {
        if(doubleM == 1) doubleM = 2;
        else if(doubleM == 2)
        {
            if(inputStr[i] == '-' || inputStr[i] == '+' ||  inputStr[i] == '*' || inputStr[i] == '/' ||
                    inputStr[i] == '%' || inputStr[i] == '^' || inputStr[i] == '!')
            {
                throw "Error: Two mathmatical symbols connect or begin with mathmatical symbol.";
            }
            else doubleM = 0;
        }

        if(inputStr[i] == '-') doubleM = 1;
    }

    for(int i = 0 ; i < inputStr.size() ; i++)
    {
        if(inputStr[i] == '-') inputStr.insert(i + 1, " 1 * ");
    }

    stringstream input;
    std::size_t found = inputStr.find('=');
    istringstream check_ilegal(inputStr);
    string check;
    bool hollow = false;
    while(check_ilegal >> check){
        if(!isdigit(check[0]))
        {
            if(check != "sin" && check != "cos" && check != "tan" && check != "(" && check != ")"  && check != "x" && check != "y"
                   && check != "+" && check != "-" && check != "*" && check != "/" && check != "%" && check != "^" && check != "!" )

                throw check;//"Error: undefined variable exists";
        }
        if(check == "(") hollow = true;
        if(hollow && check == ")") throw "Error: nothing in the parentheses";
        if(hollow && check != ")" && check != "(") hollow = false;
    }

    // want no "=" in the inputStr
    if (found == std::string::npos) {
        string returnSTR = inputStr;
        returnSTR = judgeFormat(returnSTR);
        return InfixtoPosfix(returnSTR);
    }
    else throw "Error: more then one \"=\" sign in the frame";
}

string NumberProcess::judgeFormat(string infix)
{
    stringstream in;
    in << infix;
    ostringstream toReturn;
    int countLParentheses = 0;
    int countRParentheses = 0;
    string part;
    bool divide = false;  // judging divide 0 or not
    bool sign = true;     // judging two mathmatical symbols connect or not. ex: 2 * * 2 ?????? 2 + + 2 (x)
    bool number = false;  // judging two numbers connect or not. ex: 2 2 + 3 1 2 (x) -> should be 22 + 312 (o)
    bool minus = false;  // -> - <- (-5)
    double var_temp;
    bool need_a_par = false;
    for (; in >> part;) {
        if(need_a_par)
        {
            if(part[0] != '(') throw "Error: not a reasonable equation.";
            else need_a_par = false;
        }
        if (isdigit(part[0]) || (isdigit(part[1]) && part[0] == '-') || part[0] == 'x') {
            if(part[0]!='x' && part[0] != 'y')
            {
                double sub = stod(part);
                var_temp = sub;
                if (divide && var_temp == 0) throw "Error: Can't divide zero.";
            }
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
            if (part != "sin" && part != "cos" && part != "tan" )
            {
                if (minus) {
                    string temp = toReturn.str();
                    temp.erase(temp.end() - 1);
                    toReturn.clear();
                    toReturn.str(temp);
                    part.insert(part.begin(), '-');
                }
                divide = false;
                sign = false;
                number = true;
                minus = false;
            }
            else
            {
                need_a_par = true;
            }
        }
        toReturn << part << " ";
    }

    if (countLParentheses != countRParentheses) throw "Incomplete parentheses.";
    return toReturn.str();
}

double NumberProcess::calculate(QString posfix)
{
    istringstream istr(posfix.toStdString());
    string str;
    stack<double> temp;
    for (; istr >> str;)
    {
        //cout << str << '\n';
        if (isdigit(str[0]) || (isdigit(str[1]) && str[0] == '-')) {
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

                    temp.push(std::pow(b, a));
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
            case 's':
                if (temp.size() >= 1) {
                    double a(temp.top());
                    temp.pop();

                    temp.push(sin(a));
                }
                else throw "Can't sin(a) , for stack has no number!\n";
                break;
            case 'c':
                if (temp.size() >= 1) {
                    double a(temp.top());
                    temp.pop();

                    temp.push(cos(a));
                }
                else throw "Can't cos(a) !, for stack has no number!\n";
                break;
            case 't':
                if (temp.size() >= 1) {
                    double a(temp.top());
                    temp.pop();

                    temp.push(tan(a));
                }
                else throw "Can't tan(a) !, for stack has no number!\n";
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
    case '(': case 's': case 'c': case 't': return -1;
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
        if (isdigit(temp[0]) || (isdigit(temp[1]) && temp[0] == '-') || temp[0] == 'x' || (temp[1] == 'x' && temp[0] == '-') || temp[0] == 'y') {
            posfix << temp << " ";
        }
        else {  // sin ( 2 )
            switch (temp[0])
            {
            case '(': saveOperator.push('('); break;
            case 's': saveOperator.push('('); saveOperator.push('s'); in >> temp; break;
            case 'c': saveOperator.push('('); saveOperator.push('c'); in >> temp; break;
            case 't': saveOperator.push('('); saveOperator.push('t'); in >> temp; break;
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
    string chop = posfix.str().erase(posfix.str().size() - 1 );
    return chop;
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

void Widget::makeplot(){\
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
    frame->move(620,90+row_count*30);
    frame->show();
    ui->customPlot->graph(frame->graph_index)->setPen(QPen(QColor(frame->r,frame->g,frame->b)));

    FrameList.append(frame);

    connect(frame->text, SIGNAL(cursorPositionChanged(int,int)), frame, SLOT(showOption()));
    connect(frame->text, SIGNAL(returnPressed()), frame, SLOT(hideOption()));
    connect(frame->showGraph_btn, SIGNAL(clicked()), frame, SLOT(changeIcon()));
    connect(frame->deleteFrame_btn, SIGNAL(clicked()), frame, SLOT(deleteFrame()));
    connect(frame->deleteFrame_btn, SIGNAL(clicked()), frame, SLOT(deleteLater()));
    connect(frame, SIGNAL(drawGraph(QString, QString, int)), this, SLOT(drawing(QString, QString, int)));
    connect(frame, SIGNAL(showGraph(int)), this, SLOT(showing(int)));
    connect(frame, SIGNAL(hideGraph(int)), this, SLOT(hiding(int)));
    connect(frame, SIGNAL(deleteGraph(int)), this, SLOT(deleting(int)));

    frame->emitDrawing();
}

void Widget::drawing(QString var_name, QString equation, int graph_idx)
{
#define RANGE 400001
#define PRECISION 35.0

    //QMessageBox::information(NULL,equation,equation);

    if(var_name == "y"){
        std::vector<int> index;
        for(int i = 0 ; i < equation.size() ; i++)
        {
            if(equation[i] == 'x') index.push_back(i);
        }

        QVector<double> x(RANGE), y(RANGE); // initialize with entries 0..100
        for (int i=0; i<RANGE; ++i)
        {
          double x_temp = i/PRECISION - (RANGE/PRECISION/2);
          QString toReplace = equation;
          for(int j = index.size()-1 ; j >= 0; j--)
          {
              toReplace.replace(index[j], 1, QString::number(x_temp));
          }

          x[i] = x_temp; // x goes from -1 to 1
          y[i] = calculate(toReplace); // let's plot a quadratic function
        }

        // create graph and assign data to it:
        ui->customPlot->graph(graph_idx)->setData(x, y);
        ui->customPlot->replot();
    }
    else if(var_name == "x"){
        std::vector<int> index;
        for(int i = 0 ; i < equation.size() ; i++)
        {
            if(equation[i] == 'y') index.push_back(i);
        }

        QVector<double> x(RANGE), y(RANGE); // initialize with entries 0..100
        for (int i=0; i<RANGE; ++i)
        {
          double y_temp = i/PRECISION - (RANGE/PRECISION/2);
          QString toReplace = equation;
          for(int j = 0 ; j < index.size(); j++)
          {
              toReplace.replace(index[j], 1, QString::number(y_temp));
          }
          y[i] = y_temp; // x goes from -1 to 1
          //if(i == 1) QMessageBox::information(NULL,"test",toReplace);
          x[i] = calculate(toReplace); // let's plot a quadratic function
        }

        // create graph and assign data to it:
        ui->customPlot->graph(graph_idx)->setData(x, y);
        ui->customPlot->replot();
    }
    else{

    }
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

    try{
        var.postfix = RUN(var.temp);
    }
    catch(const char* error){
         color_btn->setText("E");
         return;
    }
    catch(std::string error){
        QString temp = QString::fromStdString(error);
        color_btn->setText("E");
        return;
    }
    catch(...){
        color_btn->setText("E");
        return;
    }

    color_btn->setText(" ");
    emitDrawing();
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
