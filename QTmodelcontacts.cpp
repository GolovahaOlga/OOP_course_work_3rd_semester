#include "libQTmodelcontacts.h"
#include "libcontact.h"
ContactsModel::ContactsModel(PhoneBook &book, QObject *parent)
    : QAbstractTableModel(parent), m_book(book)
{}
int ContactsModel::rowCount(const QModelIndex &) const {
    return m_book.number_contacts();
}
int ContactsModel::columnCount(const QModelIndex &) const {
    return 7;
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
    Contact contact = m_book.find_contact_by_serial_number(index.row());
    switch (index.column()) {
    case 0: return QString::fromStdString(contact.get_surname());
    case 1: return QString::fromStdString(contact.get_name());
    case 2: return QString::fromStdString(contact.get_patronymic());
    case 3: return QString::fromStdString(contact.get_email());
    case 4: return QString::fromStdString(contact.get_date());
    case 5: return QString::fromStdString(contact.get_address());
    case 6: {
        auto numbers = contact.get_number();
        if (numbers.empty())
            return QString();
        QStringList allNumbers;
        for (const auto &num : numbers) {
            allNumbers << QString::fromStdString(num);
        }
        return allNumbers.join(", ");
    }
    default: return QVariant();
    }
}
QVariant ContactsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();
    switch (section) {
    case 0: return tr("Фамилия");
    case 1: return tr("Имя");
    case 2: return tr("Отчество");
    case 3: return tr("Email");
    case 4: return tr("Дата рождения");
    case 5: return tr("Адрес");
    case 6: return tr("Телефон ");
    default: return QVariant();
    }
}
void ContactsModel::refresh() {
    beginResetModel();
    endResetModel();
}
