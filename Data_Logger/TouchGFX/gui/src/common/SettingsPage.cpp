#include <gui/common/SettingsPage.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

static const int PAD = 9;
static const int PNL_W = 232;
static const int PNL_H = 202;
static const int ROW_H = 27;
static const int GAP = 6;
static const int ROW_W = PNL_W - 2*PAD; // 214

SettingsPage::SettingsPage()
{
    setPosition(0, 0, 470, 202);

    touchgfx::colortype cBdr    = touchgfx::Color::getColorFromRGB(203,213,225); // border
    touchgfx::colortype cCard   = touchgfx::Color::getColorFromRGB(248,250,252); // card
    touchgfx::colortype cTxt    = touchgfx::Color::getColorFromRGB(15, 23, 42);  // text
    touchgfx::colortype cMuted  = touchgfx::Color::getColorFromRGB(71, 85, 105); // muted
    touchgfx::colortype cOdd    = touchgfx::Color::getColorFromRGB(241,245,249); // #f1f5f9
    touchgfx::colortype cEven   = touchgfx::Color::getColorFromRGB(255,255,255); // #ffffff
    
    touchgfx::colortype cAmber  = touchgfx::Color::getColorFromRGB(180, 83, 9);  // amber
    touchgfx::colortype cGreen  = touchgfx::Color::getColorFromRGB(4, 120, 87);  // green

    auto initTxt = [this](touchgfx::TextAreaWithOneWildcard& t,
        touchgfx::Unicode::UnicodeChar* buf,
        int x, int y, int w, int h,
        const char* str, touchgfx::colortype col, uint16_t typo)
    {
        t.setPosition(x,y,w,h);
        t.setTypedText(touchgfx::TypedText(typo));
        t.setColor(col); t.setWildcard(buf);
        touchgfx::Unicode::strncpy(buf, str, 16);
        add(t); t.invalidate();
    };

    auto initPanel = [this, cBdr, cCard](touchgfx::Box& outer, touchgfx::Box& inner, int x) {
        outer.setPosition(x, 0, PNL_W, PNL_H);
        outer.setColor(cBdr); add(outer);
        inner.setPosition(x+1, 1, PNL_W-2, PNL_H-2);
        inner.setColor(cCard); add(inner);
    };

    auto initRow = [this, cBdr, cOdd, cEven, cTxt, cMuted, initTxt](
        int panelX, int rowIdx, 
        touchgfx::Box& out, touchgfx::Box& in,
        touchgfx::TextAreaWithOneWildcard& lTxt, touchgfx::Unicode::UnicodeChar* lBuf, const char* lStr,
        touchgfx::TextAreaWithOneWildcard& rTxt, touchgfx::Unicode::UnicodeChar* rBuf, const char* rStr,
        touchgfx::colortype rCol)
    {
        int y = 4 + 28 + 6 + rowIdx*(ROW_H+GAP);
        out.setPosition(panelX + PAD, y, ROW_W, ROW_H);
        out.setColor(cBdr); add(out);
        in.setPosition(panelX + PAD + 1, y + 1, ROW_W - 2, ROW_H - 2);
        in.setColor(rowIdx % 2 == 1 ? cOdd : cEven); add(in);

        // left text
        initTxt(lTxt, lBuf, panelX + PAD + 8, y + 5, 100, 16, lStr, cMuted, T___SINGLEUSE_PPYL);
        // right text
        initTxt(rTxt, rBuf, panelX + PAD + ROW_W - 85, y + 5, 80, 16, rStr, rCol, T___SINGLEUSE_PPYL);
    };

    // ── Left Panel ────────────────────────────────────────────────────────────
    initPanel(leftOuter, leftInner, 0);
    initTxt(leftTitle, leftTitleBuf, PAD, 4, 214, 28, "Device", cTxt, T___SINGLEUSE_MK9N);

    const char* lNames[5] = {"Device name", "Run mode", "Date / Time", "Firmware", "System"};
    const char* lVals[5]  = {"DL-001", "Auto", "10:25", "v1.0.3", "Normal"};
    touchgfx::colortype lCols[5] = {cTxt, cGreen, cTxt, cTxt, cGreen};

    for(int i=0; i<5; i++) {
        initRow(0, i, lRowOuter[i], lRowInner[i],
                lLbl[i], lLblBuf[i], lNames[i],
                lVal[i], lValBuf[i], lVals[i], lCols[i]);
    }

    // ── Right Panel ───────────────────────────────────────────────────────────
    initPanel(rightOuter, rightInner, 237); // 470 - 232 = 238
    initTxt(rightTitle, rightTitleBuf, 237+PAD, 4, 214, 28, "Maintenance", cTxt, T___SINGLEUSE_MK9N);

    const char* rNames[5] = {"Screen", "Alarm sound", "Calibration", "FOTA", "Access"};
    const char* rVals[5]  = {"80%", "On", "Locked", "Ready", "Operator"};
    touchgfx::colortype rCols[5] = {cTxt, cGreen, cAmber, cGreen, cTxt};

    for(int i=0; i<5; i++) {
        initRow(237, i, rRowOuter[i], rRowInner[i],
                rLbl[i], rLblBuf[i], rNames[i],
                rVal[i], rValBuf[i], rVals[i], rCols[i]);
    }
}

void SettingsPage::setText(touchgfx::TextAreaWithOneWildcard& t,
                           touchgfx::Unicode::UnicodeChar* buf, const char* s)
{
    touchgfx::Unicode::strncpy(buf, s, 16);
    t.invalidate();
}
