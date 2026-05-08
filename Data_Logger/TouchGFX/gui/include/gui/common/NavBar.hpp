#ifndef NAVBAR_HPP
#define NAVBAR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class NavBar : public touchgfx::Container
{
public:
    NavBar();
    virtual ~NavBar() {}

    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);

    void setMenuChangedCallback(touchgfx::GenericCallback<uint8_t>& callback)
    {
        menuChangedCallback = &callback;
    }

    void setActive(uint8_t index);

private:
    touchgfx::Box background;
    touchgfx::Box topBorder;
    
    touchgfx::Box btnBorders[5];
    touchgfx::Box btnBackgrounds[5];
    touchgfx::TextAreaWithOneWildcard btnTexts[5];
    touchgfx::Unicode::UnicodeChar btnBuffers[5][12];

    uint8_t currentIndex;
    touchgfx::GenericCallback<uint8_t>* menuChangedCallback;

    void setText(touchgfx::TextAreaWithOneWildcard& text, touchgfx::Unicode::UnicodeChar* buffer, const char* value);
};

#endif
