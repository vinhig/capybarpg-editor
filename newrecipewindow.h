#ifndef NEWRECIPEWINDOW_H
#define NEWRECIPEWINDOW_H

#include <QDialog>

namespace Ui {
class NewRecipeWindow;
}

class NewRecipeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewRecipeWindow(QWidget *parent = nullptr);
    ~NewRecipeWindow();

private:
    Ui::NewRecipeWindow *ui;
};

#endif // NEWRECIPEWINDOW_H
