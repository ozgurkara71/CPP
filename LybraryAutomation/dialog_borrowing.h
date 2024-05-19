#ifndef DIALOG_BORROWING_H
#define DIALOG_BORROWING_H

#include <QDialog>
#include <QPalette>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPalette>
#include <QSqlError>

namespace Ui {
class Dialog_borrowing;
}

class Dialog_borrowing : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_borrowing(QSqlDatabase, QWidget *parent = nullptr);
    ~Dialog_borrowing();

    void display();
    void prepare_topic();

private slots:
    void on_tableView_members_clicked(const QModelIndex &index);

    void on_tableView_books_clicked(const QModelIndex &index);

    void on_pushButton_borrow_clicked();

private:
    Ui::Dialog_borrowing *ui;

    QSqlQuery *query_to_borrowing;
    QSqlQueryModel *model_to_borrowing;
    int number_of_book;
};

#endif // DIALOG_BORROWING_H
