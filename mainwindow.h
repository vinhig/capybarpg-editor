#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
} // namespace Ui
QT_END_NAMESPACE

class QLabel;
class QProgressBar;
class QVBoxLayout;
class QDir;
class QDialog;
class SettingsWindow;
class NewScriptWindow;
class NewRecipeWindow;
class QSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;
    NewScriptWindow *newScriptWindow;
    NewRecipeWindow *newRecipeWindow;

    QDir *projectDir;

    QVBoxLayout *layoutStatusBar;
    QLabel *infoStatusBar;
    QProgressBar *progressStatusBar;

    void onExit();
    void onPreferences();
    void onOpenProject();

    void onBuild();
    void onRun();

    void onPreferencesAccepted();

protected:
};
#endif // MAINWINDOW_H
