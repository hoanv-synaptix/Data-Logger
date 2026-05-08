#ifndef NETWORK_PAGE_HPP
#define NETWORK_PAGE_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>

class NetworkPage : public touchgfx::Container
{
public:
    NetworkPage();

    void setMainLinkState(bool isUp);
    void setDeviceIP(const char* ip);
private:
    void setText(touchgfx::TextAreaWithOneWildcard& t,
                 touchgfx::Unicode::UnicodeChar* buf, const char* s);

    // Left Panel
    touchgfx::Box leftOuter, leftInner;
    touchgfx::TextAreaWithOneWildcard leftTitle;
    touchgfx::Unicode::UnicodeChar    leftTitleBuf[16];

    touchgfx::Box lRowOuter[5], lRowInner[5];
    touchgfx::TextAreaWithOneWildcard lLbl[5];
    touchgfx::Unicode::UnicodeChar    lLblBuf[5][16];
    touchgfx::TextAreaWithOneWildcard lVal[5];
    touchgfx::Unicode::UnicodeChar    lValBuf[5][16];

    // Right Panel
    touchgfx::Box rightOuter, rightInner;
    touchgfx::TextAreaWithOneWildcard rightTitle;
    touchgfx::Unicode::UnicodeChar    rightTitleBuf[16];

    touchgfx::Box rRowOuter[5], rRowInner[5];
    touchgfx::TextAreaWithOneWildcard rLbl[5];
    touchgfx::Unicode::UnicodeChar    rLblBuf[5][16];
    touchgfx::TextAreaWithOneWildcard rVal[5];
    touchgfx::Unicode::UnicodeChar    rValBuf[5][16];
};

#endif
