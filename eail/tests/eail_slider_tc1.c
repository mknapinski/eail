/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailSlider
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailSlider
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailSlider")

const char * const slider_name = "slider-name";
const double slider_min = -50;
const double slider_max = 50;
const double slider_set = 20;

static void
_init_slider(Evas_Object *win)
{
   Evas_Object *bg, *bx, *sl;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   sl = elm_slider_add(win);
   elm_object_text_set(sl, slider_name);
   elm_slider_min_max_set(sl, slider_min, slider_max);
   elm_slider_value_set(sl, slider_set);
   elm_slider_unit_format_set(sl, "%2.1f");
   elm_slider_span_size_set(sl, 120);
   evas_object_size_hint_align_set(sl, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sl);
   evas_object_show(sl);
}

static void
_do_test(AtkObject *obj)
{
   const char *name = atk_object_get_name(obj);

   g_assert(ATK_IS_OBJECT(obj));
   g_assert_cmpstr(name, ==, slider_name);
   g_assert(atk_object_get_role(obj) == ATK_ROLE_SLIDER);
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_slider(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
