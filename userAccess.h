#ifndef USERACCESS_H
#define USERACCESS_H

class UserAccess
{
public:
    UserAccess(const QString& username, const QString& accessLevel) : m_username(username), m_accessLevel(accessLevel) {}
    QString username() const { return m_username; }
    QString accessLevel() const { return m_accessLevel; }
private:
    QString m_username;
    QString m_accessLevel;
};

#endif // USERACCESS_H
