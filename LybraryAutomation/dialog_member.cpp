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

