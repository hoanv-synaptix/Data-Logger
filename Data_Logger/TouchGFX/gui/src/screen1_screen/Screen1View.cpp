#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Color.hpp>

Screen1View::Screen1View() :
    menuChangedCallback(this, &Screen1View::onMenuChanged)
{
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

    // Main Area Background
    mainBackground.setPosition(0, 0, 480, 272);
    mainBackground.setColor(touchgfx::Color::getColorFromRGB(219, 227, 238));
    add(mainBackground);

    // Top Bar
    topBar.setPosition(0, 0, 480, 32);
    add(topBar);

    // Nav Bar
    navBar.setPosition(0, 244, 480, 28);
    navBar.setMenuChangedCallback(menuChangedCallback);
    add(navBar);

    // Pages (5,37,470,202) — only one visible at a time
    dashboardPage.setPosition(5, 37, 470, 202);
    sensorsPage.setPosition(5, 37, 470, 202);
    alarmsPage.setPosition(5, 37, 470, 202);
    networkPage.setPosition(5, 37, 470, 202);
    settingsPage.setPosition(5, 37, 470, 202);

    add(dashboardPage);
    add(sensorsPage);
    add(alarmsPage);
    add(networkPage);
    add(settingsPage);

    // Default: show Dashboard
    dashboardPage.setVisible(true);
    sensorsPage.setVisible(false);
    alarmsPage.setVisible(false);
    networkPage.setVisible(false);
    settingsPage.setVisible(false);

    // Init dashboard with placeholder values
    dashboardPage.setPM25(35);
    dashboardPage.setPM10(58);
    dashboardPage.setCO2(510);
    dashboardPage.setAQI(72);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::updateNetworkData(bool isUp, const char* ip)
{
    networkPage.setMainLinkState(isUp);
    networkPage.setDeviceIP(ip);
}

void Screen1View::onMenuChanged(uint8_t index)
{
    dashboardPage.setVisible(index == 0);
    sensorsPage.setVisible(index == 1);
    alarmsPage.setVisible(index == 2);
    networkPage.setVisible(index == 3);
    settingsPage.setVisible(index == 4);
    invalidate();
}

void Screen1View::updateSensorData(int pm25, int pm10, int co2, int aqi)
{
    dashboardPage.setPM25(pm25);
    dashboardPage.setPM10(pm10);
    dashboardPage.setCO2(co2);
    dashboardPage.setAQI(aqi);

    sensorsPage.setPM25(pm25);
    sensorsPage.setPM10(pm10);
    sensorsPage.setCO2(co2);
}