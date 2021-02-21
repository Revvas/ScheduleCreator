#include "infobox.h"


InfoBox::InfoBox(QString WidgetName, QSqlTableModel *_model, QString _ColumnName):
    model(_model), ColumnName(_ColumnName)
{

    InfoWidget = new QGroupBox(WidgetName);
    QGridLayout *qll = new QGridLayout();


    textSearch = new QTextEdit();

    textSearch->setMaximumHeight(30);


    connect(textSearch, SIGNAL(textChanged()), this, SLOT(on_textEdit_textChanged()));

    qll->addWidget(textSearch, 0,0,1,2);



    QPushButton *addButton = new QPushButton("Добавить");
    connect(addButton, SIGNAL(clicked()), this, SLOT(on_addButon_clicked()));

    qll->addWidget(addButton, 1,0);

    QPushButton *deleteButton = new QPushButton("Удалить");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(on_deleteButon_clicked()));

    qll->addWidget(deleteButton, 1,1);


    //Table
        tab = new QTableViewDragDrop();
        tab->setModel(model);


        tab->setColumnHidden(0, true);
        //
        tab->setDragEnabled(true);
        tab->setDragDropMode(QAbstractItemView::DragOnly);
        tab->setSelectionMode(QAbstractItemView::SingleSelection);

        tab->setSortingEnabled(1);

        tab->resizeColumnsToContents();


        qll->addWidget(tab, 2, 0, 1, 2);

    InfoWidget->setLayout(qll);
    InfoWidget->resize(240, 500);
}


void InfoBox::on_addButon_clicked(){
     model->insertRow(model->rowCount());
     QModelIndex ind;
     for(int i = 1; i<model->columnCount(); i++){
         ind = model->index(model->rowCount()-1, i);
         model->setData(ind, "");
     }
     model->submitAll();
     on_textEdit_textChanged();
}

void InfoBox::on_deleteButon_clicked(){
    QModelIndex ind = tab->currentIndex();
    model->removeRow(ind.row());
    on_textEdit_textChanged();
}


void InfoBox::on_textEdit_textChanged(){

    QString in_str = textSearch->toPlainText();
    QString curr_col = model->headerData(1, Qt::Horizontal, Qt::DisplayRole).toString();
    //model.fil
    model->setFilter(QString("%1 like ('\%%2\%')").arg(ColumnName).arg(in_str));
}
