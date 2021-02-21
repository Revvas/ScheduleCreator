#include "qtableviewdragdrop.h"


#include <QtDebug>
#include <QDrag>

#include <QStandardItemModel>

QTableViewDragDrop::QTableViewDragDrop(QWidget *parent)
    : QTableView (parent)
{

}

void QTableViewDragDrop::adaptColumn(){

    for(int i=0; i< this->model()->columnCount(); i++)
        this->setColumnWidth(i, this->width()/3);

}

void QTableViewDragDrop::resizeEvent(QResizeEvent *event)
{
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

            QString col = this->model()->headerData(ind.column(), Qt::Horizontal).toString();

            mimeData->setText(col +"#" + this->model()->data(ind).toString());

            drag->setMimeData(mimeData);

            Qt::DropAction dropAction = drag->exec();
           }

    event->accept();

    QTableView::mousePressEvent(event);
}

void QTableViewDragDrop::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void QTableViewDragDrop::dropEvent(QDropEvent *event)
{
    QString Delement = event->mimeData()->text();

    QModelIndex ind = this->indexAt(event->pos());

    this->model()->setData(ind, Delement);

    event->acceptProposedAction();
}
