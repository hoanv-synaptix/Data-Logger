#include <gui/common/SensorsPage.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

// Layout constants for 470x202 main area
static const int PAD = 5;
static const int TITLE_H = 25;
static const int TBL_Y = TITLE_H + 6;
static const int TBL_H = 202 - TBL_Y; // 202 - 31 = 171
static const int HDR_H = 24;
static const int ROW_H = (TBL_H - HDR_H) / 5; // 147 / 5 = 29
static const int TBL_W = 470;

// Columns: 134, 88, 146, 100 (Total 468 + 2px border = 470)
static const int COL_W[4] = {134, 88, 146, 100};
static const int COL_X[4] = {1, 135, 223, 369}; // starts inside 1px border

SensorsPage::SensorsPage()
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
    touchgfx::colortype cGreen  = touchgfx::Color::getColorFromRGB(4, 120, 87);  // #047857

    auto initTxt = [this](touchgfx::TextAreaWithOneWildcard& t,
        touchgfx::Unicode::UnicodeChar* buf,
        int x, int y, int w, int h,
        const char* str, touchgfx::colortype col, uint16_t typo)
    {
        t.setPosition(x,y,w,h);
        t.setTypedText(touchgfx::TypedText(typo));
        t.setColor(col); t.setWildcard(buf);
        touchgfx::Unicode::strncpy(buf, str, 20);
        add(t); t.invalidate();
    };

    // Title
    initTxt(titleTxt, titleBuf, 4, 0, 200, 28, "Sensors", cTxt, T___SINGLEUSE_MK9N);

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

    const char* hdrNames[4] = {"Name", "Bus", "Value", "State"};
    for(int c=0; c<4; c++) {
        initTxt(hdrTxt[c], hdrBuf[c], COL_X[c]+7, TBL_Y+1+5, COL_W[c]-14, 16, hdrNames[c], cTxt, T___SINGLEUSE_PPYL);
    }

    // Default Rows
    const char* defNames[5] = {"PM2.5", "PM10", "CO2", "Temp/Humi", "Rain"};
    const char* defBus[5]   = {"RS485", "RS485", "RS485", "RS485", "DI1"};
    const char* defVals[5]  = {"35 ug/m3", "58 ug/m3", "510 ppm", "24C / 62%", "OFF"};

    // Rows
    for(int r=0; r<5; r++) {
        int rY = TBL_Y + 1 + HDR_H + r*ROW_H;
        
        rowBg[r].setPosition(1, rY, TBL_W-2, ROW_H);
        rowBg[r].setColor((r % 2 == 1) ? cOdd : cEven);
        add(rowBg[r]);

        rowBorder[r].setPosition(1, rY+ROW_H-1, TBL_W-2, 1);
        rowBorder[r].setColor(cRowBdr);
        add(rowBorder[r]);

        // Cells
        initTxt(cellTxt[r][0], cellBuf[r][0], COL_X[0]+7, rY+5, COL_W[0]-14, 16, defNames[r], cTxt, T___SINGLEUSE_PPYL);
        initTxt(cellTxt[r][1], cellBuf[r][1], COL_X[1]+7, rY+5, COL_W[1]-14, 16, defBus[r],   cTxt, T___SINGLEUSE_PPYL);
        initTxt(cellTxt[r][2], cellBuf[r][2], COL_X[2]+7, rY+5, COL_W[2]-14, 16, defVals[r],  cTxt, T___SINGLEUSE_PPYL);
        initTxt(cellTxt[r][3], cellBuf[r][3], COL_X[3]+7, rY+5, COL_W[3]-14, 16, "ONLINE",    cGreen, T___SINGLEUSE_PPYL);
    }
}

void SensorsPage::setText(touchgfx::TextAreaWithOneWildcard& t,
                          touchgfx::Unicode::UnicodeChar* buf, const char* s)
{
    touchgfx::Unicode::strncpy(buf, s, 20);
    t.invalidate();
}

void SensorsPage::setPM25(int v) {
    touchgfx::Unicode::snprintf(cellBuf[0][2], 20, "%d ug/m3", v);
    cellTxt[0][2].invalidate();
}
void SensorsPage::setPM10(int v) {
    touchgfx::Unicode::snprintf(cellBuf[1][2], 20, "%d ug/m3", v);
    cellTxt[1][2].invalidate();
}
void SensorsPage::setCO2(int v) {
    touchgfx::Unicode::snprintf(cellBuf[2][2], 20, "%d ppm", v);
    cellTxt[2][2].invalidate();
}
void SensorsPage::setTempHumi(int tTenths, int hPct) {
    int whole = tTenths / 10;
    int frac  = tTenths % 10;
    touchgfx::Unicode::snprintf(cellBuf[3][2], 20, "%d.%dC / %d%%", whole, frac, hPct);
    cellTxt[3][2].invalidate();
}
void SensorsPage::setRain(bool raining) {
    setText(cellTxt[4][2], cellBuf[4][2], raining ? "ON" : "OFF");
}
