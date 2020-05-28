#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QTimerEvent>
#include <QDir>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    QDateTime *dateTime;
    inline QString GetCurrentTime(const QString &format = "hh:mm:ss") { return dateTime->currentDateTime().toString(format); }

    QSerialPort *serialPort;

    QString isNewLine;
    bool isAddTimeFlag;

private slots:
    void on_serialSettingsAction_triggered();

    void on_systemSettingsAction_triggered();

    void on_serialConnectButton_clicked();

    void on_serialDisconnectButton_clicked();

    void on_serialRefreshButton_clicked();

    void on_serialClearSendButton_clicked();

    void on_serialClearReceiveSendButton_clicked();

    void on_serialClearReceiveButton_clicked();

    void on_serialSingleSendButton_clicked();

    void on_isNewLineCheckBox_toggled(bool checked);

    void on_isAddTimeCheckBox_toggled(bool checked);

    void on_chooseReferencePathFileButton_clicked();

    void on_chooseAutorunPathFileButton_clicked();

    void on_serialPort_readyRead();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
