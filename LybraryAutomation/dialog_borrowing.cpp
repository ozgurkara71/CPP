#include "dialog_borrowing.h"
#include "ui_dialog_borrowing.h"

Dialog_borrowing::Dialog_borrowing(QSqlDatabase data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_borrowing)
{
    ui->setupUi(this);
    query_to_borrowing = new QSqlQuery(data_base);
    prepare_topic();
    display();
}

Dialog_borrowing::~Dialog_borrowing()
{
    delete ui;
}

void Dialog_borrowing::display()
{
    query_to_borrowing -> prepare("select * from borrowed");
    if(!query_to_borrowing -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
        return;
    }
    model_to_borrowing = new QSqlQueryModel();
    model_to_borrowing -> setQuery(*query_to_borrowing);
    ui -> tableView_borrowed_books -> setModel(model_to_borrowing);

    query_to_borrowing -> prepare("select * from member");
    if(!query_to_borrowing -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
        return;
    }
    model_to_borrowing = new QSqlQueryModel();
    model_to_borrowing -> setQuery(*query_to_borrowing);
    ui -> tableView_members -> setModel(model_to_borrowing);

    query_to_borrowing -> prepare("select * from book");
    if(!query_to_borrowing -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
        return;
    }
    model_to_borrowing = new QSqlQueryModel();
    model_to_borrowing -> setQuery(*query_to_borrowing);
    ui -> tableView_books -> setModel(model_to_borrowing);
}

void Dialog_borrowing::prepare_topic()
{
    QPalette sample_palette;

    sample_palette.setColor(QPalette::WindowText, Qt::red);
    ui -> label_topic -> setPalette(sample_palette);
}

void Dialog_borrowing::on_tableView_members_clicked(const QModelIndex &index)
{
    query_to_borrowing -> prepare("select * from member");
    if(!query_to_borrowing -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
        return;
    }
    model_to_borrowing = new QSqlQueryModel();
    model_to_borrowing -> setQuery(*query_to_borrowing);
    ui -> lineEdit_member_no -> setText(model_to_borrowing -> index(index.row(), 0).data().toString());
}


void Dialog_borrowing::on_tableView_books_clicked(const QModelIndex &index)
{
    query_to_borrowing -> prepare("select * from book");
    if(!query_to_borrowing -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
        return;
    }
    model_to_borrowing = new QSqlQueryModel();
    model_to_borrowing -> setQuery(*query_to_borrowing);
    ui -> lineEdit_book_no_2 -> setText(model_to_borrowing -> index(index.row(), 0).data().toString());
    number_of_book = model_to_borrowing -> index(index.row(), 2).data().toInt();
}


void Dialog_borrowing::on_pushButton_borrow_clicked()
{
    int row_count = 0;

    if(ui -> lineEdit_book_no_2 -> text() == "" || ui -> lineEdit_member_no -> text() == "")
    {
        QMessageBox :: warning(this, "Warning!", "You can not save this registry!"
                                                 "\nFill in the required fields.", "ok");
        return;
    }
    query_to_borrowing -> prepare("select * from borrowed where member_no = ?");
    query_to_borrowing -> addBindValue(ui -> lineEdit_member_no -> text().toInt());
    if(!query_to_borrowing -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
        return;
    }
    if(query_to_borrowing -> next())
    {
        query_to_borrowing -> prepare("select * from borrowed where book_no = ?");
        query_to_borrowing -> addBindValue(ui -> lineEdit_book_no_2 -> text().toInt());
        if(!query_to_borrowing -> exec())
        {
            QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
            return;
        }
        //model_to_borrowing = new QSqlQueryModel();
        //model_to_borrowing -> setQuery(*query_to_borrowing);
        while(query_to_borrowing -> next())
        {
            //alttakileri disari al???
            model_to_borrowing = new QSqlQueryModel();
            model_to_borrowing -> setQuery(*query_to_borrowing);
            //coloumn 1 is member_no and coloumn 2 is book_no
            if(model_to_borrowing -> index(row_count, 0).data() == ui -> lineEdit_member_no -> text().toInt() &&
                    model_to_borrowing -> index(row_count, 1).data() == ui -> lineEdit_book_no_2 -> text().toInt())
            {
                QMessageBox :: warning(this, "Warning!", "This member can not borrow this book!"
                                                         "\nThis member has already borrowed one of these books.", "ok");
                return;
            }
            row_count++;
        }

    }
    //checking stock number
    if(number_of_book <= 0)
    {
        QMessageBox :: warning(this, "Warning!", "This member can not borrow this book!"
                                                 "\nThis book is out of stock.", "ok");
        return;
    }
    else
    {
        query_to_borrowing -> prepare("update book set number_of_books = ? where book_no = ?");
        query_to_borrowing -> addBindValue(--number_of_book);
        query_to_borrowing -> addBindValue(ui -> lineEdit_book_no_2 -> text().toInt());
        if(!query_to_borrowing -> exec())
        {
            QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
            return;
        }

    }
    //inserting data to borrowed table
    query_to_borrowing -> prepare("insert into borrowed (member_no, book_no, borrowing_date) values (?, ?, ?)");
    query_to_borrowing -> addBindValue(ui -> lineEdit_member_no -> text().toInt());
    query_to_borrowing -> addBindValue(ui-> lineEdit_book_no_2 -> text());
    query_to_borrowing -> addBindValue(ui -> dateEdit -> text());
    if(!query_to_borrowing -> exec())
    {
        QMessageBox :: critical(this, "Error!", query_to_borrowing -> lastError().text(), "ok");
        return;
    }
    display();
}

