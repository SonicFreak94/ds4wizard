#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "Ds4DeviceManager.h"
#include "Logger.h"
#include "Ds4ItemModel.h"
#include "DeviceProfileItemModel.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

	void changeEvent(QEvent* e) override;
	bool wndProc(tagMSG* msg) const;

private:
	std::shared_ptr<Ds4DeviceManager> deviceManager;
	Ds4ItemModel* ds4Items = nullptr;
	DeviceProfileItemModel* profileItems = nullptr;

	Ui::MainWindow ui;
	bool supportsSystemTray = false;
	QSystemTrayIcon* trayIcon = nullptr;
	PVOID notificationHandle = nullptr;
	std::future<void> startupTask;
	EventToken onLineLogged_;
	void onLineLogged(void* sender, std::shared_ptr<LineLoggedEventArgs> args) const;

	void registerDeviceNotification();
	void unregisterDeviceNotification();
	void findController(const std::wstring& name) const;

protected:

#if !defined(QT_IS_BROKEN)
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif

protected slots:
	void closeEvent(QCloseEvent* event) override;
	void toggleHide(QSystemTrayIcon::ActivationReason reason);
	void devicePropertiesClicked();
	static void startMinimizedToggled(bool value);
	static void minimizeToTrayToggled(bool value);
	static void preferredConnectionChanged(int value);
	void systemTrayShowHide(bool checked);
	void systemTrayExit(bool checked);

	void onProfilesLoaded();
	void deviceSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected) const;

signals:
	void s_onProfilesLoaded();
};
