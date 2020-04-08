#include "mainwidget.h"
#include <QDateTime>
#include <QDir>
#include <QDropEvent>
#include <QFileInfo>
#include <QHeaderView>

MainWidget::MainWidget(QWidget* parent) : QTableWidget(parent)
{
    QStringList headers;
    headers << "Имя"
            << "Дата изменения"
            << "Местоположение";
    this->setColumnCount(3);
    this->setHorizontalHeaderLabels(headers);
    this->setColumnWidth(0, 100);
    this->setColumnWidth(1, 110);
    this->setColumnWidth(2, 250);
    this->setAcceptDrops(true);
    this->verticalHeader()->hide();
}

void MainWidget::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    auto insert = [this](QFileInfo file) {
        int row = this->rowCount();
        this->insertRow(row);
        this->setItem(row, 0, new QTableWidgetItem(file.fileName()));
        this->setItem(
                row,
                1,
                new QTableWidgetItem(
                        file.lastModified().toString(Qt::ISODate)));
        this->setItem(row, 2, new QTableWidgetItem(file.filePath()));
    };

    if (!mimeData->hasUrls())
        return;
    for (QUrl url : mimeData->urls()) {
        if (url.scheme() != "file") {
            continue;
        }
        QFileInfo file = url.toLocalFile();

        if (file.isDir()) {
            QDir dir(file.absoluteFilePath());
            for (auto obj : dir.entryInfoList()) {
                if (obj.isFile())
                    insert(obj);
            }
        } else {
            insert(file);
        }
    }

    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    event->acceptProposedAction();
}

void MainWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void MainWidget::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

void MainWidget::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}
