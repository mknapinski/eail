/*
 * Tested interface: AtkValue
 *
 * Tested AtkObject: EailColorselector
 *
 * Description: Test AtkValue interface
 *
 * Test input: accessible object representing EailColorselector
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailColorselector")

static void
_init_colorselector(Evas_Object *obj)
{
   Evas_Object *bx = NULL;
   Evas_Object *cs = NULL;
   Evas_Object *fr = NULL;

   bx = elm_box_add(obj);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_resize_object_add(obj, bx);
   evas_object_show(bx);

   fr = elm_frame_add(obj);
   evas_object_size_hint_weight_set(fr, 1.0, 0.5);
   evas_object_size_hint_align_set(fr, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(fr, "Color Selector");
   elm_box_pack_end(bx, fr);
   evas_object_show(fr);

   cs = elm_colorselector_add(obj);
   elm_colorselector_palette_name_set(cs, "painting");
   evas_object_size_hint_weight_set(cs, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(cs, EVAS_HINT_FILL, 0.0);
   elm_colorselector_color_set(cs, 100, 100, 100, 133);
   evas_object_show(cs);
   elm_object_content_set(fr, cs);

   evas_object_resize(obj, 320, 480);
}

static void
_get_rgba(const GValue *color, int *r, int *g, int *b, int *a)
{
   int col = g_value_get_int(color);
   *a = (0xFF000000 & (guint)col) >> 24;
   *r = (0x00FF0000 & col) >> 16;
   *g = (0x0000FF00 & col) >> 8;
   *b = 0x000000FF & col;
}

static void
_set_rgba(GValue *color, int r, int g, int b, int a)
{
   int col = (a << 24) | (r << 16) | (g << 8) | b;
   g_value_set_int(color, col);
}

static void
_do_test(AtkObject *obj)
{
   gint r = 0;
   gint g = 0;
   gint b = 0;
   gint a = 0;
   GValue value = G_VALUE_INIT;

   g_assert(ATK_IS_VALUE(obj));

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   _get_rgba(&value, &r, &g, &b, &a);
   g_assert(r == 100 && g == 100 && b == 100 && a == 133);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   _get_rgba(&value, &r, &g, &b, &a);
   g_assert(r == 255 && g == 255 && b == 255 && a == 255);

   _set_rgba(&value, 77, 88, 10, 180);
   g_assert(atk_value_set_current_value(ATK_VALUE(obj), &value));
   atk_value_get_current_value(ATK_VALUE(obj), &value);
   _get_rgba(&value, &r, &g, &b, &a);
   g_assert(r == 77 && g == 88 && b == 10 && a == 180);

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   _get_rgba(&value, &r, &g, &b, &a);
   g_assert(r == 0 && g == 0 && b == 0 && a == 0);

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   _get_rgba(&value, &r, &g, &b, &a);
   g_assert(r == 1 && g == 1 && b == 1 && a == 1);

   eailu_test_atk_focus(obj, TRUE);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win = NULL;

   win = eailu_create_test_window_with_glib_init(NULL, _on_focus_in);
   _init_colorselector(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()

