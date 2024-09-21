#include "romstable.h"

ROMsTable::ROMsTable(HeaderParser headerParser, QWidget *parent, QString romsDir)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHeaderData(0, Qt::Horizontal, tr("Game Title"));
    model->setHeaderData(1, Qt::Horizontal, tr("Publisher"));
    model->setHeaderData(2, Qt::Horizontal, tr("ROM Size"));
    model->setHeaderData(3, Qt::Horizontal, tr("File name"));

    ui.romsTable->setModel(model);
    this->headerParser = headerParser;

    loadROMsFromDir(QDir::currentPath() + romsDir);
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

    addTableRow(fileName);
}

void ROMsTable::loadROMsFromDir(QString romsPath) {
    QDir romsDirectory(romsPath);

    if (!romsDirectory.exists()) {
        return;
    }

    QStringList fList = romsDirectory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (QString f : fList) {
        addTableRow(romsPath + f);
    }
}

void ROMsTable::addTableRow(QString filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file: "));
        return;
    }

    QByteArray fileContent = file.readAll();
    file.close();

    headerParser.loadHeader(std::string(fileContent.constData(), fileContent.length()));

    QList<QStandardItem*> newRow;
    newRow.append(new QStandardItem(QString::fromStdString(headerParser.getTitle())));
    newRow.append(new QStandardItem(QString::fromStdString(headerParser.getPublisher())));
    newRow.append(new QStandardItem(QString::number(headerParser.getSize()) + " KBs"));
    newRow.append(new QStandardItem(QFileInfo(file).fileName()));
    model->appendRow(newRow);
}

