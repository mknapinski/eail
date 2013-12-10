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
 * @file eail_index.c
 * @brief EailIndex implementation
 */

/* enabling beta API support for Eo parts*/
#define EFL_BETA_API_SUPPORT

#include <Elementary.h>

#include "eail_index.h"
#include "eail_factory.h"
#include "eail_item_parent.h"
#include "eail_utils.h"
/* internal elm_widget api for listening internal index parts */
#define ELM_INTERNAL_API_ARGESFSDFEFC
#include <elm_widget.h>
#include <elm_widget_index.h>

#ifndef ELM_OBJ_INDEX_CLASS
#ifndef ELM_INDEX_CLASS
#include <elm_index_eo.h>
#endif
#endif

#include "eail_priv.h"

static void eail_item_parent_interface_init(EailItemParentIface *iface);
static void atk_selection_interface_init(AtkSelectionIface *iface);

/**
 * @brief EailIndex GObject definition
 *
 * It extends EAIL_TYPE_WIDGET class and implements EAIL_TYPE_ITEM_PARENT and
 * ATK_TYPE_SELECTION interfaces
 */
G_DEFINE_TYPE_WITH_CODE(EailIndex,
                        eail_index,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init));

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObject
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_index_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_index_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_LIST;
}

/**
 * @brief EailIndex GObject instance initializer
 *
 * @param button EailIndex instance
 */
static void
eail_index_init(EailIndex *button)
{
}

/**
 * @brief Gets the list of Elm_Object_Item* that are contained in an index widget
 *
 * @param obj AtkObject instance
 *
 * @returns const Eina_List with Elm_Object_Item* for every index
 */
static const Eina_List *
eail_index_get_index_items(AtkObject *obj) {
   Evas_Object *index_widget = NULL;

   index_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   g_return_val_if_fail(index_widget, NULL);

   ELM_INDEX_DATA_GET(index_widget, index_sd);
   g_return_val_if_fail(index_sd, NULL);

   return index_sd->items;
}
/**
 * @brief Gets the number of accessible children of the accessible.
 *
 * Implementation of AtkObject->get_n_children callback.
 *
 * @param obj AtkObject instance
 *
 * @returns integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_index_get_n_children(AtkObject *obj)
{
   return eina_list_count(eail_index_get_index_items(obj));
}

/**
 * @brief Gets a reference to the specified accessible child of the object.
 *
 * The accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * Implementation of AtkObject->ref_child callback.
 *
 * @param obj AtkObject instance
 * @param i index of a child
 *
 * @returns AtkObject representing the specified accessible child of the
 * accessible
 */
static AtkObject *
eail_index_ref_child(AtkObject *obj, gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;

   items = eail_index_get_index_items(obj);
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_item_atk_obj
                          (eina_list_nth(items, i), ATK_ROLE_LIST_ITEM, obj);

        g_object_ref(child);
     }

   return child;
}

/**
 * @brief Gets the accessible name of the accessible.
 *
 * Implementation of AtkObject->get_name callback.
 *
 * @param obj AtkObject instance
 *
 * @returns string containing the accessible description of
 * the accessible
 */
static const gchar *
eail_index_get_name(AtkObject *obj)
{
   Evas_Object *widget;
   const char *atk_name;

   g_return_val_if_fail(EAIL_IS_INDEX(obj), NULL);

   atk_name = ATK_OBJECT_CLASS(eail_index_parent_class)->get_name(obj);
   if (atk_name)
     return atk_name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   return elm_object_text_get(widget);
}

/**
 * @brief Destructor of EailIndex object
 * @param object GObject object to be finalized
 */
static void
eail_index_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_index_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailIndex GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailIndex instance
 */
static void
eail_index_class_init(EailIndexClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   atk_class->initialize = eail_index_initialize;
   atk_class->get_n_children = eail_index_get_n_children;
   atk_class->ref_child = eail_index_ref_child;
   atk_class->get_name = eail_index_get_name;
   gobject_class->finalize = eail_index_finalize;
}

/**
 * @brief Checks if content get is supported
 *
 * Implementation of EailItemParent->is_content_get_supported callback.
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 * @returns always FALSE
 */
