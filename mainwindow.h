#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

#include "bonsai.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QTextEdit *m_q = nullptr;
    Branch m_b = Branch::makeRoot(50, 0);
};

#endif // MAINWINDOW_H
