#include "grouptabledrop.h"

GroupTableDrop::GroupTableDrop(QComboBox *comboGroup_p, QComboBox *comboWeek_p,
                               QString day_p, QSqlDatabase db, QString _TableName)
    :comboGroup(comboGroup_p),comboWeek(comboWeek_p), day(day_p), database(db), TableName(_TableName)
{
    this->setDragEnabled(false);
    this->setDragDropMode(QAbstractItemView::DropOnly);
    // this->setSortingEnabled(false);
    //this.
    //this->model()->sort(0,Qt::AscendingOrder);

    this->sortByColumn(0, Qt::AscendingOrder);
    //this->horizontalHeader()
    // this->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);


}

void GroupTableDrop::dragMoveEvent(QDragMoveEvent *event)
{
    //event->accept();
    event->acceptProposedAction();
}

Qt::DropActions GroupTableDrop::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

void GroupTableDrop::dragEnterEvent(QDragEnterEvent *event)
{

    event->acceptProposedAction();
}

void GroupTableDrop::dropEvent(QDropEvent *event)
{
    QString Delement = event->mimeData()->text();
    QModelIndex ind = this->indexAt(event->pos());

    //

    //CheckError(event);

    QString drag_col_ = "";
    QString curr_col = this->model()->headerData(ind.column(), Qt::Horizontal).toString();
    if(this->model()->index(-1,-1)!=ind){
        int str_index =Delement.indexOf('#');
        QString drag_col_ = Delement;
        drag_col_.remove(str_index, Delement.length());

        qDebug() << curr_col << "==" <<drag_col_;
        if(curr_col != drag_col_){
            event->accept();
            return;
        }
        Delement.remove(0, str_index+1);
    }
    //

    qDebug() << ind << "ind";

    if(this->model()->index(-1,-1)==ind){
        event->accept();
    }else{

        QString curr_text = this->model()->data(ind, 0).toString();

        //if(curr_text != Delement ){
        qDebug() <<  curr_text<< "!=" << Delement << "index" << ind;

        if (curr_col == "Группа" && curr_text!=""){
            if (!curr_text.contains(Delement))
                Delement = curr_text + ", " + Delement;
            else Delement = curr_text;
        }

        int id = this->model()->data(this->model()->index(ind.row(), 8), 0).toInt();


        qDebug() << "MODEL______" << this->model();
        old_text = this->model()->data(ind, 0).toString();
        QString new_text = Delement;

        if(curr_col!="Тип пары" && curr_col!="Кафедра"&& curr_col!="Академическая специальность")
            //if (){
            Soul(id, curr_col, new_text);
        //this->model()->setData(ind, Delement);
        comboGroup->currentTextChanged(comboGroup->currentText());
        // }

        //}



    }

    event->acceptProposedAction();
    //event->accept();
    //QTableView::dropEvent(event);
}

void GroupTableDrop::keyReleaseEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_Return)
    {
        if(CellCheck){
            QModelIndex ind = this->selectionModel()->currentIndex();
            QString new_text = this->model()->data(ind, 0).toString();

            QString curr_col = this->model()->headerData(ind.column(), Qt::Horizontal).toString();


            ind = this->model()->index(ind.row(), 8);
            int id = this->model()->data(ind, 0).toInt();

            //if(old_text!=new_text){
            if(new_text!=""){
                Soul( id, curr_col, new_text);
            }else{
                qDebug()<<"kek"<< (new_text=="");
                Soul_Separate(id, curr_col, new_text);
            }
            comboGroup->currentTextChanged(comboGroup->currentText());
            //}


            CellCheck = 0;
        }
        else{
            QModelIndex ind = this->selectionModel()->currentIndex();
            old_text = this->model()->data(ind, 0).toString();


            QString curr_col = this->model()->headerData(ind.column(), Qt::Horizontal).toString();

            if(curr_col!="Тип пары" && curr_col!="Кафедра"&& curr_col!="Академическая специальность"){
                this->edit(ind);
                CellCheck = 1;
            }

        }
    }
    if(event->key() == Qt::Key_Delete)
    {
        QModelIndexList selection = this->selectionModel()->selectedRows();
        for(int i=0; i< selection.count(); i++)
        {
            QModelIndex index = selection.at(i);
            this->model()->removeRow(index.row());
            comboGroup->currentTextChanged(comboGroup->currentText());
        }
    }
}







