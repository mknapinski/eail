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
 * @file eail_list.c
 * @brief Implementation of list widget
 */

#include <Elementary.h>

#include "eail_list.h"
#include "eail_item_parent.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void eail_item_parent_interface_init(EailItemParentIface *iface);
static void atk_selection_interface_init(AtkSelectionIface *iface);

/**
 * @brief Definition of EailList as GObject
 *
 * EailList is extended ScrollableWidget with EAIL_TYPE_ITEM_PARENT and
 * ATK_TYPE_SELECTION implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailList,
                        eail_list,
                        EAIL_TYPE_SCROLLABLE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init));

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj an AtkObject
 * @param data Initialization data
 */
static void
eail_list_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_list_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_LIST;

   g_return_if_fail(EAIL_IS_WIDGET(obj));
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
}

/**
 * @brief Prepares Eina_List filled with Elm_Object_Item* objects
 * representing items in the list
 *
 * @param list an EailList
 *
 * @return filled list with list items. Call eina_list_free on that list when
 * results processing has been finished
 */
static Eina_List *
eail_list_get_items(EailList *list)
{
   Eina_List *items = NULL;
   Elm_Object_Item *item;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(list));

   item = elm_list_first_item_get(widget);
   while (item)
     {
        items = eina_list_append(items, item);
        item = elm_list_item_next(item);
     }

   return items;
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
eail_list_get_n_children(AtkObject *obj)
{
   gint n_items;
   Eina_List *items;

   items = eail_list_get_items(EAIL_LIST(obj));
   n_items = eina_list_count(items);

   eina_list_free(items);

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
eail_list_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items;
   AtkObject *child = NULL;

   items = eail_list_get_items(EAIL_LIST(obj));
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_item_atk_obj
                          (eina_list_nth(items, i), ATK_ROLE_LIST_ITEM, obj);

        g_object_ref(child);
     }

   eina_list_free(items);

   return child;
}

/**
 * @brief Implementation AtkObject->ref_state_set callback
 *
 * ATK doc says:\n
 * Gets a reference to the state set of the accessible; the caller must
 * unreference it when it is no longer needed.
 *
 * @param obj an AtkObject
 *
 * @returns reference to an AtkStateSet which is the state set of the
 * accessible
 */
static AtkStateSet *
eail_list_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

   state_set = ATK_OBJECT_CLASS(eail_list_parent_class)->ref_state_set(obj);

   if (!widget) return state_set;

   if (elm_list_multi_select_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_MULTISELECTABLE);

   return state_set;
}

/**
 * @brief Initializer for GObject EailList instance
 *
 * @param list an EailList
 */
static void
eail_list_init(EailList *list)
{
}

/**
 * @brief Initializer for GObject class (defines callbacks for base AtkObject)
 *
 * @param klass an EailList class
 */
static void
eail_list_class_init(EailListClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_list_initialize;
   class->get_n_children = eail_list_get_n_children;
   class->ref_child = eail_list_ref_child;
   class->ref_state_set = eail_list_ref_state_set;
}

/*
 * Functions for *EailItemParent* interface
 */

/**
 * @brief Implementation of EailItemParent->get_item_role callback
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 *
 * @returns role of target item in the list
 */
static AtkRole
eail_list_get_item_role(EailItemParent  *parent,
                        EailItem        *item)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!widget || !it) return ATK_ROLE_INVALID;

   if (elm_list_item_separator_get(it))
     return ATK_ROLE_SEPARATOR;

   return ATK_ROLE_LIST_ITEM;
}

/**
 * @brief Implementation of EailItemParent->ref_item_state_set callback
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 * @param state_set an AtkStateSet
 * @returns items state set
 */
static AtkStateSet *
eail_list_ref_item_state_set(EailItemParent *parent,
                             EailItem       *item,
                             AtkStateSet    *state_set)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!widget || !it) return state_set;

   if (elm_list_select_mode_get(widget) != ELM_OBJECT_SELECT_MODE_NONE)
     {
        atk_state_set_add_state(state_set, ATK_STATE_SELECTABLE);

        if (elm_list_item_selected_get(it))
          atk_state_set_add_state(state_set, ATK_STATE_SELECTED);
     }

   atk_state_set_add_state(state_set, ATK_STATE_VISIBLE);
   atk_state_set_add_state(state_set, ATK_STATE_SHOWING);

   return state_set;
}

/**
 * @brief Implementation of EailItemParent->ref_item_state_set callback
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_list_grab_item_focus(EailItemParent    *parent,
                          EailItem          *item)
{
   /* it is not possible to focus entire list object */
   return FALSE;
}

/**
 * @brief Implementation of EailItemParent->get_evas_obj callback
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 * @returns item object
 */
static Evas_Object *
eail_list_get_evas_obj(EailItemParent   *parent,
                       EailItem         *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!it)
     {
        ERR("Error. Could not get Elm_Object_item from EailItem");
        return NULL;
     }

   return elm_list_item_object_get(it);
}

/**
 * @brief Implementation of EailItemParent->get_actions_supported callback
 *
 * @param parent an EailItemParent
 * @param item an EailItem
 * @returns enum representing supported actions
 */
static gint
eail_list_get_actions_supported(EailItemParent   *parent,
                                EailItem         *item)
{
   return EAIL_ACTION_SUPPORTED_CLICK;
}

