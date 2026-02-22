#pragma once
#include <QDialog>

class QLineEdit;

class SearchDialog : public QDialog {
    Q_OBJECT
public:
    explicit SearchDialog(QWidget *parent = nullptr);
    QString getName() const;
    QString getSurname() const;
    QString getEmail() const;

private:
    QLineEdit *nameEdit;
    QLineEdit *surnameEdit;
    QLineEdit *emailEdit;
};
