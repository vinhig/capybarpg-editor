#include "newrecipewindow.h"
#include "ui_newrecipewindow.h"

NewRecipeWindow::NewRecipeWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewRecipeWindow)
{
    ui->setupUi(this);
}

NewRecipeWindow::~NewRecipeWindow()
{
    delete ui;
}
