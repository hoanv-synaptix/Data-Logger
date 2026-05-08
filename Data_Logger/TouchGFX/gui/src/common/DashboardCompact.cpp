/*
 * DashboardCompact.cpp
 * Layout (470x202 at X=5,Y=37):
 *   Row1 (H=60): 4 cards [0,118,236,354], W=[114,114,114,116], gap=4
 *   Row2 (Y=64, H=138): Air(W=148) | gap=4 | Trend(W=318)
 *
 * Fonts: Large=26px, Medium=12px, Small=9px, Tiny=7px
 */
#include <gui/common/DashboardCompact.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

// ── Layout constants ─────────────────────────────────────────────────────────
static const int CX0=0,   CX1=118, CX2=236, CX3=354;
static const int CW0=114, CW1=114, CW2=114, CW3=116;
static const int CH=60, PAD=8;

static const int R2Y=64, R2H=138;
static const int AIR_W=148, TRD_X=152, TRD_W=318;

// Chart inside Trend panel: leave 20px left column for Y-axis labels
static const int YLBL_W=20;
static const int CHT_X=TRD_X+2+YLBL_W;       // = 174
static const int CHT_Y=R2Y+22;                // = 86
static const int CHT_W=TRD_W-4-YLBL_W;        // = 294
static const int CHT_H=R2H-24;                // = 114
static const int BAR_N=14, BAR_GAP=2;
static const int BAR_W=(CHT_W-(BAR_N-1)*BAR_GAP)/BAR_N; // =(294-26)/14=19

