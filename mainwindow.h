#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMdiArea>
#include <QtSql/QSql>
#include <QtSql/QSqlTableModel>
#include <QtDebug>

#include "groupswidget.h"
#include "teacherswidget.h"
#include "auditorieswidget.h"
#include "infobox.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_addButton_clicked();
    void on_comboBox_activated(int index);

    void on_reloadTable();

    void on_updateButton_clicked();

private:
    Ui::MainWindow *ui;

    void ChoiceBox(int a);
    void CreateBox(QGroupBox *qgb);

    void CreateGroupsScheduleBox();
    void CreateTeachersScheduleBox();
    void CreateAuditoriesScheduleBox();

    void CreateGroupsBox();
    void CreateTeachersBox();
    void CreateAuditoriesBox();
    void CreateSubjectsBox();

    QList<GroupsWidget *>p_gw;
    QList<TeachersWidget *>p_tw;
    QList<AuditoriesWidget *>p_aw;

    QList<QMdiSubWindow *>p_doc_gw;
    QList<QMdiSubWindow *>p_doc_tw;
    QList<QMdiSubWindow *>p_doc_aw;


    QSqlDatabase _database;

};
#endif // MAINWINDOW_H
