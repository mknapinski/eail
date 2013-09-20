/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailSeparator
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree.
 *
 * Test input: accessible object representing EailSeparator
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailSeparator")

static void
_init_separator(Evas_Object *win)
{
   Evas_Object *bg, *bxh, *bxv, *spv, *sph, *bt;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bxh = elm_box_add(win);
   evas_object_size_hint_weight_set(bxh, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_horizontal_set(bxh, EINA_TRUE);
   elm_win_resize_object_add(win, bxh);
   evas_object_show(bxh);

   bxv = elm_box_add(win);
   evas_object_size_hint_weight_set(bxv, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bxh, bxv);
   evas_object_show(bxv);

   // horizontal separator
   bt = elm_button_add(win);
   elm_object_text_set(bt, "Top button");
   elm_box_pack_end(bxv, bt);
   evas_object_show(bt);

   sph = elm_separator_add(win);
   elm_separator_horizontal_set(sph, EINA_TRUE);
   elm_box_pack_end(bxv, sph);
   evas_object_show(sph);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Bottom button");
   elm_box_pack_end(bxv, bt);
   evas_object_show(bt);

   // vertical separator
   bt = elm_button_add(win);
   elm_object_text_set(bt, "Left button");
   elm_box_pack_end(bxh, bt);
   evas_object_show(bt);

   spv = elm_separator_add(win);
   elm_box_pack_end(bxh, spv);
   evas_object_show(spv);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Right button");
   elm_box_pack_end(bxh, bt);
   evas_object_show(bt);
}

static void
_do_test(AtkObject *obj)
{
   AtkStateSet *state_set = atk_object_ref_state_set(obj);
   static int test_count, test_vertical_count, test_horizontal_count;

   test_count++;

   g_assert(atk_object_get_role(obj) == ATK_ROLE_SEPARATOR);
   g_assert(!atk_state_set_is_empty(state_set));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   if (atk_state_set_contains_state(state_set, ATK_STATE_VERTICAL))
     {
        test_vertical_count++;
        _printf("atk_object_ref_state_set: vertical\n");
     }

   if (atk_state_set_contains_state(state_set, ATK_STATE_HORIZONTAL))
     {
        test_horizontal_count++;
        _printf("atk_object_ref_state_set: horizontal\n");
     }

   g_assert((test_vertical_count + test_horizontal_count) == test_count);

   if (2 == test_count)
     g_assert(1 == test_vertical_count && 1 == test_horizontal_count);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_separator(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
