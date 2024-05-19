#ifndef DIALOG_BORROW_DELIVERING_H
#define DIALOG_BORROW_DELIVERING_H

#include <QDialog>
#include <QPalette>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>

namespace Ui {
class Dialog_borrow_delivering;
}

class Dialog_borrow_delivering : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_borrow_delivering(QSqlDatabase, QWidget *parent = nullptr);
    ~Dialog_borrow_delivering();

    void display();
    void prepare_topic();

private slots:
    void on_tableView_borrowed_books_clicked(const QModelIndex &index);

    void on_pushButton_deliver_borrowed_clicked();

private:
    Ui::Dialog_borrow_delivering *ui;

    QSqlQuery *query_to_delivering;
    QSqlQueryModel *model_to_borrowed;
    QSqlQueryModel *model_to_delivered;
    int number_of_book;
};

#endif // DIALOG_BORROW_DELIVERING_H
