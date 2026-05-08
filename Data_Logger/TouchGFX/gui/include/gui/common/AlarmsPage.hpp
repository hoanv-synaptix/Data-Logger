#ifndef ALARMS_PAGE_HPP
#define ALARMS_PAGE_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdint.h>

class AlarmsPage : public touchgfx::Container
{
public:
    AlarmsPage();

    void setAlarm(uint8_t row, const char* time, const char* level, const char* event, const char* state);

private:
    void setText(touchgfx::TextAreaWithOneWildcard& t,
                 touchgfx::Unicode::UnicodeChar* buf, const char* s);

    // Title
    touchgfx::TextAreaWithOneWildcard titleTxt;
    touchgfx::Unicode::UnicodeChar    titleBuf[16];

    // Buttons (Ack, Clear) - visual only for now
    touchgfx::Box btnAckBg, btnClearBg;
    touchgfx::TextAreaWithOneWildcard btnAckTxt, btnClearTxt;
    touchgfx::Unicode::UnicodeChar    btnAckBuf[8], btnClearBuf[8];

    // Table container
    touchgfx::Box tableOuter, tableInner;
    
    // Table Header
    touchgfx::Box hdrBg, hdrBorder;
    touchgfx::TextAreaWithOneWildcard hdrTxt[4];
    touchgfx::Unicode::UnicodeChar    hdrBuf[4][10];

    // Table Rows (4 rows)
    touchgfx::Box rowBg[4];
    touchgfx::Box rowBorder[4];
    touchgfx::TextAreaWithOneWildcard cellTxt[4][4];
    touchgfx::Unicode::UnicodeChar    cellBuf[4][4][32];
};

#endif
