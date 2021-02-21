#include "grouptabledrop.h"

GroupTableDrop::GroupTableDrop(QComboBox *comboGroup_p, QComboBox *comboWeek_p,
                               QString day_p, QSqlDatabase db, QString _TableName)
    :comboGroup(comboGroup_p),comboWeek(comboWeek_p), day(day_p), database(db), TableName(_TableName)
{
    this->setDragEnabled(false);
    this->setDragDropMode(QAbstractItemView::DropOnly);

    this->sortByColumn(0, Qt::AscendingOrder);


}

void GroupTableDrop::dragMoveEvent(QDragMoveEvent *event)
{
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
            if(TableName=="GroupSchedule"){
                this->model()->setData(ind, Delement);
                Soul(id, curr_col, new_text);
            }
        comboGroup->currentTextChanged(comboGroup->currentText());



    }

    event->acceptProposedAction();
}

void GroupTableDrop::keyReleaseEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_Return && TableName=="GroupSchedule")
    {
        if(CellCheck){
            QModelIndex ind = this->selectionModel()->currentIndex();
            QString new_text = this->model()->data(ind, 0).toString();

            QString curr_col = this->model()->headerData(ind.column(), Qt::Horizontal).toString();


            ind = this->model()->index(ind.row(), 8);
            int id = this->model()->data(ind, 0).toInt();

            if(new_text!=""){
                Soul( id, curr_col, new_text);
            }else{
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
                if(TableName=="GroupSchedule"){
                    this->edit(ind);
                    CellCheck = 1;
                }
            }

        }
    }
    if(event->key() == Qt::Key_Delete)
    {
        if(TableName=="GroupSchedule"){
            QModelIndexList selection = this->selectionModel()->selectedRows();
            for(int i=0; i< selection.count(); i++)
            {
                QModelIndex index = selection.at(i);
                this->model()->removeRow(index.row());
                comboGroup->currentTextChanged(comboGroup->currentText());
            }
        }
    }
}


void GroupTableDrop::Soul_Separate(int id, QString column, QString newText){

    QSqlTableModel *CurrentTableModel = new QSqlTableModel(0, database);
    CurrentTableModel->setTable(TableName);
    CurrentTableModel->select();


    if(column=="Аудитория"){

        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type
        CurrentTableModel->setData(CurrentTableModel->index(0,5), newText);
        CurrentTableModel->submitAll();

        QString Subject = CurrentTableModel->data(CurrentTableModel->index( 0 , 2), 0).toString();

        if(Subject==""){
            CurrentTableModel->setData(CurrentTableModel->index(0,3), newText);
            CurrentTableModel->submitAll();
            return;
        }
        return;

    }
    if(column=="Предмет"){

        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type
        CurrentTableModel->setData(CurrentTableModel->index(0,3), newText);
        CurrentTableModel->submitAll();

        QString Auditor = CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString();

        CurrentTableModel->setData(CurrentTableModel->index( 0 , 9), "");
        CurrentTableModel->setData(CurrentTableModel->index( 0 , 10), "");
        CurrentTableModel->submitAll();
        return;
        if(Auditor==""){

            CurrentTableModel->setData(CurrentTableModel->index(0,3), newText);
            CurrentTableModel->submitAll();
            return;

        }
        return;
    }


}