void GroupTableDrop::Soul_Separate(int id, QString column, QString newText){

    QSqlTableModel *CurrentTableModel = new QSqlTableModel(0, database);
    CurrentTableModel->setTable(TableName);
    CurrentTableModel->select();



    if(column == "Предмет"){
        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type

        CurrentTableModel->setData(CurrentTableModel->index( 0 , 9), newText);
        CurrentTableModel->setData(CurrentTableModel->index( 0 , 10), newText);
        CurrentTableModel->setData(CurrentTableModel->index( 0 , 2), newText);
        if (CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString()==""){
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 3), newText);
        }
        if (CurrentTableModel->data(CurrentTableModel->index( 0 , 4), 0).toString()!="" and
                CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString()!=""){

            CurrentTableModel->submitAll();
            return;
        }else CurrentTableModel->submitAll();



        //Update
        QSqlTableModel *GroupScheduleModel = Soul_Relationship(CurrentTableModel);

        GroupScheduleModel->setFilter("Para = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 0), 0).toString() +
                                      "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                      "' and Teacher = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 4), 0).toString() +
                                      "' and Auditor = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString() +
                                      "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString() +
                                      "' and Groupe like ('%" + CurrentTableModel->data(CurrentTableModel->index( 0 , 6), 0).toString() +
                                      "%')");


        if(GroupScheduleModel->rowCount()==0){
            return;
        }else{

            GroupScheduleModel->setData(GroupScheduleModel->index( 0 , 9), newText);
            GroupScheduleModel->setData(GroupScheduleModel->index( 0 , 10),newText);
            GroupScheduleModel->setData(GroupScheduleModel->index( 0 , 2), newText);
            if (GroupScheduleModel->data(GroupScheduleModel->index( 0 , 5), 0).toString()==""){
                GroupScheduleModel->setData(GroupScheduleModel->index( 0 , 3), newText);
            }

            GroupScheduleModel->submitAll();
            if(GroupScheduleModel->rowCount())
                qDebug() << "GroupScheduleModel->rowCount() =" << GroupScheduleModel->rowCount();
        }


    }


}



