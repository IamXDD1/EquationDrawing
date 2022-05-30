#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EquationDrawing.h"

class EquationDrawing : public QMainWindow
{
    Q_OBJECT

public:
    EquationDrawing(QWidget *parent = Q_NULLPTR);

private:
    Ui::EquationDrawingClass ui;
};
