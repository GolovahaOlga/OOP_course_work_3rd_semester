#include "searchdialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
SearchDialog::SearchDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Search Contacts");
    QFormLayout *layout = new QFormLayout(this);
    nameEdit = new QLineEdit(this);
    surnameEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    layout->addRow("First Name:", nameEdit);
    layout->addRow("Last Name:", surnameEdit);
    layout->addRow("Email:", emailEdit);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
QString SearchDialog::getName() const
{
    return nameEdit->text().trimmed();
}
QString SearchDialog::getSurname() const
{
    return surnameEdit->text().trimmed();
}
QString SearchDialog::getEmail() const
{
    return emailEdit->text().trimmed();
}
