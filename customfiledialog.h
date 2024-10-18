#ifndef CUSTOMFILEDIALOG_H
#define CUSTOMFILEDIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QMessageBox>

class CustomFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    CustomFilterProxyModel(QObject *parent, const QString &rootPath, int accessLevel);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString rootPath;
    int accessLevel;
};

class CustomFileDialog : public QDialog {
    Q_OBJECT
public:
    CustomFileDialog(QWidget *parent = nullptr, bool isAdmin = false, int accessLevel = 1);

private slots:
    void onItemDoubleClicked(const QModelIndex &index);

private:
    QTreeView *treeView;
    QFileSystemModel *fileSystemModel;
    CustomFilterProxyModel *proxyModel;
    bool isAdmin;
    int accessLevel;
};

#endif // CUSTOMFILEDIALOG_H
