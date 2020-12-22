#include "formchoicestring.h"
#include "ui_formchoicestring.h"

FormChoiceString::FormChoiceString(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormChoiceString)
{
    ui->setupUi(this);
}

FormChoiceString::~FormChoiceString()
{
    delete ui;
}

void FormChoiceString::on_pushButton_clicked()//Заменить
{
    closed("Заменить");
    this->close();
}

void FormChoiceString::on_pushButton_2_clicked()//Поменять местами
{
    closed("Поменять местами");
    this->close();
}

void FormChoiceString::on_pushButton_3_clicked()//Отмена
{
    closed("Отменить");
    this->close();
}
