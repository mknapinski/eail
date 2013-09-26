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
 * @file eail_slideshow.c
 * @brief EailSlideshow implementation
 */

#include <Elementary.h>

#include "eail_slideshow.h"
#include "eail_factory.h"
#include "eail_priv.h"

/**
 * @brief Default timeout in seconds
 */
#define SLIDESHOW_DEFAULT_TIMEOUT 3.0

static void atk_selection_interface_init(AtkSelectionIface *iface);
static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief Define GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailSlideshow, eail_slideshow, EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/**
 * @brief Gets a reference to the specified accessible child of the object.
 *
 * The accessible children are 0-based so the first accessible child
 * is at index 0, the second at index 1 and so on.
 *
 * Implementation of ref_child from AtkObject interface.
 *
 * @param obj AtkObject instance
 * @param i index of a child object
 *
 * @returns AtkObject representing the specified accessible child
 */
static AtkObject*
eail_slideshow_ref_child(AtkObject *obj, gint i)
{
   Evas_Object *widget, *child_widget;
   Elm_Object_Item *it;

   g_return_val_if_fail(EAIL_IS_SLIDESHOW(obj), NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   it = elm_slideshow_item_nth_get(widget, i);
   if (!it) return NULL;

   child_widget = elm_slideshow_item_object_get(it);
   if (!child_widget)
     {
        WRN("Given index %d is beyond realized (cached) views. Returning NULL",
            i);
        return NULL;
     }

   return eail_factory_get_accessible(child_widget);
}

/**
 * @brief Gets the number of accessible children of the accessible
 *
 * Implementation of get_n_children from AtkObject interface.
 *
 * @param obj AtkObject instance
 * @returns integer representing the number of widget's children
 */
static gint
eail_slideshow_n_children_get(AtkObject *obj)
{
   Evas_Object *widget;
   gint items_count;

   g_return_val_if_fail(EAIL_IS_SLIDESHOW(obj), 0);

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return 0;
   /*the real number of items stored in slideshow widget is returned by
     elm_slideshow_count_get. However we will not be able to return all items
     widget since only those stored in cache are created.*/
   items_count = elm_slideshow_count_get(widget);
   return items_count;
}

/**
 * @brief Initialize EailSlideshow as AtkObject
 *
 * @param obj AtkObject instance
 * @param data user data passed to initialization
 */
static void
eail_slideshow_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_slideshow_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_DOCUMENT_PRESENTATION;
}

/**
 * @brief EailSlideshow object initialization
 *
 * @param slideshow EailSlideshow instance
 */
static void
eail_slideshow_init(EailSlideshow *slideshow)
{
   slideshow->next_description = NULL;
   slideshow->previous_description = NULL;
   slideshow->stop_description = NULL;
   slideshow->start_description = NULL;
}

/**
 * @brief Destructor of EailSlideshow object
 *
 * @param object GObject instance
 */
static void
eail_slideshow_finalize(GObject *object)
{
   EailSlideshow *slideshow = EAIL_SLIDESHOW(object);
   if (slideshow->next_description) free(slideshow->next_description);
   if (slideshow->previous_description) free(slideshow->previous_description);
   if (slideshow->stop_description) free(slideshow->stop_description);
   if (slideshow->start_description) free(slideshow->start_description);

   G_OBJECT_CLASS(eail_slideshow_parent_class)->finalize(object);

}

/**
 * @brief EailSlideshow class initialization
 *
 * @param klass EailSlideshowClass instance
 */
static void
eail_slideshow_class_init(EailSlideshowClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   atk_class->initialize = eail_slideshow_initialize;
   atk_class->get_n_children = eail_slideshow_n_children_get;
   atk_class->ref_child = eail_slideshow_ref_child;

   g_object_class->finalize = eail_slideshow_finalize;
}