bool GroupTableDrop::Soul(int id, QString column, QString newText){//0 new_text = old_text


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


    if(column=="Аудитория"){

        qDebug() << "catch me -3";
        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type

        CurrentTableModel->setData(CurrentTableModel->index(0,5), newText);
        CurrentTableModel->submitAll();

        InfoAuditor->setFilter(QString("Number = '%1'").arg(newText));
        if (InfoSubject->rowCount()==0) {
            QMessageBox msg;
            msg.warning(this, "Ошибка", "Данной аудитории нет в базе данных");
            CurrentTableModel->setData(CurrentTableModel->index(0,5), old_text);
            CurrentTableModel->submitAll();
            return 0;

            qDebug() << "catch me -2";
        }

        QSqlTableModel *OtherModel = new QSqlTableModel(0, database);
        OtherModel->setTable(TableName);
        OtherModel->select();

        OtherModel->setFilter("Para = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 0), 0).toString() +
                                                  "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                                  "' and Auditor = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString() +
                                                  "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString() +
                                                  "'");


        qDebug() << "catch me -1";
        if(OtherModel->rowCount()>1){
            qDebug() << "catch me 0";
            QMessageBox msg;
            msg.warning(this, "Ошибка", "Данная аудитория занята");
            CurrentTableModel->setData(CurrentTableModel->index(0,5), old_text);
            CurrentTableModel->submitAll();
            return 0;
        }else{
            QString Type = CurrentTableModel->data(CurrentTableModel->index( 0 , 3), 0).toString();
            QString Subject = CurrentTableModel->data(CurrentTableModel->index( 0 , 2), 0).toString();
            QString OtherType =  InfoAuditor->data(InfoAuditor->index(0,2), 0).toString();

                qDebug() << "catch me 1";
            if(Subject=="" or OtherType == "Общая"){
                CurrentTableModel->setData(CurrentTableModel->index(0,3), OtherType);
                CurrentTableModel->submitAll();
                qDebug() << "catch me 2";
                return 1;
            }else if(Type != OtherType){
                QMessageBox msg;
                msg.warning(this, "Ошибка", "В " +CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString()
                                        + " аудитории тип пары не " + Type);
                CurrentTableModel->setData(CurrentTableModel->index(0,5), old_text);
                CurrentTableModel->submitAll();
                return 0;
            }else{
                return 1;
            }
        }

    }
    if(column == "Предмет"){

        qDebug() << "catch me -3";
        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type

        CurrentTableModel->setData(CurrentTableModel->index(0,2), newText);
        CurrentTableModel->submitAll();


        //Select info
        InfoSubject->setFilter(QString("Subject = '%1'").arg(newText));
        if(InfoSubject->rowCount() > 1){
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
            CurrentTableModel->setData(CurrentTableModel->index(0,2), old_text);
            CurrentTableModel->submitAll();
            return 0;
        }

        //Compare info
        QString typeInfo = InfoSubject->data(InfoSubject->index(0, 2), 0).toString();
        QString kafedraInfo = InfoSubject->data(InfoSubject->index(0, 3), 0).toString();
        QString academSpecInfo = InfoSubject->data(InfoSubject->index(0, 4), 0).toString();

        QString typeCurrent = CurrentTableModel->data(CurrentTableModel->index( 0 , 3), 0).toString();

        QString Auditor=  CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString();

        if(Auditor == "" or typeCurrent=="Общая" or typeInfo==typeCurrent){
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 2), newText);
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 3), typeInfo);
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 9), kafedraInfo);
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 10), academSpecInfo);
            CurrentTableModel->submitAll();
            return 1;
        }else if (typeCurrent!=typeInfo){
            QMessageBox msg;
            msg.warning(this, "Ошибка", "В " +CurrentTableModel->data(CurrentTableModel->index( 0 , 5), 0).toString()
                        + " аудитории тип пары не " + typeInfo);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            CurrentTableModel->setData(CurrentTableModel->index( 0 , 2), old_text);
            CurrentTableModel->submitAll();
            return 0;
        }else{
            return 0;
        }


    }

    if(column == "Преподаватель"){
        qDebug() << "catch me -3";
        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type

        CurrentTableModel->setData(CurrentTableModel->index(0,4), newText);
        CurrentTableModel->submitAll();

        InfoTeacher->setFilter(QString("Name = '%1'").arg(newText));
        if (InfoTeacher->rowCount()==0) {
            QMessageBox msg;
            msg.warning(this, "Ошибка", "Данного преподавателя нет в базе данных");
            CurrentTableModel->setData(CurrentTableModel->index(0,4), old_text);
            CurrentTableModel->submitAll();
            return 0;
        }

       QSqlTableModel *OtherModel = new QSqlTableModel(0, database);
       OtherModel->setTable(TableName);
       OtherModel->select();

       QString Auditory = CurrentTableModel->data(CurrentTableModel->index(0, 5),0).toString();
       QString Subject = CurrentTableModel->data(CurrentTableModel->index(0, 2),0).toString();

       OtherModel->setFilter("Para = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 0), 0).toString() +
                             "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                             "' and Teacher = '" + newText +
                             "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString()
                             +"'");

       if(OtherModel->rowCount()>1){
           //обеденить или отменить
           QTableView *qtw= new QTableView();
           qtw->setModel(OtherModel);
           qtw->show();

           QMessageBox::StandardButton reply;
           reply = QMessageBox::question(this, "Внимание", "Обьеденить пары, указанные в открывшейся таблице?",
                                         QMessageBox::Yes|QMessageBox::No);
           if (reply == QMessageBox::Yes) {
               OtherModel->setFilter("Para = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 0), 0).toString() +
                                     "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                     "' and Teacher = '" + newText +
                                     "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString()
                                     +"' and id !='"+id+"'");

               CurrentTableModel->setData(CurrentTableModel->index(0,2), OtherModel->data(OtherModel->index(0,2),0).toString());
               CurrentTableModel->setData(CurrentTableModel->index(0,3), OtherModel->data(OtherModel->index(0,3),0).toString());
               CurrentTableModel->setData(CurrentTableModel->index(0,5), OtherModel->data(OtherModel->index(0,5),0).toString());
               CurrentTableModel->setData(CurrentTableModel->index(0,9), OtherModel->data(OtherModel->index(0,9),0).toString());
               CurrentTableModel->setData(CurrentTableModel->index(0,10), OtherModel->data(OtherModel->index(0,10),0).toString());
               CurrentTableModel->setData(CurrentTableModel->index(0,4), OtherModel->data(OtherModel->index(0,4),0).toString());
               CurrentTableModel->submitAll();
               qtw->close();
               return 0;
           }else{
               CurrentTableModel->setData(CurrentTableModel->index(0,4), old_text);
               CurrentTableModel->submitAll();
               qtw->close();
               return 0;
           }
       }

    }
    if(column == "Пара"){
        CurrentTableModel->setFilter(QString("id = '%1'").arg(id));//Type

        CurrentTableModel->setData(CurrentTableModel->index(0,0), newText);
        CurrentTableModel->submitAll();



        QSqlTableModel *OtherModel = new QSqlTableModel(0, database);
        OtherModel->setTable(TableName);
        OtherModel->select();

        QString Auditor= CurrentTableModel->data(CurrentTableModel->index(0, 5),0).toString();
        QString Teacher= CurrentTableModel->data(CurrentTableModel->index(0, 4),0).toString();

        QSqlTableModel *OtherModel_1 = new QSqlTableModel(0, database);
        OtherModel_1->setTable(TableName);
        OtherModel_1->select();




        OtherModel->setFilter("Para = '" + newText +
                              "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                              "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString() +
                              "' and Groupe = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 6), 0).toString() +
                              "' and id <>'"+QString::number(id)+"'");




        if(OtherModel->rowCount()>=1){
            for(int i=0;i<OtherModel->rowCount();i++){
                OtherModel->setData(OtherModel->index(i,0),"");
            }



            if(Auditor!=""){
                OtherModel_1->setFilter("Para = '" + newText +
                                          "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                          "' and Auditor = '" + Auditor +
                                          "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString() +
                                          "'");

                if(OtherModel_1->rowCount()>1){
                    QMessageBox msg;
                    msg.warning(this, "Ошибка", "Аудитория №" + Auditor +" занята на " + newText + " паре");
                    CurrentTableModel->setData(CurrentTableModel->index(0,0), old_text);
                    CurrentTableModel->submitAll();
                    return 0;
                }
            }else if(Teacher!=""){
                OtherModel_1->setFilter("Para = '" + newText +
                                      "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                      "' and Teacher = '" + Teacher +
                                      "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString()+
                                      "'");

                if(OtherModel_1->rowCount()>1){
                    QMessageBox msg;
                    msg.warning(this, "Ошибка", "Преподаватель " + Teacher +" занят на " + newText + " паре");
                    CurrentTableModel->setData(CurrentTableModel->index(0,0), old_text);
                    CurrentTableModel->submitAll();
                    return 0;
                }
            }






            QTableView *qtw= new QTableView();
            qtw->setModel(OtherModel);
            qtw->show();


            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Внимание", "Освободить пары, указанные в открывшейся таблице?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {


                qtw->close();
                return 1;
            }else{
                for(int i=0;i<OtherModel->rowCount();i++){
                    OtherModel->setData(OtherModel->index(i,0),newText);
                }
                CurrentTableModel->setData(CurrentTableModel->index(0,0), old_text);
                CurrentTableModel->submitAll();
                qtw->close();
                return 0;
            }
        }else{
            if(Auditor!=""){
                OtherModel_1->setFilter("Para = '" + newText +
                                          "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                          "' and Auditor = '" + Auditor +
                                          "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString() +
                                          "'");

                if(OtherModel_1->rowCount()>1){
                    QMessageBox msg;
                    msg.warning(this, "Ошибка", "Аудитория №" + Auditor +" занята на " + newText + " паре");
                    CurrentTableModel->setData(CurrentTableModel->index(0,0), old_text);
                    CurrentTableModel->submitAll();
                    return 0;
                }
            }else if(Teacher!=""){
                OtherModel_1->setFilter("Para = '" + newText +
                                      "' and UpDown = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 1), 0).toString() +
                                      "' and Teacher = '" + Teacher +
                                      "' and Day = '" + CurrentTableModel->data(CurrentTableModel->index( 0 , 7), 0).toString()
                                      +"'");

                if(OtherModel_1->rowCount()>1){
                    QMessageBox msg;
                    msg.warning(this, "Ошибка", "Преподаватель " + Teacher +" занят на " + newText + " паре");
                    CurrentTableModel->setData(CurrentTableModel->index(0,0), old_text);
                    CurrentTableModel->submitAll();
                    return 0;
                }
            }
        }

        return 1;

    }

    return 1;
}


void GroupTableDrop::searchClosed(QString string){
    searchResult = string;
}

