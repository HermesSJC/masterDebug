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

    //添加波特率选项 默认115200
    ui->serialBandRateComboBox->addItem(QString("9600"), QSerialPort::Baud9600);
    ui->serialBandRateComboBox->addItem(QString("38400"), QSerialPort::Baud38400);
    ui->serialBandRateComboBox->addItem(QString("57600"), QSerialPort::Baud57600);
    ui->serialBandRateComboBox->addItem(QString("115200"), QSerialPort::Baud115200);
    ui->serialBandRateComboBox->setCurrentIndex(3);

    //隐藏断开串口按钮
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
        statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 没有发现串口"), 2000);
        return;
    }

    //设置串口参数
    serialPort->setPortName(ui->serialPortComboBox->currentText());
    serialPort->setBaudRate(static_cast<QSerialPort::BaudRate>(ui->serialBandRateComboBox->currentData().toInt()));
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);

    //尝试打开串口
    if(serialPort->open(QIODevice::ReadWrite))
    {
        //禁用一些功能 防止乱更改参数
        ui->serialPortComboBox->setDisabled(true);
        ui->serialBandRateComboBox->setDisabled(true);
        ui->serialRefreshButton->setDisabled(true);
        ui->serialConnectButton->setDisabled(true);
        ui->serialDisconnectButton->setEnabled(true);
        //提示
        statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 串口打开成功"), 2000);
    }
    else
    {
        //提示
        statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 串口打开失败"), 2000);
    }
}

void MainWindow::on_serialDisconnectButton_clicked()
{
    //关闭
    serialPort->close();

    //检查是否关闭
    if(serialPort->isOpen())
    {
        statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 串口关闭失败"), 2000);
        return;
    }

    //复位按钮
    ui->serialPortComboBox->setEnabled(true);
    ui->serialBandRateComboBox->setEnabled(true);
    ui->serialRefreshButton->setEnabled(true);
    ui->serialConnectButton->setEnabled(true);
    ui->serialDisconnectButton->setDisabled(true);

    //提示
    statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 串口关闭成功"), 2000);
}

void MainWindow::on_serialRefreshButton_clicked()
{
    //清除原有内容
    ui->serialPortComboBox->clear();

    //扫描现有串口 判断是否非空
    const auto infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 没有发现串口"), 2000);
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
