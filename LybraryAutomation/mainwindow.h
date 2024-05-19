#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPalette>
#include <QIcon>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void set_palette();
    void set_icon();
    void prepare_data_base();

    QIcon my_image;
    QPixmap pixmap_to_image;
    QSqlDatabase data_base = QSqlDatabase :: addDatabase("QSQLITE");
    QSqlTableModel *department_model;

private slots:
    void on_pushButton_member_transections_clicked();

    void on_pushButton_book_tansections_clicked();

    void on_pushButton_borrowing_transections_clicked();

    void on_pushButton_borrow_delivering_transections_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
