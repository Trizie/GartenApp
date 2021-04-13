#include "listenanzeige.h"
#include <QTableView>

listenanzeige::listenanzeige()
{
    setWindowTitle("Listenanzeige");
      QSqlTableModel *modell = new QSqlTableModel(this);

    modell->setTable("feuchtigkeit");
    modell->setHeaderData(0, Qt::Horizontal, "ID");
    modell->setHeaderData(1, Qt::Horizontal, "DateTime");
    modell->setHeaderData(2, Qt::Horizontal, "Bodenfeuchte");

    modell->select();

    QTableView *ansicht = new QTableView(this);
    ansicht->setModel(modell);
    ansicht->resizeColumnsToContents();
    ansicht->resize(800,300);
}
