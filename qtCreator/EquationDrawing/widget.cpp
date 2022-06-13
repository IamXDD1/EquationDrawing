#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

QString addSpace(QString input)
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

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    Widget::makeplot();
    row_count = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::makeplot(){
    // generate some data:
    QVector<double> x(201), y(201); // initialize with entries 0..100
    for (int i=0; i<201; ++i)
    {
      x[i] = i/50.0 - 2; // x goes from -1 to 1
      y[i] = sin(x[i]); // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(-1, 1);
    ui->customPlot->yAxis->setRange(0, 1);
    ui->customPlot->setInteraction(QCP::iRangeDrag,true);
    ui->customPlot->setInteraction(QCP::iRangeZoom,true);
    ui->customPlot->replot();
}

void Widget::createFrame(){
    MyFrame* frame = new MyFrame(this);
    frame->setParent(this);
    frame->resize(460,30);
    frame->move(500,80+row_count*30);
    frame->show();

    FrameList.append(frame);

    //this->addAction();

    connect(frame->text, SIGNAL(cursorPositionChanged(int,int)), frame, SLOT(showOption()));
    connect(frame->text, SIGNAL(returnPressed()), frame, SLOT(hideOption()));
    connect(frame->showGraph_btn, SIGNAL(clicked()), frame, SLOT(changeIcon()));
    connect(frame->deleteFrame_btn, SIGNAL(clicked()), frame, SLOT(deleteFrame()));
    connect(frame->deleteFrame_btn, SIGNAL(clicked()), frame, SLOT(deleteLater()));
}

void Widget::on_CreateFunction_clicked()
{
    createFrame();
    row_count++;
}

void MyFrame::initialize()
{
    isEquationShow = true;
    int btn_size = 28;

    color_btn = new QPushButton;
    color_btn->setParent(this);
    r = QString::number(rand()%256);
    g = QString::number(rand()%256);
    b = QString::number(rand()%256);
    color_btn->setStyleSheet("background-color: rgb(" + r + ", " + g + ", " + b + ");");
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

    equation = text->text();
    //Gabriel
    equation = addSpace(equation);
    //qDebug()<<equation;
    drawGraph();
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
    deleteGraph();
}

void MyFrame::showOption()
{
    showGraph_btn->show();
    deleteFrame_btn->show();
}

void MyFrame::hideOption()
{
    text->setCursorPosition(0);
    showGraph_btn->hide();
    deleteFrame_btn->hide();

    equation = text->text();
    //QMessageBox::information(NULL,equation,equation);
    deleteGraph(); //enter new equation
    //Gabriel
    equation = addSpace(equation);
   // qDebug()<<equation;
    drawGraph();
}

void MyFrame::changeIcon()
{
    if(isEquationShow){
        isEquationShow = false;
        showGraph_btn->setIcon(QIcon(image_path + "hide equation.png"));
        hideGraph();
    }
    else{
        isEquationShow = true;
        showGraph_btn->setIcon(QIcon(image_path + "show equation.png"));
        showGraph();
    }
}

void MyFrame::showGraph()
{
    //TODO...
}

void MyFrame::hideGraph()
{
    //TODO...
}

void MyFrame::drawGraph()
{
    //TODO...
    showGraph();
}

void MyFrame::deleteGraph()
{
    //TODO...
    hideGraph();
}
