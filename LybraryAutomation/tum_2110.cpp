//dialog_book.cpp
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
//dialog_borrow_delivering.cpp
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

//dialog_borrowing.cpp
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

//dialog_member.cpp
#include "dialog_member.h"
#include "ui_dialog_member.h"

Dialog_member::Dialog_member(QSqlDatabase data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_member)
{
    ui->setupUi(this);
    query = new QSqlQuery(data_base);
    prepare_topic();
    display();
}

Dialog_member::~Dialog_member()
{
    delete ui;
}

void Dialog_member::display()
{
    query -> prepare("select * from member");
    if(!query -> exec())
    {
        QMessageBox :: critical(this, "Error!", query -> lastError().text(), "ok");
        return;
    }
    member_model = new QSqlQueryModel();
    member_model -> setQuery(*query);
    ui -> tableView -> setModel(member_model);
}

void Dialog_member::prepare_topic()
{
    QPalette sample_palette;

    sample_palette.setColor(QPalette::WindowText, Qt::red);
    ui -> label_topic -> setPalette(sample_palette);
}

void Dialog_member::on_pushButton_new_registry_clicked()
{
    if(ui -> lineEdit_member_name -> text() == "" || ui -> lineEdit_member_surname -> text() == "")
    {
        QMessageBox :: warning(this, "Warning!", "You can not save this registry!"
                                               "\nFill in the required fields.", "ok");
        return;
    }
    query -> prepare("insert into member (member_name, member_surname) values (?, ?)");
    query -> addBindValue(ui -> lineEdit_member_name -> text());
    query -> addBindValue(ui -> lineEdit_member_surname -> text());
    if(!query -> exec())
    {
        QMessageBox :: critical(this, "Error!", query -> lastError().text(), "ok");
        return;
    }
    display();
}


void Dialog_member::on_tableView_clicked(const QModelIndex &index)
{
    ui -> lineEdit_member_no -> setText(member_model -> index(index.row(), 0).data().toString());
    ui -> lineEdit_member_name -> setText(member_model -> index(index.row(), 1).data().toString());
    ui -> lineEdit_member_surname -> setText(member_model -> index(index.row(), 2).data().toString());
}


void Dialog_member::on_pushButton_update_clicked()
{
    if(ui -> lineEdit_member_name -> text() == "" || ui -> lineEdit_member_surname -> text() == "")
    {
        QMessageBox :: warning(this, "Warning!", "You can not save this registry!"
                                               "\nFill in the required fields.", "ok");
        return;
    }
    query -> prepare("update member set member_name = ?, member_surname = ? where member_no = ?");
    query -> addBindValue(ui -> lineEdit_member_name -> text());
    query -> addBindValue(ui -> lineEdit_member_surname -> text());
    query -> addBindValue(ui -> lineEdit_member_no -> text().toInt());
    if(!query -> exec())
    {
        QMessageBox :: critical(this, "Error!", query -> lastError().text(), "ok");
        return;
    }
    display();
}


void Dialog_member::on_pushButton_delete_clicked()
{
    //kitabi var mi kontrol et
    query -> prepare("select * from borrowed where member_no = ?");
    query -> addBindValue(ui -> lineEdit_member_no -> text().toInt());
    if(!query->exec())
    {
        QMessageBox :: critical(this, "Error!", query -> lastError().text(), "ok");
        return;
    }
    if(query -> next())
    {
        QMessageBox :: warning(this, "Error!", "You can not delete this member!"
                                                 "\nThis member has got borrowed books which did not delivered back.", "ok");
        return;
    }
    query -> prepare("delete from member where member_no = ?");
    query -> addBindValue(ui -> lineEdit_member_no -> text().toInt());
    if(!query->exec())
    {
        QMessageBox :: critical(this, "Error!", query -> lastError().text(), "ok");
        return;
    }
    display();
}
//main.cpp
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(w.width(), w.height());
    w.show();
    return a.exec();
}

//mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog_member.h"
#include "dialog_book.h"
#include "dialog_borrowing.h"
#include "dialog_borrow_delivering.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_palette();
    set_icon();
    prepare_data_base();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_palette()
{
    QPalette sample_palette;

    sample_palette.setColor(QPalette::WindowText, Qt::red);
    ui -> label_topic -> setPalette(sample_palette);
}

void MainWindow::set_icon()
{
    //burayi duzelt
    if(pixmap_to_image.load(":/components/user.jpg"))
        my_image.addPixmap(pixmap_to_image);
    ui -> pushButton_member_transections -> setIcon(pixmap_to_image);
    ui -> pushButton_member_transections -> setIconSize(QSize(150, 150));
    ui -> pushButton_member_transections -> setFixedSize(QSize(150, 150));
    if(pixmap_to_image.load(":/components/book.jpg"))
        my_image.addPixmap(pixmap_to_image);
    ui -> pushButton_book_tansections -> setIcon(pixmap_to_image);
    ui -> pushButton_book_tansections -> setIconSize(QSize(150, 150));
    ui -> pushButton_book_tansections -> setFixedSize(QSize(150, 150));
    if(pixmap_to_image.load(":/components/odunc_1.jpg"))
        my_image.addPixmap(pixmap_to_image);
    ui -> pushButton_borrowing_transections -> setIcon(pixmap_to_image);
    ui -> pushButton_borrowing_transections -> setIconSize(QSize(150, 150));
    ui -> pushButton_borrowing_transections -> setFixedSize(QSize(150, 150));
    if(pixmap_to_image.load(":/components/odunc_2.jpg"))
        my_image.addPixmap(pixmap_to_image);
    ui -> pushButton_borrow_delivering_transections -> setIcon(pixmap_to_image);
    ui -> pushButton_borrow_delivering_transections -> setIconSize(QSize(150, 150));
    ui -> pushButton_borrow_delivering_transections -> setFixedSize(QSize(150, 150));

}

void MainWindow::prepare_data_base()
{
    data_base.setDatabaseName("C:/Users/Public/Qt/Programlar/odev_2/lybrary.db");
    if(!data_base.open())
        ui -> statusbar -> showMessage("Database error!");
    else
        ui -> statusbar -> showMessage("Connected to database.");
}


void MainWindow::on_pushButton_member_transections_clicked()
{
    Dialog_member *member_screen = new Dialog_member(data_base);
    member_screen -> setFixedSize(member_screen -> width(), member_screen -> height());
    member_screen -> exec();
}


void MainWindow::on_pushButton_book_tansections_clicked()
{
    Dialog_book *book_screen = new Dialog_book(data_base);
    book_screen -> setFixedSize(book_screen -> width(), book_screen -> height());
    book_screen -> exec();
}


void MainWindow::on_pushButton_borrowing_transections_clicked()
{
    Dialog_borrowing *borrowing_screen = new Dialog_borrowing(data_base);

    borrowing_screen -> setFixedSize(borrowing_screen -> width(), borrowing_screen -> height());
    borrowing_screen -> exec();
}


void MainWindow::on_pushButton_borrow_delivering_transections_clicked()
{
    Dialog_borrow_delivering *borrow_delivering_screen = new Dialog_borrow_delivering(data_base);
    borrow_delivering_screen -> setFixedSize(borrow_delivering_screen -> width(), borrow_delivering_screen -> height());
    borrow_delivering_screen -> exec();
}

