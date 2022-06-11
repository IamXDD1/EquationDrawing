#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextCharFormat>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QList>
#include <QMetaType>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class MyFrame;

static int row_count = 0;
static QList<MyFrame*> FrameList;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void createFrame();

public slots:


private slots:
    void on_CreateFunction_clicked();

private:
    Ui::Widget *ui;
};

class MyFrame : public QFrame {
    Q_OBJECT
public:
    QPushButton* color_btn,* showGraph_btn,* deleteFrame_btn;
    QLineEdit* text;
    QString r,g,b;
    bool isEquationShow;
    QString image_path = "C:/Users/XDD/Desktop/EquationDrawing/EquationDrawing/qtcreator/EquationDrawing/image/";
    QString equation;

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
    void drawGraph(); // TODO
    void deleteGraph(); // TODO
    void showGraph(); // TODO
    void hideGraph(); // TODO

public slots:
    void showOption();
    void hideOption();
    void changeIcon();
    void deleteFrame();
};

#endif // WIDGET_H
