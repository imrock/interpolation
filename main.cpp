#include "qinterp.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QInterp w;
    w.show();



    return app.exec();
}
