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

#define EAIL_ACTION_FOR_MAXIMALIZE "maximalize"
#define EAIL_ACTION_FOR_MINIMALIZE "minimalize"

INIT_TEST("EailWindow")

static int maximalize_test_passed = 0;
static int minimalize_test_passed = 0;
static int resize_test_passed = 0;

static Evas_Object *global_win = NULL;

void maximalize_callback()
{
   maximalize_test_passed = 1;
}

void minimalize_callback()
{
   minimalize_test_passed = 1;
}

void resize_callback()
{
   resize_test_passed = 1;
}

static void
_setup_signals(AtkObject *obj)
{
   g_signal_connect(G_OBJECT(obj), "maximize",
                    G_CALLBACK(maximalize_callback), NULL);
   g_signal_connect(G_OBJECT(obj), "minimize",
                    G_CALLBACK(minimalize_callback), NULL);
   g_signal_connect(G_OBJECT(obj), "resize",
                    G_CALLBACK(resize_callback), NULL);
}

static void
_do_test(AtkObject *obj)
{
   _setup_signals(obj);

   g_assert(ATK_IS_ACTION(obj));
   g_assert(2 == atk_action_get_n_actions(ATK_ACTION(obj)));

   eailu_test_action_activate(ATK_ACTION(obj), EAIL_ACTION_FOR_MAXIMALIZE);
   eailu_test_action_activate(ATK_ACTION(obj), EAIL_ACTION_FOR_MINIMALIZE);

   g_assert(maximalize_test_passed);
   g_assert(minimalize_test_passed);

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
