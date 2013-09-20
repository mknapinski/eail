/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailProgressbar
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree.
 *
 * Test input: accessible object representing EailProgressbar
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

INIT_TEST("EailProgressbar")

const char * const progressbar_name = "progress-bar-name";
const double progressbar_set = 0.2;

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_OBJECT(obj));
   g_assert(atk_object_get_role(obj) == ATK_ROLE_PROGRESS_BAR);

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   eailu_test_code_called = 1;
}

static void
_init_progressbar(Evas_Object *win)
{
   Evas_Object *bg, *bx, *pb;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   elm_win_resize_object_add(win, bx);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bx);

   pb = elm_progressbar_add(win);
   elm_object_text_set(pb, progressbar_name);
   elm_progressbar_span_size_set(pb, 260);
   elm_progressbar_value_set(pb, progressbar_set);
   evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(bx, pb);
   evas_object_show(pb);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_progressbar(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
