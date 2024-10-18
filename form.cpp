#include "form.h"
#include "ui_form.h"
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>
#include <QSet>
#include <QString>
#include <QMessageBox>
#include <QRegExpValidator>
#include "Huffman.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    loadUsersFromFile();
    QRegExp noSpaceRegExp("\\S*");
    QRegExpValidator* noSpaceValidator = new QRegExpValidator(noSpaceRegExp, this);
    ui->login->setValidator(noSpaceValidator);
    ui->password->setValidator(noSpaceValidator);
    ui->email->setValidator(noSpaceValidator);

    QRegExp emailRegExp("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}");
    QRegExpValidator* emailValidator = new QRegExpValidator(emailRegExp, this);
    ui->email->setValidator(emailValidator);
}

Form::~Form()
{
    delete ui;
}

bool Form::isPasswordUnique(const QString &password)
{
    QSet<QChar> uniqueChars;

    for (const QChar &ch : password)
    {
        if (uniqueChars.contains(ch))
        {
            return false;
        }

        uniqueChars.insert(ch);
    }

    return true;
}

void Form::on_SignUpButton_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();
    QString email = ui->email->text();

    if (login.isEmpty() || password.isEmpty() || email.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Please fill in all fields.");
        return;
    }

    if (!email.contains('@') && !(email.endsWith('.com') || email.endsWith('.ru')))
    {
        QMessageBox::critical(this, "Error", "Error email");
        return;
    }

    if (login.length() < 2 || password.length() < 2)
    {
        QMessageBox::critical(this, "Error", "Small login or password");
        return;
    }

    if (!isPasswordUnique(password))
    {
        QMessageBox::critical(this, "Error", "Password must contain only unique characters.");
        return;
    }

    bool userExists = false;
    for (const User& user : userList)
    {
        if (user.originalLogin() == login)
        {
            userExists = true;
            break;
        }
    }

    if (userExists)
    {
        QMessageBox::critical(this, "Error", "User already exists!");
    }
    else
    {
        QString compressedLogin = compressString(login);
        QString compressedPassword = compressString(password);
        User newUser(compressedLogin, compressedPassword, email, login, password);
        userList.append(newUser);

        saveUsersToFile();
        saveNewUser(login);

        ui->login->clear();
        ui->password->clear();
        ui->email->clear();

        QMessageBox::information(this, "Success", "User registered successfully!");
    }
}

QString Form::compressString(const QString &str)
{
    huffman.buildHuffmanTree(str.toStdString());
    string encodedString = huffman.getHuffmanEncoding(str.toStdString());
    return QString::fromStdString(encodedString);
}

QString Form::decompressString(const QString &compressedStr)
{
    string encodedString = compressedStr.toStdString();
    string decodedString = huffman.decodeHuffmanEncoding(encodedString);
    return QString::fromStdString(decodedString);
}

void Form::saveUsersToFile()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/users.txt";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (const User& user : userList)
        {
            out << user.login() << "," << user.password() << "," << user.email() << ","
                << user.originalLogin() << "," << user.originalPassword() << "\n";
        }
        file.close();
    }
}

void Form::loadUsersFromFile()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/users.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() == 5)
            {
                User user(fields[0], fields[1], fields[2], fields[3], fields[4]);
                userList.append(user);
            }
        }

        file.close();
    }
}

void Form::saveNewUser(const QString &username)
{
    QString filePath = "D:/user_access_levels.txt";
    QFile file(filePath);

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << username << ",1\n";
        file.close();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Unable to open file for writing.");
    }
}


void Form::slot()
{
    this->show();
}

void Form::on_pushButton_clicked()
{
    this->close();
    emit updateComboBoxRequested();
    emit signal_back();
}
