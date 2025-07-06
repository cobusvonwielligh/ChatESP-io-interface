#include "GuiTheme.h"
#include "../weather_icons.h"
#include <string.h>

namespace UI {

lv_style_t widgetStyle;
lv_font_t *emojiFont = nullptr;

static bool emoji_path_cb(const lv_font_t * font, void * img_src, uint16_t len,
                          uint32_t unicode, uint32_t unicode_next)
{
    LV_UNUSED(font);
    LV_UNUSED(unicode_next);

    if(len < sizeof(lv_img_dsc_t *)) return false;
    const lv_img_dsc_t *img = NULL;
    if(unicode == 0xF617) img = &icon_sun;
    else if(unicode == 0xF600) img = &icon_rain;
    else return false;

    memcpy(img_src, &img, sizeof(img));
    return true;
}

void initTheme()
{
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

    emojiFont = lv_imgfont_create(32, emoji_path_cb);
    if(emojiFont) emojiFont->fallback = LV_FONT_DEFAULT;
}

} // namespace UI
