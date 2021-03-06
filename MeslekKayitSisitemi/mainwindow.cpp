#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList jobList = { "Asker", "Polis", "Savci" };
    ui->comboBox->addItems(jobList);

    ui->name->setPlaceholderText("Enter Your Name");
    ui->surname->setPlaceholderText("Enter Your Surname");
    ui->tc_id->setPlaceholderText("Enter Your TC Id");
    ui->username->setPlaceholderText("Enter Your Username");
    ui->password->setPlaceholderText("Enter Your Password");

    ui->username_2->setPlaceholderText("Please Enter Username");
    ui->password_2->setPlaceholderText("Please Enter Password");

    // Connecting to database
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/faruk/OneDrive/Documents/MeslekKayitSisitemi/savePeople.sqlite");
    database.setHostName("localhost");
    database.setUserName("root");
    database.setPassword("");


    //    if(!database.open())
    //        ui->status->setText("Failed to open the database");
    //    else
    //        ui->status->setText("Connected...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerbutton_clicked()
{

    if(database.open()) {

        //retreive data from input fields
        QString name = ui->name->text();
        QString surname = ui->surname->text();
        QString tc_id = ui->tc_id->text();
        QString username = ui->username->text();
        QString password = ui->password->text();


        // Run our insert query

        QSqlQuery qry;

        qDebug() << ui->comboBox->currentText();

        if(ui->comboBox->currentText() == "Asker") {
            qry.prepare("INSERT INTO askerList(name, surname, tc_id, username, password)"
                        "VALUES (:name, :surname, :tc_id, :username, :password)");
        }

        else if(ui->comboBox->currentText() == "Polis") {
            qry.prepare("INSERT INTO polisList(name, surname, tc_id, username, password)"
                        "VALUES (:name, :surname, :tc_id, :username, :password)");
        }

        else {
            qry.prepare("INSERT INTO savciList(name, surname, tc_id, username, password)"
                        "VALUES (:name, :surname, :tc_id, :username, :password)");
        }

        qry.bindValue(":name", name);
        qry.bindValue(":surname", surname);
        qry.bindValue(":tc_id", tc_id);
        qry.bindValue(":username", username);
        qry.bindValue(":password", password);

        if(qry.exec()) {

            QMessageBox::information(this, "Inserted", "Data Inserted Successfully");

        } else {

            // qDebug() << qry.lastError().text();
            QMessageBox::information(this, "Not Inserted", "Data Is Not Inserted");

        }

    } else {
        QMessageBox::information(this, "Not Connected", "Database is not connected");
    }
}

void MainWindow::on_loginbutton_clicked()
{
    QSqlDatabase db;

    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName("C:/Users/faruk/OneDrive/Documents/MeslekKayitSisitemi/savePeople.sqlite");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");

    //retreive data from input fields
    QString username = ui->username_2->text();
    QString password = ui->password_2->text();

    if(db.open()) {

        // Run our insert query
        QSqlQuery qry_asker(QSqlDatabase::database("Connection"));
        QSqlQuery qry_polis(QSqlDatabase::database("Connection"));
        QSqlQuery qry_savci(QSqlDatabase::database("Connection"));

        qry_asker.prepare(QString("SELECT * FROM askerList WHERE username = :username AND password = :password"));
        qry_polis.prepare(QString("SELECT * FROM polisList WHERE username = :username AND password = :password"));
        qry_savci.prepare(QString("SELECT * FROM savciList WHERE username = :username AND password = :password"));

        qry_asker.bindValue(":username", username);
        qry_asker.bindValue(":password", password);

        qry_polis.bindValue(":username", username);
        qry_polis.bindValue(":password", password);

        qry_savci.bindValue(":username", username);
        qry_savci.bindValue(":password", password);


        if(!qry_asker.exec() || !qry_polis.exec() || !qry_savci.exec()) {

            QMessageBox::information(this, "Failed", "Query Failed To Execute");

        } else {

            if(qry_asker.next()) {

                QMessageBox::information(this, "Success", "Dear Asker, Login Success");

                hide();
                mydialog dialog;
                dialog.setModal(true);
                dialog.exec();
            }

            else if(qry_polis.next()) {

                QMessageBox::information(this, "Success", "Dear Polis, Login Success");

                hide();
                mydialog dialog;
                dialog.setModal(true);
                dialog.exec();
            }

            else if(qry_savci.next()) {

                QMessageBox::information(this, "Success", "Dear Savci, Login Success");

                hide();
                mydialog dialog;
                dialog.setModal(true);
                dialog.exec();
            }

            else
            {
                QMessageBox::information(this, "Failed", "Login Failed");
            }
        }
    }

    else {
        QMessageBox::information(this, "Database Failed", "Database Connection Failed");
    }
}


