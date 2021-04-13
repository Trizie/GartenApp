#include "gartenapp.h"
#include "ui_gartenapp.h"
#include "listenanzeige.h"

#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>

#include <QInputDialog>
#include <QSql>
#include <QtSql>
//#include "chartanzeige.h"

#include <QtCharts>
#include <QLineSeries>
#include <QChartView>



Gartenapp::Gartenapp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gartenapp)
{
    ui->setupUi(this);

    // Arduino über Serial Port verbinden

    /*arduino_is_available = false;
    arduino_port_name = " ";
    arduino = new QSerialPort;

    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
      qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
      if(serialPortInfo.hasVendorIdentifier()){
          qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
            }
       qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
       if(serialPortInfo.hasProductIdentifier()){
         qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
            }
        }

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
      if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
        if(serialPortInfo.vendorIdentifier() == arduino_mkr_vendor_id){
          if(serialPortInfo.productIdentifier() == arduino_mkr_product_id){
             arduino_port_name = serialPortInfo.portName();
             arduino_is_available = true;}

               }
           }
    }

    if(arduino_is_available){
      // open and configure the serialport
      arduino->setPortName(arduino_port_name);
      arduino->open(QSerialPort::WriteOnly);
      arduino->setBaudRate(QSerialPort::Baud9600);
      arduino->setDataBits(QSerialPort::Data8);
      arduino->setParity(QSerialPort::NoParity);
      arduino->setStopBits(QSerialPort::OneStop);
      arduino->setFlowControl(QSerialPort::NoFlowControl);
      QMessageBox::information(this, "connection", "Arduino is available");
      }
    else{
      // give error message if not available
      QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
        }*/


    //mysql Datenbank verbinden

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("############"); //127.0.0.1
    db.setUserName("##########");
    db.setPassword("#####");
    db.setDatabaseName("gartentest");


    //QList für Feuchtewerte erstellen
      test_query(feuchte);

    //Chartdarstellung
    chartView = new QChartView(createLineChart());
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);



}

Gartenapp::~Gartenapp()
{
    /*if(arduino->isOpen())
    {
           arduino->close();
       }*/
    delete ui;
}



void Gartenapp::on_on_button_clicked()
{
    //Serielle Steuerung
      /*arduino->write("1");
        else qDebug() << "Couldn't write to serial!";*/

    //Steuerung über DB
    if(db.open()){
        qDebug() << "Opened";

        QSqlQuery query;
        query.exec("INSERT INTO gartentest.steuerung (Pumpenmodus) VALUE ('1')");
    }
    else qDebug() << "no input into database for ON";

}


void Gartenapp::on_off_button_clicked()
{
    //Serielle Steuerung
    /*if(arduino->isWritable())
        arduino->write("2");
    else qDebug() << "Couldn't write to serial!";*/

    //Steuerung über DB
      if(db.open()){
        qDebug() << "Opened";

    QSqlQuery query;
    query.exec("INSERT INTO gartentest.steuerung (Pumpenmodus) VALUE ('2')");
    }
    else qDebug() << "no input into database for OFF";
}

void Gartenapp::on_auto_button_clicked()
{
    //Steuerung über DB
    if(db.open()){
        qDebug() << "Opened";

        QSqlQuery query;
        query.exec("INSERT INTO gartentest.steuerung (Pumpenmodus) VALUE ('3')");
    }
    else qDebug() << "no input into database for AUTO";
}


void Gartenapp::on_pushButton_clicked()
{
    //Anzeige der Bodenfeuchte-Werte über Liste

    if(db.open()) {
        QMessageBox::information(this, "Connection", "Database connected");

        listenanzeige *anzeige = new listenanzeige();
        anzeige->show();
    }
    else {
        QMessageBox::information(this, "Connection", "Database not connected");
    }


}

void Gartenapp::on_pushButton_2_clicked()
{
    //Chart aktualisieren
    test_query(feuchte);
    createLineChart();
    chartView = new QChartView(createLineChart());

    //foreach(int element, feuchte)
    //ui->listWidget->addItem(QString::number(element));

}

QChart *Gartenapp::createLineChart() const
{

    int nameIndex = 0;

    QLineSeries *series = new QLineSeries();

    foreach (int data, feuchte){
      series->append(nameIndex, data);
      nameIndex++;
             }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Bodenfeuchte");

    QPen pen(QRgb(0x000000));
    pen.setWidth(3);
    series->setPen(pen);

    chart->setAnimationOptions(QChart::AllAnimations);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    //axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Datum");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    return chart;
}

QString Gartenapp::test_query(QList<int>& bodenfeuchteListe)
{
    QString result = "false";
    //bodenfeuchteListe.clear();


    if(db.open()) {

        QSqlQuery query;
        query.exec("SELECT Bodenfeuchtigkeit FROM feuchtigkeit;");
        int fieldNo = query.record().indexOf("Bodenfeuchtigkeit");

        while (query.next()){
            int bodenfeuchte = query.value(fieldNo).toInt();
            bodenfeuchteListe.append(bodenfeuchte);

            result = "true";

        }}
    else
        qDebug() << "Database Not Connected!";



    return result;
}


void Gartenapp::on_pushButton_VentilON_clicked()
{
    //Steuerung über DB
    if(db.open()){
        qDebug() << "Opened";

        QSqlQuery query;
        query.exec("INSERT INTO gartentest.steuerung (Ventilmodus) VALUE ('1')");
    }
    else qDebug() << "no input into database for ON";
}


void Gartenapp::on_pushButton_VentilOFF_clicked()
{
    //Steuerung über DB
    if(db.open()){
        qDebug() << "Opened";

        QSqlQuery query;
        query.exec("INSERT INTO gartentest.steuerung (Ventilmodus) VALUE ('2')");
    }
    else qDebug() << "no input into database for ON";
}
