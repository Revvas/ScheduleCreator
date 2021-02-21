#ifndef TEACHERSWIDGET_H
#define TEACHERSWIDGET_H


#include <QtWidgets>
#include <QtSql/QSql>
#include <QtSql/QSqlTableModel>
#include <QSqlRecord>
#include <QAbstractItemDelegate>
#include <QWaitCondition>


#include "grouptabledrop.h"
#include "formsearch.h"

class TeachersWidget : public QWidget
{
    Q_OBJECT
public:
    QSqlDatabase _database;
    TeachersWidget(QSqlDatabase database);
    ~TeachersWidget();
    QGroupBox *teacherWidget;

    const int PARA_COUNT = 8;

    void MakeGroup(QString group_name);



    void Reload();

    QSqlTableModel *ModelAr[6];
    GroupTableDrop *TabAr[6];
    QString QsAr[6] = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};

    QComboBox *comboGroup;
    QComboBox *comboWeek;

    void AddTable(QGridLayout *qll, QSqlDatabase _database);

    void Search();

    QPushButton *addButton[6];

private slots:
    void currentGroupChanged(const QString &text);
    void currentWeakChanged(const QString &text);

    void searchClosed(QString filter);

    void searchClicked(bool checked);
    void on_addButton_click();


signals:
    void TableChanged();

};

#endif // TEACHERSWIDGET_H


