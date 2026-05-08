#ifndef DASHBOARD_COMPACT_HPP
#define DASHBOARD_COMPACT_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdint.h>

class DashboardCompact : public touchgfx::Container
{
public:
    DashboardCompact();
    void setPM25(int value);
    void setPM10(int value);
    void setCO2(int value);
    void setAQI(int value);

private:
    void setText(touchgfx::TextAreaWithOneWildcard& t,
                 touchgfx::Unicode::UnicodeChar* buf, const char* val);
    void setNumber(touchgfx::TextAreaWithOneWildcard& t,
                   touchgfx::Unicode::UnicodeChar* buf,
                   touchgfx::TextAreaWithOneWildcard* unit, int val);
    void setTrend(const uint8_t* values, uint8_t count);

    // Metric Cards (4x)
    touchgfx::Box pm25CardOuter, pm25Card;
    touchgfx::Box pm10CardOuter, pm10Card;
    touchgfx::Box co2CardOuter,  co2Card;
    touchgfx::Box aqiCardOuter,  aqiCard;

    touchgfx::TextAreaWithOneWildcard pm25Label, pm10Label, co2Label, aqiLabel;
    touchgfx::Unicode::UnicodeChar pm25LabelBuf[8], pm10LabelBuf[8], co2LabelBuf[8], aqiLabelBuf[8];

    touchgfx::TextAreaWithOneWildcard pm25Value, pm10Value, co2Value, aqiValue;
    touchgfx::Unicode::UnicodeChar pm25Buf[8], pm10Buf[8], co2Buf[8], aqiBuf[8];

    touchgfx::TextAreaWithOneWildcard pm25Unit, pm10Unit, co2Unit;
    touchgfx::Unicode::UnicodeChar pm25UnitBuf[8], pm10UnitBuf[8], co2UnitBuf[8];

    touchgfx::Box pm25PillBox, pm10PillBox, co2PillBox, aqiPillBox;
    touchgfx::TextAreaWithOneWildcard pm25PillTxt, pm10PillTxt, co2PillTxt, aqiPillTxt;
    touchgfx::Unicode::UnicodeChar pm25PillBuf[6], pm10PillBuf[6], co2PillBuf[6], aqiPillBuf[6];

    // Air Status Panel
    touchgfx::Box airOuter, airInner, airDivider;
    touchgfx::TextAreaWithOneWildcard airTitle, airState;
    touchgfx::Unicode::UnicodeChar airTitleBuf[12], airStateBuf[8];

    touchgfx::Box tempOuter, tempInner;
    touchgfx::TextAreaWithOneWildcard tempLabel, tempVal;
    touchgfx::Unicode::UnicodeChar tempLabelBuf[6], tempValBuf[8];

    touchgfx::Box humiOuter, humiInner;
    touchgfx::TextAreaWithOneWildcard humiLabel, humiVal;
    touchgfx::Unicode::UnicodeChar humiLabelBuf[6], humiValBuf[8];

    // Trend Panel
    touchgfx::Box trendOuter, trendInner;
    touchgfx::TextAreaWithOneWildcard trendTitle;
    touchgfx::Unicode::UnicodeChar trendTitleBuf[20];
    touchgfx::Box trendLiveBox;
    touchgfx::TextAreaWithOneWildcard trendLiveTxt;
    touchgfx::Unicode::UnicodeChar trendLiveBuf[6];

    touchgfx::Box chartOuter, chartBg;
    touchgfx::Box chartBars[14];
    touchgfx::Box gridLine0, gridLine1, gridLine2;

    touchgfx::TextAreaWithOneWildcard axisY100, axisY50, axisY0;
    touchgfx::Unicode::UnicodeChar axisY100Buf[5], axisY50Buf[5], axisY0Buf[5];
    touchgfx::TextAreaWithOneWildcard axisXOld, axisXNow;
    touchgfx::Unicode::UnicodeChar axisXOldBuf[5], axisXNowBuf[5];
};

#endif