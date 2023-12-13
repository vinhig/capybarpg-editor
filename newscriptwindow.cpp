#include "newscriptwindow.h"
#include "ui_newscriptwindow.h"

NewScriptWindow::NewScriptWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewScriptWindow)
{
    ui->setupUi(this);
}

NewScriptWindow::~NewScriptWindow()
{
    delete ui;
}
