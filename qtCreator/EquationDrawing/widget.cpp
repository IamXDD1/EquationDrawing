#include "widget.h"
#include "ui_widget.h"

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
    QString temp = "" + equation[equation.size()-1];
    int num = temp.toInt();
    //QMessageBox::information(NULL,equation,equation);

    /*
     *QVector<double> x(20001), y(20001); // initialize with entries 0..100
     *for (int i=0; i<20001; ++i)
     *{
     *   x[i] = i; // x goes from -1 to 1
     *   //sometig convert x to i
     *   y[i] = RUN(equation); // let's plot a quadratic function
     *}
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

    equation = text->text();
    equation = addSpace(equation);
    color_btn->setText(" ");
}

void MyFrame::judgeError()
{
    // if error
    color_btn->setText("E");
    // else
    color_btn->setText(" ");
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

    equation = text->text();
    equation = addSpace(equation);
    //QMessageBox::information(NULL,equation,equation);
    emitdeleting(); //enter new equation
    emitDrawing();
}

void MyFrame::changeIcon()
{
    if(isEquationShow){
        isEquationShow = false;
        showGraph_btn->setIcon(QIcon(image_path + "hide equation.png"));
        emitHiding();
    }
    else{
        isEquationShow = true;
        showGraph_btn->setIcon(QIcon(image_path + "show equation.png"));
        emitShowing();
    }
}