gboolean
eail_index_content_get_support(EailItemParent   *parent,
                               EailItem         *item)
{
   return FALSE;
}

/**
 * @brief Gets item's name
 *
 * @param parent EailItemParent instance
 * @param item EailItem instance
 * @return string containing item's name
 */
const gchar *
eail_index_get_item_name(EailItemParent   *parent,
                         EailItem         *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);

   return elm_index_item_letter_get(it);
}

/**
 * @brief Gets index in parent
 *
 * @param parent EailItemParent instance
 * @param item EailItem child instance
 *
 * @returns int representing the index in parent
 */
static gint
eail_index_get_item_index_in_parent(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   const Eina_List *items;
   int i = 0;

   if (!it) return -1;

   items = eail_index_get_index_items(ATK_OBJECT(parent));
   for (i=0; i<eina_list_count(items); ++i)
   {
      if (eina_list_nth(items, i) == it)
      {
         return i;
      }
   }

   return -1;
}

/**
 * @brief Initialization of EailItemParentIface callbacks
 *
 * @param iface EailItemParentIface instance
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name = eail_index_get_item_name;
   iface->is_content_get_supported = eail_index_content_get_support;
   iface->get_item_index_in_parent = eail_index_get_item_index_in_parent;
}

/**
 * @brief Adds the specified accessible child of the object to the object's selection.
 *
 * Implementation of AtkSelection->add_selection callback.
 *
 * @param selection AtkSelection instance
 * @param i index of object
 *
 * @returns TRUE if operation was successful, FALSE otherwise
 */
static gboolean
eail_index_add_selection(AtkSelection *selection,
                         gint i)
{
   const Eina_List *items = eail_index_get_index_items(ATK_OBJECT(selection));
   if (!items) return FALSE;
   Elm_Object_Item *item = eina_list_nth(items, i);

   if (!item) return FALSE;

   elm_index_item_selected_set(item, EINA_TRUE);
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
 * @param i index of object
 *
 * @returns AtkObject representing the selected accessible or NULL if
 * selection does not implement this interface
 */
static AtkObject *
eail_index_ref_selection(AtkSelection *selection,
                         gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), NULL);

   return eail_index_ref_child(ATK_OBJECT(selection), i);
}

/**
 * @brief Clears the selection in the object so that no children in the object are
 * selected.
 *
 * Implementation of AtkSelection->clear_selection callback.
 *
 * @param selection aAtkSelection instance
 *
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_index_clear_selection(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   g_return_val_if_fail(object, FALSE);

   Elm_Object_Item *selected_item = elm_index_selected_item_get(object, 0);

   /* no selected item found so selection is already clear, doing nothing and
    * returning TRUE to mark success*/
   if (!selected_item) return TRUE;

   elm_index_item_selected_set(selected_item, EINA_FALSE);

   return TRUE;
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
 * @returns integer representing the number of selected elements
 */
static gint
eail_index_get_selection_count(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));

   if (elm_index_selected_item_get(object, 0))
     return 1;
   else
     return 0;
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
 * @param i index of object
 *
 * @returns TRUE if the specified child is selected or
 * FALSE if selection does not implement this interface
 */
static gboolean
eail_index_is_child_selected(AtkSelection *selection,
                             gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_index_selected_item_get(object, 0);
   const Eina_List *items = eail_index_get_index_items(ATK_OBJECT(selection));
   Elm_Object_Item *item_witn_n_index = NULL;

   /* if no item is selected, then we are sure that none of the child is
    * selected*/
   if (!selected_item) return FALSE;

   /* no items on list so no child is selected*/
   if (!items) return FALSE;

   item_witn_n_index =  eina_list_nth(items, i);

   if (item_witn_n_index == selected_item)
     return TRUE;
   else
     return FALSE;
}

/**
 * @brief Initializer of AtkSelectionIface interface
 *
 * @param iface AtkSelectionIface
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   iface->add_selection        = eail_index_add_selection;
   iface->clear_selection      = eail_index_clear_selection;
   iface->ref_selection        = eail_index_ref_selection;
   iface->get_selection_count  = eail_index_get_selection_count;
   iface->is_child_selected    = eail_index_is_child_selected;
}
