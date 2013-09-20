/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailColorselector
 *
 * Description: Test AtkObject interface
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
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_OBJECT(obj));
   g_assert(atk_object_get_role(obj) == ATK_ROLE_COLOR_CHOOSER);

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

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

