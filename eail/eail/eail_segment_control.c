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
 * @file eail_segment_control.c
 * @brief Implementation of Segment Control widget
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_segment_control.h"
#include "eail_factory.h"
#include "eail_item_parent.h"

static void atk_selection_interface_init(AtkSelectionIface *iface);
static void eail_item_parent_interface_init(EailItemParentIface *iface);

/**
 * @brief Definition of EailSegmentControl as GObject
 *
 * EailSegmentControl is extended EAIL_TYPE_WIDGET with ATK_TYPE_SELECTION and
 * EAIL_TYPE_ITEM_PARENT interfaces implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailSegmentControl, eail_segment_control, EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init)
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                            eail_item_parent_interface_init));

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj an AtkObject
 * @param data additional initialization data
 */
static void
eail_segment_control_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_segment_control_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_LIST;
}

/**
 * @brief Initializer for GObject class
 *
 * @param segment_control an EailSegmentControl
 */
static void
eail_segment_control_init(EailSegmentControl *segment_control)
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
eail_segment_control_get_n_children(AtkObject *obj)
{
   gint n_items;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   n_items = elm_segment_control_item_count_get(widget);

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
eail_segment_control_ref_child(AtkObject *obj, gint i)
{
   AtkObject *child = NULL;
   Evas_Object *widget = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   if (elm_segment_control_item_count_get(widget) > i)
     {
         child = eail_factory_get_item_atk_obj
             (elm_segment_control_item_get(widget, i), ATK_ROLE_LIST_ITEM, obj);

        g_object_ref(child);
     }

   return child;
}

/**
 * @brief Destructor for Segment Control object
 * @param object a GObject
 */
static void
eail_segment_control_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_segment_control_parent_class)->finalize(object);
}

/**
 * @brief Initializer for GObject EailSegmentControl class (defines callbacks
 * for base AtkObject)
 *
 * @param klass an EailSegmentControl class
 */
static void
eail_segment_control_class_init(EailSegmentControlClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_segment_control_initialize;
   class->get_n_children = eail_segment_control_get_n_children;
   class->ref_child = eail_segment_control_ref_child;
   gobject_class->finalize = eail_segment_control_finalize;
}

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
eail_selection_add_selection(AtkSelection *selection,
                             gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *item = elm_segment_control_item_get(object, i);
   if (!item) return FALSE;
   elm_segment_control_item_selected_set(item, EINA_TRUE);
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
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_selection_clear_selection(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *item = elm_segment_control_item_selected_get(object);
   elm_segment_control_item_selected_set(item, EINA_FALSE);
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
eail_selection_ref_selection(AtkSelection *selection,
                             gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), NULL);

   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *item = elm_segment_control_item_get(object, i);
   if (!item) return NULL;

   return eail_segment_control_ref_child(ATK_OBJECT(selection), i);
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
eail_selection_get_selection_count(AtkSelection *selection)
{
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *item = elm_segment_control_item_selected_get(object);
   if (item) return 1;
   return 0;
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
eail_selection_is_child_selected(AtkSelection *selection,
                                 gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_segment_control_item_selected_get(object);
   Elm_Object_Item *item = elm_segment_control_item_get(object, i);
   return (selected_item == item);
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
   Elm_Object_Item *item = elm_segment_control_item_get(object, i);
   if (!item) return FALSE;
   elm_segment_control_item_selected_set(item, EINA_FALSE);
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
eail_selection_select_all_selection(AtkSelection *selection)
{
   return FALSE;
}

/**
 * @brief Initializer of AtkSelectionIface interface
 *
 * @param iface an AtkSelection interface
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   iface->add_selection = eail_selection_add_selection;
   iface->clear_selection = eail_selection_clear_selection;
   iface->ref_selection = eail_selection_ref_selection;
   iface->get_selection_count = eail_selection_get_selection_count;
   iface->is_child_selected = eail_selection_is_child_selected;
   iface->remove_selection = eail_selection_remove_selection;
   iface->select_all_selection = eail_selection_select_all_selection;
}

/**
 * @brief Get name of segment control child
 *
 * @param parent instance
 * @param item child instance
 *
 * @returns const gchar * representing name of the child
 */
static const gchar *
eail_segment_control_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief Initialization for EailItemParentIface interface
 *
 * @param iface an EailItemParent interface
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name = eail_segment_control_item_name_get;
}
