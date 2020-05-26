#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_serialSettingsAction_triggered()
{

}

void MainWindow::on_systemSettingsAction_triggered()
{

}

void MainWindow::on_serialConnectButton_clicked()
{

}

void MainWindow::on_serialDisconnectButton_clicked()
{

}

void MainWindow::on_serialRefreshButton_clicked()
{

}
