#include "romstable.h"

ROMsTable::ROMsTable(HeaderParser* headerParser, GameWindow* gameWindow, QWidget *parent, QString romsDir)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    model = new QStandardItemModel(0, 5, this);
    model->setHeaderData(0, Qt::Horizontal, tr("Game Title"));
    model->setHeaderData(1, Qt::Horizontal, tr("Publisher"));
    model->setHeaderData(2, Qt::Horizontal, tr("ROM Size"));
    model->setHeaderData(3, Qt::Horizontal, tr("File name"));
    model->setHeaderData(4, Qt::Horizontal, tr("Absolute path to ROM"));

    ui.romsTable->setModel(model);
    ui.romsTable->hideColumn(4); // absolute path is used to load the File
    this->headerParser = headerParser;
    this->gameWindow = gameWindow;

    connect(ui.romsTable, &QTableView::doubleClicked, this, &ROMsTable::on_tableRow_doubleClicked);

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

void ROMsTable::on_tableRow_doubleClicked(const QModelIndex& index)
{
    if (!index.isValid()) return;

    QString fileName = model->item(index.row(), 4)->text();

    // TODO: load data to memory
    gameWindow->show(fileName);
}

void ROMsTable::addTableRow(QString filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file: "));
        return;
    }

    QByteArray fileContent = file.readAll();
    file.close();

    headerParser->loadHeader(std::string(fileContent.constData(), fileContent.length()));

    QList<QStandardItem*> newRow;
    newRow.append(new QStandardItem(QString::fromStdString(headerParser->getTitle())));
    newRow.append(new QStandardItem(QString::fromStdString(headerParser->getPublisher())));
    newRow.append(new QStandardItem(QString::number(headerParser->getSize()) + " KBs"));
    newRow.append(new QStandardItem(QFileInfo(file).fileName()));
    newRow.append(new QStandardItem(file.fileName()));

    for (QStandardItem* item : newRow) {
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }

    model->appendRow(newRow);
}
