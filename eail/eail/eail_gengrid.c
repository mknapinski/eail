/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * @file eail_gengrid.c
 * @brief EailGengrid implementation
 */

#include <Elementary.h>

#include "eail_gengrid.h"
#include "eail_item_parent.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void eail_item_parent_interface_init(EailItemParentIface *iface);
static void atk_table_interface_init(AtkTableIface *iface);
static void atk_selection_interface_init(AtkSelectionIface *iface);

/** @brief GObject definition for EailGengrid implementation*/
G_DEFINE_TYPE_WITH_CODE(EailGengrid,
                        eail_gengrid,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TABLE,
                                              atk_table_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init));

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Handler for realized event, used to notify about gengrid content
 * changes
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info (item is passed here)
 */
void
eail_gengrid_item_handle_realized_event(void *data,
                                        Evas_Object *obj,
                                        void *event_info)
{
   Elm_Object_Item *item = (Elm_Object_Item *) event_info;
   AtkObject *atk_item = NULL, *atk_parent = NULL;

   atk_parent = ATK_OBJECT(data);
   if (!atk_parent) return;

   atk_item = eail_factory_get_item_atk_obj
                                         (item, ATK_ROLE_LIST_ITEM, atk_parent);

   if (!atk_item) return;

   eail_emit_children_changed_obj(TRUE, atk_parent, atk_item);
}

/**
 * @brief Handler for unrealized event, used to notify about gengrid content
 * changes
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info (item is passed here)
 */
void
eail_gengrid_item_handle_unrealized_event(void *data,
                                          Evas_Object *obj,
                                          void *event_info)
{
   Elm_Object_Item *item = (Elm_Object_Item *) event_info;
   AtkObject *atk_item = NULL, *atk_parent = NULL;

   atk_parent = ATK_OBJECT(data);
   if (!atk_parent) return;

   atk_item = eail_factory_get_item_atk_obj
                                         (item, ATK_ROLE_LIST_ITEM, atk_parent);

   if (!atk_item) return;

   eail_emit_children_changed_obj(FALSE, atk_parent, atk_item);
   atk_object_notify_state_change(atk_item, ATK_STATE_DEFUNCT, TRUE);

   DBG("Unregistering item from cache...");
   eail_factory_unregister_item_from_cache(item);
}

/**
 * @brief Initializes EailGengrid object
 *
 * @param obj EailGengrid instance
 * @param data user data passed for initialization
 */
static void
eail_gengrid_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_gengrid_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_TABLE;

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget)
     {
        ERR("No evas object inside EailWidget was found");
        return;
     }

   evas_object_smart_callback_add(nested_widget, "selected",
                                eail_list_item_handle_selected_event, obj);
   evas_object_smart_callback_add(nested_widget, "unselected",
                                eail_list_item_handle_unselected_event, obj);
   evas_object_smart_callback_add(nested_widget, "realized",
                                eail_gengrid_item_handle_realized_event, obj);
   evas_object_smart_callback_add(nested_widget, "unrealized",
                                eail_gengrid_item_handle_unrealized_event, obj);
}

/**
 * @brief Gets the list of created widgets
 *
 * @param gengrid EailGengrid instance
 *
 * @returns Eina_List representing the list of widgets elm_gengrid holds
 */
static Eina_List *
eail_gengrid_items_get(EailGengrid *gengrid)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(gengrid), NULL);

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(gengrid));

   return elm_gengrid_realized_items_get(widget);
}

/**
 * @brief Gets the number of created widgets
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the number of created widgets
 */
static gint
eail_gengrid_n_items_get(AtkObject *obj)
{
   gint n_items;
   Eina_List *items = eail_gengrid_items_get(EAIL_GENGRID(obj));
   n_items  = eina_list_count(items);
   eina_list_free(items);

   return n_items;
}

/**
 * @brief Gets the reference to the specified elm_gengrid's child
 *
 * @param obj AtkObject instance
 * @param i child index
 *
 * @returns AtkObject representing the elm_gengrid's child
 */
