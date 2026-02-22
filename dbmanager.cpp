#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
DatabaseManager::DatabaseManager(const QString &host, int port,
                                 const QString &dbName, const QString &user,
                                 const QString &password)
    : m_host(host), m_port(port), m_dbName(dbName), m_user(user), m_password(password)
{
    m_db = QSqlDatabase::addDatabase("QPSQL", "pg_connection");
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

bool DatabaseManager::connect()
{
    if (m_db.isOpen())
        return true;

    m_db.setHostName(m_host);
    m_db.setPort(m_port);
    m_db.setDatabaseName(m_dbName);
    m_db.setUserName(m_user);
    m_db.setPassword(m_password);

    if (!m_db.open()) {
        qDebug() << "Database connection error:" << m_db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::disconnect()
{
    if (m_db.isOpen())
        m_db.close();
}

bool DatabaseManager::isConnected() const
{
    return m_db.isOpen();
}

PhoneBook DatabaseManager::loadAll()
{
    PhoneBook book;
    if (!connect()) {
        qDebug() << "Cannot connect to database";
        return book;
    }
    QSqlQuery query(m_db);
    query.exec("SELECT id, surname, name, patronymic, email, birth_date, address FROM contacts");
    while (query.next()) {
        Contact c;
        int id = query.value(0).toInt();  // ID может пригодиться для связи, но пока не сохраняем
        c.set_surname(query.value(1).toString().toStdString());
        c.set_name(query.value(2).toString().toStdString());
        c.set_patronymic(query.value(3).toString().toStdString());
        c.set_email(query.value(4).toString().toStdString());
        c.set_date(query.value(5).toString().toStdString());
        c.set_address(query.value(6).toString().toStdString());

        // Загружаем телефоны для этого контакта
        QSqlQuery phoneQuery(m_db);
        phoneQuery.prepare("SELECT number FROM phones WHERE contact_id = :id");
        phoneQuery.bindValue(":id", id);
        if (phoneQuery.exec()) {
            while (phoneQuery.next()) {
                c.add_number(phoneQuery.value(0).toString().toStdString());
            }
        }
        book.add_contact_in_book(c);
    }
    return book;
}

bool DatabaseManager::saveAll(const PhoneBook &book)
{
    if (!connect()) {
        qDebug() << "Cannot connect to database";
        return false;
    }
    m_db.transaction();
    QSqlQuery clearPhones(m_db);
    QSqlQuery clearContacts(m_db);
    clearPhones.exec("DELETE FROM phones");
    clearContacts.exec("DELETE FROM contacts");
    if (clearPhones.lastError().isValid() || clearContacts.lastError().isValid()) {
        m_db.rollback();
        return false;
    }
    std::list<Contact> contacts = book.get_all_contact();
    for (const auto &c : contacts) {
        QSqlQuery insertContact(m_db);
        insertContact.prepare("INSERT INTO contacts (surname, name, patronymic, email, birth_date, address) "
            "VALUES (:surname, :name, :patronymic, :email, :birth_date, :address) RETURNING id");
        insertContact.bindValue(":surname", QString::fromStdString(c.get_surname()));
        insertContact.bindValue(":name", QString::fromStdString(c.get_name()));
        insertContact.bindValue(":patronymic", QString::fromStdString(c.get_patronymic()));
        insertContact.bindValue(":email", QString::fromStdString(c.get_email()));
        insertContact.bindValue(":birth_date", QString::fromStdString(c.get_date()));
        insertContact.bindValue(":address", QString::fromStdString(c.get_address()));
        if (!insertContact.exec()) {
            m_db.rollback();
            return false;
        }
        int contactId = insertContact.lastInsertId().toInt();
        for (const auto &num : c.get_number()) {
            QSqlQuery insertPhone(m_db);
            insertPhone.prepare("INSERT INTO phones (contact_id, number) VALUES (:contact_id, :number)");
            insertPhone.bindValue(":contact_id", contactId);
            insertPhone.bindValue(":number", QString::fromStdString(num));
            if (!insertPhone.exec()) {
                m_db.rollback();
                return false;
            }
        }
    }
    m_db.commit();
    return true;
}
bool DatabaseManager::addContact(const Contact &contact)
{
    if (!connect()) return false;
    m_db.transaction();
    QSqlQuery insertContact(m_db);
    insertContact.prepare(
        "INSERT INTO contacts (surname, name, patronymic, email, birth_date, address) "
        "VALUES (:surname, :name, :patronymic, :email, :birth_date, :address) RETURNING id");
    insertContact.bindValue(":surname", QString::fromStdString(contact.get_surname()));
    insertContact.bindValue(":name", QString::fromStdString(contact.get_name()));
    insertContact.bindValue(":patronymic", QString::fromStdString(contact.get_patronymic()));
    insertContact.bindValue(":email", QString::fromStdString(contact.get_email()));
    insertContact.bindValue(":birth_date", QString::fromStdString(contact.get_date()));
    insertContact.bindValue(":address", QString::fromStdString(contact.get_address()));

    if (!insertContact.exec()) {
        m_db.rollback();
        return false;
    }
    int contactId = insertContact.lastInsertId().toInt();
    for (const auto &num : contact.get_number()) {
        QSqlQuery insertPhone(m_db);
        insertPhone.prepare("INSERT INTO phones (contact_id, number) VALUES (:contact_id, :number)");
        insertPhone.bindValue(":contact_id", contactId);
        insertPhone.bindValue(":number", QString::fromStdString(num));
        if (!insertPhone.exec()) {
            m_db.rollback();
            return false;
        }
    }
    m_db.commit();
    return true;
}
bool DatabaseManager::updateContact(int contactId, const Contact &contact)
{
    Q_UNUSED(contactId);
    Q_UNUSED(contact);
    return false;
}

bool DatabaseManager::deleteContact(int contactId)
{
    Q_UNUSED(contactId);
    return false;
}
