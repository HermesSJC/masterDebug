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

    isAddTimeFlag = false;
    isNewLine = QString("");

    //添加波特率选项 默认115200
    ui->serialBandRateComboBox->addItem(QString("9600"), QSerialPort::Baud9600);
    ui->serialBandRateComboBox->addItem(QString("38400"), QSerialPort::Baud38400);
    ui->serialBandRateComboBox->addItem(QString("57600"), QSerialPort::Baud57600);
    ui->serialBandRateComboBox->addItem(QString("115200"), QSerialPort::Baud115200);
    ui->serialBandRateComboBox->setCurrentIndex(3);

    //添加时间选项
    ui->intervalSendTimeComboBox->addItem(QString("200"), 200);
    ui->intervalSendTimeComboBox->addItem(QString("500"), 500);
    ui->intervalSendTimeComboBox->addItem(QString("1000"), 1000);

    //隐藏断开串口按钮
    ui->serialDisconnectButton->setDisabled(true);

    //刷新一次串口
    on_serialRefreshButton_clicked();


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

        //连接信号与槽
        connect(serialPort, SIGNAL(readyRead()), this, SLOT(on_serialPort_readyRead()));
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

    //断开连接
    disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(on_serialPort_readyRead()));

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
    //清空发送内容
    ui->serialSendLineEdit->clear();

    //提示
    statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 清空发送内容成功"), 2000);
}

void MainWindow::on_serialClearReceiveSendButton_clicked()
{
    //清空一切内容
    ui->serialSendLineEdit->clear();
    ui->serialReceiveTextEdit->clear();
    ui->serialReceiveTextEdit->document()->clear();

    //提示
    statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 清空收发内容成功"), 2000);
}

void MainWindow::on_serialClearReceiveButton_clicked()
{
    //清空接受内容
    ui->serialReceiveTextEdit->clear();
    ui->serialReceiveTextEdit->document()->clear();

    //提示
    statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 清空接受内容成功"), 2000);
}

void MainWindow::on_serialSingleSendButton_clicked()
{
    //判断串口是否打开
    if(!serialPort->isOpen())
    {
        statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 串口没有打开"), 2000);
        return;
    }

    //当前内容发送出去
    serialPort->write(ui->serialSendLineEdit->text().toLatin1().append(isNewLine));

    //提示
    statusBar()->showMessage(GetCurrentTime() + QStringLiteral(" 发送成功"), 2000);
}

void MainWindow::on_isNewLineCheckBox_toggled(bool checked)
{
    isNewLine = checked ? QString("\r\n") : QString("");
}

void MainWindow::on_isAddTimeCheckBox_toggled(bool checked)
{
    isAddTimeFlag = checked;
}

void MainWindow::on_chooseReferencePathFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择参考路径"), currentPath.path());
    if(filePath.isEmpty())
    {
        return;
    }
}

void MainWindow::on_chooseAutorunPathFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择自动路径"), currentPath.path());
    if(filePath.isEmpty())
    {
        return;
    }
}

void MainWindow::on_serialPort_readyRead()
{
    //自动移动到末尾
    QTextCursor textCursor = ui->serialReceiveTextEdit->textCursor();
    textCursor.movePosition(QTextCursor::End);
    ui->serialReceiveTextEdit->setTextCursor(textCursor);

    //读取串口内容
    QByteArray readData = serialPort->readAll();
    //判断有没有时间戳
    if(isAddTimeFlag)
    {
        ui->serialReceiveTextEdit->insertPlainText(GetCurrentTime().append(" - ") + readData);
    }
    else
    {
        ui->serialReceiveTextEdit->insertPlainText(readData);
    }
}

void MainWindow::on_intervalSendTimeComboBox_currentIndexChanged(int index)
{
    nIntervalSendTime = ui->intervalSendTimeComboBox->currentData().toInt();
}


void MainWindow::timerEvent(QTimerEvent *event)
{

}
