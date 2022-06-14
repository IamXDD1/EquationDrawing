#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextCharFormat>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QList>
#include <QMetaType>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class MyFrame;

static int row_count = 0;
static int graph_count = 0;
static QList<MyFrame*> FrameList;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    Ui::Widget *ui;

    void createFrame();

public slots:
    void makeplot();
    void drawing(QString equation, int graph_idx);
    void showing(int graph_idx);
    void hiding(int graph_idx);
    void deleting(int graph_idx);

private slots:
    void on_CreateFunction_clicked();

private:

};

class MyFrame : public QFrame {
    Q_OBJECT
public:
    //explicit MyFrame(QObject *parent = 0);

    QPushButton* color_btn,* showGraph_btn,* deleteFrame_btn;
    QLineEdit* text;
    QString r,g,b;
    bool isEquationShow;
    QString image_path = "C:/Users/XDD/Desktop/EquationDrawing/EquationDrawing/QTcreator/EquationDrawing/image/";
    QString equation;
    int graph_index;

    MyFrame(QWidget *parent = nullptr) {
        initialize();
    }
    ~MyFrame(){
        delete color_btn;
        delete showGraph_btn;
        delete deleteFrame_btn;
        delete text;
    }

    void initialize();
    inline void emitDrawing(){ emit drawGraph(this->equation, this->graph_index); }
    inline void emitShowing(){ emit showGraph(this->graph_index); }
    inline void emitHiding(){ emit hideGraph(this->graph_index); }
    inline void emitdeleting(){ emit deleteGraph(this->graph_index); }


public slots:
    void showOption();
    void hideOption();
    void changeIcon();
    void deleteFrame();

signals:
    void drawGraph(QString str_equation, int idx);
    void showGraph(int idx);
    void hideGraph(int idx);
    void deleteGraph(int idx);
};

#endif // WIDGET_H
