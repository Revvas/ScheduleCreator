#include "advancedtablew.h"

AdvancedTableW::AdvancedTableW(QWidget *parent)
    : QTableWidget (parent)
{
    ;
}

void AdvancedTableW::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
}

void AdvancedTableW::dropEvent(QDropEvent *event)
{
    QTableWidget::dropEvent(event);
}
