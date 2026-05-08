#ifndef TOPBAR_HPP
#define TOPBAR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class TopBar : public touchgfx::Container
{
public:
    TopBar();
    virtual ~TopBar() {}

private:
    touchgfx::Box background;
    touchgfx::Box bottomBorder;
    touchgfx::Box greenDot;
    
    touchgfx::TextAreaWithOneWildcard titleText;
    touchgfx::Unicode::UnicodeChar titleBuffer[16];

    touchgfx::Box netDot;
    touchgfx::TextAreaWithOneWildcard netText;
    touchgfx::Unicode::UnicodeChar netBuffer[4];

    touchgfx::Box mqttDot;
    touchgfx::TextAreaWithOneWildcard mqttText;
    touchgfx::Unicode::UnicodeChar mqttBuffer[5];

    touchgfx::TextAreaWithOneWildcard timeText;
    touchgfx::Unicode::UnicodeChar timeBuffer[6];

    void setText(touchgfx::TextAreaWithOneWildcard& text, touchgfx::Unicode::UnicodeChar* buffer, const char* value);
};

#endif
