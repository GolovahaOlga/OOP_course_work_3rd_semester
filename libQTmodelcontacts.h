#pragma once
#include <QAbstractTableModel>
#include "libaddressbook.h"
class ContactsModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ContactsModel(PhoneBook &book, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void refresh();
private:
    PhoneBook &m_book;
};
