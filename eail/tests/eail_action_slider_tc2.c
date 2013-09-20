/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailActionSlider
 *
 * Description: Test AtkAction interface
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

   as = elm_actionslider_add(win);
   evas_object_size_hint_weight_set(as, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(as, EVAS_HINT_FILL, 0);
   elm_object_part_text_set(as, "center", "center2");
   elm_object_part_text_set(as, "right", "right2");
   elm_actionslider_enabled_pos_set(as, ELM_ACTIONSLIDER_CENTER | ELM_ACTIONSLIDER_RIGHT);
   elm_actionslider_indicator_pos_set(as, ELM_ACTIONSLIDER_RIGHT);
   evas_object_show(as);
   elm_box_pack_end(bx, as);
}

static void
_do_test(AtkObject *obj)
{
   guint minimum, maximum, current, minimum_increment;
   Elm_Actionslider_Pos pos_test_set = ELM_ACTIONSLIDER_CENTER;
   static int test_count, test_1_count, test_2_count;

   test_count++;

   g_assert(ATK_IS_VALUE(obj));

   GValue value = G_VALUE_INIT;
   g_value_init(&value, G_TYPE_UINT);

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   minimum = g_value_get_uint(&value);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   maximum = g_value_get_uint(&value);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_uint(&value);

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   minimum_increment = g_value_get_uint(&value);

   g_assert(ELM_ACTIONSLIDER_NONE == minimum);
   g_assert(ELM_ACTIONSLIDER_NONE == minimum_increment);

   if (ELM_ACTIONSLIDER_LEFT == current)
     {
        test_1_count++;

        g_assert(ELM_ACTIONSLIDER_ALL == maximum);

        g_value_set_uint(&value, ELM_ACTIONSLIDER_ALL);
        g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));
     }
   else if (ELM_ACTIONSLIDER_RIGHT == current)
     {
        test_2_count++;

        g_assert((ELM_ACTIONSLIDER_RIGHT | ELM_ACTIONSLIDER_CENTER) == maximum);

        g_value_set_uint(&value, ELM_ACTIONSLIDER_LEFT);
        g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));
     }
   else
     {
        _printf("initialized values ​​are different from returned by ATK");
        g_assert(FALSE);
     }

   g_value_set_uint(&value, pos_test_set);
   g_assert(atk_value_set_current_value(ATK_VALUE(obj), &value));

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_uint(&value);
   g_assert(pos_test_set == current);

   g_assert((test_1_count + test_2_count) == test_count);

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
