#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "User.h"
#include "form.h"
#include "Huffman.h"
#include <adminpanel.h>
#include "userAccess.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void loadUsersWithAccess();
    void goToAdmin();
    ~MainWindow();

private slots:
    void on_LogInButton_clicked();
    void on_GoToReg_clicked();


signals:
    void signal();
    void signal_to_admin();

private:
    Ui::MainWindow *ui;
    Form *form;
    adminpanel *admin;
    QVector<User> userList;
    QVector<UserAccess> userAccessList;
    Huffman *huffman;

    void loadUsersFromFile();
    QString decompressString(const QString &compressedPassword);
    QString compressString(const QString &password);
};

#endif // MAINWINDOW_H
