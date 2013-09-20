/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailGengrid
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailGengrid
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailGengrid")

typedef struct _Example_Item
{
   const char *path;
} Example_Item;

static Elm_Gengrid_Item_Class *gic = NULL;
static Evas_Object *grid;

static const char *imgs[9] =
{
   "./data/01.jpg",
   "./data/02.jpg",
   "./data/03.jpg",
   "./data/04.jpg",
   "./data/05.jpg",
   "./data/06.jpg",
   "./data/07.jpg",
   "./data/08.jpg",
   "./data/09.jpg"
};

static char *
_grid_label_get(void *data, Evas_Object *obj, const char *part)
{
   const Example_Item *it = data;
   char buf[256];
   snprintf(buf, sizeof(buf), "Photo %s", it->path);
   return strdup(buf);
}

static Evas_Object *
_grid_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Example_Item *it = data;
   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *icon = elm_bg_add(obj);
        char buf[256];
        snprintf(buf, sizeof(buf),"%s", it->path);
        elm_bg_file_set(icon, buf, NULL);
        evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_show(icon);
        return icon;
     }
   return NULL;

}

static Eina_Bool
_grid_state_get(void *data, Evas_Object *obj, const char *part)
{
   return EINA_FALSE;
}

static void
_grid_del(void *data, Evas_Object *obj)
{
   Example_Item *it = data;
   eina_stringshare_del(it->path);
   free(it);
}

static Example_Item *
_item_new(unsigned int index)
{
   Example_Item *it = malloc(sizeof(*it));
   if (index <= 8)
     it->path = eina_stringshare_add(imgs[index]);
   else
     it->path = eina_stringshare_add(imgs[0]);
   return it;
}

void _on_selected(void *data, Evas_Object *obj, void *event_info)
{
   _printf("selected item with handle: %p\n", event_info);
}

static void
_do_test(AtkObject *obj)
{
   gint index = -1, column = -1, row = -1;
   AtkObject *two_two_child = NULL, *zero_zero_child = NULL,
               *zero_four_child = NULL, *zero_three_child = NULL;

   /* 2-2 coords should NOT have any match*/
   index = atk_table_get_index_at(ATK_TABLE(obj), 2, 2);
   g_assert(-1 == index);
   two_two_child = atk_table_ref_at(ATK_TABLE(obj), 2, 2);
   g_assert(NULL == two_two_child);

   /* 0-0 coords should be found*/
   index = atk_table_get_index_at(ATK_TABLE(obj), 0, 0);
   g_assert(-1 != index);
   zero_zero_child = atk_table_ref_at(ATK_TABLE(obj), 0, 0);
   g_assert(zero_zero_child);

   /* 0-4 coords should NOT be found*/
   index = atk_table_get_index_at(ATK_TABLE(obj), 0, 4);
   g_assert(-1 == index);
   zero_four_child = atk_table_ref_at(ATK_TABLE(obj), 0, 4);
   g_assert(NULL == zero_four_child);

   /*...but  0-3 coords should be found*/
   index = atk_table_get_index_at(ATK_TABLE(obj), 0, 3);
   g_assert(-1 != index);
   zero_three_child = atk_table_ref_at(ATK_TABLE(obj), 0, 3);
   g_assert(zero_three_child);

   /* table - columns checks*/
   /*...index-9 obj should not be found (slightly out of bounds)*/
   column = atk_table_get_column_at_index(ATK_TABLE(obj), 9);
   g_assert(-1 == column);

   /*...index-8 obj should be found, and should be '0' */
   column = atk_table_get_column_at_index(ATK_TABLE(obj), 8);
   g_assert(0 == column);

   /*...index-7 obj should be found, and column be '3' */
   column = atk_table_get_column_at_index(ATK_TABLE(obj), 7);
   g_assert(3 == column);

   /*... index-2 obj should be found, and column be 2' */
   column = atk_table_get_column_at_index(ATK_TABLE(obj), 2);
   g_assert(2 == column);

   /*... index-6 obj should be found, and column be 2' */
   column = atk_table_get_column_at_index(ATK_TABLE(obj), 6);
   g_assert(2 == column);

   /* table -rows checks*/
   /* index 9- should not be found in any row */
   row = atk_table_get_row_at_index(ATK_TABLE(obj), 9);
   g_assert(-1 == row);

   /*...index-8 obj should be found, and row should be '2' */
   row = atk_table_get_row_at_index(ATK_TABLE(obj), 8);
   g_assert(2 == row);

   /*...index-7 obj should be found, and row should be '1' */
   row = atk_table_get_row_at_index(ATK_TABLE(obj), 7);
   g_assert(1 == row);

   /*...index-2 obj should be found, and row should be '0' */
   row = atk_table_get_row_at_index(ATK_TABLE(obj), 2);
   g_assert(0 == row);

   /*...index-6 obj should be found, and row should be '1' */
   row = atk_table_get_row_at_index(ATK_TABLE(obj), 6);
   g_assert(1 == row);

   /*...index-0 obj should be found, and row should be '0' */
   row = atk_table_get_row_at_index(ATK_TABLE(obj), 0);
   g_assert(0 == row);

   eailu_test_code_called = 1;
}

static Ecore_Timer *timer_delay;

static Eina_Bool delay_test_run(void *data)
{
   Eina_List *l, *list = elm_gengrid_realized_items_get(data);
   Elm_Object_Item *it;
   int i = 0;
   Evas_Object *win = elm_object_parent_widget_get(data);
   EINA_LIST_FOREACH(list, l, it)
     {
        unsigned int x, y;
        elm_gengrid_item_pos_get(it, &x, &y);
        i++;
     }
   _on_focus_in(NULL, win, NULL);
   return EINA_FALSE;
}

static void
_init_gengrid(Evas_Object *win)
{
   Evas_Object *bg;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   grid = elm_gengrid_add(win);

   elm_gengrid_item_size_set(grid, 200, 200);
   evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, grid);
   evas_object_smart_callback_add(grid, "selected", _on_selected, NULL);
   elm_gengrid_horizontal_set(grid, EINA_FALSE);
   elm_gengrid_multi_select_set(grid, EINA_TRUE);
   evas_object_show(grid);

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = "default";
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.state_get = _grid_state_get;
        gic->func.del = _grid_del;
     } // we only create the first time its needed. we dont unref/free

   elm_gengrid_item_append(grid, gic, _item_new(0), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(1), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(2), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(3), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(4), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(5), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(6), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(7), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(8), NULL, NULL);
   evas_object_resize(win, 1000, 300);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, NULL);
   _init_gengrid(win);
   evas_object_show(win);

   timer_delay = ecore_timer_add(1, delay_test_run, grid);

   elm_run();
   elm_shutdown();
   return 0;
}
ELM_MAIN()
