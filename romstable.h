#pragma once

#include <QMainWindow>
#include "ui_romstable.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel> 

class ROMsTable : public QMainWindow
{
	Q_OBJECT

public:
	ROMsTable(QWidget *parent = nullptr);
	~ROMsTable();

private:
	Ui::ROMsTableClass ui;
	QStandardItemModel* model;

private slots:
	void on_addButton_clicked();
};
