#ifndef FORMSEARCH_H
#define FORMSEARCH_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTextEdit>
#include <QTableView>
#include <QStringListModel>
#include <QSqlRecord>

#include <QtDebug>

namespace Ui {
class FormSearch;
}

class FormSearch : public QWidget
{
    Q_OBJECT

public:
    QSqlTableModel *model;
    //QTableView *view;
    //QString s;

    explicit FormSearch(QSqlTableModel *_model,QWidget *parent = nullptr);
    ~FormSearch();

signals:
    void closed(QString filter);

private slots:
    void on_textEdit_textChanged();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::FormSearch *ui;
};

#endif // FORMSEARCH_H
