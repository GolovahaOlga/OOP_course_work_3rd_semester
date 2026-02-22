#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include "libaddressbook.h"

class DatabaseManager
{
public:
    DatabaseManager(const QString &host = "localhost",
                    int port = 5432,
                    const QString &dbName = "phonebook",
                    const QString &user = "postgres",
                    const QString &password = "");
    ~DatabaseManager();
    bool connect();
    void disconnect();
    bool isConnected() const;
    PhoneBook loadAll();
    bool saveAll(const PhoneBook &book);
    bool addContact(const Contact &contact);
    bool updateContact(int contactId, const Contact &contact);
    bool deleteContact(int contactId);

private:
    QSqlDatabase m_db;
    QString m_host;
    int m_port;
    QString m_dbName;
    QString m_user;
    QString m_password;
};

#endif // DBMANAGER_H
