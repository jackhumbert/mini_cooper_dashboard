#include "widget.hpp"

class ScreenBrightness : public Widget {
public:
    virtual void update(void) override;
    static void fade_in(void);
};
