#include "mainwindow.h"

#include <QTimer>
#include <QApplication>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_q = new QTextEdit;

    QFont font = m_q->font();
    font.setFamily("Courier New");
    m_q->setFont(font);
    m_q->setReadOnly(true);
    setCentralWidget(m_q);
    showMaximized();

    QTimer *timer = new QTimer;
    timer->setInterval(10);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, this, [=] {
        m_b.grow();
        QString text = m_b.map(QRect(0, 0, 200, 40));
        text.replace('.', ' ');
        m_q->setText(text);
    });
    timer->start();
}

MainWindow::~MainWindow()
{

}
