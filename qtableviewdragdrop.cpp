#include "qtableviewdragdrop.h"


#include <QtDebug>
#include <QDrag>

#include <QStandardItemModel>

QTableViewDragDrop::QTableViewDragDrop(QWidget *parent)
    : QTableView (parent)
{
    //tab->horizontalHeader()->setStretchLastSection(1);
    //tab->verticalHeader()->setStretchLastSection(1);
}

void QTableViewDragDrop::adaptColumn(){

    for(int i=0; i< this->model()->columnCount(); i++)
        this->setColumnWidth(i, this->width()/3);

}

void QTableViewDragDrop::resizeEvent(QResizeEvent *event) {
    //adaptColumn();

    QTableView::resizeEvent(event);
}


Qt::DropActions QTableViewDragDrop::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

void QTableViewDragDrop::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void QTableViewDragDrop::mousePressEvent(QMouseEvent *event)
{


    if (event->button() == Qt::LeftButton) {


            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            QModelIndex ind = this->indexAt(event->pos());

            //this->model()->data(ind);

            //QByteArray Delement = this->model()->data(ind);
            //qDebug() << this->model()->data(ind).toString();

            QString col = this->model()->headerData(ind.column(), Qt::Horizontal).toString();



            mimeData->setText(col +"#" + this->model()->data(ind).toString());

            //mimeData->setData()
            //mimeData->setData("plain/text", Delement);
            drag->setMimeData(mimeData);

            Qt::DropAction dropAction = drag->exec();
           }

    event->accept();

    //qDebug() << "hello";

    QTableView::mousePressEvent(event);
}

void QTableViewDragDrop::dragEnterEvent(QDragEnterEvent *event)
{
    /*
    if (event->mimeData()->hasFormat("plain/text"))//("application/x-qstandarditemmodeldatalist"))
        event->acceptProposedAction();
*/
    event->acceptProposedAction();
    //qDebug() << event->mimeData()->formats()  <<"dragenter";
    //event->accept();
    //QTableView::dragEnterEvent(event);
}

void QTableViewDragDrop::dropEvent(QDropEvent *event)
{
    /*
    QByteArray encoded = event->mimeData()->data("plain/text");//("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    QString Delement;

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;
        qDebug() << roleDataMap;

        Delement = roleDataMap[0].toString();
    }
    */
    QString Delement = event->mimeData()->text();

    QModelIndex ind = this->indexAt(event->pos());
    //qDebug() << ind << "ind";

    this->model()->setData(ind, Delement);

    event->acceptProposedAction();
}
