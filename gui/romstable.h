#pragma once

#include <QMainWindow>
#include "ui_romstable.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel> 
#include "../emulator/header_parser.h"
#include "gameboy.h"
#include "../utils/file_util.h"

class ROMsTable : public QMainWindow
{
	Q_OBJECT
		
public:
	ROMsTable(HeaderParser* headerParser, GameBoy* gameWindow, QWidget* parent = nullptr, QString romsDir = "/roms/");
	~ROMsTable();

private:
	void addTableRow(QString fileName);
	void loadROMsFromDir(QString romsPath);

	Ui::ROMsTableClass ui;
	QStandardItemModel* model;
	HeaderParser* headerParser;
	GameBoy* gameWindow;

private slots:
	void on_addButton_clicked();
	void on_tableRow_doubleClicked(const QModelIndex& index);
};
