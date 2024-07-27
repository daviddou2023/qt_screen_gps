#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSerialPort>
#include <QStringList>
#include <QTimer>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkProxyFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , myCom1(nullptr)
    , isComOpen1(false)
{
    ui->setupUi(this);

    // 关闭系统代理配置
    QNetworkProxyFactory::setUseSystemConfiguration(false);

    // 创建一个定时器，用于定期读取串口数据
    readTimer = new QTimer(this);
    connect(readTimer, &QTimer::timeout, this, &MainWindow::readMyCom1);

    // 设置UI布局
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 创建地图显示区域
    mapView = new QWebEngineView(this);
    QString mapHtml = R"(
    <div id="container" style="width:100%;height:100%;"></div>
    <div class="input-card">
        <h4>左击获取经纬度：</h4>
        <div class="input-item">
          <input type="text" readonly="true" id="lnglat">
        </div>
    </div>
    <script type="text/javascript">
      window._AMapSecurityConfig = {
        securityJsCode: "c9e3585d7ef259a3b71826aafccafeec",
      };
    </script>
    <script
      type="text/javascript"
      src="https://webapi.amap.com/maps?v=2.0&key=dca75fbd0390e9266a7f39814e1197c2"
    ></script>
    <script type="text/javascript">
      var map = new AMap.Map('container', {
        rotateEnable: true,
        resizeEnable: true,
        pitchEnable: true,
        zoom: 17,
        pitch: 50,
        rotation: -15,
        viewMode: '3D',
        zooms: [2, 20],
        center: [104.10113, 30.675639],
        layers: [
            new AMap.TileLayer()
        ]
      });

      // 添加 3D 罗盘控制
      AMap.plugin(['AMap.ControlBar'], function() {
        map.addControl(new AMap.ControlBar());
      });

      // 添加点击事件获取经纬度
      map.on('click', function(e) {
        document.getElementById("lnglat").value = e.lnglat.getLng() + ',' + e.lnglat.getLat();
      });

      // 添加卫星图层
      var satelliteLayer = new AMap.TileLayer.Satellite();
      satelliteLayer.setMap(map);

      // 添加实时路况图层
      var trafficLayer = new AMap.TileLayer.Traffic({zIndex: 10});
      trafficLayer.setMap(map);

      // 添加楼块图层
      var buildingsLayer = new AMap.Buildings({
        zooms: [16, 18],
        zIndex: 10,
        heightFactor: 2
      });
      map.add(buildingsLayer);

      // 添加卫星路网图层
      map.addLayer(new AMap.TileLayer.Satellite());
      map.addLayer(new AMap.TileLayer.RoadNet());

      // 添加楼块图层
      map.setMapStyle('amap://styles/light');
      map.addLayer(buildingsLayer);

      var marker = null;
      var infoWindow = null;

      function updateMap(lat, lon) {
        var position = new AMap.LngLat(lon, lat);
        map.setCenter(position);
        map.setZoom(17);

        if (marker) {
          marker.setPosition(position);
        } else {
          marker = new AMap.Marker({
            position: position,
            map: map,
            icon: new AMap.Icon({
              image: 'http://webapi.amap.com/theme/v1.3/markers/n/mark_b.png', // 使用红色标记图标
              size: new AMap.Size(24, 36) // 设置图标大小
            })
          });
        }

        if (infoWindow) {
          infoWindow.setPosition(position);
        } else {
          infoWindow = new AMap.InfoWindow({
            content: "<div style='color: red;'>塔吊1</div>", // 红色文字
            position: position
          });
          infoWindow.open(map, marker.getPosition());
        }
      }
    </script>
    )";
    mapView->setHtml(mapHtml);
    mainLayout->addWidget(mapView, 1);

    // 创建右侧控制区域
    QVBoxLayout *controlLayout = new QVBoxLayout();

    // 添加打开串口按钮
    QPushButton *openButton = new QPushButton("打开串口", this);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::on_openMyCom1_clicked);
    controlLayout->addWidget(openButton);

    // 添加关闭串口按钮
    QPushButton *closeButton = new QPushButton("关闭串口", this);
    closeButton->setEnabled(false); // 默认关闭串口按钮不可用
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::on_closeMyCom1_clicked);
    controlLayout->addWidget(closeButton);

    // 添加经纬度显示标签
    ui->latitudeLabel = new QLabel("纬度: ", this);
    ui->longitudeLabel = new QLabel("经度: ", this);
    controlLayout->addWidget(ui->latitudeLabel);
    controlLayout->addWidget(ui->longitudeLabel);

    mainLayout->addLayout(controlLayout);

    ui->openMyCom1 = openButton;
    ui->closeMyCom1 = closeButton;
}

