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
 * @file eail_genlist.c
 * @brief EailGenlist implementation
 */

#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>

#include "eail_genlist.h"
#include "eail_item_parent.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void eail_item_parent_interface_init   (EailItemParentIface *iface);
static void atk_selection_interface_init(AtkSelectionIface *iface);

/**
 * @brief Definition of EailGenlist as GObject
 *
 * EailList is extended ScrollableWidget with EAIL_TYPE_ITEM_PARENT and
 * ATK_TYPE_SELECTION implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailGenlist, eail_genlist, EAIL_TYPE_SCROLLABLE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                                 atk_selection_interface_init));

/**
 * @brief Genlist text part name
 */
#define EAIL_GENLIST_TEXT "elm.text"

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Handler for realized event
 *
 * Notifies about genlist's content changes.
 *
 * @param data passed to callback
 * @param obj Evas_Object that raised event
 * @param event_info additional event info (item is passed here)
 */
void
eail_list_item_handle_realized_event(void *data,
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
 * @brief Handler for unrealized event, used to notify about genlist content
 * changes
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info (item is passed here)
 */
void
eail_list_item_handle_unrealized_event(void *data,
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
 * @brief Initializer for AtkObjectClass
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_genlist_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_genlist_parent_class)->initialize(obj, data);

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
   evas_object_smart_callback_add(nested_widget, "realized",
                                  eail_list_item_handle_realized_event, obj);
   evas_object_smart_callback_add(nested_widget, "unrealized",
                                  eail_list_item_handle_unrealized_event, obj);
}

/**
 * @brief Prepares Eina_List filled with Elm_Object_Item* objects
 * representing items in the list
 *
 * Call eina_list_free on the returned list when results processing has been finished.
 *
 * @param genlist EailGenlist instance
 *
 * @return Eina_List representing the list of list's items
 */
static Eina_List *
eail_genlist_get_items(EailGenlist *genlist)
{
    Eina_List *items = NULL;
    Elm_Object_Item *item;
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(genlist));

    item = elm_genlist_first_item_get(widget);
    while (item) {
        items = eina_list_append(items, item);
        item = elm_genlist_item_next_get(item);
    }

    return items;
}

/**
 * @brief Gets the number of accessible children of the accessible
 *
 * Implementation of AtkObject->get_n_children callback.
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_genlist_get_n_children(AtkObject *obj)
{
    gint n_items;
    Eina_List *items;

    items = eail_genlist_get_items(EAIL_GENLIST(obj));
    n_items = eina_list_count(items);

    eina_list_free(items);

    return n_items;
}

/**
 * @brief Gets a reference to the specified accessible child of the object.
 *
 * The accessible children are 0-based so the first accessible child is
 * at index 0, the second at index 1 and so on.
 *
 * Implementation of AtkObject->ref_child callback.
 *
 * @param obj AtkObject instance
 * @param i child's index
 *
 * @returns AtkObject representing the specified accessible child of the
 * accessible
 */
static AtkObject *
eail_genlist_ref_child(AtkObject *obj, gint i)
{
    Eina_List *items;
    AtkObject *child = NULL;

    items = eail_genlist_get_items(EAIL_GENLIST(obj));
    if (eina_list_count(items) > i) {
       child = eail_factory_get_item_atk_obj
                             (eina_list_nth(items, i), ATK_ROLE_LIST_ITEM, obj);

       g_object_ref(child);
    }

    eina_list_free(items);

    return child;
}

/**
 * @brief Gets a reference to the state set of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * Implementation of AtkObject->ref_state_set callback.
 *
 * @param obj AtkObject instance
 *
 * @returns AtkStateSet representing the state set of the
 * accessible
 */
static AtkStateSet *
eail_genlist_ref_state_set(AtkObject *obj)
{
    AtkStateSet *state_set;
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    state_set = ATK_OBJECT_CLASS(eail_genlist_parent_class)->ref_state_set(obj);

    if (!widget) {
        return state_set;
    }

    if (elm_genlist_multi_select_get(widget)) {
        atk_state_set_add_state(state_set, ATK_STATE_MULTISELECTABLE);
    }

    return state_set;
}

