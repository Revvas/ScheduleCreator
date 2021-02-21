#ifndef QTABLEVIEWDRAGDROP_H
#define QTABLEVIEWDRAGDROP_H

#include <QWidget>
#include <QTableView>
#include <QMouseEvent>
#include <QMimeData>

class QTableViewDragDrop : public QTableView
{
    Q_OBJECT
public:
    QTableViewDragDrop(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent * event);
    void dropEvent(QDropEvent * event);
    Qt::DropActions supportedDropActions() const;
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void adaptColumn();

private slots:
    void resizeEvent(QResizeEvent *event);
};

#endif // QTABLEVIEWDRAGDROP_H
