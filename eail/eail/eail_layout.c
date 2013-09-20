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
 * @file eail_layout.c
 *
 * @brief Implementation of Layout widget
*/

#include <Elementary.h>

#include "eail_layout.h"
#include "eail_dynamic_content.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void eail_dynamic_content_interface_init(EailDynamicContentIface *iface);

/**
 * @brief Define EailLayout GObject type
 *
 * It extends EAIL_TYPE_WIDGET
 */
G_DEFINE_TYPE_WITH_CODE(EailLayout, eail_layout, EAIL_TYPE_WIDGET,
                    G_IMPLEMENT_INTERFACE(EAIL_TYPE_DYNAMIC_CONTENT,
                                          eail_dynamic_content_interface_init));

/**
 * @brief Gets elementary widgets from layout edje object
 * Iterates over edje smart objects and if object is elementary widget
 * then it is append to the list
 *
 * @param edje layout edje object
 *
 * @returns list of Elementary widgets
 */
static Eina_List *
_get_widgets_from_edje(const Evas_Object *edje)
{
   Evas_Object *o;
   Eina_List *widgets, *l, *list = evas_object_smart_members_get(edje);

   widgets = NULL;
   EINA_LIST_FOREACH(list, l, o)
     {
        if (elm_object_widget_check(o))
          widgets = eina_list_append(widgets, o);
     }

   return widgets;
}

/**
 * @brief Implementations of get_widget_children from EailWidget
 *
 * @param object EailLayout instance
 *
 * @returns list of Evas_Objects
 */
static Eina_List*
eail_layout_widget_children_get(EailWidget *object)
{
   Evas_Object *edje;
   Evas_Object *widget = eail_widget_get_widget(object);

   if (!widget) return NULL;
   edje = elm_layout_edje_get(widget);

   return _get_widgets_from_edje(edje);
}

/**
 * @brief Initialize EailLayout object
 *
 * @param object EailLayout instance
 * @param data user data passed to initialization
 */
static void
eail_layout_initialize(AtkObject *object, gpointer data)
{
   ATK_OBJECT_CLASS(eail_layout_parent_class)->initialize(object, data);
   EailLayout *eail_layout = NULL;
   object->role = ATK_ROLE_FILLER;

   if (!EAIL_IS_LAYOUT(object))
     {
        DBG("Not EAIL_LAYOUT!. Returning");
        return;
     }

   /* storing last numbers of children to be for checking if children-changed
    * signal has to be propagated */
   eail_layout = EAIL_LAYOUT(object);
   eail_layout->child_count_last = atk_object_get_n_accessible_children(object);
}

/**
 * @brief Init EailLayout instance
 *
 * @param layout EailLayout instance
 */
static void
eail_layout_init(EailLayout *layout)
{
}

/**
 * @brief Init EailLayout class
 *
 * @param klass EailLayoutClass object
 */
static void
eail_layout_class_init(EailLayoutClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   class->initialize = eail_layout_initialize;
   widget_class->get_widget_children = eail_layout_widget_children_get;
}

/**
 * @param dynamic_content_holder an EailDynamicContent object (EailLayout)
 */
void
eail_layout_update_descendants(EailDynamicContent *dynamic_content_holder)
{
   gint n_children = 0;
   EailLayout *layout = NULL;
   if (!EAIL_IS_LAYOUT(dynamic_content_holder))
     {
        DBG("No EailLayout found. Returning");
        return;
     }

   layout = EAIL_LAYOUT(dynamic_content_holder);

   n_children = atk_object_get_n_accessible_children(ATK_OBJECT(layout));
   if (n_children && n_children > layout->child_count_last)
     {
        eail_emit_children_changed(TRUE, ATK_OBJECT(layout), n_children - 1);
     }
   else if (n_children < layout->child_count_last)
     {
         eail_emit_children_changed
                     (FALSE, ATK_OBJECT(layout), layout->child_count_last);
     }

   layout->child_count_last = n_children;
}

/**
 * @brief Initializer for dynamic content interface, used for handling objects
 * children hierarchy changes
 *
 * @param iface an EailDynamicContentIface
 */
static void
eail_dynamic_content_interface_init(EailDynamicContentIface *iface)
{
   iface->update_hierarchy = eail_layout_update_descendants;
}