/**
 * @brief Initializer for GObject EailList instance
 *
 * @param genlist EailGenlist instance
 */
static void
eail_genlist_init(EailGenlist *genlist)
{
}

/**
 * @brief Initializer for GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailGenlistClass instance
 */
static void
eail_genlist_class_init(EailGenlistClass *klass)
{
    AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

    class->initialize = eail_genlist_initialize;
    class->get_n_children = eail_genlist_get_n_children;
    class->ref_child = eail_genlist_ref_child;
    class->ref_state_set = eail_genlist_ref_state_set;
}

/*
 * Implementation of the *EailItemParent* interface
 */

/**
 * @brief Gets the name of EailItem
 *
 * Implementation of EailItemParent->get_name callback.
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 *
 * @returns character string representing the accessible name of
 * the item
 */
static const gchar *
eail_genlist_get_item_name(EailItemParent   *parent,
                           EailItem         *item)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!widget || !it) return NULL;

   return elm_object_item_part_text_get(it, EAIL_GENLIST_TEXT);
}

/**
 * @brief Gets index of element in parent object
 *
 * Implementation of EailItemParent->get_item_index_in_parent.
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 *
 * @returns integer representing the index of the accessible in its parent
 */
static gint
eail_genlist_get_item_index_in_parent(EailItemParent    *parent,
                                      EailItem          *item)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
    Elm_Object_Item *it = eail_item_get_item(item);

    if (!widget || !it) {
        return -1;
    }

    return elm_genlist_item_index_get(it);
}

/**
 * @brief Gets a reference to the state set of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * Implementation EailItemParent->ref_state_set callback.
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem object
 * @param state_set current state set
 *
 * @returns AtkStateSet representing the state set of the accessible
 */
static AtkStateSet *
eail_genlist_ref_item_state_set(EailItemParent  *parent,
                                EailItem        *item,
                                AtkStateSet       *state_set)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
    Elm_Object_Item *it = eail_item_get_item(item);
    Eina_List *list;

    if (!widget || !it) {
        return state_set;
    }

    if (elm_genlist_select_mode_get(widget) != ELM_OBJECT_SELECT_MODE_NONE &&
        elm_genlist_item_select_mode_get(it) != ELM_OBJECT_SELECT_MODE_NONE) {
        atk_state_set_add_state(state_set, ATK_STATE_SELECTABLE);

        if (elm_genlist_item_selected_get(it)) {
            atk_state_set_add_state(state_set, ATK_STATE_SELECTED);
        }
    }

    if (elm_genlist_item_type_get(it) == ELM_GENLIST_ITEM_TREE) {
        atk_state_set_add_state(state_set, ATK_STATE_EXPANDABLE);

        if (elm_genlist_item_expanded_get(it)) {
            atk_state_set_add_state(state_set, ATK_STATE_EXPANDED);
        }
    }

    atk_state_set_add_state(state_set, ATK_STATE_VISIBLE);

    list = elm_genlist_realized_items_get(widget);
    if (eina_list_data_find(list, it)) {
        atk_state_set_add_state(state_set, ATK_STATE_SHOWING);
    }
    eina_list_free(list);

    return state_set;
}

/**
 * @brief Grabs focus of item
 *
 * Implementation of EailItemParent->grab_item_focus callback.
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 *
 * @returns TRUE if operation was successfull, FALSE otherwise
 */
static gboolean
eail_genlist_grab_item_focus(EailItemParent *parent,
                             EailItem       *item)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
    Elm_Object_Item *it = eail_item_get_item(item);

    if (!widget || !it) {
        return FALSE;
    }

    return FALSE;
}

