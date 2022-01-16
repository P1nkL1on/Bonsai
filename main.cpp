#include <QApplication>
#include <QTest>

#include "mainwindow.h"
#include "asciiarrows_tests.h"

int main(int argc, char *argv[])
{
    AsciiArrows::Tests t;
    const int testErr = QTest::qExec(&t, argc, argv);
    if (testErr)
        return testErr;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
