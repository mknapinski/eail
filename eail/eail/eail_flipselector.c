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
 * @file eail_flipselector.c
 * @brief EailFlipselector implementation.
 */

#include <Elementary.h>

#include "eail_flipselector.h"
#include "eail_factory.h"
#include "eail_item_parent.h"
#include "eail_priv.h"

/**
 * @brief Maximum number of selections
 */
#define FLIPSELECTOR_MAX_SELECTIONS 1

static void atk_selection_interface_init(AtkSelectionIface *iface);
static void atk_action_interface_init(AtkActionIface *iface);
static void eail_item_parent_interface_init(EailItemParentIface *iface);

/**
 * @brief EailFlipselector type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailFlipselector,
                        eail_flipselector,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/**
 * @brief Function executed on selected event
 *
 * @param data user data passed to callback
 * @param obj source object
 * @param event_info event info
 */
static void
_on_selected(void *data,
             Evas_Object *obj,
             void *event_info)
{
    EailFlipselector *flip = EAIL_FLIPSELECTOR(data);
    atk_object_notify_state_change(ATK_OBJECT(flip), ATK_STATE_SELECTED, TRUE);
}

/**
 * @brief EailFlipselector initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_flipselector_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *widget;

   ATK_OBJECT_CLASS(eail_flipselector_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_LIST;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));

   evas_object_smart_callback_add(widget, "selected", _on_selected, obj);
   evas_object_smart_callback_add(widget, "selected", _on_selected, obj);
}

/**
 * @brief EailFlipselector finalizer
 *
 * Frees memory assigned to object.
 *
 * @param object GObject instance
 */
static void
eail_flipselector_finalize(GObject *object)
{
   EailFlipselector *flipselector = EAIL_FLIPSELECTOR(object);

   if (flipselector->next_description) free(flipselector->next_description);
   if (flipselector->prev_description) free(flipselector->prev_description);

   G_OBJECT_CLASS(eail_flipselector_parent_class)->finalize(object);
}

/**
 * @brief Gets obj's items list
 *
 * @param obj EailFlipselector instance
 * @return Eina_List representing the list of obj's items
 */
static const Eina_List *
eail_flipselector_items_get(EailFlipselector *obj)
{
   Evas_Object *widget;
   const Eina_List *items = NULL;

   g_return_val_if_fail(EAIL_FLIPSELECTOR(obj), NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   items = elm_flipselector_items_get(widget);
   if (!items) return NULL;

   return items;
}

/**
 * @brief Gets a reference to the i-th child of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param obj AtkObject instance
 * @param i child index
 * @return AtkObject representing the i-th child of the accessible
 */
static AtkObject *
eail_flipselector_ref_child(AtkObject *obj,
                            gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;

   g_return_val_if_fail(EAIL_IS_FLIPSELECTOR(obj), NULL);

   items = eail_flipselector_items_get(EAIL_FLIPSELECTOR(obj));
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_item_atk_obj
                           (eina_list_nth(items, i), ATK_ROLE_LABEL, obj);

        g_object_ref(child);
     }
   else
     ERR("Tried to ref child with index %d out of bounds!", i);

   return child;
}

/**
 * @brief Gets the number of children of the accessible
 *
 * @param obj AtkObject instance
 * @return integer representing the number children of the accessible
 */
static gint
eail_flipselector_get_n_children(AtkObject *obj)
{
   const Eina_List *list;

   g_return_val_if_fail(EAIL_IS_FLIPSELECTOR(obj), 0);

   list = eail_flipselector_items_get(EAIL_FLIPSELECTOR(obj));

   return eina_list_count(list);
}

/**
 * @brief EailFlipselector interface initializer
 *
 * @param flipselector EailFlipselector instance
 */
static void
eail_flipselector_init(EailFlipselector *flipselector)
{
   flipselector->next_description = NULL;
   flipselector->prev_description = NULL;
}



/**
 * @brief Gets the name of a flipselector's child
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 *
 * @returns string representing the name of the child
 */
static const gchar *
eail_flipselector_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief Checks if content get is supported
 *
 * Implementation of EailItemParent->is_content_get_supported callback.
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 * @returns FALSE as there is no content support
 */
