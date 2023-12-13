#ifndef NEWSCRIPTWINDOW_H
#define NEWSCRIPTWINDOW_H

#include <QDialog>

namespace Ui {
class NewScriptWindow;
}

class NewScriptWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewScriptWindow(QWidget *parent = nullptr);
    ~NewScriptWindow();

private:
    Ui::NewScriptWindow *ui;
};

#endif // NEWSCRIPTWINDOW_H
