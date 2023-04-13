#include "errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
    errorLabel = findChild<QLabel *>("labelError");
    setWindowTitle("Error!!!");
}

void ErrorDialog::setMessage(const std::string &message) {
    errorLabel->setText(message.c_str());
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::on_pushButton_clicked()
{
    errorLabel->setText("Error!");
    this->hide();
}

