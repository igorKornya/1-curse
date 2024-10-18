#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include <QStringList>
#include "userAccess.h"

namespace Ui {
class adminpanel;
}

class adminpanel : public QWidget
{
    Q_OBJECT

public:
    explicit adminpanel(QWidget *parent = nullptr);
    ~adminpanel();
    void AddToComboBox(const QString &user);
    void loadUsersWithAccess();

private:
    Ui::adminpanel *ui;
    QStringList userList;
    QVector<UserAccess> userAccessList;
    void loadUsersFromFile();

public slots:
    void admin_slot();
    void saveUserAccessLevel();
    void updateComboBox();

signals:
    void signal_to_main();

private slots:
    void on_ExitButton_clicked();
    void on_pushButton_clicked();
};

#endif // ADMINPANEL_H
