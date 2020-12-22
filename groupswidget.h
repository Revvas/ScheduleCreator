#ifndef GROUPSWIDGET_H
#define GROUPSWIDGET_H

#include <QtWidgets>
#include <QtSql/QSql>
#include <QtSql/QSqlTableModel>
#include <QSqlRecord>
#include <QAbstractItemDelegate>
#include <QWaitCondition>


#include "grouptabledrop.h"
#include "formsearch.h"

//#include "pushashap.h"

class GroupsWidget : public QWidget
{
    Q_OBJECT

public:
    QSqlDatabase _database;
    GroupsWidget(QSqlDatabase database);
    ~GroupsWidget();
    QGroupBox *groupWidget;

    //QSqlTableModel *model;
    //QModelIndex index;

    const int PARA_COUNT = 8;
     QSqlTableModel *GModel;

    void Reload();


    //QSqlTableModel *MoModel;
    QSqlTableModel *ModelAr[6];
    GroupTableDrop *TabAr[6];
    QString QsAr[6] = {"Понидельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};

    QComboBox *comboGroup;
    QComboBox *comboWeek;

    void AddTable(QGridLayout *qll, QSqlDatabase _database);

    void Search();

    QPushButton *addButton[6];
private slots:
    void on_addButton_click();

    void currentGroupChanged(const QString &text);
    void currentWeakChanged(const QString &text);

    void searchClosed(QString filter);

    void searchClicked(bool checked);

signals:
    void TableChanged();


};

#endif // GROUPSWIDGET_H


