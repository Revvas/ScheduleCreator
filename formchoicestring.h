#ifndef FORMCHOICESTRING_H
#define FORMCHOICESTRING_H

#include <QWidget>

namespace Ui {
class FormChoiceString;
}

class FormChoiceString : public QWidget
{
    Q_OBJECT

public:
    QString result;
    explicit FormChoiceString(QWidget *parent = nullptr);
    ~FormChoiceString();

signals:
    void closed(QString filter);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::FormChoiceString *ui;
};

#endif // FORMCHOICESTRING_H
