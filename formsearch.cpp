#include "formsearch.h"
#include "ui_formsearch.h"

FormSearch::FormSearch(QSqlTableModel *_model, QWidget *parent) :
    QWidget(parent),
    model(_model),
    ui(new Ui::FormSearch)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::ApplicationModal);

    qDebug() << "h2";

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(2, true);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//not editable table
     qDebug() << "h3";
}

FormSearch::~FormSearch()
{
    delete ui;
}

void FormSearch::on_textEdit_textChanged()
{

    QString in_str = ui->textEdit->toPlainText();
    QStringList stringList;
       for(int i = 0; i < model->rowCount(); i++)
        stringList.append( model->record(i).value(1).toString() );

    QStringList result;

    for(int i=0; i<stringList.length(); i++){
        if(stringList[i].contains(in_str))
            result.append(stringList[i]);
    }


    QStringListModel *strmo = new QStringListModel(result);
    ui->tableView->setModel(strmo);
    ui->tableView->setColumnHidden(0, false);

}

void FormSearch::on_tableView_clicked(const QModelIndex &index)
{
    QString s = model->data(index, 0).toString();
    closed(s);
    this->close();
}
