#include "formchoice.h"
#include "ui_formchoice.h"

FormChoice::FormChoice(QSqlTableModel *_model,QWidget *parent) :
    model(_model),
    QWidget(parent),
    ui(new Ui::FormChoice)
{
    ui->setupUi(this);

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->horizontalHeader()->setVisible(false);

}

FormChoice::~FormChoice()
{
    delete ui;
}

void FormChoice::on_tableView_clicked(const QModelIndex &index)
{

    QModelIndex ind = model->index(index.row(), 0);
    QString id = model->data(ind, 0).toString();

    model->setFilter(QString("id = '%1'").arg(id));

    closed(id);
    this->close();
}
