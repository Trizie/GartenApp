#ifndef GARTENAPP_H
#define GARTENAPP_H

#include <QMainWindow>
//#include <QDialog>
#include <QSerialPort>
//#include <QtSql>
#include <QSqlDatabase>
//#include <QtGui>
//#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
//#include <QtSql/QSqlQuery>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Gartenapp;
}

class Gartenapp : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gartenapp(QWidget *parent = 0);
    ~Gartenapp();

private slots:
    void on_on_button_clicked();
    void on_off_button_clicked();
    void on_auto_button_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_VentilON_clicked();

    void on_pushButton_VentilOFF_clicked();

private:
    Ui::Gartenapp *ui;
    QSerialPort* arduino;
    static const quint16 arduino_mkr_vendor_id = 9025;
    static const quint16 arduino_mkr_product_id = 32846;
    QString arduino_port_name;
    bool arduino_is_available;
    QChart *createLineChart() const;

    QList<int> feuchte;
    QSqlDatabase db;
    QString test_query(QList<int> &bodenfeuchteListe);
    QChartView *chartView;



};

#endif // GARTENAPP_H
