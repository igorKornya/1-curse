#include "adminpanel.h"
#include "ui_adminpanel.h"
#include <QFile>
#include <QComboBox>
#include <QTextStream>
#include <QCoreApplication>
#include <QMessageBox>
#include <QDir>
#include "userAccess.h"
#include <QTimer>

adminpanel::adminpanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminpanel)
{
    ui->setupUi(this);
    loadUsersFromFile();
    updateComboBox();
    connect(ui->EnterChange, &QPushButton::clicked, this, &adminpanel::saveUserAccessLevel);
}

adminpanel::~adminpanel()
{
    delete ui;
}

void adminpanel::loadUsersWithAccess()
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
                ui->comboBox_2->addItem(userAccess.username());
            }
        }
    }
}

void adminpanel::admin_slot()
{
    this->show();
}

void adminpanel::on_ExitButton_clicked()
{
    this->close();
    emit signal_to_main();
}

void adminpanel::loadUsersFromFile()
{
    QString filePath = "D:/user_access_levels.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        userList.clear();

        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() == 2)
            {
                QString username = fields.at(0);
                ui->comboBox_2->addItem(username);
                ui->comboBox_2->repaint();
                ui->comboBox_2->update();
                ui->comboBox_2->clearFocus();
                ui->comboBox_2->setFocus();
                ui->comboBox_2->parentWidget()->update();
            }
        }

        file.close();
    }
    updateComboBox();
}



void adminpanel::updateComboBox()
{
    ui->comboBox_2->repaint();
    ui->comboBox_2->update();
    ui->comboBox_2->clearFocus();
    ui->comboBox_2->setFocus();
    ui->comboBox_2->parentWidget()->update();
}


void adminpanel::saveUserAccessLevel()
{
    QString username = ui->comboBox_2->currentText();
    QString accessLevel = ui->comboBox->currentText();

    if (username.isEmpty() || accessLevel.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please select a user and an access level.");
        return;
    }

    QString filePath = "D:/user_access_levels.txt";
    QFile file(filePath);
    QStringList lines;
    bool userFound = false;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList userAccessData = line.split(",");
            if (userAccessData.size() == 2)
            {
                if (userAccessData.at(0) == username)
                {
                    lines.append(username + "," + accessLevel);
                    userFound = true;
                }
                else
                {
                    lines.append(line);
                }
            }
        }
        file.close();
    }

    if (!userFound)
    {
        lines.append(username + "," + accessLevel);
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (const QString &line : lines)
        {
            out << line << "\n";
        }
        file.close();
        QMessageBox::information(this, "Success", "User access level saved successfully.");
    }
    else
    {
        QMessageBox::critical(this, "Error", "Unable to open file for writing.");
    }
    ui->comboBox_2->clear();
    loadUsersFromFile();
    updateComboBox();
}

void adminpanel::on_pushButton_clicked()
{
    ui->comboBox_2->clear();
    loadUsersWithAccess();
    ui->comboBox_2->clear();
    loadUsersWithAccess();
}

