#ifndef FORMCHOICE_H
#define FORMCHOICE_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class FormChoice;
}

class FormChoice : public QWidget
{
    Q_OBJECT

public:
    QSqlTableModel *model;
    explicit FormChoice(QSqlTableModel *_model, QWidget *parent = nullptr);
    ~FormChoice();

signals:
    void closed(QString filter);

private slots:
    void on_tableView_clicked(const QModelIndex &index);

private:

    Ui::FormChoice *ui;
};

#endif // FORMCHOICE_H
