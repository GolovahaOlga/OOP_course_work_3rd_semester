#pragma once
#include <QString>
#include "libaddressbook.h"

class File {
public:
    explicit File(const QString &filename = "contacts.txt");
    bool save(const PhoneBook &book);
    PhoneBook load();
private:
    QString m_filename;
    QString contactToLine(const Contact &c) const;
    Contact lineToContact(const QString &line) const;
};
