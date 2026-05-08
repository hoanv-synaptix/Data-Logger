#include <gui/common/NavBar.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

NavBar::NavBar() : currentIndex(0), menuChangedCallback(0)
{
    setPosition(0, 0, 480, 28);

    background.setPosition(0, 0, 480, 28);
    background.setColor(touchgfx::Color::getColorFromRGB(17, 24, 39)); // COLORS.shell
    add(background);

    topBorder.setPosition(0, 0, 480, 2);
    topBorder.setColor(touchgfx::Color::getColorFromRGB(51, 65, 85)); // #334155
    add(topBorder);

    const char* names[5] = {"Dashboard", "Sensors", "Alarms", "Network", "Settings"};
    int widths[5] = {93, 94, 94, 94, 93};
    int xPos = 2;

    setTouchable(true);

    // Tab label centering calibration for Inter-Bold 12px (Medium typography):
    // Measured avg char width ≈ 7px for mixed upper/lower case
    // Dashboard(9)=63, Sensors(7)=49, Alarms(6)=42, Network(7)=49, Settings(8)=56
    int textWidths[5] = {63, 49, 42, 49, 56};

    for (int i = 0; i < 5; i++)
    {
        // Outer border box
        btnBorders[i].setPosition(xPos, 4, widths[i], 22);
        btnBorders[i].setColor(touchgfx::Color::getColorFromRGB(51, 65, 85));
        add(btnBorders[i]);

        // Inner background
        btnBackgrounds[i].setPosition(xPos + 1, 5, widths[i] - 2, 20);
        btnBackgrounds[i].setColor(touchgfx::Color::getColorFromRGB(31, 41, 55));
        add(btnBackgrounds[i]);

        // Text centered horizontally: centeredX = innerLeft + (innerW - textW) / 2
        int innerLeft = xPos + 1;
        int innerW    = widths[i] - 2;
        int centeredX = innerLeft + (innerW - textWidths[i]) / 2;

        // Vertically: inner Y=5, H=20, font cap-height ~8px (top-bearing ~2px)
        // Optical center: inner_Y + (inner_H - cap_height) / 2 - top_bearing
        //               = 5 + (20 - 8) / 2 - 2 = 5 + 6 - 2 = 9 → but renders 1-2px low
        // Empirical fix: use Y=7
        btnTexts[i].setPosition(centeredX, 7, textWidths[i], 14);
        btnTexts[i].setTypedText(touchgfx::TypedText(T___SINGLEUSE_PPYL));
        btnTexts[i].setColor(touchgfx::Color::getColorFromRGB(203, 213, 225));
        btnTexts[i].setWildcard(btnBuffers[i]);
        setText(btnTexts[i], btnBuffers[i], names[i]);
        add(btnTexts[i]);

        xPos += widths[i] + 2;
    }

    setActive(0);
}

void NavBar::setText(touchgfx::TextAreaWithOneWildcard& text, touchgfx::Unicode::UnicodeChar* buffer, const char* value)
{
    touchgfx::Unicode::strncpy(buffer, value, 12);
    text.invalidate();
}

void NavBar::setActive(uint8_t index)
{
    if (index >= 5) return;
    
    // Reset old
    btnBorders[currentIndex].setColor(touchgfx::Color::getColorFromRGB(51, 65, 85)); // inactive border
    btnBackgrounds[currentIndex].setColor(touchgfx::Color::getColorFromRGB(31, 41, 55)); // inactive bg (#1f2937)
    btnTexts[currentIndex].setColor(touchgfx::Color::getColorFromRGB(203, 213, 225)); // inactive text (#cbd5e1)
    btnBorders[currentIndex].invalidate();
    btnBackgrounds[currentIndex].invalidate();
    btnTexts[currentIndex].invalidate();

    // Set new
    currentIndex = index;
    btnBorders[currentIndex].setColor(touchgfx::Color::getColorFromRGB(186, 230, 253)); // active border (#bae6fd)
    btnBackgrounds[currentIndex].setColor(touchgfx::Color::getColorFromRGB(3, 105, 161)); // active bg (#0369a1)
    btnTexts[currentIndex].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255)); // active text
    btnBorders[currentIndex].invalidate();
    btnBackgrounds[currentIndex].invalidate();
    btnTexts[currentIndex].invalidate();
}

void NavBar::handleClickEvent(const touchgfx::ClickEvent& evt)
{
    if (evt.getType() == touchgfx::ClickEvent::PRESSED)
    {
        int x = evt.getX();
        int widths[5] = {93, 94, 94, 94, 93};
        int xPos = 2;
        for (int i = 0; i < 5; i++)
        {
            if (x >= xPos && x <= xPos + widths[i])
            {
                if (i != currentIndex)
                {
                    setActive(i);
                    if (menuChangedCallback && menuChangedCallback->isValid())
                    {
                        menuChangedCallback->execute(i);
                    }
                }
                break;
            }
            xPos += widths[i] + 2;
        }
    }
}