static AtkObject *
eail_gengrid_ref_child(AtkObject *obj, gint i)
{
   g_return_val_if_fail(EAIL_IS_GENGRID(obj), NULL);

   AtkObject *child = NULL;

   Eina_List *list = eail_gengrid_items_get(EAIL_GENGRID(obj));
   if (eina_list_count(list) > i)
     {
        child = eail_factory_get_item_atk_obj
                             (eina_list_nth(list, i), ATK_ROLE_LIST_ITEM, obj);

        g_object_ref(child);
     }

   return child;
}

/**
 * @brief Gets a reference to the state set of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * Implementation of ref_state_set from AtkObject.
 *
 * @param obj AtkObject instance
 *
 * @returns AtkStateSet representing the state set of the accessible
 */
static AtkStateSet *
eail_gengrid_ref_state_set(AtkObject *obj)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   AtkStateSet *state_set =
      ATK_OBJECT_CLASS(eail_gengrid_parent_class)->ref_state_set(obj);

   if (!widget) return state_set;

   if (elm_gengrid_multi_select_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_MULTISELECTABLE);

   return state_set;
}

/**
 * @brief Initializes EailGengrid object
 *
 * @param gengrid EailGengrid instance
 */
static void
eail_gengrid_init(EailGengrid *gengrid)
{
}

/**
 * @brief Initializes EailGengrid class
 *
 * @param klass EailGengridClass instance
 */
static void
eail_gengrid_class_init(EailGengridClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);

   atk_class->initialize = eail_gengrid_initialize;
   atk_class->get_n_children = eail_gengrid_n_items_get;
   atk_class->ref_child = eail_gengrid_ref_child;
   atk_class->ref_state_set = eail_gengrid_ref_state_set;
}

/**
 * @brief Gets the name of elm_gengrid child
 *
 * @param parent EailItemParent instance
 * @param item EailItem child instance
 *
 * @returns string representing the item's name
 */
static const gchar *
eail_gengrid_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   /*that will work only for default theme*/
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, "elm.text");
}

/**
 * @brief Gets the item's index in parent object
 *
 * @param parent EailItemParent instance
 * @param item EailItem child instance
 *
 * @returns integer representing the child's index in parent
 */
static int
eail_gengrid_item_index_in_parent_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!it) return -1;

   return elm_gengrid_item_index_get(it)-1;
}

/**
 * @brief Gets the state set of EailGengrid item
 *
 * @param parent EailItemParent instance
 * @param item EailItem child instance
 * @param state_set AtkStateSet to fill
 *
 * @returns AtkStateSet representing the state set of the child
 */
static AtkStateSet *
eail_gengrid_ref_item_state_set(EailItemParent *parent,
                                EailItem *item,
                                AtkStateSet *state_set)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!widget || !it) return state_set;

   Evas_Object *cell = elm_object_item_widget_get(it);

   if ((elm_gengrid_select_mode_get(widget) != ELM_OBJECT_SELECT_MODE_NONE) &&
       (elm_gengrid_item_select_mode_get(it) != ELM_OBJECT_SELECT_MODE_NONE))
     {
        atk_state_set_add_state(state_set, ATK_STATE_SELECTABLE);
        if (elm_gengrid_selected_item_get(cell))
          atk_state_set_add_state(state_set, ATK_STATE_SELECTED);
     }

   if (evas_object_visible_get(cell))
     atk_state_set_add_state(state_set, ATK_STATE_VISIBLE);

   atk_state_set_add_state(state_set, ATK_STATE_SHOWING);

   return state_set;
}

/**
 * @brief Grabs focus of EailGengrid item
 *
 * @param parent EailGengrid instance
 * @param item EailGengrid child instance
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_grab_item_focus(EailItemParent *parent,
                             EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   if (!it) return FALSE;

   Evas_Object *obj = elm_object_item_widget_get(it);
   if (!elm_object_focus_allow_get(obj)) return FALSE;

   elm_object_focus_set(obj, EINA_TRUE);

   return TRUE;
}

/**
 * @brief Gets the rectangle which gives the extent of the component
 *
 * Implementation of get_extents from AtkComponent for EailGengrid child.
 *
 * @param parent EailGengrid instance
 * @param item EailGengrid child instance
 * @param [out] x x coordinate
 * @param [out] y coordinate
 * @param [out] width width of the rectangle
 * @param [out] height height of the rectangle
 * @param coord_type specifies whether the coordinates are relative to the screen or to the components top level window
 */
