#ifndef SENSORS_PAGE_HPP
#define SENSORS_PAGE_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdint.h>

class SensorsPage : public touchgfx::Container
{
public:
    SensorsPage();

    void setPM25(int v);
    void setPM10(int v);
    void setCO2(int v);
    void setTempHumi(int tTenths, int hPct);
    void setRain(bool raining);

private:
    void setText(touchgfx::TextAreaWithOneWildcard& t,
                 touchgfx::Unicode::UnicodeChar* buf, const char* s);

    // Title
    touchgfx::TextAreaWithOneWildcard titleTxt;
    touchgfx::Unicode::UnicodeChar    titleBuf[20];

    // Table container
    touchgfx::Box tableOuter, tableInner;
    
    // Table Header
    touchgfx::Box hdrBg, hdrBorder;
    touchgfx::TextAreaWithOneWildcard hdrTxt[4];
    touchgfx::Unicode::UnicodeChar    hdrBuf[4][10];

    // Table Rows (5)
    touchgfx::Box rowBg[5];
    touchgfx::Box rowBorder[5];
    touchgfx::TextAreaWithOneWildcard cellTxt[5][4];
    touchgfx::Unicode::UnicodeChar    cellBuf[5][4][20];
};

#endif