/**
 * @brief Get name of list child
 *
 * @param parent EailList instance
 * @param item EailList child instance
 *
 * @returns const gchar * representing name of the child
 */
static const gchar *
eail_list_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   /*that will work only for default theme*/
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief Initialization of EailItemParentIface callbacks
 *
 * @param iface an EailItemParent interface
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name            = eail_list_item_name_get;
   iface->get_item_role            = eail_list_get_item_role;
   iface->ref_item_state_set       = eail_list_ref_item_state_set;
   iface->grab_item_focus          = eail_list_grab_item_focus;
   iface->get_evas_obj             = eail_list_get_evas_obj;
   iface->get_actions_supported    = eail_list_get_actions_supported;
}

/*
 * Functions for the *AtkSelection* interface
 */

/**
 * @brief Implementation of AtkSelection->add_selection callback
 *
 * As described in ATK doc:\n
 * Adds the specified accessible child of the object to the object's selection.
 *
 * @param selection an AtkSelection
 * @param i index of object
 *
 * @returns TRUE if operation ended successfully, FALSE otherwise
 */
static gboolean
eail_list_add_selection(AtkSelection *selection,
                        gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   const Eina_List *items = elm_list_items_get(object);
   Elm_Object_Item *item = eina_list_nth(items, i);

   if (!item) return FALSE;

   elm_list_item_selected_set(item, EINA_TRUE);
   return TRUE;
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
 * @param i index of object
 *
 * @returns an AtkObject representing the selected accessible , or NULL if
 * selection does not implement this interface
 */
static AtkObject *
eail_list_ref_selection(AtkSelection *selection,
                        gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), NULL);

   return eail_list_ref_child(ATK_OBJECT(selection), i);
}

/**
 * @brief Implementation of AtkSelection->get_selection_count callback
 *
 * As described in ATK doc:\n
 * Gets the number of accessible children currently selected. Note: callers
 * should not rely on NULL or on a zero value for indication of whether
 * AtkSelectionIface is implemented, they should use type checking/interface
 * checking macros or the atk_get_accessible_value() convenience method.

 * @param selection an AtkSelection
 *
 * @returns gint representing number of selected elements
 */
static gint
eail_list_get_selection_count(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   const Eina_List *sel_objs = elm_list_selected_items_get(object);

   if (!sel_objs) return 0;

   return eina_list_count(sel_objs);
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
 * @param i index of object
 *
 * @returns a gboolean representing if the specified child is selected, or
 * FALSE if selection does not implement this interface.
 */
static gboolean
eail_list_is_child_selected(AtkSelection *selection,
                            gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_list_selected_item_get(object);
   Elm_Object_Item *last_item = elm_list_last_item_get(object);
   Elm_Object_Item *item = elm_list_first_item_get(object);
   gint n = 0;

   /* if there is no selected object, that naturally means that none
    * of children is selected */
   if (!selected_item) return FALSE;

   do
     {
        if (selected_item == item) break;

        item = elm_list_item_next(item);
        n++;
     } while (item != last_item);

   return i == n;
}

/**
 * @brief Implementation of AtkSelection->remove_selection callback
 *
 * As described in ATK doc:\n
 * Removes the specified child of the object from the object's selection.
 *
 * @param selection an AtkSelection
 * @param i slection index
 *
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_list_remove_selection(AtkSelection *selection,
                           gint i)
{
   const Eina_List *items = NULL;
   Evas_Object *object = NULL;
   Elm_Object_Item *item = NULL;

   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   object = eail_widget_get_widget(EAIL_WIDGET(selection));
   g_return_val_if_fail(object, FALSE);

   items = elm_list_items_get(object);
   g_return_val_if_fail((eina_list_count(items) > i), FALSE);

   item = eina_list_nth(items, i);
   elm_list_item_selected_set(item, EINA_FALSE);

   /* if change was successful, then selected item now should be different */
   return (elm_list_selected_item_get(object) != item);
}

/**
 * @brief Helper function that selects/unselects all items in the list depending
 * on "selected" param value
 *
 * @param object an Evas_Object
 * @param selected selected state (TRUE means selected)
 *
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_list_mark_all_selected_state(const Evas_Object *object, Eina_Bool selected)
{
   const Eina_List *items = NULL;
   int i = 0;

   items = elm_list_items_get(object);
   for (i = 0; i < eina_list_count(items); ++i)
     {
        Elm_Object_Item *item = eina_list_nth(items, i);
        if (!item) return FALSE;

        elm_list_item_selected_set(item, selected);
     }

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
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_list_select_all_selection(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));

   /* can select all only when multiselect is enabled */
   if (!elm_list_multi_select_get(object)) return FALSE;

   return eail_list_mark_all_selected_state(object, EINA_TRUE);
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
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_list_clear_selection(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));

   return eail_list_mark_all_selected_state(object, EINA_FALSE);
}

/**
 * @brief Initializer of AtkSelectionIface interface
 *
 * @param iface an AtkSelection interface
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   iface->add_selection        = eail_list_add_selection;
   iface->clear_selection      = eail_list_clear_selection;
   iface->ref_selection        = eail_list_ref_selection;
   iface->get_selection_count  = eail_list_get_selection_count;
   iface->is_child_selected    = eail_list_is_child_selected;
   iface->remove_selection     = eail_list_remove_selection;
   iface->select_all_selection = eail_list_select_all_selection;
}