static void
eail_gengrid_item_extents_get(EailItemParent *parent,
                              EailItem *item,
                              gint *x,
                              gint *y,
                              gint *width,
                              gint *height,
                              AtkCoordType coord_type)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
   Elm_Object_Item *it = eail_item_get_item(item);

   if ((!widget) || (!it)) return;

   Evas_Object *it_widget = elm_object_item_widget_get(it);
   evas_object_geometry_get(it_widget, x, y, width, height);

   if (coord_type == ATK_XY_SCREEN) {
        int ee_x, ee_y;
        Ecore_Evas *ee =
           ecore_evas_ecore_evas_get(evas_object_evas_get(it_widget));

        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        *x += ee_x;
        *y += ee_y;
   }
}

/**
 * @brief Gets supported actions
 *
 * Implementation of EailItemParent->get_actions_supported callback.
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 * @returns implementation representing supported actions
 */
static gint
eail_gengrid_get_actions_supported(EailItemParent   *parent,
                                   EailItem         *item)
{
   return EAIL_ACTION_SUPPORTED_CLICK | EAIL_ACTION_SUPPORTED_PRESS |
            EAIL_ACTION_SUPPORTED_RELEASE;
}

/**
 * @brief EailItemParent interface initialization
 *
 * @param iface EailItemParentIface instance
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   if (!iface) return;

   iface->get_item_name            = eail_gengrid_item_name_get;
   iface->get_item_index_in_parent = eail_gengrid_item_index_in_parent_get;
   iface->ref_item_state_set       = eail_gengrid_ref_item_state_set;
   iface->grab_item_focus          = eail_gengrid_grab_item_focus;
   iface->get_item_extents         = eail_gengrid_item_extents_get;
   iface->get_actions_supported    = eail_gengrid_get_actions_supported;
}

/**
 * @brief Helper function for getting rows and columns count
 *
 * @param object elm_gengrid instance
 * @param [out] rows rows count
 * @param [out] columns columns count
 */
static void
_get_n_rows_columns(const Evas_Object *object,
                    int *rows, int *columns)
{
   Elm_Object_Item *it;
   Eina_List *l, *list = elm_gengrid_realized_items_get(object);
   int row, column;

   if (list == NULL)
     {
        *rows = 0;
        *columns = 0;
        return;
     }

   row = column = 0;
   EINA_LIST_FOREACH(list, l, it)
     {
        unsigned int item_row, item_col;
        elm_gengrid_item_pos_get(it, &item_col, &item_row);
        if (item_row > row) row++;
        if (item_col > column) column++;
     }

   eina_list_free(list);
   *rows = row + 1;
   *columns = column + 1;
}


/**
 * @brief Gets the number of rows in the table
 *
 * Implementation of get_n_rows from AtkTable.
 *
 * @param table AtkTable instance
 *
 * @returns integer representing the number of rows
 */
static gint
eail_gengrid_n_rows_get(AtkTable *table)
{
   Evas_Object *widget;
   int rows, columns;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), -1);

   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   if (!widget) return -1;

   _get_n_rows_columns(widget, &rows, &columns);
   return rows;
}

/**
 * @brief Gets the number of columns in the table
 *
 * Implementation of get_n_columns from AtkTable.
 *
 * @param table AtkTable instance
 *
 * @returns integer representing the number of columns
 */
static gint
eail_gengrid_n_columns_get(AtkTable *table)
{
   Evas_Object *widget;
   int rows, columns;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), -1);

   widget = eail_widget_get_widget(EAIL_WIDGET(table));

   _get_n_rows_columns(widget, &rows, &columns);

   return columns;
}

/**
 * @brief Gets a gint representing the index at the specified row and column
 *
 * Implementation of get_index_at from AtkTable.
 *
 * @param table AtkTable instance
 * @param row number of row in table
 * @param column number of column in table
 *
 * @returns integer representing the index of the specified position
 */
