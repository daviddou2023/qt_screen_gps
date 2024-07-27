#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QWebEngineView>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openMyCom1_clicked(); // 打开串口按钮的槽函数
    void on_closeMyCom1_clicked(); // 关闭串口按钮的槽函数
    void readMyCom1(); // 读取串口数据的槽函数

private:
    void Com1Init(); // 初始化串口
    void sendInitializationCommand(); // 发送初始化指令
    void clear_gps(); // 清除GPS显示数据
    void parseGPSData(const QString &data); // 解析GPS数据
    void processBuffer(); // 处理缓冲区数据

    Ui::MainWindow *ui;
    QSerialPort *myCom1; // 串口对象
    bool isComOpen1; // 串口是否打开的标志
    QTimer *readTimer; // 定时器对象
    QWebEngineView *mapView; // 地图视图对象
    QString buffer; // 缓冲区用于处理串口数据
};

#endif // MAINWINDOW_H
