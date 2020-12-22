#include "mainwindow.h"
#include "ui_mainwindow.h"


//#include "createboxes.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    QFileDialog *file = new QFileDialog();
    QMessageBox msgBox;
    msgBox.setWindowTitle("Внимание");
    msgBox.setText("Выберите базу данных");
    msgBox.setModal(1);
    msgBox.exec();



    _database = QSqlDatabase::addDatabase("QSQLITE");

    _database.setDatabaseName(file->getOpenFileName());
    //if(!subjectBD.open()) this->close();
    qDebug() << "Open?" << _database.open();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateBox(QGroupBox *qgb){

    QDockWidget *doc = new QDockWidget("Document");

    doc->setWidget(qgb);
   // doc->setWindowFlag(Qt::SubWindow);
    QMdiSubWindow *sw = ui->mdiArea->addSubWindow(doc);
    sw->resize(qgb->size());
    sw->show();

    //p_doc.append(doc);

    //ui->gridL->addWidget(qgb, 0, ui->gridL->columnCount()+1);


}

void MainWindow::CreateGroupsScheduleBox(){

    GroupsWidget *gw = new GroupsWidget(_database);
    //CreateBox(gw->groupWidget);

    p_gw.append(gw);
    //p_gw[0]->Reload();

    connect(gw, SIGNAL(TableChanged()), this, SLOT(on_reloadTable()));

    QScrollArea *qsoo = new QScrollArea();
    //QDockWidget *doc = new QDockWidget("Document");
    //doc->setWidget(qsoo);
    qsoo->setWidget(gw->groupWidget);
    qsoo->setWidgetResizable(true);
    //doc->setWindowFlag(Qt::SubWindow);
    QMdiSubWindow *sw = ui->mdiArea->addSubWindow(qsoo);
    sw->resize(gw->groupWidget->size().width(), ui->mdiArea->size().height()-20);
    sw->show();

    p_doc_gw.append(sw);
}


void MainWindow::CreateTeachersScheduleBox(){
    TeachersWidget *tw = new TeachersWidget(_database);
    //CreateBox(tw->teacherWidget);
    p_tw.append(tw);

    connect(tw, SIGNAL(TableChanged()), this, SLOT(on_reloadTable()));


    QScrollArea *qsoo = new QScrollArea();
    //QDockWidget *doc = new QDockWidget("Document");
    //doc->setWidget(qsoo);
    qsoo->setWidget(tw->teacherWidget);
    qsoo->setWidgetResizable(true);
   // doc->setWindowFlag(Qt::SubWindow);
    QMdiSubWindow *sw = ui->mdiArea->addSubWindow(qsoo);
    sw->resize(tw->teacherWidget->size());
    sw->show();

    p_doc_tw.append(sw);

}
void MainWindow::CreateAuditoriesScheduleBox(){
    AuditoriesWidget *aw = new AuditoriesWidget(_database);
    //CreateBox(aw->auditoriesWidget);
    p_aw.append(aw);

    connect(aw, SIGNAL(TableChanged()), this, SLOT(on_reloadTable()));

    QScrollArea *qsoo = new QScrollArea();
    //QDockWidget *doc = new QDockWidget("Document");
    //doc->setWidget(qsoo);
    qsoo->setWidget(aw->auditoriesWidget);
    qsoo->setWidgetResizable(true);

   // doc->setWindowFlag(Qt::SubWindow);
    QMdiSubWindow *sw = ui->mdiArea->addSubWindow(qsoo);
    sw->resize(aw->auditoriesWidget->size());
    sw->show();

    p_doc_aw.append(sw);
    //https://www.youtube.com/watch?v=Aiay8I5IPB8&list=PLn4GvABOzCQursVQ7qMU9CkNaKz4RgrVM
}

