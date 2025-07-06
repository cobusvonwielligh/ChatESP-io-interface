#include "GuiTheme.h"
#include "../weather_icons.h"

namespace UI {

lv_style_t widgetStyle;
lv_font_t *emojiFont = nullptr;

static const void * emoji_path_cb(const lv_font_t * font, uint32_t unicode, uint32_t unicode_next,
                                  int32_t * offset_y, void * user_data)
{
    LV_UNUSED(font);
    LV_UNUSED(unicode_next);
    LV_UNUSED(offset_y);
    LV_UNUSED(user_data);

    if(unicode == 0xF617) return &icon_sun;
    if(unicode == 0xF600) return &icon_rain;
    return NULL;
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

    emojiFont = lv_imgfont_create(32, emoji_path_cb, NULL);
    if(emojiFont) emojiFont->fallback = LV_FONT_DEFAULT;
}

} // namespace UI
