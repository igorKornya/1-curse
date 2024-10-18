#ifndef USER_H
#define USER_H
#include <QString>

class User {
public:
    User(const QString& login, const QString& password, const QString& email, const QString& originalLogin, const QString& originalPassword)
        : m_login(login), m_password(password), m_email(email), m_originalLogin(originalLogin), m_originalPassword(originalPassword){}

    QString login() const { return m_login; }
    QString password() const { return m_password; }
    QString email() const { return m_email; }
    QString originalLogin() const { return m_originalLogin; }
    QString originalPassword() const { return m_originalPassword; }

private:
    QString m_login;
    QString m_password;
    QString m_email;
    QString m_originalLogin;
    QString m_originalPassword;
};


#endif // USER_H
