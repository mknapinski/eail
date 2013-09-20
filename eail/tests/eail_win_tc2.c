/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailWindow
 *
 * Description: Test "moved" signal emit by EailWindow
 *
 * Test input: accessible object representing EailWindow
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailWindow")

static int move_test_passed = 0;

static Evas_Object *global_win = NULL;
static Ecore_Timer *init_delay = NULL, *test_delay = NULL;

void move_callback()
{
   move_test_passed = 1;
}

static void
_setup_signals(AtkObject *obj)
{
   g_signal_connect(G_OBJECT(obj), "move",
                    G_CALLBACK(move_callback), NULL);
}

static Eina_Bool
delayed_test_body(void *data)
{
   _on_focus_in(NULL, NULL, NULL);

   return EINA_FALSE;
}

static Eina_Bool
delayed_test_init(void *data)
{
   AtkObject *root = atk_get_root();
   AtkObject *atk_window = atk_object_ref_accessible_child(root, 0);

   g_assert(atk_window);
   _setup_signals(atk_window);

   elm_win_center(global_win, EINA_TRUE, EINA_TRUE);

   evas_object_move(global_win, 1, 1);

   test_delay = ecore_timer_add(2, delayed_test_body, NULL);

   return EINA_FALSE;
}

static void
_do_test(AtkObject *obj)
{
   g_assert(move_test_passed);
   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(NULL, NULL);

   init_delay = ecore_timer_add(1, delayed_test_init, NULL);
   global_win = win;
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
