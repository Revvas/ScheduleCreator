#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::CreateGroupsBox(){
    QGroupBox *qgb = new QGroupBox();
    QGridLayout *qll = new QGridLayout();



    QLabel *ql = new QLabel("test");



    qll->addWidget(ql);
    qgb->setLayout(qll);
    ui->gridLayout->addWidget(qgb, 1,0);

}


void MainWindow::CreateTeachersBox(){}
void MainWindow::CreateAuditoriesBox(){}

void MainWindow::on_addButton_clicked()
{
    int a = ui->comboBox->currentIndex();//0-gr, 1-teach, 2-audit
    switch(a){
        case 0:{ CreateGroupsBox(); break;}
        case 1:{ CreateTeachersBox(); break;}
        case 2:{ CreateAuditoriesBox(); break;}
    }
}
