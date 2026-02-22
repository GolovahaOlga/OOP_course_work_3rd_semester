#include "mainwindow.h"
#include "QTaddeditdialog.h"
#include "searchdialog.h"
#include <QTableView>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QCloseEvent>
#include <QHeaderView>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_file("contacts.txt")
{
    loadFromFile();
    m_model = new ContactsModel(m_book, this);
    setupTable();
    createActions();
    createMenus();
    createToolBar();
    statusBar()->showMessage(tr("Готово"));
}

MainWindow::~MainWindow() {}

void MainWindow::setupTable() {
    m_tableView = new QTableView(this);
    m_tableView->setModel(m_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->resizeColumnsToContents();
    setCentralWidget(m_tableView);
}

void MainWindow::createActions()
{
    addAct = new QAction(tr("&Добавить"), this);
    addAct->setShortcut(QKeySequence::New);
    connect(addAct, &QAction::triggered, this, &MainWindow::addContact);

    editAct = new QAction(tr("&Редактировать"), this);
    editAct->setShortcut(QKeySequence::Open);
    connect(editAct, &QAction::triggered, this, &MainWindow::editContact);

    deleteAct = new QAction(tr("&Удалить"), this);
    deleteAct->setShortcut(QKeySequence::Delete);
    connect(deleteAct, &QAction::triggered, this, &MainWindow::deleteContact);

    searchAct = new QAction(tr("&Поиск"), this);
    searchAct->setShortcut(QKeySequence::Find);
    connect(searchAct, &QAction::triggered, this, &MainWindow::searchContacts);

    sortAct = new QAction(tr("&Сортировка"), this);
    connect(sortAct, &QAction::triggered, this, &MainWindow::sortContacts);

    saveAct = new QAction(tr("&Сохранить"), this);
    saveAct->setShortcut(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveToFile);

    exitAct = new QAction(tr("&Выход"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    QMenu *editMenu = menuBar()->addMenu(tr("&Правка"));
    editMenu->addAction(addAct);
    editMenu->addAction(editAct);
    editMenu->addAction(deleteAct);

    QMenu *searchMenu = menuBar()->addMenu(tr("&Поиск"));
    searchMenu->addAction(searchAct);

    QMenu *sortMenu = menuBar()->addMenu(tr("&Сортировка"));
    sortMenu->addAction(sortAct);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar(tr("Main"));
    toolBar->addAction(addAct);
    toolBar->addAction(editAct);
    toolBar->addAction(deleteAct);
    toolBar->addSeparator();
    toolBar->addAction(searchAct);
    toolBar->addAction(sortAct);
    toolBar->addSeparator();
    toolBar->addAction(saveAct);
}

void MainWindow::addContact() {
    AddEditDialog dlg(this, false);
    if (dlg.exec() == QDialog::Accepted) {
        Contact newContact = dlg.getContact();
        m_book.add_contact_in_book(newContact);
        m_model->refresh();
        m_tableView->resizeColumnsToContents();
        saveToFile();
    }
}

void MainWindow::editContact() {
    QModelIndex current = m_tableView->currentIndex();
    if (!current.isValid()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Выберите контакт для редактирования."));
        return;
    }
    int row = current.row();
    Contact contact = m_book.find_contact_by_serial_number(row);
    AddEditDialog dlg(this, true);
    dlg.setContact(contact);
    if (dlg.exec() == QDialog::Accepted) {
        Contact edited = dlg.getContact();
        m_book.delete_contact_from_book(row);
        m_book.add_contact_in_book(edited);
        m_model->refresh();
        saveToFile();
    }
}

void MainWindow::deleteContact() {
    QModelIndex current = m_tableView->currentIndex();
    if (!current.isValid()) return;
    int row = current.row();
    if (QMessageBox::question(this, tr("Подтверждение"),
                               tr("Удалить выбранный контакт?"),
                               QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_book.delete_contact_from_book(row);
        m_model->refresh();
        saveToFile();
    }
}

void MainWindow::searchContacts() {
    SearchDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        QString name = dlg.getName();
        QString surname = dlg.getSurname();
        QString email = dlg.getEmail();
        list<string> positions;
        positions.push_back(name.toStdString());
        positions.push_back(surname.toStdString());
        positions.push_back(email.toStdString());
        list<Contact> results = m_book.search_contact_in_book(positions);
        if (results.empty()) {
            QMessageBox::information(this, tr("Поиск"), tr("Контакты не найдены."));
        } else {
            QMessageBox::information(this, tr("Результат"),
                                     tr("Найдено %1 контакт(ов).").arg(results.size()));
        }
    }
}

void MainWindow::sortContacts() {
    QStringList items;
    items << tr("Фамилия") << tr("Имя") << tr("Отчество") << tr("Email")
          << tr("Дата рождения") << tr("Адрес");
    bool ok;
    QString selected = QInputDialog::getItem(this, tr("Сортировка"),
                                              tr("Выберите поле:"), items, 0, false, &ok);
    if (!ok) return;
    int field = items.indexOf(selected) + 1;

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Сортировка"),
                                        tr("Сортировать по возрастанию?"),
                                        QMessageBox::Yes | QMessageBox::No);
    bool ascending = (reply == QMessageBox::Yes);

    m_book.sort_contacts(field, ascending);
    m_model->refresh();
    saveToFile();
}
void MainWindow::saveToFile() {
    if (!m_file.save(m_book)) {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось сохранить файл."));
    } else {
        statusBar()->showMessage(tr("Файл сохранён"), 2000);
    }
}

void MainWindow::loadFromFile() {
    m_book = m_file.load();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}