bool GroupTableDrop::Soul(int id, QString column, QString newText){//0 new_text = old_text
    qDebug() << "Made with Soul @Михаил";
    //database
    //this->model().
    QSqlTableModel *CurrentTableModel = new QSqlTableModel(0, database);
    CurrentTableModel->setTable(TableName);
    CurrentTableModel->select();


    QSqlTableModel *InfoGroup = new QSqlTableModel(0, database);
    InfoGroup->setTable("Groupes");
    InfoGroup->select();
    QSqlTableModel *InfoTeacher = new QSqlTableModel(0, database);
    InfoTeacher->setTable("Teachers");
    InfoTeacher->select();
    QSqlTableModel *InfoAuditor = new QSqlTableModel(0, database);
    InfoAuditor->setTable("Auditories");
    InfoAuditor->select();
    QSqlTableModel *InfoSubject = new QSqlTableModel(0, database);
    InfoSubject->setTable("Subjects");
    InfoSubject->select();




    if(column == "Предмет"){
        //Select info
        InfoSubject->setFilter(QString("Subject = '%1'").arg(newText));
        if(InfoSubject->rowCount() > 1){
            //cell = QMessageBox !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


            FormChoice *fs = new FormChoice(InfoSubject);
            fs->show();
            connect(fs, SIGNAL(closed(QString)), this, SLOT(searchClosed(QString)));
            QEventLoop SignalWaitLoop;
            connect(fs, SIGNAL(closed(QString)), &SignalWaitLoop, SLOT(quit()));
            SignalWaitLoop.exec();


            qDebug() << searchResult;

        }
        else if (InfoSubject->rowCount()==0) {
            QMessageBox msg;
            msg.warning(this, "Ошибка", "Данного предмета нет в базе данных");
            return 0;
        }


        //Compare info
        QString typeInfo = InfoSubject->data(InfoSubject->index(0, 2), 0).toString();
        QString kafedraInfo = InfoSubject->data(InfoSubject->index(0, 3), 0).toString();
        QString academSpecInfo = InfoSubject->data(InfoSubject->index(0, 4), 0).toString();

        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type

        QString typeCurrent = CurrentTableModel->data(CurrentTableModel->index( 0 , 3), 0).toString();

        if ( ""==CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString() ||
             (""!=CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString() &&
              (typeInfo == typeCurrent or typeCurrent==""))){

            CurrentTableModel->setData(CurrentTableModel->index( 0 , 2), newText);
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 3), typeInfo);
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 9), kafedraInfo);
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 10), academSpecInfo);

            CurrentTableModel->submitAll();

            //Update
            QSqlTableModel *GroupScheduleModel = Soul_Relationship(CurrentTableModel);
            if(GroupScheduleModel == nullptr) return 1;


            qDebug() <<"update_0" << CurrentTableModel->data(CurrentTableModel->index( 0 , 0), 0).toString();
            qDebug() <<"update_0_0" << GroupScheduleModel->data(GroupScheduleModel->index( 0 , 0), 0).toString();
            //GroupScheduleModel->setFilter("Para = ''");



            GroupScheduleModel->setFilter("Para = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 0), 0).toString() +
                                          "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                          "' and Teacher = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 4), 0).toString() +
                                          "' and Auditor = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString() +
                                          "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString() +
                                          "' and Groupe like ('%" + CurrentTableModel->data(CurrentTableModel->index( 0 , 6), 0).toString() +
                                          "%')");


            qDebug() << "update_1";
            if(GroupScheduleModel->rowCount()==0){
                return 1;
            }else{
                qDebug() << "update_2";
                GroupScheduleModel->setData(CurrentTableModel->index( 0 , 2), newText);
                GroupScheduleModel->setData(CurrentTableModel->index( 0 , 3), typeInfo);
                GroupScheduleModel->setData(CurrentTableModel->index( 0 , 9), kafedraInfo);
                GroupScheduleModel->setData(CurrentTableModel->index( 0 , 10), academSpecInfo);

                GroupScheduleModel->submitAll();
                if(GroupScheduleModel->rowCount())
                    qDebug() << "GroupScheduleModel->rowCount() =" << GroupScheduleModel->rowCount();
            }
            qDebug() << "update_3";


        }
        else {//Not Update
            QMessageBox msg;
            msg.warning(this, "Ошибка", "В " +CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString()
                        + " аудитории тип пары не " + typeInfo);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 2), old_text);
            CurrentTableModel->submitAll();
            return 0;
        }
    }// end Subject
    if(column == "Преподаватель"){
        InfoTeacher->setFilter(QString("Name = '%1'").arg(newText));
        if(InfoTeacher->rowCount() > 1){
            //cell = QMessageBox !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            FormChoice *fs = new FormChoice(InfoTeacher);
            fs->show();
            connect(fs, SIGNAL(closed(QString)), this, SLOT(searchClosed(QString)));
            QEventLoop SignalWaitLoop;
            connect(fs, SIGNAL(closed(QString)), &SignalWaitLoop, SLOT(quit()));
            SignalWaitLoop.exec();


            qDebug() << searchResult;

        }
        else if (InfoTeacher->rowCount()==0) {
            QMessageBox msg;
            msg.warning(this, "Ошибка", "Данного преподавателя нет в базе данных");
            return 0;
        }

        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type

        QSqlTableModel *GroupScheduleModel = Soul_Relationship(CurrentTableModel);
        if(GroupScheduleModel == nullptr) return 1;

        GroupScheduleModel->setFilter("Para = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 0), 0).toString() +
                                      "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                      "' and Teacher = '" + newText +
                                      "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString());

        if(GroupScheduleModel->rowCount()==1){


            QString Auditory = CurrentTableModel->data(CurrentTableModel->index(0, 5),0).toString();
            QString Subject = CurrentTableModel->data(CurrentTableModel->index(0, 2),0).toString();
            QString OtherAuditory = GroupScheduleModel->data(GroupScheduleModel->index(0, 5),0).toString();
            QString OtherSubject = GroupScheduleModel->data(GroupScheduleModel->index(0, 2),0).toString();
            if(Auditory == OtherAuditory && Subject == OtherSubject){
                //обьединение
            }else if(Auditory != OtherAuditory && Subject == OtherSubject){
                QMessageBox msg;  msg.warning(this, "Ошибка", "Данного преподавателя нет в базе данных");
                return 0;
            }else if(Auditory == OtherAuditory && Subject != OtherSubject){
                QMessageBox msg;  msg.warning(this, "Ошибка", "Данного преподавателя нет в базе данных");
                return 0;
            }else{
                QMessageBox msg;  msg.warning(this, "Ошибка", "Данного преподавателя нет в базе данных");
                return 0;
            }


        }else{
            //add/update
        }



        //проверка не занят ли препод
            //не занят - вставляем и обновляем
            //если совпадает предмет и аудитория, то обединяем и обновляем (ВОПРОС)
            //занят -ошибка



    }
    if(column == "Аудитория"){
        InfoAuditor->setFilter(QString("Number = '%1'").arg(newText));
        if(InfoAuditor->rowCount() > 1){
            //cell = QMessageBox !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            FormChoice *fs = new FormChoice(InfoAuditor);
            fs->show();
            connect(fs, SIGNAL(closed(QString)), this, SLOT(searchClosed(QString)));
            QEventLoop SignalWaitLoop;
            connect(fs, SIGNAL(closed(QString)), &SignalWaitLoop, SLOT(quit()));
            SignalWaitLoop.exec();


            qDebug() << searchResult;

        }
        else if (InfoAuditor->rowCount()==0) {
            QMessageBox msg;
            msg.warning(this, "Ошибка", "Данного преподавателя нет в базе данных");
            return 0;
        }

        //проверка не занят ли аудитория
            //не занят - вставляем и обновляем
            //если совпадает предмет и преподаватель, то обединяем и обновляем (ВОПРОС)
            //занят -ошибка

    }
    //if(column == "Группа"){ }




    /*
    QTableView *qtw= new QTableView();
    qtw->setModel(InfoSubject);
    qtw->show();
*/

    return 1;
}


