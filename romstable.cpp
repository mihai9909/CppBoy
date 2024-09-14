#include "romstable.h"

ROMsTable::ROMsTable(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    model = new QStandardItemModel(this);
    model->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, tr("ROM Path"));
    model->setHeaderData(1, Qt::Horizontal, tr("ROM Size"));

    ui.romsTable->setModel(model);
}

ROMsTable::~ROMsTable()
{
}

void ROMsTable::on_addButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select GameBoy ROM"), "",
        tr("Text Files (*.txt);;All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file"));
        return;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    QList<QStandardItem*> newRow;
    newRow.append(new QStandardItem(file.fileName()));
    newRow.append(new QStandardItem(QString::number(file.size()) + " Bytes"));
    model->appendRow(newRow);
    // parse ROM contents
}
