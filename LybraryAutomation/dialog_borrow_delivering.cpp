#include "dialog_borrow_delivering.h"
#include "ui_dialog_borrow_delivering.h"

Dialog_borrow_delivering::Dialog_borrow_delivering(QSqlDatabase data_base_for_delivering, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_borrow_delivering)
{
    ui->setupUi(this);
    query_to_delivering = new QSqlQuery(data_base_for_delivering);
    prepare_topic();
    display();

}

Dialog_borrow_delivering::~Dialog_borrow_delivering()
{
    delete ui;
}

void Dialog_borrow_delivering::display()
{
    query_to_delivering -> prepare("select * from borrowed");
    if(!query_to_delivering -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_delivering -> lastError().text(), "ok");
        return;
    }
    model_to_borrowed = new QSqlQueryModel();
    if(query_to_delivering -> next())
        model_to_borrowed -> setQuery(*query_to_delivering);
    ui -> tableView_borrowed_books -> setModel(model_to_borrowed);

    query_to_delivering -> prepare("select * from borrow_delivery");
    if(!query_to_delivering -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_delivering -> lastError().text(), "ok");
        return;
    }
    model_to_delivered = new QSqlQueryModel();
    if(query_to_delivering -> next())
        model_to_delivered -> setQuery(*query_to_delivering);
    ui -> tableView_delivered_books -> setModel(model_to_delivered);
}

void Dialog_borrow_delivering::prepare_topic()
{
    QPalette sample_palette;

    sample_palette.setColor(QPalette::WindowText, Qt::red);
    ui -> label_topic -> setPalette(sample_palette);
}

void Dialog_borrow_delivering::on_tableView_borrowed_books_clicked(const QModelIndex &index)
{
    ui -> lineEdit_book_no -> setText(model_to_borrowed -> index(index.row(), 1).data().toString());
    ui -> lineEdit_member_no -> setText(model_to_borrowed -> index(index.row(), 0).data().toString());
}


void Dialog_borrow_delivering::on_pushButton_deliver_borrowed_clicked()
{
    int row_count = 0, days, dept;
    QDate date_of_receipt, date_of_delivery;
    QSqlQueryModel *model_to_deliver_borrowed = new QSqlQueryModel();
    QSqlQueryModel *model_to_number_of_book = new QSqlQueryModel();
    QSqlQueryModel *model_to_remove_from_borrowed;
    QString date;

    if(ui -> lineEdit_book_no -> text() == "" || ui -> lineEdit_member_no -> text() == "")
    {
        QMessageBox :: warning(this, "Warning!", "You can not save this registry!"
                                                 "\nFill in the required fields.", "ok");
        return;
    }
    query_to_delivering -> prepare("select * from book");
    if(!query_to_delivering -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_delivering -> lastError().text(), "ok");
        return;
    }
    model_to_number_of_book -> setQuery(*query_to_delivering);
    model_to_number_of_book -> index(0, 0).data();
    while (query_to_delivering)
    {
        if(model_to_number_of_book -> index(row_count, 0).data().toInt() == ui -> lineEdit_book_no -> text().toInt())
        {
            number_of_book = model_to_number_of_book -> index(row_count, 2).data().toInt();
            query_to_delivering -> prepare("update book set number_of_books = ? where book_no = ?");
            query_to_delivering -> addBindValue(++number_of_book);
            query_to_delivering -> addBindValue(ui -> lineEdit_book_no -> text().toInt());
            if(!query_to_delivering -> exec())
            {
                QMessageBox :: critical(this, "Error!", query_to_delivering -> lastError().text(), "ok");
                return;
            }
            break;
        }
        row_count++;
    }
    row_count = 0;
    query_to_delivering -> prepare("select * from borrowed");
    if(!query_to_delivering -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_delivering -> lastError().text(), "ok");
        return;
    }
    model_to_deliver_borrowed -> setQuery(*query_to_delivering);
    model_to_deliver_borrowed -> index(0, 0).data();
    while (query_to_delivering)
    {
        if(model_to_deliver_borrowed -> index(row_count, 0).data().toInt() == ui -> lineEdit_member_no -> text().toInt())
        {
            date = model_to_deliver_borrowed -> index(row_count, 2).data().toString();
            date_of_receipt.setDate(date.split(".")[2].toInt(), date.split(".")[1].toInt(), date.split(".")[0].toInt());
            date_of_delivery = ui -> dateEdit_delivery -> date();
            days = date_of_receipt.daysTo(date_of_delivery);
            if(days > 15)
                dept = (days - 15) * 1;
            else
                dept = 0;
            query_to_delivering -> prepare("insert into borrow_delivery (member_no, book_no, date_of_receipt, date_of_delivery, dept) "
                                           " values (?, ?, ?, ?, ?)");
            query_to_delivering -> addBindValue(ui -> lineEdit_member_no -> text().toInt());
            query_to_delivering -> addBindValue(ui -> lineEdit_book_no -> text().toInt());
            query_to_delivering -> addBindValue(date_of_receipt.toString());
            query_to_delivering -> addBindValue(date_of_delivery.toString());
            query_to_delivering -> addBindValue(dept);
            if(!query_to_delivering -> exec())
            {
                QMessageBox :: critical(this, "Error!", query_to_delivering -> lastError().text(), "ok");
                return;
            }


            query_to_delivering -> prepare("delete from borrowed where member_no = ? and book_no = ?");
            query_to_delivering -> bindValue(0, ui -> lineEdit_member_no -> text().toInt());
            query_to_delivering -> bindValue(1, ui -> lineEdit_book_no -> text().toInt());
            //query_to_delivering -> addBindValue(ui -> lineEdit_member_no -> text().toInt());
            //query_to_delivering -> addBindValue(ui -> lineEdit_book_no -> text().toInt());
            if(!query_to_delivering -> exec())
            {
                QMessageBox :: critical(this, "Error!", query_to_delivering -> lastError().text(), "ok");
                return;
            }
            ui -> lineEdit_book_no -> setText("");
            ui -> lineEdit_member_no -> setText("");
            display();
            break;
        }
        row_count++;
    }
}

