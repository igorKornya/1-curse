#ifndef USER_LEVEL_ACCESS_H
#define USER_LEVEL_ACCESS_H
#include <QString>

class User_level
{
public:
    User_level(const QString& login, const QString& level_access)
        : m_login(login), m_level_access(level_access) {}

    QString login() const { return m_login; }
    QString level_access() const { return m_level_access; }

private:
    QString m_login;
    QString m_level_access;
};

#endif // USER_LEVEL_ACCESS_H
