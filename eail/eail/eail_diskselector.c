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
 * @file eail_diskselector.c
 * @brief Implementation of Diskselector widget
 */

#include <Elementary.h>

#include "eail_diskselector.h"
#include "eail_factory.h"
#include "eail_item_parent.h"
#include "eail_utils.h"
#include "eail_priv.h"

#define EAIL_diskselector_ACTION_NEXT "next" /**< @brief 'next' action name*/
#define EAIL_diskselector_ACTION_PREV "previous" /**< @brief 'prev' action name*/

static void atk_selection_interface_init(AtkSelectionIface *iface);
static void eail_item_parent_interface_init(EailItemParentIface *iface);
static void eail_diskselector_actions_table_init(EailDiskselector *widget);
/**
 * @brief Definition of EailDiskselector as GObject
 *
 * EailDiskselector is extended ScrollableWidget with EAIL_TYPE_ITEM_PARENT and
 * ATK_TYPE_SELECTION implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailDiskselector,
                        eail_diskselector,
                        EAIL_TYPE_SCROLLABLE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init)
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init));

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj an AtkObject
 * @param data Initialization data
 */
static void
eail_diskselector_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_diskselector_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_LIST;

   EailDiskselector *widget = EAIL_DISKSELECTOR(obj);
   eail_diskselector_actions_table_init(widget);

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget)
     {
        ERR("No evas object inside EailWidget was found");
        return;
     }

   evas_object_smart_callback_add(nested_widget, "selected",
                                  eail_list_item_handle_selected_event, obj);
}

/**
 * @brief Initializer for GObject EailList instance
 *
 * @param diskselector an EailDiskselector
 */
static void
eail_diskselector_init(EailDiskselector *diskselector)
{
}

/**
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 *
 * @param obj an AtkObject
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_diskselector_get_n_children(AtkObject *obj)
{
   gint n_items;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   n_items = eina_list_count(elm_diskselector_items_get(widget));

   return n_items;
}

/**
 * @brief Implementation AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param obj an AtkObject
 * @param i child index
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_diskselector_ref_child(AtkObject *obj, gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;
   Evas_Object *widget = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   items = elm_diskselector_items_get(widget);

   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_item_atk_obj
                             (eina_list_nth(items, i), ATK_ROLE_LIST_ITEM, obj);

        g_object_ref(child);
     }

   return child;
}

/**
 * @brief Destructor for EailDiskselector class
 *
 * @param object GObject object to be finalized
 */
static void
eail_diskselector_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_diskselector_parent_class)->finalize(object);
}

/**
 * @brief Initializer for GObject EailDiskselectorClass class (defines callbacks
 * for base AtkObject)
 *
 * @param klass an EailDiskselectorClass
 */
static void
eail_diskselector_class_init(EailDiskselectorClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_diskselector_initialize;
   class->get_n_children = eail_diskselector_get_n_children;
   class->ref_child = eail_diskselector_ref_child;
   gobject_class->finalize = eail_diskselector_finalize;
}

/*
 * Implementation of the *AtkSelection* interface
 */

/**
 * @brief Implementation of AtkSelection->add_selection callback
 *
 * As described in ATK doc:\n
 * Adds the specified accessible child of the object to the object's selection.
 *
 * @param selection an AtkSelection
 * @param i selection index
 *
 * @returns TRUE if operation ended successfully, FALSE otherwise
 */
static gboolean
eail_selection_add_selection(AtkSelection *selection,
                             gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   const Eina_List *items = elm_diskselector_items_get(object);
   Elm_Object_Item *item = eina_list_nth(items, i);
   if (!item) return FALSE;
   elm_diskselector_item_selected_set(item, EINA_TRUE);
   return TRUE;
}

/**
 * @brief Implementation of AtkSelection->clear_selection callback
 *
 * As described in ATK doc:\n
 * Clears the selection in the object so that no children in the object are
 * selected.
 *
 * @param selection an AtkSelection
 *
 * @returns always FALSE - this operation is not supported for Diskselector
 */
static gboolean
eail_selection_clear_selection(AtkSelection *selection)
{
   return FALSE;
}

/**
 * @brief Implementation of AtkSelection->ref_selection callback
 *
 * As described in ATK doc:\n
 * Gets a reference to the accessible object representing the specified selected
 * child of the object. Note: callers should not rely on NULL or on a zero value
 * for indication of whether AtkSelectionIface is implemented, they should use
 * type checking/interface checking macros or the atk_get_accessible_value()
 * convenience method.
 *
 * @param selection an AtkSelection
 * @param i selection index
 *
 * @returns an AtkObject representing the selected accessible , or NULL if
 * selection does not implement this interface
 */
static AtkObject *
eail_selection_ref_selection(AtkSelection *selection,
                             gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), NULL);

   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   const Eina_List *items = elm_diskselector_items_get(object);
   Elm_Object_Item *item = eina_list_nth(items, i);
   if (!item) return NULL;

   AtkObject *obj;
   obj = g_object_new(EAIL_TYPE_DISKSELECTOR, NULL);
   obj->role = ATK_ROLE_LIST_ITEM;

   if (EAIL_IS_ITEM_PARENT(selection))
     atk_object_set_parent(obj, (AtkObject *)selection);

   atk_object_initialize(obj, item);
   g_object_ref(obj);
   return obj;
}