static gint
eail_gengrid_index_at_get(AtkTable *table, gint row, gint column)
{
   Evas_Object *widget;
   gint n_rows, n_columns;
   gint ret_val = -1;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), -1);
   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   if (!widget) return -1;

   _get_n_rows_columns(widget, &n_rows, &n_columns);

   if ((row >= n_rows) || (column >= n_columns)) return -1;

   if (elm_gengrid_horizontal_get(widget))
     ret_val = ((n_columns) * column) + row;
   else
     ret_val = ((n_columns) * row) + column;

   /* testing if result is within bounds */
   if (ret_val >= eail_gengrid_n_items_get(ATK_OBJECT(table)))
     return -1;

   return ret_val;
}

/**
 * @brief Get a reference to the table cell at row, column
 *
 * Implementation of ref_at from AtkTable.
 *
 * @param table AtkTable instance
 * @param row number of row in table
 * @param column number of column in table
 *
 * @returns AtkObject representing the child object
 */
static AtkObject *
eail_gengrid_ref_at(AtkTable *table, gint row, gint column)
{
   gint index;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), NULL);

   index = eail_gengrid_index_at_get(table, row, column);
   if (index == -1) return NULL;

   return eail_gengrid_ref_child(ATK_OBJECT(table), index);
}

/**
 * @brief Helper function for getting object's position in table at given index
 *
 * @param widget Evas_Object instance
 * @param [out] row row number
 * @param [out] column column number
 * @param index index of child item
 */
static void
_get_item_pos(const Evas_Object *widget, gint *row, gint *column, gint index)
{
   Elm_Object_Item *it;
   Eina_List *list;

   if (!widget)
     {
        *row = -1;
        *column = -1;
        return;
     }

   list = elm_gengrid_realized_items_get(widget);
   it = eina_list_nth(list, index);
   elm_gengrid_item_pos_get(it, (unsigned int *)column, (unsigned int *)row);

   eina_list_free(list);
}

/**
 * @brief Gets an integer representing the column at the specified index
 *
 * Implementation of get_column_at_index from AtkTable.
 *
 * @param table AtkTable instance
 * @param index child index
 *
 * @returns integer representing the column number in table
 */
static gint
eail_gengrid_column_at_index_get(AtkTable *table, gint index)
{
   Evas_Object *widget;
   int row, column;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), -1);
   if (index >= eail_gengrid_n_items_get(ATK_OBJECT(table)))
     return -1;

   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   _get_item_pos(widget, &row, &column, index);

   return column;
}

/**
 * @brief Gets a gint representing the row at the specified index
 *
 * Implementation of get_row_at_index from AtkTable.
 *
 * @param table AtkTable instance
 * @param index child index
 *
 * @returns integer representing the row number in table
 */
static gint
eail_gengrid_row_at_index_get(AtkTable *table, gint index)
{
   Evas_Object *widget;
   int row, column;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), -1);
   if (index >= eail_gengrid_n_items_get(ATK_OBJECT(table)))
     return -1;

   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   _get_item_pos(widget, &row, &column, index);

   return row;
}

/**
 * @brief Gets a boolean value indicating whether the accessible object
 * at the specified row and column is selected
 *
 * Implementation of is_selected from AtkTable.
 *
 * @param table AtkTable instance
 * @param row row's number
 * @param column column's number
 *
 * @returns TRUE if selected, FALSE otherwise
 */
static gboolean
eail_gengrid_is_selected(AtkTable *table, gint row, gint column)
{
   Evas_Object *widget;
   Elm_Object_Item *it;
   int index;
   Eina_List *list;
   gboolean result;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   index = eail_gengrid_index_at_get(table, row, column);
   if (index == -1) return FALSE;

   list = elm_gengrid_realized_items_get(widget);
   it = eina_list_nth(list, index);
   result = elm_gengrid_item_selected_get(it);

   eina_list_free(list);
   return result;
}

/**
 * @brief Gets a boolean value indicating whether the specified row is selected
 *
 * Implementation of is_row_selected from AtkTable interface.
 *
 * @param table AtkTable instance
 * @param row number of row in table
 *
 * @returns TRUE if selected, FALSE otherwise
 */
