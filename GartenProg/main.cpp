#include "gartenapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gartenapp w;
    w.show();

    return a.exec();
}
