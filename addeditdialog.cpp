#include "QTaddeditdialog.h"
#include "libvalidator.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <functional>

AddEditDialog::AddEditDialog(QWidget *parent, bool editMode)
    : QDialog(parent), m_editMode(editMode)
{
    setWindowTitle(editMode ? tr("Редактирование контакта") : tr("Добавление контакта"));
    nameEdit = new QLineEdit(this);
    surnameEdit = new QLineEdit(this);
    patronymicEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    dateEdit = new QLineEdit(this);
    dateEdit->setPlaceholderText("дд.мм.гггг");
    numbersList = new QListWidget(this);
    addNumberBtn = new QPushButton(tr("Добавить номер"), this);
    removeNumberBtn = new QPushButton(tr("Удалить номер"), this);
    okBtn = new QPushButton(tr("ОК"), this);
    cancelBtn = new QPushButton(tr("Отмена"), this);
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Имя*:"), nameEdit);
    formLayout->addRow(tr("Фамилия*:"), surnameEdit);
    formLayout->addRow(tr("Отчество:"), patronymicEdit);
    formLayout->addRow(tr("Email*:"), emailEdit);
    formLayout->addRow(tr("Адрес:"), addressEdit);
    formLayout->addRow(tr("Дата рождения:"), dateEdit);
    formLayout->addRow(tr("Телефоны:"), numbersList);
    QHBoxLayout *numButtonsLayout = new QHBoxLayout;
    numButtonsLayout->addWidget(addNumberBtn);
    numButtonsLayout->addWidget(removeNumberBtn);
    numButtonsLayout->addStretch();
    QHBoxLayout *dialogButtonsLayout = new QHBoxLayout;
    dialogButtonsLayout->addStretch();
    dialogButtonsLayout->addWidget(okBtn);
    dialogButtonsLayout->addWidget(cancelBtn);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(numButtonsLayout);
    mainLayout->addLayout(dialogButtonsLayout);
    connect(addNumberBtn, &QPushButton::clicked, this, &AddEditDialog::addNumber);
    connect(removeNumberBtn, &QPushButton::clicked, this, &AddEditDialog::removeNumber);
    connect(okBtn, &QPushButton::clicked, this, &AddEditDialog::validateAndAccept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AddEditDialog::addNumber() {
    bool ok;
    QString num = QInputDialog::getText(this, tr("Номер телефона"),
                                         tr("Введите номер:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !num.isEmpty()) {
        Validator v = Validator::valid_number(num.toStdString());
        if (v.get_success()) {
            numbersList->addItem(QString::fromStdString(v.get_norm()));
        } else {
            QMessageBox::warning(this, tr("Ошибка"),
                                 QString::fromStdString(v.get_error()));
        }
    }
}
void AddEditDialog::removeNumber() {
    QListWidgetItem *item = numbersList->currentItem();
    if (item) {
        delete item;
    }
}
void AddEditDialog::validateAndAccept() {
    if (nameEdit->text().trimmed().isEmpty() ||
        surnameEdit->text().trimmed().isEmpty() ||
        emailEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Имя, фамилия и email обязательны."));
        return;
    }
    Validator nameVal = Validator::valid_name_surname_patronymic(nameEdit->text().trimmed().toStdString());
    if (!nameVal.get_success()) {
        QMessageBox::warning(this, tr("Ошибка"), QString::fromStdString(nameVal.get_error()));
        return;
    }
    Validator surnameVal = Validator::valid_name_surname_patronymic(surnameEdit->text().trimmed().toStdString());
    if (!surnameVal.get_success()) {
        QMessageBox::warning(this, tr("Ошибка"), QString::fromStdString(surnameVal.get_error()));
        return;
    }
    std::string patronymicNorm;
    if (!patronymicEdit->text().trimmed().isEmpty()) {
        Validator patrVal = Validator::valid_name_surname_patronymic(patronymicEdit->text().trimmed().toStdString());
        if (!patrVal.get_success()) {
            QMessageBox::warning(this, tr("Ошибка"), QString::fromStdString(patrVal.get_error()));
            return;
        }
        patronymicNorm = patrVal.get_norm();
    }
    Validator emailVal = Validator::valid_email(emailEdit->text().trimmed().toStdString());
    if (!emailVal.get_success()) {
        QMessageBox::warning(this, tr("Ошибка"), QString::fromStdString(emailVal.get_error()));
        return;
    }
    std::string addressNorm;
    if (!addressEdit->text().trimmed().isEmpty()) {
        Validator addrVal = Validator::valid_address(addressEdit->text().trimmed().toStdString());
        if (!addrVal.get_success()) {
            QMessageBox::warning(this, tr("Ошибка"), QString::fromStdString(addrVal.get_error()));
            return;
        }
        addressNorm = addrVal.get_norm();
    }
    std::string dateNorm;
    if (!dateEdit->text().trimmed().isEmpty()) {
        Validator dateVal = Validator::valid_date(dateEdit->text().trimmed().toStdString());
        if (!dateVal.get_success()) {
            QMessageBox::warning(this, tr("Ошибка"), QString::fromStdString(dateVal.get_error()));
            return;
        }
        dateNorm = dateVal.get_norm();
    }
    if (numbersList->count() == 0) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Должен быть хотя бы один номер телефона."));
        return;
    }
    Contact newContact;
    newContact.set_name(nameVal.get_norm());
    newContact.set_surname(surnameVal.get_norm());
    newContact.set_patronymic(patronymicNorm);
    newContact.set_email(emailVal.get_norm());
    newContact.set_address(addressNorm);
    newContact.set_date(dateNorm);
    for (int i = 0; i < numbersList->count(); ++i) {
        newContact.add_number(numbersList->item(i)->text().toStdString());
    }
    m_contact = newContact;
    accept();
}
void AddEditDialog::setContact(const Contact &c) {
    m_contact = c;
    nameEdit->setText(QString::fromStdString(c.get_name()));
    surnameEdit->setText(QString::fromStdString(c.get_surname()));
    patronymicEdit->setText(QString::fromStdString(c.get_patronymic()));
    emailEdit->setText(QString::fromStdString(c.get_email()));
    addressEdit->setText(QString::fromStdString(c.get_address()));
    dateEdit->setText(QString::fromStdString(c.get_date()));
    numbersList->clear();
    for (const auto &num : c.get_number()) {
        numbersList->addItem(QString::fromStdString(num));
    }
}
Contact AddEditDialog::getContact() const {
    return m_contact;
}