static gboolean
eail_gengrid_is_row_selected(AtkTable *table, gint row)
{
   Evas_Object *widget;
   Eina_List *list;
   const Eina_List *selected;
   gboolean result;
   int list_count, n_rows;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   /*it is possible to select row only in multiselct mode*/
   if (!elm_gengrid_multi_select_get(widget))
     return FALSE;

   selected = elm_gengrid_selected_items_get(widget);
   if (!selected) return FALSE;

   n_rows = eail_gengrid_n_rows_get(table);
   if (row > n_rows) return FALSE;

   list = elm_gengrid_realized_items_get(widget);
   list_count = eina_list_count(list);

   result = TRUE;
   for (int i = 0; i < list_count; i++)
     {
        Elm_Object_Item *it = eina_list_nth(list, i);
        unsigned int x, y;
        elm_gengrid_item_pos_get(it, &x, &y);
        if((row == x) && (!elm_gengrid_item_selected_get(it)))
          {
             /*row is considered selected if all items in row are selected*/
             result = FALSE;
             break;
          }
     }
   eina_list_free(list);
   return result;
}

/**
 * @brief Gets the selected rows of the table by initializing selected
 * with the selected row numbers
 *
 * This array should be freed by the caller.
 *
 * Implementation of get_selected_rows from AtkTable interface.
 *
 * @param table AtkTable instance
 * @param [out] selected selected rows number
 *
 * @returns integer representing the selected rows count
 */
static gint
eail_gengrid_selected_rows_get(AtkTable *table, gint **selected)
{
   GArray *array;
   Evas_Object *widget;
   int n_rows, n_selected;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), 0);
   widget = eail_widget_get_widget(EAIL_WIDGET(table));

   if (!widget) return 0;
   if (!elm_gengrid_multi_select_get(widget)) return 0;

   n_rows = eail_gengrid_n_rows_get(table);
   array = g_array_new(FALSE, FALSE, sizeof(gint));
   for (int i = 0; i < n_rows; i++)
     {
        gboolean success = eail_gengrid_is_row_selected(table, i);
        if (success) g_array_append_val(array, i);
     }
   n_selected = array->len;
   *selected = (gint *) g_array_free(array, FALSE);
   return n_selected;
}

/**
 * @brief Helper function for setting row selection
 *
 * @param table AtkTable instance
 * @param row row number
 * @param selection EINA_TRUE for selection, EINA_FALSE for removing selection
 */
static void
_set_row_selection(AtkTable *table, int row, Eina_Bool selection)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(table));
   Eina_List *list = elm_gengrid_realized_items_get(widget);
   int list_count = eina_list_count(list);

   for (int i = 0; i < list_count; i++)
     {
        unsigned int x, y;
        Elm_Object_Item *it = eina_list_nth(list, i);
        elm_gengrid_item_pos_get(it, &x, &y);
        if (x == row)
          {
             elm_gengrid_item_selected_set(it, selection);
          }
     }

   eina_list_free(list);
}

/**
 * @brief Adds the specified row to the selection
 *
 * Implementation of add_row_selection from AtkTable.
 *
 * @param table AtkTable instance
 * @param row row number in table
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_add_row_selection(AtkTable *table, gint row)
{
   Evas_Object *widget;
   int n_rows;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(table));

   if (!widget) return FALSE;
   if (!elm_gengrid_multi_select_get(widget)) return FALSE;
   n_rows = eail_gengrid_n_rows_get(table);
   if (row > n_rows) return FALSE;

   _set_row_selection(table, row, EINA_TRUE);
   return TRUE;
}

/**
 * @brief Removes the specified row from the selection
 *
 * Implementation of remove_row_selection from AtkTable interface.
 *
 * @param table AtkTable instance
 * @param row number of row in table
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_remove_row_selection(AtkTable *table, gint row)
{
   Evas_Object *widget;
   int n_rows;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   if (!widget) return FALSE;
   if (!elm_gengrid_multi_select_get(widget)) return FALSE;
   n_rows = eail_gengrid_n_rows_get(table);
   if (row > n_rows) return FALSE;

   _set_row_selection(table, row, EINA_FALSE);
   return TRUE;

}

/**
 * @brief Gets a boolean value indicating whether the specified column is selected
 *
 * Implementation of is_column_selected from AtkTable interface.
 *
 * @param table AtkTable implementation
 * @param column number of column in table
 *
 * @returns TRUE on selection, FALSE otherwise
 */
