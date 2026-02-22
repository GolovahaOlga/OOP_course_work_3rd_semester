#pragma once
#include <QDialog>
#include "libcontact.h"
#include "libvalidator.h"

class QLineEdit;
class QListWidget;
class QPushButton;

class AddEditDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddEditDialog(QWidget *parent = nullptr, bool editMode = false);
    void setContact(const Contact &c);
    Contact getContact() const;
private slots:
    void addNumber();
    void removeNumber();
    void validateAndAccept();
private:
    bool validateField(const QString &text, const QString &fieldName,
                       std::function<Validator(const std::string&)> validatorFunc,
                       QString &output);

    QLineEdit *nameEdit;
    QLineEdit *surnameEdit;
    QLineEdit *patronymicEdit;
    QLineEdit *emailEdit;
    QLineEdit *addressEdit;
    QLineEdit *dateEdit;
    QListWidget *numbersList;
    QPushButton *addNumberBtn;
    QPushButton *removeNumberBtn;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    Contact m_contact;
    bool m_editMode;
};
