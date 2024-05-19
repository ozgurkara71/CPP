#ifndef DIALOG_BOOK_H
#define DIALOG_BOOK_H

#include <QDialog>
#include <QPalette>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class Dialog_book;
}

class Dialog_book : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_book(QSqlDatabase, QWidget *parent = nullptr);
    ~Dialog_book();

    void display();
    void prepare_topic();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_new_registry_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::Dialog_book *ui;

    QSqlQuery *query_to_book;
    QSqlQueryModel *book_model;
    QSqlQueryModel *current_model;
    QSqlQueryModel *history_model;
};

#endif // DIALOG_BOOK_H