static gboolean
eail_gengrid_is_column_selected(AtkTable *table, gint column)
{
   Evas_Object *widget;
   Eina_List *list;
   const Eina_List *selected;
   int n_columns, list_count;
   gboolean result;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   if (!elm_gengrid_multi_select_get(widget)) return FALSE;
   n_columns = eail_gengrid_n_columns_get(table);
   if (column > n_columns) return FALSE;
   selected = elm_gengrid_selected_items_get(widget);
   if (!selected) return FALSE;

   list = elm_gengrid_realized_items_get(widget);
   list_count = eina_list_count(list);
   result = TRUE;
   for (int i = 0; i < list_count; i++)
     {
        Elm_Object_Item *it = eina_list_nth(list, i);
        unsigned int x, y;
        elm_gengrid_item_pos_get(it, &x, &y);
        if ((y == column) && (!elm_gengrid_item_selected_get(it)))
          {
             result = FALSE;
             break;
          }
     }

   eina_list_free(list);
   return result;
}

/**
 * @brief Helper function for setting column selection in elm_gengrid
 *
 * @param table AtkTable instance
 * @param column number of column in table
 * @param selection EINA_TRUE for selection, EINA_FALSE otherwise
 */
static void
_set_column_selection(AtkTable *table, int column, Eina_Bool selection)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(table));
   Eina_List *list = elm_gengrid_realized_items_get(widget);
   int list_count = eina_list_count(list);

   for (int i = 0; i < list_count; i++)
     {
        unsigned int x, y;
        Elm_Object_Item *it = eina_list_nth(list, i);
        elm_gengrid_item_pos_get(it, &x, &y);
        if (y == column)
          {
             elm_gengrid_item_selected_set(it, selection);
          }
     }

   eina_list_free(list);
}

/**
 * @brief Adds the specified column to the selection
 *
 * Implementation of add_column_selection from AtkTable interface.
 *
 * @param table AtkTable instance
 * @param column number of column in table
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_add_column_selection(AtkTable *table, gint column)
{
   Evas_Object *widget;
   int n_columns;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   if (!widget) return FALSE;
   if (!elm_gengrid_multi_select_get(widget)) return FALSE;
   n_columns = eail_gengrid_n_columns_get(table);
   if (column > n_columns) return FALSE;

   _set_column_selection(table, column, EINA_TRUE);
   return TRUE;
}

/**
 * @brief Removes the specified column from the selection
 *
 * Implementation of remove_column_selection from AtkTable.
 *
 * @param table AtkTable instance
 * @param column column number in table
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_remove_column_selection(AtkTable *table, gint column)
{
   Evas_Object *widget;
   int n_columns;

   g_return_val_if_fail(EAIL_IS_GENGRID(table), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(table));
   if (!widget) return FALSE;
   if (!elm_gengrid_multi_select_get(widget)) return FALSE;
   n_columns = eail_gengrid_n_columns_get(table);
   if (column > n_columns) return FALSE;

   _set_column_selection(table, column, EINA_FALSE);
   return TRUE;

}

/**
 * @brief AtkTable interface initialization
 *
 * @param iface AtkTableIface instance
 */
static void
atk_table_interface_init(AtkTableIface *iface)
{
   if (!iface) return;

   iface->ref_at = eail_gengrid_ref_at;
   iface->get_n_rows = eail_gengrid_n_rows_get;
   iface->get_n_columns = eail_gengrid_n_columns_get;
   iface->get_index_at = eail_gengrid_index_at_get;
   iface->get_column_at_index = eail_gengrid_column_at_index_get;
   iface->get_row_at_index = eail_gengrid_row_at_index_get;
   iface->is_row_selected = eail_gengrid_is_row_selected;
   iface->is_selected = eail_gengrid_is_selected;
   iface->get_selected_rows = eail_gengrid_selected_rows_get;
   iface->add_row_selection = eail_gengrid_add_row_selection;
   iface->remove_row_selection = eail_gengrid_remove_row_selection;
   iface->is_column_selected = eail_gengrid_is_column_selected;
   iface->add_column_selection = eail_gengrid_add_column_selection;
   iface->remove_column_selection = eail_gengrid_remove_column_selection;

   /*unsupported*/
   iface->get_column_extent_at = NULL;
   iface->get_row_extent_at = NULL;
   iface->get_column_header = NULL;
   iface->get_column_description = NULL;
}

