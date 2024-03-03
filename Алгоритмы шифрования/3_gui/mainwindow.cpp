#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui -> pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
    QString tmp;

    ui -> lineEdit -> setText(tmp.sprintf("%u",4050550955));
    ui -> lineEdit_2 -> setText(tmp.sprintf("%u",1605609391));
    ui -> lineEdit_3 -> setText(tmp.sprintf("%u",4072650953));
    ui -> lineEdit_4 -> setText(tmp.sprintf("%u",2152509687));

    ui -> lineEdit_5 -> setText(tmp.sprintf("%u",61904));
    ui -> lineEdit_6 -> setText(tmp.sprintf("%u",17310));
    ui -> lineEdit_7 -> setText(tmp.sprintf("%u",49332));
    ui -> lineEdit_8 -> setText(tmp.sprintf("%u",16838));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    unsigned long key1, key2, key3, key4;
    unsigned short v1,v2,v3,v4;
    QString tmp;

    tmp = ui -> lineEdit -> text();
    key1 = tmp.toULong();

    tmp = ui -> lineEdit_2 -> text();
    key2 = tmp.toULong();

    tmp = ui -> lineEdit_3 -> text();
    key3 = tmp.toULong();

    tmp = ui -> lineEdit_4 -> text();
    key4 = tmp.toULong();


    tmp = ui -> lineEdit_5 -> text();
    v1 = tmp.toUShort();

    tmp = ui -> lineEdit_6 -> text();
    v2 = tmp.toUShort();

    tmp = ui -> lineEdit_7 -> text();
    v3 = tmp.toUShort();

    tmp = ui -> lineEdit_8 -> text();
    v4 = tmp.toUShort();


    idea.setKey(key1,key2,key3,key4);
    idea.setValue(v1,v2,v3,v4);
    idea.encrypt();

    v1 = idea.getValue1();
    v2 = idea.getValue2();
    v3 = idea.getValue3();
    v4 = idea.getValue4();


    ui -> lineEdit_9 -> setText(tmp.sprintf("%u",v1));
    ui -> lineEdit_10 -> setText(tmp.sprintf("%u",v2));
    ui -> lineEdit_11 -> setText(tmp.sprintf("%u",v3));
    ui -> lineEdit_12 -> setText(tmp.sprintf("%u",v4));
}