MainWindow::~MainWindow()
{
    if (myCom1) {
        if (myCom1->isOpen()) {
            myCom1->close();
        }
        delete myCom1;
    }
    delete ui;
}

// 打开串口并读取北斗数据
void MainWindow::on_openMyCom1_clicked()
{
    ui->openMyCom1->setEnabled(false);
    ui->closeMyCom1->setEnabled(true);

    Com1Init(); // 初始化串口
    if (isComOpen1) {
        sendInitializationCommand(); // 发送初始化指令
        readTimer->start(1000); // 启动定时器，每秒读取一次串口数据
    } else {
        ui->latitudeLabel->setText("串口打开失败：" + myCom1->errorString());
    }
}

// 关闭串口
void MainWindow::on_closeMyCom1_clicked()
{
    if (myCom1) {
        if (myCom1->isOpen()) {
            myCom1->close();
        }
        delete myCom1;
        myCom1 = nullptr;
        isComOpen1 = false;
        readTimer->stop();
        clear_gps();
        ui->openMyCom1->setEnabled(true);
        ui->closeMyCom1->setEnabled(false);
    }
}

// 初始化串口
void MainWindow::Com1Init()
{
    if (myCom1) {
        delete myCom1;
    }

    QString portName1 = "COM12"; // 获取串口名
    myCom1 = new QSerialPort(portName1);

    if (!myCom1->open(QIODevice::ReadWrite)) {
        qDebug() << "串口打开失败：" << myCom1->errorString();
        ui->latitudeLabel->setText("串口打开失败：" + myCom1->errorString());
        delete myCom1;
        myCom1 = nullptr;
        isComOpen1 = false;
        return;
    }

    isComOpen1 = true;
    myCom1->setBaudRate(QSerialPort::Baud9600);
    myCom1->setDataBits(QSerialPort::Data8);
    myCom1->setParity(QSerialPort::NoParity);
    myCom1->setStopBits(QSerialPort::OneStop);
    myCom1->setFlowControl(QSerialPort::NoFlowControl);
    qDebug() << "串口打开成功";
}

// 发送初始化指令
void MainWindow::sendInitializationCommand()
{
    if (myCom1 && myCom1->isOpen()) {
        QString command = "$PCAS03,0,0,0,0,1,0,0,0,0,0,,,0,0*03\r\n";
        myCom1->write(command.toUtf8());
    }
}

// 读取串口数据
void MainWindow::readMyCom1()
{
    if (myCom1 && myCom1->isOpen()) {
        QByteArray temp1 = myCom1->readAll();
        if (!temp1.isEmpty()) {
            QString dataStr = QString::fromStdString(temp1.toStdString());
            buffer += dataStr;
            processBuffer();
        }
    } else {
        qDebug() << "尝试读取数据时，串口未打开";
    }
}

// 处理缓冲区中的数据
void MainWindow::processBuffer()
{
    int startIndex = buffer.indexOf("$GNRMC");
    if (startIndex == -1) {
        return;
    }

    int endIndex = buffer.indexOf("\r\n", startIndex);
    if (endIndex == -1) {
        return;
    }

    QString gpsData = buffer.mid(startIndex, endIndex - startIndex);
    buffer.remove(0, endIndex + 2);
    parseGPSData(gpsData);
}

// 解析GPS数据并显示经纬度
void MainWindow::parseGPSData(const QString &data)
{
    if (data.startsWith("$GNRMC")) {
        QStringList dataList = data.split(",");
        if (dataList.size() > 6) {
            QString latitude = dataList[3];
            QString latitudeDirection = dataList[4];
            QString longitude = dataList[5];
            QString longitudeDirection = dataList[6];

            double lat = latitude.mid(0, 2).toDouble() + (latitude.mid(2).toDouble() / 60.0);
            if (latitudeDirection == "S") lat = -lat;

            double lon = longitude.mid(0, 3).toDouble() + (longitude.mid(3).toDouble() / 60.0);
            if (longitudeDirection == "W") lon = -lon;

            qDebug() << "纬度: " << lat << ", 经度: " << lon;

            ui->latitudeLabel->setText("纬度: " + QString::number(lat, 'f', 6));
            ui->longitudeLabel->setText("经度: " + QString::number(lon, 'f', 6));

            QString jsCode = QString("updateMap(%1, %2);").arg(lat).arg(lon);
            mapView->page()->runJavaScript(jsCode);
        }
    }
}

// 清除GPS显示数据
void MainWindow::clear_gps()
{
    ui->latitudeLabel->clear();
    ui->longitudeLabel->clear();
}