DashboardCompact::DashboardCompact()
{
    setPosition(0, 0, 470, 202);

    // ── Colors ───────────────────────────────────────────────────────────────
    touchgfx::colortype cBdr  = touchgfx::Color::getColorFromRGB(148,163,184);
    touchgfx::colortype cCard = touchgfx::Color::getColorFromRGB(248,250,252);
    touchgfx::colortype cBg2  = touchgfx::Color::getColorFromRGB(238,242,247);
    touchgfx::colortype cTxt  = touchgfx::Color::getColorFromRGB(15, 23, 42);
    touchgfx::colortype cMut  = touchgfx::Color::getColorFromRGB(71, 85,105);
    touchgfx::colortype cDiv  = touchgfx::Color::getColorFromRGB(203,213,225);
    touchgfx::colortype cGrnT = touchgfx::Color::getColorFromRGB(4, 120, 87);
    touchgfx::colortype cGrnB = touchgfx::Color::getColorFromRGB(220,252,231);
    touchgfx::colortype cAmbT = touchgfx::Color::getColorFromRGB(180, 83,  9);
    touchgfx::colortype cAmbB = touchgfx::Color::getColorFromRGB(254,243,199);
    touchgfx::colortype cBluT = touchgfx::Color::getColorFromRGB(3, 105,161);
    touchgfx::colortype cBluB = touchgfx::Color::getColorFromRGB(219,234,254);

    // ── Helpers (proven pattern — identical to original working code) ─────────
    auto initCard = [this, cBdr, cCard](touchgfx::Box& outer, touchgfx::Box& inner,
        int x, int y, int w, int h)
    {
        outer.setPosition(x,y,w,h); outer.setColor(cBdr); add(outer);
        inner.setPosition(x+1,y+1,w-2,h-2); inner.setColor(cCard); add(inner);
    };

    auto initTxt = [this](touchgfx::TextAreaWithOneWildcard& t,
        touchgfx::Unicode::UnicodeChar* buf,
        int x, int y, int w, int h,
        const char* str, touchgfx::colortype col, uint16_t typo)
    {
        t.setPosition(x,y,w,h);
        t.setTypedText(touchgfx::TypedText(typo));
        t.setColor(col);
        t.setWildcard(buf);
        touchgfx::Unicode::strncpy(buf, str, 12);
        add(t);
        t.invalidate();
    };

    auto initPill = [this](touchgfx::Box& box, touchgfx::TextAreaWithOneWildcard& t,
        touchgfx::Unicode::UnicodeChar* buf,
        int x, int y, int pw, const char* str,
        touchgfx::colortype bg, touchgfx::colortype fg)
    {
        box.setPosition(x,y,pw,16); box.setColor(bg); add(box);
        // Estimate text pixel width: Small(9px) uppercase avg ~8px/char
        int len = 0; while(str[len]) len++;
        int textW = len * 8;
        if(textW > pw - 6) textW = pw - 6;  // clamp to pill width
        int textX = x + (pw - textW) / 2;   // center horizontally
        t.setPosition(textX, y+2, textW, 12);
        t.setTypedText(touchgfx::TypedText(T___SINGLEUSE_ZEP0));
        t.setColor(fg); t.setWildcard(buf);
        touchgfx::Unicode::strncpy(buf,str,6);
        add(t); t.invalidate();
    };

    // ── ROW 1: Metric Cards ───────────────────────────────────────────────────
    // Layout per card (H=60):
    //  Y=5  : Label(Small9) + Pill(Tiny7) right
    //  Y=20 : Value(Large26) left, Unit(Tiny7) right-of-value at Y=35
    initCard(pm25CardOuter, pm25Card, CX0, 0, CW0, CH);
    initCard(pm10CardOuter, pm10Card, CX1, 0, CW1, CH);
    initCard(co2CardOuter,  co2Card,  CX2, 0, CW2, CH);
    initCard(aqiCardOuter,  aqiCard,  CX3, 0, CW3, CH);

    // Labels
    initTxt(pm25Label, pm25LabelBuf, CX0+PAD, 5, 55, 12, "PM2.5", cMut, T___SINGLEUSE_ZEP0);
    initTxt(pm10Label, pm10LabelBuf, CX1+PAD, 5, 55, 12, "PM10",  cMut, T___SINGLEUSE_ZEP0);
    initTxt(co2Label,  co2LabelBuf,  CX2+PAD, 5, 55, 12, "CO2",   cMut, T___SINGLEUSE_ZEP0);
    initTxt(aqiLabel,  aqiLabelBuf,  CX3+PAD, 5, 55, 12, "AQI",   cMut, T___SINGLEUSE_ZEP0);

    // Status pills — Small(9px) font, H=16, right-anchored
    // "OK" needs W~30px, "WARN" needs W~42px at Small(9px)
    initPill(pm25PillBox, pm25PillTxt, pm25PillBuf, CX0+CW0-PAD-30, 3, 30, "OK",   cGrnB, cGrnT);
    initPill(pm10PillBox, pm10PillTxt, pm10PillBuf, CX1+CW1-PAD-30, 3, 30, "OK",   cGrnB, cGrnT);
    initPill(co2PillBox,  co2PillTxt,  co2PillBuf,  CX2+CW2-PAD-30, 3, 30, "OK",   cGrnB, cGrnT);
    initPill(aqiPillBox,  aqiPillTxt,  aqiPillBuf,  CX3+CW3-PAD-42, 3, 42, "WARN", cAmbB, cAmbT);

    // Values (Large26 — left-padded)
    initTxt(pm25Value, pm25Buf, CX0+PAD, 20, 52, 28, "35",  cTxt, T___SINGLEUSE_MK9N);
    initTxt(pm10Value, pm10Buf, CX1+PAD, 20, 52, 28, "58",  cTxt, T___SINGLEUSE_MK9N);
    initTxt(co2Value,  co2Buf,  CX2+PAD, 20, 52, 28, "510", cTxt, T___SINGLEUSE_MK9N);
    initTxt(aqiValue,  aqiBuf,  CX3+PAD, 20, 52, 28, "72",  cTxt, T___SINGLEUSE_MK9N);

    // Units (Tiny7 — right-of-value, baseline Y=35)
    initTxt(pm25Unit, pm25UnitBuf, CX0+PAD+54, 35, 42, 10, "ug/m3", cMut, T___SINGLEUSE_62E6);
    initTxt(pm10Unit, pm10UnitBuf, CX1+PAD+54, 35, 42, 10, "ug/m3", cMut, T___SINGLEUSE_62E6);
    initTxt(co2Unit,  co2UnitBuf,  CX2+PAD+54, 35, 34, 10, "ppm",   cMut, T___SINGLEUSE_62E6);

    // ── ROW 2: Air Status Panel (X=0, Y=64, W=148, H=138) ────────────────────
    initCard(airOuter, airInner, 0, R2Y, AIR_W, R2H);

    initTxt(airTitle, airTitleBuf, PAD, R2Y+5, AIR_W-2*PAD, 12, "AIR STATUS", cMut, T___SINGLEUSE_ZEP0);

    // Divider
    airDivider.setPosition(PAD, R2Y+19, AIR_W-2*PAD, 1);
    airDivider.setColor(cDiv); add(airDivider);

    // "GOOD" centered: inner W=146, estimate 4 chars × 17px = 68px
    // Center: X = 1 + (146-80)/2 = 1+33 = 34; use W=80 to safely contain text
    initTxt(airState, airStateBuf, 34, R2Y+24, 80, 30, "GOOD", cGrnT, T___SINGLEUSE_MK9N);

    // MiniBox TEMP — 3px below GOOD zone
    tempOuter.setPosition(PAD, R2Y+60, AIR_W-2*PAD, 30);
    tempOuter.setColor(cDiv); add(tempOuter);
    tempInner.setPosition(PAD+1, R2Y+61, AIR_W-2*PAD-2, 28);
    tempInner.setColor(cBg2); add(tempInner);
    initTxt(tempLabel, tempLabelBuf, PAD+8, R2Y+69, 48, 14, "TEMP", cMut, T___SINGLEUSE_PPYL);
    initTxt(tempVal,   tempValBuf,   PAD+40, R2Y+69, 83, 14, "24.0C", cTxt, T_TEXT_RIGHT_MED);

    // MiniBox HUMI — 3px below GOOD zone
    humiOuter.setPosition(PAD, R2Y+94, AIR_W-2*PAD, 30);
    humiOuter.setColor(cDiv); add(humiOuter);
    humiInner.setPosition(PAD+1, R2Y+95, AIR_W-2*PAD-2, 28);
    humiInner.setColor(cBg2); add(humiInner);
    initTxt(humiLabel, humiLabelBuf, PAD+8, R2Y+103, 48, 14, "HUMI", cMut, T___SINGLEUSE_PPYL);
    initTxt(humiVal,   humiValBuf,   PAD+40, R2Y+103, 83, 14, "62%",  cTxt, T_TEXT_RIGHT_MED);

    // ── ROW 2: AQI Trend Panel (X=152, Y=64, W=318, H=138) ──────────────────
    initCard(trendOuter, trendInner, TRD_X, R2Y, TRD_W, R2H);

    initTxt(trendTitle, trendTitleBuf, TRD_X+PAD, R2Y+5, 140, 12, "AQI Trend", cTxt, T___SINGLEUSE_PPYL);

    // LIVE pill
    initPill(trendLiveBox, trendLiveTxt, trendLiveBuf, TRD_X+TRD_W-36, R2Y+5, 32, "LIVE", cBluB, cBluT);

    // Chart border + bar background
    chartOuter.setPosition(TRD_X+2, CHT_Y, TRD_W-4, CHT_H);
    chartOuter.setColor(cDiv); add(chartOuter);
    chartBg.setPosition(CHT_X, CHT_Y, CHT_W, CHT_H);
    chartBg.setColor(cBg2); add(chartBg);

    // Y-axis labels (Tiny7) — 20px left column above bar area
    initTxt(axisY100, axisY100Buf, TRD_X+3, CHT_Y+1,               YLBL_W-3, 9, "100", cMut, T___SINGLEUSE_62E6);
    initTxt(axisY50,  axisY50Buf,  TRD_X+3, CHT_Y+CHT_H/2-5,       YLBL_W-3, 9, "50",  cMut, T___SINGLEUSE_62E6);
    initTxt(axisY0,   axisY0Buf,   TRD_X+3, CHT_Y+CHT_H-9,         YLBL_W-3, 9, "0",   cMut, T___SINGLEUSE_62E6);

    // X-axis labels — placed at very bottom of chart, 10px from bottom
    initTxt(axisXOld, axisXOldBuf, CHT_X+2,        CHT_Y+CHT_H-10, 24, 9, "14d", cMut, T___SINGLEUSE_62E6);
    initTxt(axisXNow, axisXNowBuf, CHT_X+CHT_W-26, CHT_Y+CHT_H-10, 26, 9, "now", cMut, T___SINGLEUSE_62E6);

    // Grid lines (at 25%, 50%, 75%)
    gridLine0.setPosition(CHT_X, CHT_Y + CHT_H/4,   CHT_W, 1); gridLine0.setColor(cDiv); add(gridLine0);
    gridLine1.setPosition(CHT_X, CHT_Y + CHT_H/2,   CHT_W, 1); gridLine1.setColor(cDiv); add(gridLine1);
    gridLine2.setPosition(CHT_X, CHT_Y + 3*CHT_H/4, CHT_W, 1); gridLine2.setColor(cDiv); add(gridLine2);

    const uint8_t initBars[14] = {42,48,46,52,50,58,54,61,59,64,67,70,66,72};
    setTrend(initBars, 14);
}

