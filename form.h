#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QObject>
#include <QFormLayout>
#include <QMessageBox>
#include <QList>
#include <QFile>
#include <QTextStream>
#include "user.h"
#include "Huffman.h"
#include "adminpanel.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    bool isPasswordUnique(const QString &password);
    void saveNewUser(const QString &username);
    void loadUsersFromFile();
    void saveUsersToFile();
    void updateComboBox();
    ~Form();

private slots:
    void on_SignUpButton_clicked();
    void on_pushButton_clicked();

private:
    Ui::Form *ui;
    QList<User> userList;


    Huffman huffman;
    adminpanel *admin;

    QString compressString(const QString &password);
    QString decompressString(const QString &password);

public slots:
    void slot();

signals:
    void signal_to_adminpanel(const QString &username);
    void updateComboBoxRequested();
    void signal_back();
};

#endif // FORM_H
