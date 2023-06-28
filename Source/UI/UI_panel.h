#include "UI_struct.h"

UI_panel *UI_newPanel(UI_menu *menu, int width, int height, UI_anchor *anchorLT, int sizeFactor, UI_panelType type);
UI_panelButton *UI_newButtonPanel(UI_panel *parent, char **text, SDL_Rect rect, int orientation,void (*onToggle)(void *self));
UI_panelButton *UI_newButtonPanelIcon(UI_panel *parent, SDL_Rect rect, int orientation,void (*onToggle)(void *self),char *icon);
void UI_updatePanel(void *self);
void UI_updateButtonPanel(void *self);
void UI_panelButtonHandleMouseEvent(UI_panelButton *buttonPanel, bool isDown);
void UI_renderButtonPanel(void *self);
void UI_renderPanel(void *self);
void UI_FreePanel(void *self);
void UI_FreeButtonPanel(void *self);