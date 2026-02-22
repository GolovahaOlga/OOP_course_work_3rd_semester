#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "libaddressbook.h"
#include "libQTfile.h"
#include "libQTmodelcontacts.h"

QT_BEGIN_NAMESPACE
class QTableView;
class QAction;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void addContact();
    void editContact();
    void deleteContact();
    void searchContacts();
    void sortContacts();
    void saveToFile();
    void loadFromFile();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void setupTable();

    PhoneBook m_book;
    ContactsModel *m_model;
    QTableView *m_tableView;
    File m_file;

    QAction *addAct;
    QAction *editAct;
    QAction *deleteAct;
    QAction *searchAct;
    QAction *sortAct;
    QAction *saveAct;
    QAction *exitAct;
};

#endif
