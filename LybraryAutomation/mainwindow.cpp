#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog_member.h"
#include "dialog_book.h"
#include "dialog_borrowing.h"
#include "dialog_borrow_delivering.h"
//hocam veri tabanının dizinini degistirmeyi unutmayin lutfen
//mainwindow.cpp satir 61'de
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
    data_base.setDatabaseName("C:/Users/Public/Qt/odev_2_kopya/odev_2/lybrary.db");
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

