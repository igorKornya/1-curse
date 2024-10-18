#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QCoreApplication>
#include <QRegExpValidator>
#include <QFileDialog>
#include <qdebug.h>
#include "customfiledialog.h"
#include "Huffman.h"
#include "userAccess.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadUsersFromFile();
    QRegExp noSpaceRegExp("\\S*");
    QRegExpValidator* noSpaceValidator = new QRegExpValidator(noSpaceRegExp, this);
    ui->loginLog->setValidator(noSpaceValidator);
    ui->passwordLog->setValidator(noSpaceValidator);

    form = new Form;
    huffman = new Huffman;
    admin = new adminpanel;

    connect(this, &MainWindow::signal, form, &Form::slot);
    connect(this, &MainWindow::signal_to_admin, admin, &adminpanel::admin_slot);
    connect(form, &Form::signal_back, this, &MainWindow::show);
    connect(admin,&adminpanel::signal_to_main, this, &MainWindow::show);
    connect(form, &Form::updateComboBoxRequested, admin, &adminpanel::updateComboBox);
}

void MainWindow::loadUsersWithAccess()
{
    QString filePath = "D:/user_access_levels.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList userAccessData = line.split(",");
            if (userAccessData.size() == 2)
            {
                UserAccess userAccess(userAccessData.at(0), userAccessData.at(1));
                userAccessList.append(userAccess);
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
    delete huffman;
}
void MainWindow::loadUsersFromFile()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/users.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList userData = line.split(",");
            if (userData.size() == 5)
            {
                User user(userData.at(0), userData.at(1), userData.at(2), userData.at(3), userData.at(4));
                userList.append(user);
            }
        }

        file.close();
    }
}

QString MainWindow::compressString(const QString &password)
{
    huffman->buildHuffmanTree(password.toStdString());
    string encodedString = huffman->getHuffmanEncoding(password.toStdString());
    return QString::fromStdString(encodedString);
}

QString MainWindow::decompressString(const QString &compressedPassword)
{
    string encodedString = compressedPassword.toStdString();
    string decodedString = huffman->decodeHuffmanEncoding(encodedString);
    return QString::fromStdString(decodedString);
}

void MainWindow::on_LogInButton_clicked()
{
    QString login = ui->loginLog->text();
    QString password = ui->passwordLog->text();
    if (login.isEmpty() || password.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Please fill in all fields.");
        return;
    }
    QString compressedPassword = compressString(password);
    QString compressedLogin = compressString(login);
    bool userFound = false;

    QString filePath = QCoreApplication::applicationDirPath() + "/users.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList userData = line.split(",");
            if (userData.size() == 5)
            {
                QString fileLogin = userData.at(0);
                QString filePassword = userData.at(1);

                if (fileLogin == compressedLogin && filePassword == compressedPassword)
                {
                    userFound = true;
                    break;
                }
            }
        }

        file.close();
    }

    if (login == "admin" && password == "root")
    {
        QMessageBox::information(this, "Success", "Your root confirmed, " + login + "!");
        QFileDialog::getOpenFileName(this, "Файловая система администратора", "D:/", "All files");
        ui->loginLog->clear();
        ui->passwordLog->clear();
    }
    else if (login == "admin" && password == "admin")
    {
        QMessageBox::information(this, "adminPanel", "Welcome to adminPanel!");
        ui->loginLog->clear();
        ui->passwordLog->clear();
        this->close();
        emit signal_to_admin();
    }
    else if (userFound)
    {
        QString filePath = "D:/user_access_levels.txt";
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);

            while (!in.atEnd())
            {
                QString line = in.readLine();
                QStringList userAccessData = line.split(",");
                if (userAccessData.size() == 2)
                {
                    QString fileUsername = userAccessData.at(0);
                    QString fileAccess = userAccessData.at(1);
                    int accessLevel = fileAccess.toInt();
                    if (login == fileUsername)
                    {
                        QMessageBox::information(this, "Success", "Welcome back, " + login + "!");
                        ui->loginLog->clear();
                        ui->passwordLog->clear();
                        CustomFileDialog dialog(this, false, accessLevel);
                        dialog.exec();
                        return;
                    }
                }
            }
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "User not found or password incorrect.");
    }
}

void MainWindow::on_GoToReg_clicked()
{
    this->close();
    emit signal();
}

