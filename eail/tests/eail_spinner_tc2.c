/*
 * Tested interface: AtkValue
 *
 * Tested AtkObject: EailSpinner
 *
 * Description: Test AtkValue interface
 *
 * Test input: accessible object representing EailSpinner
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailSpinner")

struct
{
   double min, max, set, step;
}
static sp[4] = { { 0,   100, 80,  1   },
                 { 0,   100, 2.2, 1   },
                 { -50, 250, 100, 1.5 },
                 { 0,   100, 13,  1   } };

static void
_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
   _printf("Value changed to %0.f\n", elm_spinner_value_get(obj));
}

static void
_init_spinner(Evas_Object *win)
{
   Evas_Object *bg, *bx, *sp1, *sp2, *sp3, *sp4;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   // default
   sp1 = elm_spinner_add(win);
   elm_spinner_value_set(sp1, sp[0].set);
   evas_object_size_hint_align_set(sp1, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sp1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sp1);
   evas_object_show(sp1);

   // format
   sp2 = elm_spinner_add(win);
   elm_spinner_label_format_set(sp2, "Percentage %%%1.2f something");
   elm_spinner_value_set(sp2, sp[1].set);
   evas_object_size_hint_align_set(sp2, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sp2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sp2);
   evas_object_show(sp2);

   // min max, step and wrap
   sp3 = elm_spinner_add(win);
   elm_spinner_label_format_set(sp3, "%1.1f units");
   elm_spinner_step_set(sp3, sp[2].step);
   elm_spinner_wrap_set(sp3, EINA_TRUE);
   elm_spinner_min_max_set(sp3, sp[2].min, sp[2].max);
   elm_spinner_value_set(sp3, sp[2].set);
   evas_object_size_hint_align_set(sp3, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sp3, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sp3);
   evas_object_show(sp3);

   sp4 = elm_spinner_add(win);
   elm_spinner_value_set(sp4, sp[3].set);
   evas_object_size_hint_align_set(sp4, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sp4, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sp4);
   evas_object_show(sp4);
   evas_object_smart_callback_add(sp4, "changed", _changed_cb, NULL);

   eailu_test_code_called = 1;
}

static void
_do_test(AtkObject *obj)
{
   double minimum, maximum, current, minimum_increment;
   const double val_test_set = 33.3;

   GValue value = G_VALUE_INIT;
   g_value_init(&value, G_TYPE_DOUBLE);

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   minimum = g_value_get_double(&value);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   maximum = g_value_get_double(&value);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_double(&value);

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   minimum_increment = g_value_get_double(&value);

   if ((float)sp[0].set == (float)current && (float)sp[0].min == (float)minimum &&
       (float)sp[0].max == (float)maximum && (float)sp[0].step == (float)minimum_increment)
     {
        _printf("spinner 1: initialized values - correctly\n");
     }
   else if ((float)sp[1].set == (float)current && (float)sp[1].min == (float)minimum &&
            (float)sp[1].max == (float)maximum && (float)sp[1].step == (float)minimum_increment)
     {
        _printf("spinner 2: initialized values - correctly\n");
     }
   else if ((float)sp[2].set == (float)current && (float)sp[2].min == (float)minimum &&
            (float)sp[2].max == (float)maximum && (float)sp[2].step == (float)minimum_increment)
     {
        _printf("spinner 3: initialized values - correctly\n");
     }
   else if ((float)sp[3].set == (float)current && (float)sp[3].min == (float)minimum &&
            (float)sp[3].max == (float)maximum && (float)sp[3].step == (float)minimum_increment)
     {
        _printf("spinner 4: initialized values - correctly\n");
     }
   else
     {
        _printf("initialized values ​​are different from returned by ATK");
        g_assert(FALSE);
     }

   g_value_set_double(&value, val_test_set);
   g_assert(atk_value_set_current_value(ATK_VALUE(obj), &value));
   _printf("atk_value_set_current_value: %0.2f\n", val_test_set);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_double(&value);
   _printf("atk_value_get_current_value: %0.2f\n", current);

   g_assert((float)val_test_set == (float)current);

   g_value_set_double(&value, minimum - 1);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   g_value_set_double(&value, maximum + 1);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_spinner(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
