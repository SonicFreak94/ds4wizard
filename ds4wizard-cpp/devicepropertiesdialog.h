#pragma once

#include <QDialog>
#include "ui_devicepropertiesdialog.h"

class DevicePropertiesDialog : public QDialog
{
	Q_OBJECT
	std::wstring deviceKey;
	std::shared_ptr<Ds4DeviceManager> manager;
	std::shared_ptr<Ds4Device> device;

public:
	DevicePropertiesDialog(QWidget* parent, std::wstring deviceKey, std::shared_ptr<Ds4DeviceManager> manager);
	~DevicePropertiesDialog();
	void populateForm(const DeviceSettings& settings);

private:
	Ui::DevicePropertiesDialog ui;
};
