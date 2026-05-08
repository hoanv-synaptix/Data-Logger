#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

#include <gui/common/DashboardCompact.hpp>
#include <gui/common/SensorsPage.hpp>
#include <gui/common/AlarmsPage.hpp>
#include <gui/common/NetworkPage.hpp>
#include <gui/common/SettingsPage.hpp>
#include <gui/common/TopBar.hpp>
#include <gui/common/NavBar.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}

    virtual void setupScreen();
    virtual void tearDownScreen();
    void updateSensorData(int pm25, int pm10, int co2, int aqi);
    void updateNetworkData(bool isUp, const char* ip);
    void onMenuChanged(uint8_t index);

private:
    TopBar topBar;
    NavBar navBar;
    touchgfx::Box mainBackground;

    // Pages (index 0-4 matching NavBar tabs)
    DashboardCompact dashboardPage;
    SensorsPage      sensorsPage;
    AlarmsPage       alarmsPage;
    NetworkPage      networkPage;
    SettingsPage     settingsPage;

    touchgfx::Callback<Screen1View, uint8_t> menuChangedCallback;
};

#endif