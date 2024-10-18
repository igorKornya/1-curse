#include "customfiledialog_pro.h"

CustomFilterProxyModel::CustomFilterProxyModel(QObject *parent, const QString &rootPath)
    : QSortFilterProxyModel(parent), rootPath(rootPath)
{
}

bool CustomFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QFileSystemModel *fsModel = qobject_cast<QFileSystemModel *>(sourceModel());
    if (!fsModel)
        return false;

    QModelIndex index = fsModel->index(sourceRow, 0, sourceParent);
    QString folderName = fsModel->data(index).toString();
    if (fsModel->filePath(sourceParent) == rootPath) {
        return (folderName == "1" || folderName == "2");
    }

    return true;
}

CustomFileDialog::CustomFileDialog(QWidget *parent, bool isProUser)
    : QDialog(parent), isProUser(isProUser)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    treeView = new QTreeView(this);
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath("D:/Files");

    if (isProUser) {
        proxyModel = new CustomFilterProxyModel(this, "D:/Files");
        proxyModel->setSourceModel(fileSystemModel);
        treeView->setModel(proxyModel);
        treeView->setRootIndex(proxyModel->mapFromSource(fileSystemModel->index("D:/Files")));
    } else {
        treeView->setModel(fileSystemModel);
        treeView->setRootIndex(fileSystemModel->index("D:/Files"));
    }

    layout->addWidget(treeView);
    setLayout(layout);

    connect(treeView, &QTreeView::doubleClicked, this, &CustomFileDialog::onItemDoubleClicked);
}

void CustomFileDialog::onItemDoubleClicked(const QModelIndex &index)
{
    QString selectedPath = fileSystemModel->filePath(proxyModel->mapToSource(index));
    if (isProUser || selectedPath.startsWith("D:/Files/1") || selectedPath.startsWith("D:/Files/2") || selectedPath.startsWith("D:/Files/3") || selectedPath.startsWith("D:/Files/4")
        || selectedPath.startsWith("D:/Files/5") || selectedPath.startsWith("D:/Files/кот")) {
        QMessageBox::information(this, "Selected Path", selectedPath);
    }
    else
    {
        QMessageBox::critical(this, "Error", "Access denied.");
    }
}