/**
 * @brief Gets the rectangle which gives the extent of the component
 *
 * Implementation of get_extents from AtkComponent.
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem object instance
 * @param [out] x x coordinate
 * @param [out] y y coordinate
 * @param [out] width width of the rectangle
 * @param [out] height height of the rectangle
 * @param coord_type specifies whether the coordinates are relative to the
 * screen or to the components top level window
 */
static void
eail_genlist_get_item_extents(EailItemParent    *parent,
                              EailItem          *item,
                              gint                *x,
                              gint                *y,
                              gint                *width,
                              gint                *height,
                              AtkCoordType         coord_type)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(parent));
    Elm_Object_Item *it = eail_item_get_item(item);

    if (!widget || !it) {
        return;
    }
}

/**
 * @brief Gets supported actions
 *
 * Implementation of EailItemParent->get_actions_supported callback.
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 * @returns integer representing supported actions
 */
static gint
eail_genlist_get_actions_supported(EailItemParent   *parent,
                                   EailItem         *item)
{
   return EAIL_ACTION_SUPPORTED_CLICK | EAIL_ACTION_SUPPORTED_PRESS |
            EAIL_ACTION_SUPPORTED_RELEASE;
}

/**
 * @brief Initialization of EailItemParentIface callbacks
 *
 * @param iface EailItemParentIface instance
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
    iface->get_item_name = eail_genlist_get_item_name;
    iface->get_item_index_in_parent = eail_genlist_get_item_index_in_parent;
    iface->ref_item_state_set = eail_genlist_ref_item_state_set;
    iface->grab_item_focus = eail_genlist_grab_item_focus;
    iface->get_item_extents = eail_genlist_get_item_extents;
    iface->get_actions_supported = eail_genlist_get_actions_supported;
}

/*
 * AtkSelection
 */

/**
 * @brief Adds the specified accessible child of the object to the object's selection
 *
 * Implementation of AtkSelection->add_selection callback.
 *
 * @param selection AtkSelection instance
 * @param i selection index
 *
 * @returns TRUE if operation was successfull, FALSE otherwise
 */
static gboolean
eail_genlist_add_selection(AtkSelection *selection,
                           gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Eina_List *items = eail_genlist_get_items(EAIL_GENLIST(selection));
   Elm_Object_Item *item = eina_list_nth(items, i);

   if (!item) return FALSE;

   elm_genlist_item_selected_set(item, EINA_TRUE);

   eina_list_free(items);

   return TRUE;
}

/**
 * @brief Gets a reference to the accessible object representing the specified selected
 * child of the object.
 *
 * Note: callers should not rely on NULL or on a zero value
 * for indication of whether AtkSelectionIface is implemented, they should use
 * type checking/interface checking macros or the atk_get_accessible_value()
 * convenience method.
 *
 * Implementation of AtkSelection->ref_selection callback.
 *
 * @param selection AtkSelection instance
 * @param i selection index
 *
 * @returns AtkObject representing the selected accessible or NULL if
 * selection does not implement this interface
 */
static AtkObject *
eail_genlist_ref_selection(AtkSelection *selection,
                        gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), NULL);

   return eail_genlist_ref_child(ATK_OBJECT(selection), i);
}

/**
 * @brief Gets the number of accessible children currently selected.
 *
 * Note: callers should not rely on NULL or on a zero value for indication of whether
 * AtkSelectionIface is implemented, they should use type checking/interface
 * checking macros or the atk_get_accessible_value() convenience method.
 *
 * Implementation of AtkSelection->get_selection_count callback.
 *
 * @param selection AtkSelection instance
 *
 * @returns integer representing the number of selected children
 */
static gint
eail_genlist_get_selection_count(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   const Eina_List *sel_objs = elm_genlist_selected_items_get(object);

   if (!sel_objs) return 0;

   return eina_list_count(sel_objs);
}

