#include <gui/common/TopBar.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

TopBar::TopBar()
{
    setPosition(0, 0, 480, 32);

    background.setPosition(0, 0, 480, 32);
    background.setColor(touchgfx::Color::getColorFromRGB(17, 24, 39)); // COLORS.shell (#111827)
    add(background);

    bottomBorder.setPosition(0, 30, 480, 2);
    bottomBorder.setColor(touchgfx::Color::getColorFromRGB(51, 65, 85)); // #334155
    add(bottomBorder);

    // Left side: "DATA LOGGER" label — left-aligned, no status dot
    titleText.setPosition(10, 10, 160, 20);
    titleText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_PPYL));
    titleText.setColor(touchgfx::Color::getColorFromRGB(203, 213, 225));
    titleText.setWildcard(titleBuffer);
    setText(titleText, titleBuffer, "DATA LOGGER");
    add(titleText);

    // Right side: ETH, MQTT, 10:25
    
    // ETH
    netDot.setPosition(316, 12, 8, 8);
    netDot.setColor(touchgfx::Color::getColorFromRGB(34, 197, 94));
    add(netDot);
    netText.setPosition(328, 11, 30, 20); // Y=11 (Font 9, Box 32)
    netText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_ZEP0));
    netText.setColor(touchgfx::Color::getColorFromRGB(203, 213, 225));
    netText.setWildcard(netBuffer);
    setText(netText, netBuffer, "ETH");
    add(netText);
    
    // MQTT
    mqttDot.setPosition(368, 12, 8, 8);
    mqttDot.setColor(touchgfx::Color::getColorFromRGB(34, 197, 94));
    add(mqttDot);
    mqttText.setPosition(380, 11, 40, 20); // Y=11
    mqttText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_ZEP0));
    mqttText.setColor(touchgfx::Color::getColorFromRGB(203, 213, 225));
    mqttText.setWildcard(mqttBuffer);
    setText(mqttText, mqttBuffer, "MQTT");
    add(mqttText);

    timeText.setPosition(430, 11, 40, 20); // Y=11
    timeText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_ZEP0));
    timeText.setColor(touchgfx::Color::getColorFromRGB(203, 213, 225));
    timeText.setWildcard(timeBuffer);
    setText(timeText, timeBuffer, "10:25");
    add(timeText);
}

void TopBar::setText(touchgfx::TextAreaWithOneWildcard& text, touchgfx::Unicode::UnicodeChar* buffer, const char* value)
{
    touchgfx::Unicode::strncpy(buffer, value, 20);
    text.invalidate();
}

