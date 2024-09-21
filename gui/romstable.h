#pragma once

#include <QMainWindow>
#include "ui_romstable.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel> 
#include "../emulator/header_parser.h"

class ROMsTable : public QMainWindow
{
	Q_OBJECT

public:
	ROMsTable(HeaderParser headerParser, QWidget* parent = nullptr, QString romsDir = "/roms/");
	~ROMsTable();

private:
	void addTableRow(QString fileName);
	void loadROMsFromDir(QString romsPath);

	Ui::ROMsTableClass ui;
	QStandardItemModel* model;
	HeaderParser headerParser;

private slots:
	void on_addButton_clicked();
};
