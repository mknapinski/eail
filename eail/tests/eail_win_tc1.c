/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailWindow
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailWindow
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_ACTION_FOR_MAXIMIZE "maximize"
#define EAIL_ACTION_FOR_MINIMIZE "minimize"
#define EAIL_ACTION_FOR_RESTORE "restore"

INIT_TEST("EailWindow")

static int maximize_test_passed = 0;
static int minimize_test_passed = 0;
static int resize_test_passed = 0;
static int restore_test_passed = 0;

static Evas_Object *global_win = NULL;

void maximize_callback()
{
   maximize_test_passed = 1;
}

void minimize_callback()
{
   minimize_test_passed = 1;
}

void resize_callback()
{
   resize_test_passed = 1;
}

void restore_callback()
{
   restore_test_passed = 1;
}

static void
_setup_signals(AtkObject *obj)
{
   g_signal_connect(G_OBJECT(obj), "maximize",
                    G_CALLBACK(maximize_callback), NULL);
   g_signal_connect(G_OBJECT(obj), "minimize",
                    G_CALLBACK(minimize_callback), NULL);
   g_signal_connect(G_OBJECT(obj), "resize",
                    G_CALLBACK(resize_callback), NULL);
   g_signal_connect(G_OBJECT(obj), "restore",
                    G_CALLBACK(restore_callback), NULL);
}

static void
_do_test(AtkObject *obj)
{
   _setup_signals(obj);

   g_assert(ATK_IS_ACTION(obj));
   g_assert(3 == atk_action_get_n_actions(ATK_ACTION(obj)));

   eailu_test_action_activate(ATK_ACTION(obj), EAIL_ACTION_FOR_MAXIMIZE);
   eailu_test_action_activate(ATK_ACTION(obj), EAIL_ACTION_FOR_MINIMIZE);
   eailu_test_action_activate(ATK_ACTION(obj), EAIL_ACTION_FOR_RESTORE);

   g_assert(maximize_test_passed);
   g_assert(minimize_test_passed);
   g_assert(restore_test_passed);

   evas_object_resize(global_win, 450, 450);
   g_assert(resize_test_passed);
   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   global_win = win;
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
