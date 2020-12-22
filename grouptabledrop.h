#ifndef GROUPTABLEDROP_H
#define GROUPTABLEDROP_H

#include <QWidget>
#include <QTableView>
#include <QMouseEvent>
#include <QMimeData>
#include <QComboBox>
#include <QSqlTableModel>
#include <QGroupBox>
#include <QEventLoop>
#include <QtWidgets>

#include <QDebug>

#include <QMessageBox>
#include <QSqlDatabase>

//#include "Soul.cpp"
//#include "formsearch.h"
#include "formchoice.h"
#include "formchoicestring.h"


class GroupTableDrop: public QTableView
{
    Q_OBJECT
public:
    QSqlDatabase database;
    GroupTableDrop(QComboBox *comboGroup_p, QComboBox *comboWeek_p, QString day_p, QSqlDatabase database, QString _TableName);

    QComboBox *comboGroup;
    QComboBox *comboWeek;
    QString day;

    QString old_text;



    void dropEvent(QDropEvent * event);
    Qt::DropActions supportedDropActions() const;
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

    QString TableName;

    QString searchResult;

private:
    bool Soul(int id, QString column, QString newText);
    void Soul_Separate(int id, QString column, QString newText);

    QSqlTableModel *Soul_Relationship(QSqlTableModel *CurrentTableModel);

protected:
    bool CellCheck = 0;
    void keyReleaseEvent(QKeyEvent *);

//protected slots:
    //void rowCountChanged(int oldCount, int newCount);

private slots:
    void searchClosed(QString string);

signals:
    void CheckError(QDropEvent *event);
    //void rowCountChanged(int,int);
};

#endif // GROUPTABLEDROP_H
