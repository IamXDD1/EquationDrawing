#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    row_count = 0;
}

Widget::~Widget()
{
    delete ui;
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
