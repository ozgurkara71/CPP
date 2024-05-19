#include "dialog_book.h"
#include "ui_dialog_book.h"

Dialog_book::Dialog_book(QSqlDatabase data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_book)
{
    ui->setupUi(this);
    query_to_book = new QSqlQuery(data_base);
    prepare_topic();
    display();
}

Dialog_book::~Dialog_book()
{
    delete ui;
}

void Dialog_book::display()
{
    query_to_book -> prepare("select * from book");
    if(!query_to_book -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_book -> lastError().text(), "ok");
        return;
    }
    book_model = new QSqlQueryModel();
    book_model -> setQuery(*query_to_book);
    ui -> tableView -> setModel(book_model);

}

void Dialog_book::prepare_topic()
{
    QPalette sample_palette;

    sample_palette.setColor(QPalette::WindowText, Qt::red);
    ui -> label_topic -> setPalette(sample_palette);
}

void Dialog_book::on_tableView_clicked(const QModelIndex &index)
{
    ui -> lineEdit_book_no -> setText(book_model -> index(index.row(), 0).data().toString());
    ui -> lineEdit_book_name -> setText(book_model -> index(index.row(), 1).data().toString());
    ui -> lineEdit_stock -> setText(book_model -> index(index.row(), 2).data().toString());

    query_to_book -> prepare("select * from borrowed where book_no = ?");
    query_to_book -> addBindValue(ui -> lineEdit_book_no -> text().toInt());
    if(!query_to_book -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_book -> lastError().text(), "ok");
        return;
    }
    current_model = new QSqlQueryModel();
    if(query_to_book -> next())
        current_model -> setQuery(*query_to_book);
    ui -> tableView_current_situation -> setModel(current_model);
    query_to_book -> prepare("select * from borrow_delivery where book_no = ?");
    query_to_book -> addBindValue(ui -> lineEdit_book_no -> text().toInt());
    if(!query_to_book -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_book -> lastError().text(), "ok");
        return;
    }
    history_model = new QSqlQueryModel();
    if(query_to_book -> next())
        history_model -> setQuery(*query_to_book);
    ui -> tableView_history -> setModel(history_model);

}


void Dialog_book::on_pushButton_new_registry_clicked()
{
    if(ui -> lineEdit_book_name -> text() == "" || ui -> lineEdit_stock -> text() == "" || ui -> lineEdit_stock -> text() == "0")
    {
        QMessageBox :: warning(this, "Warning!", "You can not save this registry!"
                                                 "\nFill in the required fields.", "ok");
        return;
    }
    query_to_book -> prepare("insert into book (book_name, number_of_books) values (?, ?)");
    query_to_book -> addBindValue(ui -> lineEdit_book_name -> text());
    query_to_book -> addBindValue(ui -> lineEdit_stock -> text().toInt());
    if(!query_to_book->exec())
    {
        QMessageBox :: critical(this, "Error!", "Taken error while inserting into book table!", "ok");
        return;
    }
    display();
}


void Dialog_book::on_pushButton_update_clicked()
{
    if(ui -> lineEdit_book_name -> text() == "" || ui -> lineEdit_stock -> text() == "")
    {
        QMessageBox :: warning(this, "Warning!", "You can not save this registry!"
                                                 "\nFill in the required fields.", "ok");
        return;
    }
    query_to_book -> prepare("update book set book_name = ?, number_of_books = ? where book_no = ?");
    query_to_book -> addBindValue(ui -> lineEdit_book_name -> text());
    query_to_book -> addBindValue(ui -> lineEdit_stock -> text().toInt());
    query_to_book -> addBindValue(ui -> lineEdit_book_no -> text().toInt());
    if(!query_to_book -> exec())
    {
        QMessageBox :: critical(this, "Error!", "Taken error while updating the book table!", "ok");
        return;
    }
    display();
}


void Dialog_book::on_pushButton_delete_clicked()
{
    query_to_book -> prepare("select * from borrowed where book_no = ?");
    query_to_book -> addBindValue(ui -> lineEdit_book_no -> text().toInt());
    if(!query_to_book -> exec())
    {
        QMessageBox :: critical(this, "Error!", "Taken error while taking data from the book table!", "ok");
        return;
    }
    if(query_to_book -> next())
    {
        QMessageBox :: warning(this, "Warning!", "You can not delete this book!"
                                                 "\nThis book is borrowed.", "ok");
        return;
    }
    query_to_book -> prepare("delete from book where book_no = ?");
    query_to_book -> addBindValue(ui -> lineEdit_book_no -> text().toInt());
    if(!query_to_book -> exec())
    {
        QMessageBox :: critical(this, "Error!", "Taken error while deleting data from the book table!", "ok");
        return;
    }
    display();
}

