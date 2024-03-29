#include "auditorieswidget.h"

AuditoriesWidget::~AuditoriesWidget()
{
    for(int i=0; i<6; i++)ModelAr[i]->submitAll();
}

void AuditoriesWidget::searchClosed(QString filter){
    comboGroup->setCurrentText(filter);
    currentGroupChanged(filter);
}

void AuditoriesWidget::Reload(){
    QString text = comboGroup->currentText();

    for(int i=0; i<6; i++){
        ModelAr[i]->submitAll();//ModelAr[i]

        if(comboWeek->currentText() == "Нижняя")
           ModelAr[i]->setFilter(QString("Auditor = '%1' AND UpDown = '0' AND Day ='%2'").arg(text).arg(QsAr[i]));
        else
            ModelAr[i]->setFilter(QString("Auditor = '%1' AND UpDown = '1' AND Day ='%2'").arg(text).arg(QsAr[i]));

        ModelAr[i]->select();
        TabAr[i]->setFixedHeight(TabAr[i]->rowHeight(0)*(TabAr[i]->model()->rowCount()+1));
    }

}

void AuditoriesWidget::searchClicked(bool checked){
    QSqlTableModel *searchModel = new QSqlTableModel(0, _database);
    searchModel->setTable("Auditories");//GroupMo
    searchModel->select();

    qDebug() << "hi1";
    FormSearch *fs = new FormSearch(searchModel);
    fs->show();

    connect(fs, SIGNAL(closed(QString)), this, SLOT(searchClosed(QString)));
}

AuditoriesWidget::AuditoriesWidget(QSqlDatabase database) : _database(database)
{

//////////////////////////////////

    auditoriesWidget = new QGroupBox("Аудитории");
    QGridLayout *qll = new QGridLayout();

    //Search button
    QPushButton *search_butt = new QPushButton("Поиск");

    connect(search_butt, SIGNAL(clicked(bool)), this, SLOT(searchClicked(bool)));

    //Group name
    QLabel *ql = new QLabel("Аудитория");
    comboGroup = new QComboBox();


    QSqlTableModel *GModel = new QSqlTableModel(0, _database);
    GModel->setTable("Auditories");//GroupMo
    GModel->select();
    comboGroup->setModel(GModel);
    comboGroup->setModelColumn(1);

    connect(comboGroup, SIGNAL(currentTextChanged(QString)), this, SLOT(currentGroupChanged(QString)));

    //Week up/down
    QLabel *ql2 = new QLabel("Неделя");
    comboWeek = new QComboBox();
    comboWeek->addItem("Верхняя"); comboWeek->addItem("Нижняя");
    connect(comboWeek, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentWeakChanged(QString)));


    qll->addWidget(search_butt, 0, 0, 1, 2);
    qll->addWidget(ql, 1, 0);
    qll->addWidget(comboGroup, 1, 1);
    qll->addWidget(ql2, 2, 0);
    qll->addWidget(comboWeek, 2, 1);


    AddTable(qll, _database);

    currentGroupChanged(comboGroup->currentText());


    auditoriesWidget->setLayout(qll);
    auditoriesWidget->resize(700, 500);

    for(int i=0; i<6; i++)
    TabAr[i]->setFixedHeight(TabAr[i]->rowHeight(0)*(TabAr[i]->model()->rowCount()+1));

}


void AuditoriesWidget::AddTable(QGridLayout *qll, QSqlDatabase _database){


    QLabel *LabAr[6];


    auto glambda = [](GroupTableDrop *tab, QSqlTableModel *mod, QString str) {
        mod->setTable("GroupSchedule");
        mod->setEditStrategy(QSqlTableModel::OnFieldChange);
        mod->setFilter(QString("Day ='%1'").arg(str));
        mod->select();
        tab->setModel(mod);
    };

    for(int i=0; i<6; i++){
        TabAr[i] = new GroupTableDrop(comboGroup, comboWeek, QsAr[i],_database, "Schedule");
        ModelAr[i] = new QSqlTableModel(0, _database);
        glambda(TabAr[i], ModelAr[i], QsAr[i]);
        LabAr[i] = new QLabel(QsAr[i]);

        ModelAr[i]->setHeaderData(0, Qt::Horizontal, tr("Пара"));
        ModelAr[i]->setHeaderData(2, Qt::Horizontal, tr("Предмет"));
        ModelAr[i]->setHeaderData(3, Qt::Horizontal, tr("Тип пары"));
        ModelAr[i]->setHeaderData(4, Qt::Horizontal, tr("Преподаватель"));
        //ModelAr[i]->setHeaderData(5, Qt::Horizontal, tr("Аудитория"));
        ModelAr[i]->setHeaderData(6, Qt::Horizontal, tr("Группа"));
        ModelAr[i]->setHeaderData(9, Qt::Horizontal, tr("Кафедра"));
        ModelAr[i]->setHeaderData(10, Qt::Horizontal, tr("Академическая специальность"));


        TabAr[i]->horizontalHeader()->setStretchLastSection(1);
        TabAr[i]->setSortingEnabled(1);
        TabAr[i]->resizeColumnsToContents();


        TabAr[i]->setColumnHidden(1, true);
        //TabAr[i]->setColumnHidden(4, true);
        TabAr[i]->setColumnHidden(5, true);
        //TabAr[i]->setColumnHidden(6, true);
        TabAr[i]->setColumnHidden(7, true);
        TabAr[i]->setColumnHidden(8, true);


        TabAr[i]->setEditTriggers(QAbstractItemView::NoEditTriggers);//not editable table

        qll->addWidget(LabAr[i], qll->rowCount()+1, 0);
        qll->addWidget(TabAr[i], qll->rowCount()+1, 0, 1, 2);

      }

}

void AuditoriesWidget::on_addButton_click(){
    for(int i=0; i<6; i++){
       if(addButton[i] == QObject::sender()){

          QModelIndex ind;
          ModelAr[i]->insertRow(ModelAr[i]->rowCount());

          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 0);
          ModelAr[i]->setData(ind, ModelAr[i]->rowCount());
          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 2);
          ModelAr[i]->setData(ind, "");
          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 3);
          ModelAr[i]->setData(ind, "");
          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 4);
          ModelAr[i]->setData(ind, "");
          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 6);//-
          ModelAr[i]->setData(ind, "");
          //8 = id
          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 9);
          ModelAr[i]->setData(ind, "");
          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 10);
          ModelAr[i]->setData(ind, "");

          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 1);
          if(comboWeek->currentText()=="Верхняя")
              ModelAr[i]->setData(ind, "1");
          else
              ModelAr[i]->setData(ind, "0");

          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 5);
          ModelAr[i]->setData(ind, comboGroup->currentText());

          ind = ModelAr[i]->index(ModelAr[i]->rowCount()-1, 7);
          ModelAr[i]->setData(ind, QsAr[i]);

           currentGroupChanged(comboGroup->currentText());

       }
    }
}


void AuditoriesWidget::currentGroupChanged(const QString &text){
    TableChanged();
    for(int i=0; i<6; i++){
        ModelAr[i]->submitAll();

        if(comboWeek->currentText() == "Нижняя")
           ModelAr[i]->setFilter(QString("Auditor = '%1' AND UpDown = '0' AND Day ='%2'").arg(text).arg(QsAr[i]));
        else
            ModelAr[i]->setFilter(QString("Auditor = '%1' AND UpDown = '1' AND Day ='%2'").arg(text).arg(QsAr[i]));

        ModelAr[i]->select();
    }
}

void AuditoriesWidget::currentWeakChanged(const QString &text){
   currentGroupChanged(comboGroup->currentText());
}

