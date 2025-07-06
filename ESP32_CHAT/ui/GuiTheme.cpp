#include "GuiTheme.h"
#include <string.h>

namespace UI {

lv_style_t stylePanelVista;
lv_style_t styleMenuBar;
lv_style_t styleBtnVista;
lv_style_t styleBtnVistaPressed;
lv_style_t widgetStyle;
const lv_font_t *emojiFont = nullptr;

void initTheme()
{
    // Vista glassy main panel style
    lv_style_init(&stylePanelVista);
    lv_style_set_radius(&stylePanelVista, 12);
    lv_style_set_bg_color(&stylePanelVista, lv_color_hex(0xD6E8FA));
    lv_style_set_bg_opa(&stylePanelVista, LV_OPA_60);
    lv_style_set_border_width(&stylePanelVista, 2);
    lv_style_set_border_color(&stylePanelVista, lv_color_hex(0x4A90E2));
    lv_style_set_shadow_width(&stylePanelVista, 8);

    // Menu bar style (semi-transparent, subtle top border)
    lv_style_init(&styleMenuBar);
    lv_style_set_radius(&styleMenuBar, 8);
    lv_style_set_bg_color(&styleMenuBar, lv_color_hex(0xC3D0E2));
    lv_style_set_bg_opa(&styleMenuBar, LV_OPA_70);
    lv_style_set_border_color(&styleMenuBar, lv_color_hex(0x7BAAF7));
    lv_style_set_border_width(&styleMenuBar, 2);
    lv_style_set_border_side(&styleMenuBar, LV_BORDER_SIDE_TOP);

    // Button style
    lv_style_init(&styleBtnVista);
    lv_style_set_radius(&styleBtnVista, 8);
    lv_style_set_bg_color(&styleBtnVista, lv_color_hex(0xF0F6FF));
    lv_style_set_bg_opa(&styleBtnVista, LV_OPA_80);
    lv_style_set_border_color(&styleBtnVista, lv_color_hex(0x4A90E2));
    lv_style_set_border_width(&styleBtnVista, 2);

    // Button pressed style
    lv_style_init(&styleBtnVistaPressed);
    lv_style_set_radius(&styleBtnVistaPressed, 8);
    lv_style_set_bg_color(&styleBtnVistaPressed, lv_color_hex(0xB0C4DE));
    lv_style_set_bg_opa(&styleBtnVistaPressed, LV_OPA_80);
    lv_style_set_border_color(&styleBtnVistaPressed, lv_color_hex(0x4A90E2));
    lv_style_set_border_width(&styleBtnVistaPressed, 2);

    // Fallback widget style (legacy)
    lv_style_init(&widgetStyle);
    lv_style_set_radius(&widgetStyle, 5);
    lv_style_set_bg_opa(&widgetStyle, LV_OPA_COVER);
    lv_style_set_bg_color(&widgetStyle, lv_palette_lighten(LV_PALETTE_GREY, 2));
    lv_style_set_border_width(&widgetStyle, 2);
    lv_style_set_border_color(&widgetStyle, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_pad_all(&widgetStyle, 10);
    lv_style_set_text_color(&widgetStyle, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_text_letter_space(&widgetStyle, 5);
    lv_style_set_text_line_space(&widgetStyle, 20);
    lv_style_set_text_decor(&widgetStyle, LV_TEXT_DECOR_UNDERLINE);

    emojiFont = LV_FONT_DEFAULT;
}

} // namespace UI
