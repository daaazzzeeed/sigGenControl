#include "siggencontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sigGenControl w;
    w.show();
    w.setWindowTitle("Signal Generator");
    //w.setFixedSize(w.width()*0.7,w.height()*0.7);

    return a.exec();
}
