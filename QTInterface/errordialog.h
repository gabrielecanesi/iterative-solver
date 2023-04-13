#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT

public:
    void setMessage(const std::string &message);
    explicit ErrorDialog(QWidget *parent = nullptr);
    ~ErrorDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ErrorDialog *ui;
    QLabel *errorLabel;
};

#endif // ERRORDIALOG_H
