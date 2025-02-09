#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel> 
#include "../emulator/header_parser.h"
#include "../emulator/ppu.h"
#include "../emulator/memory.h"
#include "../utils/file_util.h"
#include "gameboy.h"
#include "ui_romstable.h"

class ROMsTable : public QMainWindow
{
	Q_OBJECT
		
public:
	ROMsTable(HeaderParser* headerParser, QWidget* parent = nullptr, QString romsDir = "/roms/");
	~ROMsTable();

private:
	void addTableRow(QString fileName);
	void loadROMsFromDir(QString romsPath);

	Ui::ROMsTableClass ui;
	QStandardItemModel* model;
	HeaderParser* headerParser;
	GameBoy* gameBoy;
	GameBoyCore* core;

private slots:
	void on_addButton_clicked();
	void on_tableRow_doubleClicked(const QModelIndex& index);
};