gboolean
eail_flipselector_content_get_support(EailItemParent   *parent,
                                          EailItem         *item)
{
   return FALSE;
}

/**
 * @brief EailItemParent interface initializer
 *
 * @param iface EailItemParentIface instance
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name = eail_flipselector_item_name_get;
   iface->is_content_get_supported = eail_flipselector_content_get_support;
}

/**
 * @brief EailFlipselector class initializer
 *
 * @param klass EailFlipselectorClass instance
 */
static void
eail_flipselector_class_init(EailFlipselectorClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   atk_class->initialize = eail_flipselector_initialize;
   atk_class->get_n_children = eail_flipselector_get_n_children;
   atk_class->ref_child = eail_flipselector_ref_child;
   gobject_class->finalize = eail_flipselector_finalize;
}

/**
 * @brief Selects the i-th item of the accessible
 *
 * @param selection AtkSelection instance
 * @param i item index
 * @return TRUE if item was successfully selected, FALSE otherwise
 */
static gboolean
eail_flipselector_add_selection(AtkSelection *selection,
                                gint i)
{
   Elm_Object_Item *it;
   const Eina_List *list;

   g_return_val_if_fail(EAIL_IS_FLIPSELECTOR(selection), FALSE);

   list = eail_flipselector_items_get(EAIL_FLIPSELECTOR(selection));

   if (eina_list_count(list) > i)
     {
        it = eina_list_nth(list, i);
        if (!it) return FALSE;

        elm_flipselector_item_selected_set(it, EINA_TRUE);
     }

   return TRUE;
}

/**
 * @brief Clears selections
 *
 * @param selection AtkSelection instance
 * @return always FALSE
 */
static gboolean
eail_flipselector_clear_selection(AtkSelection *selection)
{
   return FALSE;
}

/**
 * @brief Gets the number of currently selected children
 *
 * @param selection AtkSelection instance
 * @return integer representing the number of selected children
 */
static gint
eail_flipselector_get_selection_count(AtkSelection *selection)
{
   return FLIPSELECTOR_MAX_SELECTIONS;
}

/**
 * @brief Determines if the i-th child of the accessible is selected
 *
 * @param selection AtkSelection instance
 * @param i child index
 * @return TRUE if the i-th child is selected, FALSE otherwise
 */
static gboolean
eail_flipselector_is_child_selected(AtkSelection *selection,
                                    gint i)
{
   Evas_Object *widget;
   Elm_Object_Item *sel_it = NULL;
   Elm_Object_Item *it = NULL;
   const Eina_List *list;

   g_return_val_if_fail(EAIL_IS_FLIPSELECTOR(selection), FALSE);

   list = eail_flipselector_items_get(EAIL_FLIPSELECTOR(selection));

   if (eina_list_count(list) > i)
     {
        widget = eail_widget_get_widget(EAIL_WIDGET(selection));
        if (!widget) return FALSE;

        sel_it = elm_flipselector_selected_item_get(widget);
        if (!sel_it) return FALSE;

        it = eina_list_nth(list, i);
     }

   return sel_it == it;
}

/**
 * @brief Removes the i-th selection of the accessible
 *
 * @param selection AtkSelection instance
 * @param i selection index
 * @return TRUE if selection was successfully removed, FALSE otherwise
 */
static gboolean
eail_flipselector_remove_selection(AtkSelection *selection,
                                   gint i)
{
   Evas_Object *widget;
   Elm_Object_Item *sel_it;
   Elm_Object_Item *it;
   const Eina_List *list;

   g_return_val_if_fail(EAIL_IS_FLIPSELECTOR(selection), FALSE);

   list = eail_flipselector_items_get(EAIL_FLIPSELECTOR(selection));

   if (eina_list_count(list) > i)
     {
        widget = eail_widget_get_widget(EAIL_WIDGET(selection));
        if (!widget) return FALSE;

        sel_it = elm_flipselector_selected_item_get(widget);
        if (!sel_it) return FALSE;

        it = eina_list_nth(list, i);
        if (it == sel_it)
          {
             elm_flipselector_item_selected_set(sel_it, EINA_FALSE);

             return TRUE;
          }
     }

   return FALSE;
}

