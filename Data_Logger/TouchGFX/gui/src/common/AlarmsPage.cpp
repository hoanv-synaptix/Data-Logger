#include <gui/common/AlarmsPage.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

// Layout constants for 470x202 main area
static const int TITLE_H = 25;
static const int TBL_Y = TITLE_H + 6;
static const int TBL_H = 202 - TBL_Y; // 171
static const int HDR_H = 24;
static const int ROW_H = (TBL_H - HDR_H) / 4; // 147 / 4 = 36
static const int TBL_W = 470;

// Columns: 89, 89, 200, 90 (Total 468)
static const int COL_W[4] = {89, 89, 200, 90};
static const int COL_X[4] = {1, 90, 179, 379};

AlarmsPage::AlarmsPage()
{
    setPosition(0, 0, 470, 202);

    // Colors matching React
    touchgfx::colortype cBdr    = touchgfx::Color::getColorFromRGB(203,213,225); // #cbd5e1 border
    touchgfx::colortype cCard   = touchgfx::Color::getColorFromRGB(248,250,252); // #f8fafc card
    touchgfx::colortype cHdrBg  = touchgfx::Color::getColorFromRGB(203,213,225); // #cbd5e1 header bg
    touchgfx::colortype cTxt    = touchgfx::Color::getColorFromRGB(15, 23, 42);  // #0f172a text
    touchgfx::colortype cOdd    = touchgfx::Color::getColorFromRGB(241,245,249); // #f1f5f9
    touchgfx::colortype cEven   = touchgfx::Color::getColorFromRGB(255,255,255); // #ffffff
    touchgfx::colortype cRowBdr = touchgfx::Color::getColorFromRGB(209,213,219); // #d1d5db
    
    touchgfx::colortype cRed    = touchgfx::Color::getColorFromRGB(185, 28, 28); // #b91c1c
    touchgfx::colortype cAmber  = touchgfx::Color::getColorFromRGB(180, 83, 9);  // #b45309
    touchgfx::colortype cBlue   = touchgfx::Color::getColorFromRGB(3, 105, 161); // #0369a1
    touchgfx::colortype cGreen  = touchgfx::Color::getColorFromRGB(4, 120, 87);  // #047857
    touchgfx::colortype cWhite  = touchgfx::Color::getColorFromRGB(255, 255, 255);

    auto initTxt = [this](touchgfx::TextAreaWithOneWildcard& t,
        touchgfx::Unicode::UnicodeChar* buf,
        int x, int y, int w, int h,
        const char* str, touchgfx::colortype col, uint16_t typo)
    {
        t.setPosition(x,y,w,h);
        t.setTypedText(touchgfx::TypedText(typo));
        t.setColor(col); t.setWildcard(buf);
        touchgfx::Unicode::strncpy(buf, str, 32);
        add(t); t.invalidate();
    };

    // Title
    initTxt(titleTxt, titleBuf, 4, 0, 200, 28, "Alarms", cTxt, T___SINGLEUSE_MK9N);

    // Actions: ACK, Clear
    btnAckBg.setPosition(375, 1, 40, 24);
    btnAckBg.setColor(cBlue); add(btnAckBg);
    initTxt(btnAckTxt, btnAckBuf, 375 + 8, 5, 32, 16, "ACK", cWhite, T___SINGLEUSE_PPYL);

    btnClearBg.setPosition(420, 1, 45, 24);
    btnClearBg.setColor(cRed); add(btnClearBg);
    initTxt(btnClearTxt, btnClearBuf, 420 + 6, 5, 39, 16, "Clear", cWhite, T___SINGLEUSE_PPYL);

    // Table container
    tableOuter.setPosition(0, TBL_Y, TBL_W, TBL_H);
    tableOuter.setColor(cBdr); add(tableOuter);
    tableInner.setPosition(1, TBL_Y+1, TBL_W-2, TBL_H-2);
    tableInner.setColor(cCard); add(tableInner);

    // Table Header
    hdrBg.setPosition(1, TBL_Y+1, TBL_W-2, HDR_H);
    hdrBg.setColor(cHdrBg); add(hdrBg);
    hdrBorder.setPosition(1, TBL_Y+1+HDR_H-1, TBL_W-2, 1);
    hdrBorder.setColor(cBdr); add(hdrBorder);

    const char* hdrNames[4] = {"Time", "Level", "Event", "State"};
    for(int c=0; c<4; c++) {
        initTxt(hdrTxt[c], hdrBuf[c], COL_X[c]+7, TBL_Y+1+5, COL_W[c]-14, 16, hdrNames[c], cTxt, T___SINGLEUSE_PPYL);
    }

    // Default Rows
    const char* defTime[4]  = {"10:22", "10:20", "09:55", "09:45"};
    const char* defLevel[4] = {"WARN", "ERROR", "INFO", "WARN"};
    const char* defEvent[4] = {"Network unstable", "PM2.5 lost", "SD ready", "Ethernet lost"};
    const char* defState[4] = {"Active", "Ack", "Closed", "Closed"};

    auto getColor = [cRed, cAmber, cGreen, cTxt](const char* text) {
        if (text[0] == 'E') return cRed; // ERROR
        if (text[0] == 'W') return cAmber; // WARN
        if (text[0] == 'I') return cTxt;   // INFO -> default text color
        return cTxt;
    };

    // Rows
    for(int r=0; r<4; r++) {
        int rY = TBL_Y + 1 + HDR_H + r*ROW_H;
        
        rowBg[r].setPosition(1, rY, TBL_W-2, ROW_H);
        rowBg[r].setColor((r % 2 == 1) ? cOdd : cEven);
        add(rowBg[r]);

        rowBorder[r].setPosition(1, rY+ROW_H-1, TBL_W-2, 1);
        rowBorder[r].setColor(cRowBdr);
        add(rowBorder[r]);

        touchgfx::colortype rowColor = getColor(defLevel[r]);

        // Cells
        initTxt(cellTxt[r][0], cellBuf[r][0], COL_X[0]+7, rY+9, COL_W[0]-14, 16, defTime[r],  rowColor, T___SINGLEUSE_PPYL);
        initTxt(cellTxt[r][1], cellBuf[r][1], COL_X[1]+7, rY+9, COL_W[1]-14, 16, defLevel[r], rowColor, T___SINGLEUSE_PPYL);
        initTxt(cellTxt[r][2], cellBuf[r][2], COL_X[2]+7, rY+9, COL_W[2]-14, 16, defEvent[r], rowColor, T___SINGLEUSE_PPYL);
        initTxt(cellTxt[r][3], cellBuf[r][3], COL_X[3]+7, rY+9, COL_W[3]-14, 16, defState[r], rowColor, T___SINGLEUSE_PPYL);
    }
}

void AlarmsPage::setText(touchgfx::TextAreaWithOneWildcard& t,
                         touchgfx::Unicode::UnicodeChar* buf, const char* s)
{
    touchgfx::Unicode::strncpy(buf, s, 32);
    t.invalidate();
}

void AlarmsPage::setAlarm(uint8_t row, const char* time, const char* level, const char* event, const char* state)
{
    if(row >= 4) return;
    setText(cellTxt[row][0], cellBuf[row][0], time);
    setText(cellTxt[row][1], cellBuf[row][1], level);
    setText(cellTxt[row][2], cellBuf[row][2], event);
    setText(cellTxt[row][3], cellBuf[row][3], state);
}
