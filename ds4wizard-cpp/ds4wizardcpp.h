#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ds4wizardcpp.h"

class ds4wizardcpp : public QMainWindow
{
	Q_OBJECT

public:
	ds4wizardcpp(QWidget *parent = Q_NULLPTR);

private:
	Ui::ds4wizardcppClass ui;
};