void MainWindow::on_reloadTable(){
    qDebug() << "RELOAD";

    for(int i = 0; i<p_gw.length();i++)
        if (p_doc_gw[i]->isVisible())
            p_gw[i]->Reload();

    for(int i = 0; i<p_tw.length();i++)
        if (p_doc_tw[i]->isVisible())
            p_tw[i]->Reload();

    for(int i = 0; i<p_aw.length();i++)
        if (p_doc_aw[i]->isVisible())
            p_aw[i]->Reload();
}

void MainWindow::CreateGroupsBox(){
    //Table
    QSqlTableModel *GroupModel = new QSqlTableModel(0, _database);
    GroupModel->setTable("Groupes");
    GroupModel->select();
    //GroupModel->removeColumn(0); // don't show the ID
    GroupModel->setHeaderData(1, Qt::Horizontal, tr("Группа"));
    GroupModel->setHeaderData(2, Qt::Horizontal, tr("Кафедра"));

    InfoBox *ib = new InfoBox("Группы", GroupModel, "Groupe");

    CreateBox(ib->InfoWidget);
}

void MainWindow::CreateTeachersBox(){

    //Table
    QSqlTableModel *TeacherModel = new QSqlTableModel(0, _database);
    TeacherModel->setTable("Teachers");
    TeacherModel->select();
    //TeacherModel->removeColumn(0); // don't show the ID
    TeacherModel->setHeaderData(1, Qt::Horizontal, tr("Преподаватель"));
    TeacherModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
    TeacherModel->setHeaderData(3, Qt::Horizontal, tr("Кафедра"));

    InfoBox *ib = new InfoBox("Преподаватели", TeacherModel, "Name");

    CreateBox(ib->InfoWidget);
}

void MainWindow::CreateAuditoriesBox(){

    //Table
    QSqlTableModel *AuditorModel= new QSqlTableModel(0, _database);
    AuditorModel->setTable("Auditories");
    AuditorModel->select();
    //AuditorModel->removeColumn(0); // don't show the ID
    AuditorModel->setHeaderData(1, Qt::Horizontal, tr("Аудитория"));
    AuditorModel->setHeaderData(2, Qt::Horizontal, tr("Тип пары"));


    InfoBox *ib = new InfoBox("Аудитории", AuditorModel, "Number");

    CreateBox(ib->InfoWidget);

}


void MainWindow::CreateSubjectsBox(){

    //Table
    QSqlTableModel *SubjectModel = new QSqlTableModel(0, _database);
    SubjectModel->setTable("Subjects");
    SubjectModel->select();
    //SubjectModel->removeColumn(0); // don't show the ID
    SubjectModel->setHeaderData(1, Qt::Horizontal, tr("Предмет"));
    SubjectModel->setHeaderData(2, Qt::Horizontal, tr("Тип пары"));
    SubjectModel->setHeaderData(3, Qt::Horizontal, tr("Кафедра"));
    SubjectModel->setHeaderData(4, Qt::Horizontal, tr("Академическая специальность"));


    InfoBox *ib = new InfoBox("Предметы", SubjectModel, "Subject");

    CreateBox(ib->InfoWidget);

}

void MainWindow::ChoiceBox(int a){
    switch(a){
        case 0:{ CreateGroupsScheduleBox();     break;}
        case 1:{ CreateTeachersScheduleBox();       break;}
        case 2:{ CreateAuditoriesScheduleBox();       break;}
        case 3:{ CreateGroupsBox();      break;}
        case 4:{ CreateTeachersBox();   break;}
        case 5:{ CreateAuditoriesBox();   break;}
        case 6:{ CreateSubjectsBox();   break;}
    }
}

void MainWindow::on_addButton_clicked()
{
    int a = ui->comboBox->currentIndex();//0-gr, 1-teach, 2-audit, 3-subj
    ChoiceBox(a);
}

void MainWindow::on_comboBox_activated(int index)
{
    ChoiceBox(index);
}

void MainWindow::on_updateButton_clicked()
{
    on_reloadTable();
}
