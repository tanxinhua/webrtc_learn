#include "webrtc_learn.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    webrtc_learn w;
    w.show();
    return a.exec();
}
