#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define EAIL_TEST_ENTRY_NAME "entry text"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   elm_exit();
}

static void
_test_focus_on_entry(AtkObject *obj)
{
   AtkObject *focused_obj = NULL;
   gboolean success = FALSE;

   _printf("Testing focus....\n");

   /* changing focus */
   g_assert(ATK_IS_COMPONENT(obj));
   success = atk_component_grab_focus(ATK_COMPONENT(obj));
   g_assert(success);

   /* now focus should point to our button*/
   focused_obj = atk_get_focus_object();
   g_assert(focused_obj);

   g_assert(focused_obj == obj);

   _printf("DONE. All focus test passed successfully \n");
   eail_test_code_called = TRUE;
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
                                 (aobj, "EailEntry", _test_focus_on_entry);

   g_assert(eail_test_code_called);
   elm_exit();
}

static void
_initialize_and_show_buttons(Evas_Object *win)
{
   Evas_Object *bg  = NULL, *entry = NULL;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   entry = elm_entry_add(win);
   elm_entry_entry_set(entry, EAIL_TEST_ENTRY_NAME);
   evas_object_resize(entry, 100, 60);
   evas_object_move(entry, 50, 20);
   evas_object_show(entry);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _initialize_and_show_buttons(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