/**
 * @brief Determines if the current child of this object is selected.
 *
 * Note: callers should not rely on NULL or on a zero value for indication of whether
 * AtkSelectionIface is implemented, they should use type checking/interface
 * checking macros or the atk_get_accessible_value() convenience method.
 *
 * Implementation of AtkSelection->is_child_selected callback.
 *
 * @param selection AtkSelection instance
 * @param i index of child
 *
 * @returns gboolean representing whether the specified child is selected, or 0 if
 * selection does not implement this interface
 */
static gboolean
eail_genlist_is_child_selected(AtkSelection *selection,
                               gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_genlist_selected_item_get(object);
   Elm_Object_Item *last_item = elm_genlist_last_item_get(object);
   Elm_Object_Item *item = elm_genlist_first_item_get(object);
   gint n = 0;

   /* of course if there is no selected object, that naturally means that none
    * of children is selected */
   if (!selected_item) return FALSE;

   do
     {
        if (selected_item == item) break;

        item = elm_genlist_item_next_get(item);
        n++;
     } while (item != last_item);

   return i == n;
}

/**
 * @brief Removes the specified child of the object from the object's selection
 *
 * Implementation of AtkSelection->remove_selection callback.
 *
 * @param selection AtkSelection instance
 * @param i index of child
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_genlist_remove_selection(AtkSelection *selection,
                              gint i)
{
   Eina_List *items = NULL;
   Evas_Object *object = NULL;
   Elm_Object_Item *item = NULL;

   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   object = eail_widget_get_widget(EAIL_WIDGET(selection));
   g_return_val_if_fail(object, FALSE);

   items = eail_genlist_get_items(EAIL_GENLIST(selection));
   g_return_val_if_fail((eina_list_count(items) > i), FALSE);

   item = eina_list_nth(items, i);
   elm_genlist_item_selected_set(item, EINA_FALSE);

   eina_list_free(items);

   /* if change was successful, then selected item now should be different */
   return (elm_genlist_selected_item_get(object) != item);
}

/**
 * @brief Helper function for selecting/unselecting all items in the list depending
 * on "selected" param value
 *
 * @param selection AtkSelection instance
 * @param selected selected state (TRUE means selected)
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_genlist_mark_all_selected_state(AtkSelection *selection,
                                     Eina_Bool selected)
{
   Eina_List *items = NULL;
   int i = 0;

   items = eail_genlist_get_items(EAIL_GENLIST(selection));
   for (i = 0; i < eina_list_count(items); ++i)
     {
        Elm_Object_Item *item = eina_list_nth(items, i);
        if (!item) return FALSE;

        elm_genlist_item_selected_set(item, selected);
     }

   eina_list_free(items);

   return TRUE;
}

/**
 * @brief Causes every child of the object to be selected if the object supports
 * multiple selections
 *
 * Implementation of AtkSelection->select_all_selection callback.
 *
 * @param selection AtkSelection instance
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_genlist_select_all_selection(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));

   /* can select all only when multiselect is enabled */
   if (!elm_genlist_multi_select_get(object)) return FALSE;

   return eail_genlist_mark_all_selected_state(selection, EINA_TRUE);
}

/**
 * @brief Clears the selection in the object so that no children in the object are
 * selected
 *
 * Implementation of AtkSelection->clear_selection callback.
 *
 * @param selection AtkSelection instance
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_genlist_clear_selection(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);

   return eail_genlist_mark_all_selected_state(selection, EINA_FALSE);
}

/**
 * @brief Initializer of AtkSelectionIface interface
 *
 * @param iface AtkSelectionIface instance
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   iface->add_selection        = eail_genlist_add_selection;
   iface->clear_selection      = eail_genlist_clear_selection;
   iface->ref_selection        = eail_genlist_ref_selection;
   iface->get_selection_count  = eail_genlist_get_selection_count;
   iface->is_child_selected    = eail_genlist_is_child_selected;
   iface->remove_selection     = eail_genlist_remove_selection;
   iface->select_all_selection = eail_genlist_select_all_selection;
}
