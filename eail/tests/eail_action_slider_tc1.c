/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailActionSlider
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree.
 *
 * Test input: accessible object representing EailActionSlider
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailActionSlider")

static void
_init_action_slider(Evas_Object *win)
{
   Evas_Object *bg, *bx, *as;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, 0);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, 0);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   as = elm_actionslider_add(win);
   evas_object_size_hint_weight_set(as, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(as, EVAS_HINT_FILL, 0);
   elm_object_part_text_set(as, "left", "left1");
   elm_object_part_text_set(as, "center", "center1");
   elm_object_part_text_set(as, "right", "right1");
   elm_actionslider_indicator_pos_set(as, ELM_ACTIONSLIDER_LEFT);
   evas_object_show(as);
   elm_box_pack_end(bx, as);
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_OBJECT(obj));
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
   _init_action_slider(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
