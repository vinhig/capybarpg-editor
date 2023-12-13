#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QFileDialog>
#include <QPushButton>
#include <QSettings>
#include <iostream>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    connect(ui->sdkButton, &QPushButton::clicked, [this](bool checked = false) { setupSdkPath(); });
    connect(ui->compilerButton, &QPushButton::clicked, [this](bool checked = false) {
        setupQccPath();
    });

    connect(this, &QDialog::accepted, [this]() {
        QSettings settings(QApplication::applicationDirPath() + QString("/config.ini"),
                           QSettings::IniFormat);
        settings.setValue("sdk", ui->sdkPathInput->text());
        settings.setValue("qcc", ui->compilerPathInput->text());
        settings.sync();

        std::cout << "saving this shit to "
                  << (QApplication::applicationDirPath() + QString("/config.ini")).toStdString()
                  << std::endl;
    });

    QSettings settings(QApplication::applicationDirPath() + QString("/config.ini"),
                       QSettings::IniFormat);
    if (!settings.value("qcc").isNull()) {
        ui->compilerPathInput->setText(settings.value("qcc").toString());
    }

    if (!settings.value("sdk").isNull()) {
        ui->sdkPathInput->setText(settings.value("sdk").toString());
    }
}

void SettingsWindow::setupSdkPath()
{
    std::cout << "hello" << std::endl;
    QString re = QFileDialog::getOpenFileName(this, "Set CapybaRPG Engine executable", "~");

    if (re.isNull()) {
        return;
    }

    ui->sdkPathInput->setText(re);
}

void SettingsWindow::setupQccPath()
{
    QString re = QFileDialog::getOpenFileName(this, "Set QuakeC compile executable", "~");

    if (re.isNull()) {
        return;
    }

    ui->compilerPathInput->setText(re);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}