QSqlTableModel* GroupTableDrop::Soul_Relationship(QSqlTableModel *CurrentTableModel){
    qDebug() << "Made Relationship with Soul @Михаил";

    QSqlTableModel *GroupScheduleModel = new QSqlTableModel(0, database);


    qDebug() << "TableName " << TableName;
    if (TableName == "GroupSchedule"){
        qDebug() <<CurrentTableModel->data(CurrentTableModel->index( 0 , 4), 0).toString() << "!=''";
        qDebug() <<CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString() << "!=''";
        if (CurrentTableModel->data(CurrentTableModel->index( 0 , 4), 0).toString()!="" or
                CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString()!=""){
            GroupScheduleModel->setTable("Schedule");//GroupScheduleModel == Schedule!!!!!!!!!!!
        }else{
            return nullptr;
        }

    }else{
        if (CurrentTableModel->data(CurrentTableModel->index( 0 , 6), 0).toString()!=""){
            GroupScheduleModel->setTable("GroupSchedule");
        }
        else{
            return nullptr;
        }
    }
    GroupScheduleModel->select();

    return GroupScheduleModel;
}

void GroupTableDrop::searchClosed(QString string){
    searchResult = string;
}


/*
QModelIndex index;
QString cell;
for(int i=0; i<InfoSubject->rowCount(); i++){
    index = InfoSubject->index(i, 1);
    cell = InfoSubject->data(index, 0).toString();
}
*/