/**
 * @brief Adds the specified accessible child of the object to the object's selection
 *
 * Implementation of add_selection from AtkSelection interface.
 *
 * @param selection AtkSelection instance
 * @param i index of child to select
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_selection_add(AtkSelection *selection, gint i)
{
   Evas_Object *widget;
   Elm_Object_Item *it;
   Eina_List *list;
   gboolean result;

   g_return_val_if_fail(EAIL_IS_GENGRID(selection), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   list = elm_gengrid_realized_items_get(widget);

   if (i > eina_list_count(list)) return FALSE;
   it = eina_list_nth(list, i);

   if (elm_gengrid_item_selected_get(it))
     result = FALSE;
   else
     {
        elm_gengrid_item_selected_set(it, EINA_TRUE);
        result = TRUE;
     }

   eina_list_free(list);
   return result;
}

/**
 * @brief Clears the selection in the object so that no children in the object are selected
 *
 * Implementation of clear_selection from AtkSelection.
 *
 * @param selection AtkSelection instance
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_selection_clear(AtkSelection *selection)
{
   const Eina_List *l, *list;
   Evas_Object *widget;
   Elm_Object_Item *it;

   g_return_val_if_fail(EAIL_IS_GENGRID(selection), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(selection));

   if (elm_gengrid_multi_select_get(widget))
     {
        list = elm_gengrid_selected_items_get(widget);
        EINA_LIST_FOREACH(list, l, it)
          {
             elm_gengrid_item_selected_set(it, EINA_FALSE);
          }
     }
   else
     {
        it = elm_gengrid_selected_item_get(widget);
        elm_gengrid_item_selected_set(it, EINA_FALSE);
     }

   return TRUE;
}


/**
 * @brief Gets a reference to the accessible object representing the specified
 * selected child of the object
 *
 * Note: callers should not rely on NULL
 * or on a zero value for indication of whether AtkSelectionIface is
 * implemented, they should use type checking/interface checking macros
 * or the atk_get_accessible_value() convenience method.
 *
 * Implementation of ref_selection from AtkSelection interface.
 *
 * @param selection AtkSelection instance
 * @param i index of selected child
 *
 * @returns AtkObject representing the selected object
 */
static AtkObject *
eail_gengrid_selection_ref(AtkSelection *selection, gint i)
{
   Evas_Object *widget;
   Elm_Object_Item *it;
   const Eina_List *list;
   Eina_Bool multiselect;
   AtkObject *child;

   g_return_val_if_fail(EAIL_IS_GENGRID(selection), NULL);
   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   multiselect = elm_gengrid_multi_select_get(widget);

   if ((!multiselect) && (i != 0)) return FALSE;

   if (multiselect)
     {
        list = elm_gengrid_selected_items_get(widget);
        if (i > eina_list_count(list)) return NULL;
        it = eina_list_nth(list, i);
     }
   else
     {
        it = elm_gengrid_selected_item_get(widget);
     }

   if (!it) return NULL;

   child = eail_factory_get_item_atk_obj
                              (it, ATK_ROLE_LIST_ITEM, ATK_OBJECT(selection));

   g_object_ref(child);

   return child;
}

/**
 * @brief Gets the number of accessible children currently selected.
 *
 * Note: callers should not rely on NULL or on a zero value
 * for indication of whether AtkSelectionIface is implemented,
 * they should use type checking/interface checking macros or
 * the atk_get_accessible_value() convenience method.
 *
 * Implementation of get_selection_count from AtkSelection interface.
 *
 * @param selection AtkSelection instance
 *
 * @returns integer representing the number of selected items
 */
