#pragma once

#include <QMainWindow>
#include "ui_romstable.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel> 
#include "../emulator/header_parser.h"
#include "game_window.h"
#include "../utils/file_util.h"

class ROMsTable : public QMainWindow
{
	Q_OBJECT

public:
	ROMsTable(HeaderParser* headerParser, GameWindow* gameWindow, QWidget* parent = nullptr, QString romsDir = "/roms/");
	~ROMsTable();

private:
	void addTableRow(QString fileName);
	void loadROMsFromDir(QString romsPath);

	Ui::ROMsTableClass ui;
	QStandardItemModel* model;
	HeaderParser* headerParser;
	GameWindow* gameWindow;

private slots:
	void on_addButton_clicked();
	void on_tableRow_doubleClicked(const QModelIndex& index);
};
