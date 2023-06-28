#include "UI_struct.h"

UI_slider *UI_newSlider(UI_menu *menu, float *value, int width, float sizeCoef, UI_anchor *anchorCL, bool updateOnRelease, void (*onUpdate)(void *entry));
void UI_updateSlider(void *self);
void UI_SliderHandleMouseEvent(UI_slider *slider, bool isDown);
void UI_renderSlider(void *self);
void UI_FreeSlider(void *self);