#include "romstable.h"

ROMsTable::ROMsTable(HeaderParser headerParser, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    model = new QStandardItemModel(this);
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, tr("ROM Path"));
    model->setHeaderData(1, Qt::Horizontal, tr("ROM Size"));
    model->setHeaderData(2, Qt::Horizontal, tr("Game Title"));

    ui.romsTable->setModel(model);

    this->headerParser = headerParser;
}

ROMsTable::~ROMsTable()
{
}

void ROMsTable::on_addButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select GameBoy ROM"), "",
        tr("All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file"));
        return;
    }

    QByteArray fileContent = file.readAll();
    file.close();

    headerParser.loadHeader(std::string(fileContent.constData(), fileContent.length()));

    QList<QStandardItem*> newRow;
    newRow.append(new QStandardItem(file.fileName()));
    newRow.append(new QStandardItem(QString::number(headerParser.getSize()) + " KBs"));
    newRow.append(new QStandardItem(QString::fromStdString(headerParser.getTitle())));
    model->appendRow(newRow);
    // parse other ROM contents
}
