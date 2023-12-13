#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "settingswindow.h"
#include "newrecipewindow.h"
#include "newscriptwindow.h"

#include <QApplication>
#include <QDialog>
#include <QDirIterator>
#include <QFileDialog>
#include <QLabel>
#include <QProcess>
#include <QProgressBar>
#include <QSettings>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QThread>
#include <QPushButton>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    settingsWindow = new SettingsWindow(this);
    newScriptWindow = new NewScriptWindow(this);
    newRecipeWindow = new NewRecipeWindow(this);
    ui->setupUi(this);

    connect(ui->actionExit, &QAction::triggered, [this](bool checked = false) { onExit(); });

    connect(ui->actionOpen_Project, &QAction::triggered, [this](bool checked = false) {
        onOpenProject();
    });

    connect(ui->actionPreferences, &QAction::triggered, [this](bool checked = false) {
        onPreferences();
    });

    connect(settingsWindow, &QDialog::accepted, [this]() { onPreferencesAccepted(); });

    connect(ui->buildButton, &QPushButton::clicked, [this]() { onBuild(); });
    connect(ui->runButton, &QPushButton::clicked, [this]() { onRun(); });

    layoutStatusBar = new QVBoxLayout(this);

    // The label info at the bottom left of the screen
    infoStatusBar = new QLabel(this);
    infoStatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    infoStatusBar->setText("Load an existing project or create a new one to begin.");

    // The progress bar at the bottom right of the screen
    progressStatusBar = new QProgressBar(this);
    progressStatusBar->setMinimum(0);
    progressStatusBar->setMaximum(10);
    progressStatusBar->setValue(0);

    ui->statusbar->setLayout(layoutStatusBar);
    ui->statusbar->addWidget(infoStatusBar, 2);
    ui->statusbar->addWidget(progressStatusBar, 1);
}

void MainWindow::onExit()
{
    qApp->quit();
}

void MainWindow::onOpenProject()
{
    QString projectFolder = QFileDialog::getExistingDirectory(this, "Open Project Folder", "~");

    if (projectFolder.isNull()) {
        return;
    }

    QStringList jsonFilter("*.json");
    QStringList quakecFilter{"*.qc", "*.qh"};

    projectDir = new QDir(projectFolder);

    infoStatusBar->setText(projectFolder);

    ui->treeWidget->clear();
    ui->treeWidget->setEnabled(true);

    ui->newListenerButton->setEnabled(true);
    ui->newScriptButton->setEnabled(true);
    ui->newRecipeButton->setEnabled(true);

    ui->buildButton->setEnabled(true);
    ui->runButton->setEnabled(true);
    ui->debugButton->setEnabled(true);

    // Fetch all script source code, and all source header
    // They can be everywhere
    {
        QTreeWidgetItem *scriptTreeItem = new QTreeWidgetItem(ui->treeWidget,
                                                              QStringList("Scripts"));

        QDirIterator iterator(projectFolder,
                              quakecFilter,
                              QDir::Files,
                              QDirIterator::Subdirectories);

        while (iterator.hasNext()) {
            QDir next = iterator.next();

            QString name = next.path().remove(0, projectFolder.size() + 1);

            scriptTreeItem->addChild(new QTreeWidgetItem(scriptTreeItem, QStringList("./" + name)));
        }

        ui->treeWidget->addTopLevelItem(scriptTreeItem);
        ui->treeWidget->sortItems(0, Qt::SortOrder::AscendingOrder);
    }

    // Fetch all .json recipes that are in the /recipes folder
    {
        QDir recipeDir(projectFolder.append("/recipes"));

        QStringList recipeList = recipeDir.entryList(jsonFilter);

        QTreeWidgetItem *jsonTreeItem = new QTreeWidgetItem(ui->treeWidget, QStringList("Recipes"));

        for (unsigned i = 0; i < recipeList.size(); i++) {
            std::cout << recipeList[i].toStdString() << std::endl;

            jsonTreeItem->addChild(new QTreeWidgetItem(jsonTreeItem, QStringList(recipeList[i])));
        }

        ui->treeWidget->addTopLevelItem(jsonTreeItem);
    }
}

void MainWindow::onPreferences()
{
    settingsWindow->exec();
}

void MainWindow::onPreferencesAccepted()
{
    QSettings settings(QApplication::applicationDirPath() + QString("/config.ini"),
                       QSettings::IniFormat);

    std::cout << "-->" << settings.value(QString("sdk")).toString().toStdString() << std::endl;
    std::cout << "-->" << settings.value(QString("qcc")).toString().toStdString() << std::endl;
}

QString logToHtml(QString log) {
    log.replace("\n", "<br>");
    log.replace(" warning ", "<span style='color: orange'> warning </span>");
    log.replace("error:", "<span style='color: red'>error:</span>");
    log.replace("note:", "<span style='color: blue'>note:</span>");
    log.replace("info:", "<span style='color: blue'>info:</span>");

    return QString("<html> <head> <style> * { font-family: 'Deja Vu Sans Mono'} </style> </head> <body>") + log + QString("</body></html>");
}

void MainWindow::onBuild()
{
    QSettings settings(QApplication::applicationDirPath() + QString("/config.ini"),
                       QSettings::IniFormat);

    if (settings.value(QString("qcc")).isNull()) {
        return;
    }

    QProcess compilerProcess;
    compilerProcess.start(settings.value(QString("qcc")).toString(),
                          QStringList{"-O3", projectDir->absolutePath() + "/progs.src"});
    compilerProcess.waitForFinished();
    if (compilerProcess.exitCode() == 1) {
        infoStatusBar->setText(QString("Build failed."));
    } else {
        infoStatusBar->setText(QString("Build succeed."));
    }
    ui->logTextEdit->setReadOnly(false);
    ui->logTextEdit->setHtml(logToHtml(QString(compilerProcess.readAllStandardError())));
    std::cout << "the shit is " << logToHtml(QString(compilerProcess.readAllStandardError())).toStdString()
              << std::endl;
    ui->logTextEdit->setReadOnly(true);
}

void MainWindow::onRun()
{
    QSettings settings(QApplication::applicationDirPath() + QString("/config.ini"),
                       QSettings::IniFormat);

    if (settings.value(QString("sdk")).isNull()) {
        return;
    }

    QProcess engineProcess;
    QDir workingDir(settings.value(QString("sdk")).toString());
    workingDir.cdUp();
    engineProcess.setWorkingDirectory(workingDir.absolutePath());
    engineProcess.start(settings.value(QString("sdk")).toString(),
                          QStringList{"--base", projectDir->absolutePath()});

    engineProcess.waitForFinished();

    ui->logTextEdit->setHtml(logToHtml(QString(engineProcess.readAllStandardOutput())));
}

MainWindow::~MainWindow()
{
    delete infoStatusBar;
    delete layoutStatusBar;
    delete progressStatusBar;
    delete ui;
}