static gint
eail_gengrid_selection_count_get(AtkSelection *selection)
{
   Evas_Object *widget;
   Elm_Object_Item *it;
   const Eina_List *list;
   int n_selections;

   g_return_val_if_fail(EAIL_IS_GENGRID(selection), -1);
   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   if (!elm_gengrid_multi_select_get(widget))
     {
        it = elm_gengrid_selected_item_get(widget);
        n_selections = (it ? 1 : 0);
     }
   else
     {
        list = elm_gengrid_selected_items_get(widget);
        n_selections = eina_list_count(list);
     }

   return n_selections;
}

/**
 * @brief Determines if the current child of this object is selected.
 *
 * Note: callers should not rely on NULL or on a zero value for
 * indication of whether AtkSelectionIface is implemented, they
 * should use type checking/interface checking macros or
 * the atk_get_accessible_value() convenience method.
 *
 * Implementation of is_child_selected from AtkSelection interface.
 *
 * @param selection AtkSelection instance
 * @param i index of child
 *
 * @returns TRUE on selection, FALSE otherwise
 */
static gboolean
eail_gengrid_selection_is_child_selected(AtkSelection *selection, int i)
{
   Evas_Object *widget;
   Elm_Object_Item *it;
   Eina_List *list;
   gboolean selected;

   g_return_val_if_fail(EAIL_IS_GENGRID(selection), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   list = elm_gengrid_realized_items_get(widget);

   if (i > eina_list_count(list))
     {
        eina_list_free(list);
        return FALSE;
     }

   it = eina_list_nth(list, i);
   selected = (elm_gengrid_item_selected_get(it) ? TRUE : FALSE);
   eina_list_free(list);

   return selected;
}

/**
 * @brief Removes the specified child of the object from the object's selection
 *
 * Implementation of remove_selection from AtkSelection.
 *
 * @param selection AtkSelection instance
 * @param i index of selected child
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_selection_remove(AtkSelection *selection, gint i)
{
   Evas_Object *widget;
   Elm_Object_Item *it;
   const Eina_List *list;
   Eina_Bool multiselect;

   g_return_val_if_fail(EAIL_IS_GENGRID(selection), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   multiselect = elm_gengrid_multi_select_get(widget);
   if ((!multiselect) && (i != 0)) return FALSE;

   if (!multiselect)
     {
        it = elm_gengrid_selected_item_get(widget);
        elm_gengrid_item_selected_set(it, FALSE);
     }
   else
     {
        list = elm_gengrid_selected_items_get(widget);
        if (i > eina_list_count(list)) return FALSE;

        it = eina_list_nth(list, i);
        elm_gengrid_item_selected_set(it, EINA_FALSE);
    }

   return TRUE;
}

/**
 * @brief Causes every child of the object to be selected if the object supports multiple selections
 *
 * Implementation of select_all from AtkSelection interface.
 *
 * @param selection AtkSelection instance
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_gengrid_selection_select_all(AtkSelection *selection)
{
   Evas_Object *widget;
   Elm_Object_Item *it;
   Eina_List *l, *list;

   g_return_val_if_fail(EAIL_IS_GENGRID(selection), FALSE);
   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   if (!elm_gengrid_multi_select_get(widget)) return FALSE;

   list = elm_gengrid_realized_items_get(widget);
   EINA_LIST_FOREACH(list, l, it)
     {
        elm_gengrid_item_selected_set(it, EINA_TRUE);
     }
   return TRUE;
}

/**
 * @brief AtkSelection interface initialization
 *
 * @param iface AtkSelectionIface instance
 */
static void
atk_selection_interface_init(AtkSelectionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->add_selection = eail_gengrid_selection_add;
   iface->clear_selection = eail_gengrid_selection_clear;
   iface->ref_selection = eail_gengrid_selection_ref;
   iface->get_selection_count = eail_gengrid_selection_count_get;
   iface->is_child_selected = eail_gengrid_selection_is_child_selected;
   iface->remove_selection = eail_gengrid_selection_remove;
   iface->select_all_selection = eail_gengrid_selection_select_all;
}