// ── Helpers ───────────────────────────────────────────────────────────────────
void DashboardCompact::setText(touchgfx::TextAreaWithOneWildcard& t,
                               touchgfx::Unicode::UnicodeChar* buf, const char* val)
{
    touchgfx::Unicode::strncpy(buf, val, 12);
    t.invalidate();
}

void DashboardCompact::setNumber(touchgfx::TextAreaWithOneWildcard& t,
                                 touchgfx::Unicode::UnicodeChar* buf,
                                 touchgfx::TextAreaWithOneWildcard* unit, int val)
{
    touchgfx::Unicode::snprintf(buf, 8, "%d", val);
    t.invalidate();
    if(unit) unit->invalidate();
}

void DashboardCompact::setTrend(const uint8_t* values, uint8_t count)
{
    // Leave 9px bottom for x-axis labels
    int maxH = CHT_H - 10;
    int x    = CHT_X;
    for(int i = 0; i < BAR_N && i < count; i++)
    {
        int h = (values[i] * maxH) / 100;
        if(h < 3) h = 3;
        int y = CHT_Y + maxH - h;
        chartBars[i].setPosition(x, y, BAR_W, h);
        chartBars[i].setColor(i >= 10
            ? touchgfx::Color::getColorFromRGB(180,83,9)
            : touchgfx::Color::getColorFromRGB(3,105,161));
        add(chartBars[i]);
        x += BAR_W + BAR_GAP;
    }
}

void DashboardCompact::setPM25(int v) { setNumber(pm25Value, pm25Buf, &pm25Unit, v); }
void DashboardCompact::setPM10(int v) { setNumber(pm10Value, pm10Buf, &pm10Unit, v); }
void DashboardCompact::setCO2(int v)  { setNumber(co2Value,  co2Buf,  &co2Unit,  v); }
void DashboardCompact::setAQI(int v)  { setNumber(aqiValue,  aqiBuf,  nullptr,   v); }
