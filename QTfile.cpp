#include "libQTfile.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
File::File(const QString &filename)
    : m_filename(filename)
{}
bool File::save(const PhoneBook &book) {
    QFile file(m_filename);
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           return false;

       QTextStream out(&file);
       out.setCodec("UTF-8");
       std::list<Contact> contacts = book.get_all_contact();
       for (const auto &c : contacts) {
           out << contactToLine(c) << "\n";
       }
       file.close();
       return true;
}
PhoneBook File::load() {
    PhoneBook book;
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return book;
    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        Contact c = lineToContact(line);
        if (!c.get_name().empty()) {
            book.add_contact_in_book(c);
        }
    }
    file.close();
    return book;
}
QString File::contactToLine(const Contact &c) const {
    QStringList parts;
    parts << QString::fromStdString(c.get_surname())
          << QString::fromStdString(c.get_name())
          << QString::fromStdString(c.get_patronymic())
          << QString::fromStdString(c.get_email())
          << QString::fromStdString(c.get_date())
          << QString::fromStdString(c.get_address());
    for (const auto &num : c.get_number()) {
        parts << QString::fromStdString(num);
    }
    return parts.join('|');
}
Contact File::lineToContact(const QString &line) const {
    QStringList parts = line.split('|', Qt::KeepEmptyParts);
    if (parts.size() < 6)
        return Contact();

    Contact c;
    int idx = 0;
    c.set_surname(parts[idx++].toStdString());
    c.set_name(parts[idx++].toStdString());
    c.set_patronymic(parts[idx++].toStdString());
    c.set_email(parts[idx++].toStdString());
    c.set_date(parts[idx++].toStdString());
    c.set_address(parts[idx++].toStdString());
    while (idx < parts.size()) {
            c.add_number(parts[idx++].toStdString());
        }
    return c;
}
