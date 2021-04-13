#include "chartanzeige.h"
#include "ui_gartenapp.h"

#include <QtCharts>
#include <QLineSeries>
#include <QChartView>


chartanzeige::chartanzeige()
{
   /* int wert;

    QSqlQuery query("SELECT Bodenfeuchtigkeit FROM feuchtigkeit");
            while (query.next())
            {
              wert = query.value(0).toInt();
              qWarning() << wert;
            }*/

    QLineSeries *series = new QLineSeries();
    series->append(0, 20);
    series->append(1, 15);
    series->append(2, 30);
    series->append(3, 28);
    series->append(4, 40);

    *series << QPointF(11,1) << QPointF(13, 3);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->setTitle("Bodenfeuchtigkeit");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //chartView->setParent(ui->horizontalFrame);

}
