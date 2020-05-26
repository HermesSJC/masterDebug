#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化一些变量
    dateTime = new QDateTime();
    serialPort = new QSerialPort(this);

    //隐藏断开串口
    ui->serialDisconnectButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete dateTime;

    delete ui;
}


void MainWindow::on_serialSettingsAction_triggered()
{
    //如果是隐藏的 就显示
    if(ui->serialSettingsDockWidget->isHidden())
    {
        ui->serialSettingsDockWidget->show();
    }


}

void MainWindow::on_systemSettingsAction_triggered()
{
    //如果是隐藏的 就显示
    if(ui->systemSettingsDockWidget->isHidden())
    {
        ui->systemSettingsDockWidget->show();
    }
}

void MainWindow::on_serialConnectButton_clicked()
{
    //判断是否有串口
    if(ui->serialPortComboBox->count() == 0)
    {
        statusBar()->showMessage(getCurrentTime() + QStringLiteral(" 没有发现串口"), 2000);
        return;
    }
}

void MainWindow::on_serialDisconnectButton_clicked()
{

}

void MainWindow::on_serialRefreshButton_clicked()
{
    //清除原有内容
    ui->serialPortComboBox->clear();

    //扫描现有串口 判断是否非空
    const auto infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        statusBar()->showMessage(getCurrentTime() + QStringLiteral(" 没有发现串口"), 2000);
        return;
    }

    //非空就添加串口
    for(const QSerialPortInfo &info : infos)
    {
        ui->serialPortComboBox->addItem(info.portName());
    }
}

void MainWindow::on_serialClearSendButton_clicked()
{

}

void MainWindow::on_serialClearReceiveSendButton_clicked()
{

}

void MainWindow::on_serialClearReceiveButton_clicked()
{

}

void MainWindow::on_serialSingleSendButton_clicked()
{

}

void MainWindow::on_isNewLineCheckBox_toggled(bool checked)
{

}

void MainWindow::on_isAddTimeCheckBox_toggled(bool checked)
{

}

void MainWindow::on_chooseReferencePathFileButton_clicked()
{

}

void MainWindow::on_chooseAutorunPathFileButton_clicked()
{

}
