#ifndef DIALOG_MEMBER_H
#define DIALOG_MEMBER_H

#include <QDialog>
#include <QPalette>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class Dialog_member;
}

class Dialog_member : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_member(QSqlDatabase, QWidget *parent = nullptr);
    ~Dialog_member();
    void display();
    void prepare_topic();

private slots:
    void on_pushButton_new_registry_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_update_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::Dialog_member *ui;

    QSqlQuery *query;
    QSqlQueryModel *member_model;
};

#endif // DIALOG_MEMBER_H