/**
 * @brief Implementation of AtkSelection->get_selection_count callback
 *
 * As described in ATK doc:\n
 * Gets the number of accessible children currently selected. Note: callers
 * should not rely on NULL or on a zero value for indication of whether
 * AtkSelectionIface is implemented, they should use type checking/interface
 * checking macros or the atk_get_accessible_value() convenience method.
 *
 * @param selection an AtkSelection
 *
 * @returns always '1' - in Diskselector widget something is always selected
 */
static gint
eail_selection_get_selection_count(AtkSelection *selection)
{
   return 1;
}

/**
 * @brief Implementation of AtkSelection->is_child_selected callback
 *
 * As described in ATK doc:\n
 * Determines if the current child of this object is selected Note: callers
 * should not rely on NULL or on a zero value for indication of whether
 * AtkSelectionIface is implemented, they should use type checking/interface
 * checking macros or the atk_get_accessible_value() convenience method.
 *
 * @param selection an AtkSelection
 * @param i child index
 *
 * @returns a gboolean representing the specified child is selected, or 0 if
 * selection does not implement this interface.
 */
static gboolean
eail_selection_is_child_selected(AtkSelection *selection,
                                 gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_diskselector_selected_item_get(object);
   Elm_Object_Item *last_item = elm_diskselector_last_item_get(object);
   Elm_Object_Item *item = elm_diskselector_first_item_get(object);
   gint n=0;
   do
     {
        if (selected_item==item) break;
        item = elm_diskselector_item_next_get(item);
        n++;
     } while (item!=last_item);
   return i==n;
}

/**
 * @brief Implementation of AtkSelection->remove_selection callback
 *
 * As described in ATK doc:\n
 * Removes the specified child of the object from the object's selection.
 *
 * @param selection an AtkSelection
 * @param i selection index
 *
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_selection_remove_selection(AtkSelection *selection,
                                gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_diskselector_first_item_get(object);
   elm_diskselector_item_selected_set(selected_item, TRUE);
   return TRUE;
}

/**
 * @brief Implementation of AtkSelection->select_all_selection callback
 *
 * As described in ATK doc:\n
 * Causes every child of the object to be selected if the object supports
 * multiple selections.
 *
 * @param selection an AtkSelection
 *
 * @returns FALSE - this operation is not supported for Diskselector widget
 */
static gboolean
eail_selection_select_all_selection(AtkSelection *selection)
{
   return FALSE;
}

/**
 * @brief Initialization of AtkSelectionIface interface
 *
 * Function called upon instance creation. It initializes AtkSelection interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface an AtkSelectionIface
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   iface->add_selection        = eail_selection_add_selection;
   iface->clear_selection      = eail_selection_clear_selection;
   iface->ref_selection        = eail_selection_ref_selection;
   iface->get_selection_count  = eail_selection_get_selection_count;
   iface->is_child_selected    = eail_selection_is_child_selected;
   iface->remove_selection     = eail_selection_remove_selection;
   iface->select_all_selection = eail_selection_select_all_selection;
}

/**
 * @brief Get name of disk selector child
 *
 * @param parent EailDiskselector instance
 * @param item EailDiskselector child instance
 *
 * @returns const gchar * representing name of the child
 */
static const gchar *
eail_diskselector_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief Initialization of EailItemParentIface callbacks
 *
 * @param iface an EailItemParentIface
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name = eail_diskselector_item_name_get;
}

/**
 * @brief Next action callback
 *
 * @param action an AtkAction
 * @param data data passed to callback
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_diskselector_action_next(AtkAction *action, void *data)
{
   Evas_Object *widget;
   Elm_Object_Item *it_next;
   Elm_Object_Item *it;

   g_return_val_if_fail(EAIL_IS_DISKSELECTOR(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   it = elm_diskselector_selected_item_get(widget);
   if (!it) return FALSE;

   it_next = elm_diskselector_item_next_get(it);
   if (it_next) elm_diskselector_item_selected_set(it_next, EINA_TRUE);

   evas_object_smart_callback_call(widget, "selected", NULL);

   return TRUE;
}

/**
 * @brief Prev action callback
 *
 * @param action an AtkAction
 * @param data data passed to callback
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_diskselector_action_prev(AtkAction *action, void *data)
{
   Evas_Object *widget;
   Elm_Object_Item *it;
   Elm_Object_Item *it_prev;

   g_return_val_if_fail(EAIL_IS_DISKSELECTOR(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   it = elm_diskselector_selected_item_get(widget);
   if (!it) return FALSE;

   it_prev = elm_diskselector_item_prev_get(it);
   if (it_prev) elm_diskselector_item_selected_set(it_prev, EINA_TRUE);

   evas_object_smart_callback_call(widget, "selected", NULL);

   return TRUE;
}

/**
 * @brief Adds actions for scrolling to actions table
 * @param widget widget that implements EailScrollable interface
 */
static void
eail_diskselector_actions_table_init(EailDiskselector *widget)
{
   g_return_if_fail(EAIL_IS_DISKSELECTOR(widget));

   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_diskselector_ACTION_PREV,
                                    NULL,
                                    _eail_diskselector_action_prev);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_diskselector_ACTION_NEXT,
                                    NULL,
                                    _eail_diskselector_action_next);
}
