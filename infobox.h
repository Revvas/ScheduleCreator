#ifndef INFOBOX_H
#define INFOBOX_H

#include <QGroupBox>
#include <QSqlTableModel>
#include <QTextEdit>
#include <QTableView>
#include <QStringListModel>
#include <QSqlRecord>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>

#include "qtableviewdragdrop.h"

class InfoBox: public QWidget
{
    Q_OBJECT
public:
    InfoBox(QString WidgetName, QSqlTableModel *_model, QString _ColumnName);

    QGroupBox *InfoWidget;


private:
    QString ColumnName;
    QSqlTableModel *model;
    QTableViewDragDrop *tab;

    QTextEdit *textSearch;



private slots:
    void on_textEdit_textChanged();
    void on_addButon_clicked();
    void on_deleteButon_clicked();

};

#endif // INFOBOX_H
