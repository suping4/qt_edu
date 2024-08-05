#include "widget.h"

#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPushButton quit("Quit", 0);
    quit.resize(75,35);
    quit.show();
    //Widget w;
    //w.show();
    return a.exec();
}