/**
 * @brief Gets the number of accessible children currently selected.
 *
 * Implementation of get_selection_count from AtkSelection interface.
 *
 * @param selection AtkSelection instance
 *
 * @returns integer representing the number of currently selected children
 */
static gint
eail_slideshow_selection_count(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_SLIDESHOW(selection), 0);

   return 1;
}

/**
 * @brief Adds the specified accessible child of the object to the object's selection
 *
 * Implementation of add_selection from AtkSelection interface.
 *
 * @param selection AtkSelection instance
 * @param i index of object to select
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_slideshow_add_selection(AtkSelection *selection, gint i)
{
   Evas_Object *widget;
   Elm_Object_Item *it;

   g_return_val_if_fail(EAIL_IS_SLIDESHOW(selection), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   if (!widget) return FALSE;

   it = elm_slideshow_item_nth_get(widget, i);
   if (!it) return FALSE;

   elm_slideshow_item_show(it);

   return TRUE;
}

/**
 * @brief Gets a reference to the accessible object representing
 * the specified selected child of the object.
 *
 * Implementation of ref_selecton from AtkSelection.
 *
 * @param selection AtkSelection instance
 * @param i index of selection
 *
 * @returns AtkObject representing the specified selected child
 */
static AtkObject*
eail_slideshow_ref_selection(AtkSelection *selection, gint i)
{
   Evas_Object *widget, *it_widget;
   Elm_Object_Item *it;

   g_return_val_if_fail(EAIL_IS_SLIDESHOW(selection), NULL);
   g_return_val_if_fail(i == 0, NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   it = elm_slideshow_item_current_get(widget);

   it_widget = elm_slideshow_item_object_get(it);

   return eail_factory_get_accessible(it_widget);
}

/**
 * @brief Determines if the current child of this object is selected.
 *
 * Implementation of is_child_selected from AtkSelection.
 *
 * @param selection AtkSelection instance
 * @param i index of selection
 *
 * @return TRUE if object is selected, FALSE otherwise
 */
static gboolean
eail_slideshow_is_child_selected(AtkSelection *selection, gint i)
{
   Evas_Object *widget;
   Elm_Object_Item *it, *current;

   g_return_val_if_fail(EAIL_IS_SLIDESHOW(selection), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   if (!widget) return FALSE;

   current = elm_slideshow_item_current_get(widget);
   it = elm_slideshow_item_nth_get(widget, i);

   return (it == current);
}

/**
 * @brief Initialization of AtkSelection interface
 *
 * Function called upon instance creation. It initializes AtkValue interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface AtkObject instance that implements AtkSelectionInterface
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->add_selection = eail_slideshow_add_selection;
   iface->ref_selection = eail_slideshow_ref_selection;
   iface->get_selection_count = eail_slideshow_selection_count;
   iface->is_child_selected = eail_slideshow_is_child_selected;
}

/**
 * @brief Gets the number of accessible actions available on the object
 *
 * If there are more than one, the first one is considered the "default" action of the object.
 *
 * Implementation of get_n_actions from AtkAction interface.
 *
 * @param action AtkAction instance
 * @returns integer representing the number of implemented actions
 */
static gint
eail_slideshow_n_actions_get(AtkAction *action)
{
   g_return_val_if_fail(EAIL_IS_SLIDESHOW(action), 0);

   return 4;
}

/**
 * @brief Gets the name of the specified action of the object
 *
 * Implementation of get_name from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @returns string representing the name of the specified action
 */
static const gchar*
eail_slideshow_action_name_get(AtkAction *action, int i)
{
   const gchar *action_name;
   g_return_val_if_fail(EAIL_IS_SLIDESHOW(action), NULL);
   switch (i)
     {
      case 0:
         /*advance to next image*/
         action_name = "next";
         break;
      case 1:
         /*advance to previous image*/
         action_name = "previous";
         break;
      case 2:
         /*stop the slideshow*/
         action_name = "stop";
         break;
      case 3:
         /*start the slideshow*/
         action_name = "start";
         break;
      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Compares two doubles
 *
 * @param A first number
 * @param B second number
 * @param max_ulps number of doubles between A and B
 * @returns TRUE if A equals B, FALSE otherwise
 */
static gboolean
_double_cmp(double A, double B, gint max_ulps)
{
   union {
        double d;
        gint64 i;
   }f1, f2;
   gint64 diff;

   f1.d = A;
   f2.d = B;

   g_assert(max_ulps > 0 && max_ulps < 4*1024*1024);

   if (f1.i < 0) f1.i = 0x80000000 - f1.i;
   if (f2.i < 0) f1.i = 0x80000000 - f2.i;

   diff = abs(f1.i - f2.i);
   if (diff <= max_ulps)
     return TRUE;

   return FALSE;
}

/**
 * @brief Performs the specified action on the object
 *
 * Implementation of do_action from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_slideshow_do_action(AtkAction *action, gint i)
{
   Evas_Object *widget;
   gboolean result;
   double slideshow_timeout;

   g_return_val_if_fail(EAIL_IS_SLIDESHOW(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   switch (i)
     {
       case 0:
           elm_slideshow_next(widget);
           result = TRUE;
           break;
       case 1:
           elm_slideshow_previous(widget);
           result = TRUE;
           break;
       case 2:
           elm_slideshow_timeout_set(widget, 0.0);
           result = TRUE;
           break;
       case 3:
           slideshow_timeout = elm_slideshow_timeout_get(widget);
           if (_double_cmp(slideshow_timeout, 0.0, 1))
             {
                result = TRUE;
                elm_slideshow_timeout_set(widget, SLIDESHOW_DEFAULT_TIMEOUT);
                break;
             }
           result = FALSE;
           break;
       default:
           result = FALSE;
           break;
     }

   return result;
}

/**
 * @brief Gets the description of the specified action of the object
 *
 * Implementation of get_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @returns string representing the description of the specified action
 */
static const gchar *
eail_slidshow_description_get(AtkAction *action, gint i)
{
   const gchar *action_description;
   EailSlideshow *slideshow;

   g_return_val_if_fail(EAIL_IS_SLIDESHOW(action), NULL);

   slideshow = EAIL_SLIDESHOW(action);
   switch (i)
     {
       case 0:
           action_description = slideshow->next_description;
           break;
       case 1:
           action_description = slideshow->previous_description;
           break;
       case 2:
           action_description = slideshow->stop_description;
           break;
       case 3:
           action_description = slideshow->start_description;
           break;
       default:
           action_description = NULL;
           break;
     }
   return action_description;
}

/**
 * @brief Sets a description of the specified action of the object
 *
 * Implementation of set_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 * @param description action's description to set
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_slideshow_description_set(AtkAction *action, gint i,
                               const char *description)
{
   EailSlideshow *slideshow;
   gchar **value;

   g_return_val_if_fail(EAIL_IS_SLIDESHOW(action), FALSE);

   slideshow = EAIL_SLIDESHOW(action);
   switch (i)
     {
       case 0:
           value = &slideshow->next_description;
           break;
       case 1:
           value = &slideshow->previous_description;
           break;
       case 2:
           value = &slideshow->stop_description;
           break;
       case 3:
           value = &slideshow->start_description;
           break;
       default:
           value = NULL;
           break;
     }
   if (value)
     {
        free(*value);
        *value = g_strdup(description);
        return TRUE;
     }
   return FALSE;
}

/**
 * @brief AtkAction interface initialization
 *
 * @param iface AtkActionIface instance
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->do_action = eail_slideshow_do_action;
   iface->get_n_actions = eail_slideshow_n_actions_get;
   iface->get_description = eail_slidshow_description_get;
   iface->set_description = eail_slideshow_description_set;
   iface->get_name = eail_slideshow_action_name_get;
   /*unsupported*/
   /*iface->get_localized_name;*/
   /*iface->get_keybinding;*/
}