/**
 * @brief Selects all children of the accessible
 *
 * @param selection AtkSelection instance
 * @returns always FALSE
 */
static gboolean
eail_flipselector_select_all_selection(AtkSelection *selection)
{
   return FALSE;
}

/**
 * @brief AtkSelection interface initializer
 *
 * @param iface AtkSelectionIface instance
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   iface->add_selection        = eail_flipselector_add_selection;
   iface->clear_selection      = eail_flipselector_clear_selection;
   iface->get_selection_count  = eail_flipselector_get_selection_count;
   iface->is_child_selected    = eail_flipselector_is_child_selected;
   iface->remove_selection     = eail_flipselector_remove_selection;
   iface->select_all_selection = eail_flipselector_select_all_selection;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief Performs the i-th action
 *
 * @param action AtkAction instance
 * @param i action index
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_flipselector_action_do_action(AtkAction *action,
                                   gint i)
{
   Evas_Object *widget;
   const char *action_name;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   if (!g_strcmp0("next", action_name))
     elm_flipselector_flip_next(widget);
   else if (!g_strcmp0("previous", action_name))
     elm_flipselector_flip_prev(widget);
   else
     return FALSE;

   return TRUE;
}

/**
 * @brief Gets the number of actions available on the object
 *
 * @param action AtkAction instance
 * @return integer representing the number of implemented actions
 */
static gint
eail_flipselector_action_get_n_actions(AtkAction *action)
{
   return 2;
}

/**
 * @brief Gets the description string of an action with the given index
 *
 * @param action AtkAction instance
 * @param i index of action
 *
 * @returns string representing the specified action's description
 * or NULL if one was not found
 */
static const gchar *
eail_flipselector_action_get_description(AtkAction *action,
                                         gint i)
{
   const char *action_description = NULL;
   EailFlipselector *flipselector;

   flipselector = EAIL_FLIPSELECTOR(action);
   if (!flipselector) return NULL;

   switch (i)
     {
       case 0:
           action_description = flipselector->next_description;
           break;

       case 1:
           action_description = flipselector->prev_description;
           break;

       default:
           action_description = NULL;
           break;
     }

   return action_description;
}

/**
 * @brief Gets the name string of an action with the given index
 *
 * @param action AtkAction instance
 * @param i action index
 * @return string representing the specified action's name
 * or NULL if one was not found
 */
static const gchar *
eail_flipselector_action_get_name(AtkAction *action,
                                  gint i)
{
   const char* action_name = NULL;

   switch (i)
     {
       case 0:
           /* user selects next item */
           action_name = "next";
           break;

       case 1:
           /* user selects previous item */
           action_name = "previous";
           break;

       default:
           action_name = NULL;
           break;
     }

   return action_name;
}

/**
 * @brief Sets the description of the specified action
 *
 * @param action AtkAction instance
 * @param i action's number
 * @param desc action's description
 * @return TRUE if description was set successfully, FALSE otherwise
 */
static gboolean
eail_flipselector_action_set_description(AtkAction *action,
                                         gint i,
                                         const gchar *desc)
{
   EailFlipselector *flipselector;
   char **value;

   flipselector = EAIL_FLIPSELECTOR(action);
   if (!flipselector) return FALSE;

   switch (i)
     {
       case 0:
           value = &flipselector->next_description;
           break;

       case 1:
           value = &flipselector->prev_description;
           break;

       default:
           value = NULL;
           break;
     }

   if (value)
     {
        free(*value);
        *value = g_strdup(desc);

        return TRUE;
     }

   return FALSE;
}

/**
 * @brief AtkAction interface initializer
 *
 * @param iface AtkActionIface instance
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action          = eail_flipselector_action_do_action;
   iface->get_n_actions      = eail_flipselector_action_get_n_actions;
   iface->get_description    = eail_flipselector_action_get_description;
   iface->get_name           = eail_flipselector_action_get_name;
   iface->set_description    = eail_flipselector_action_set_description;
}

