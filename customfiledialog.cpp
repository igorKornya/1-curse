#include "customfiledialog.h"
#include <QDesktopServices>
#include <QUrl>

CustomFilterProxyModel::CustomFilterProxyModel(QObject *parent, const QString &rootPath, int accessLevel)
    : QSortFilterProxyModel(parent), rootPath(rootPath), accessLevel(accessLevel)
{
}

bool CustomFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QFileSystemModel *fsModel = qobject_cast<QFileSystemModel *>(sourceModel());
    if (!fsModel)
        return false;

    QModelIndex index = fsModel->index(sourceRow, 0, sourceParent);
    QString folderName = fsModel->data(index).toString();
    QString filePath = fsModel->filePath(index);

    bool isJpgFile = filePath.endsWith(".jpg", Qt::CaseInsensitive);

    if (fsModel->filePath(sourceParent) == rootPath) {
        if (accessLevel == 1) {
            return (folderName == "1" || folderName == "2");
        } else if (accessLevel == 3) {
            return true;
        } else if (accessLevel == 2) {
            return (folderName == "1" || folderName == "2" || folderName == "3" || isJpgFile);
        }
    }

    return true;
}

CustomFileDialog::CustomFileDialog(QWidget *parent, bool isAdmin, int accessLevel)
    : QDialog(parent), isAdmin(isAdmin), accessLevel(accessLevel)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    treeView = new QTreeView(this);
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath("D:/Files");

    if (isAdmin) {
        treeView->setModel(fileSystemModel);
        treeView->setRootIndex(fileSystemModel->index("D:/Files"));
    } else {
        proxyModel = new CustomFilterProxyModel(this, "D:/Files", accessLevel);
        proxyModel->setSourceModel(fileSystemModel);
        treeView->setModel(proxyModel);
        treeView->setRootIndex(proxyModel->mapFromSource(fileSystemModel->index("D:/Files")));
    }

    layout->addWidget(treeView);
    setLayout(layout);

    connect(treeView, &QTreeView::doubleClicked, this, &CustomFileDialog::onItemDoubleClicked);
}

void CustomFileDialog::onItemDoubleClicked(const QModelIndex &index)
{
    QModelIndex sourceIndex = isAdmin ? index : proxyModel->mapToSource(index);
    QString selectedPath = fileSystemModel->filePath(sourceIndex);

    if (fileSystemModel->isDir(sourceIndex)) {
        treeView->setRootIndex(sourceIndex);
    } else {
        QFileInfo fileInfo(selectedPath);
        if (isAdmin || selectedPath.startsWith("D:/Files/1") || selectedPath.startsWith("D:/Files/2") ||
            selectedPath.startsWith("D:/Files/3") || selectedPath.endsWith(".jpg", Qt::CaseInsensitive)) {

            if (fileInfo.suffix().toLower() == "jpg") {
                QDesktopServices::openUrl(QUrl::fromLocalFile(selectedPath));
            } else {
                QDesktopServices::openUrl(QUrl::fromLocalFile(selectedPath));
            }
        } else {
            QMessageBox::critical(this, "Error", "Access denied.");
        }
    }
}

