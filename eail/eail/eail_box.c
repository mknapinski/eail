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
 * @file eail_box.c
 * @brief EailBox implementation
 */

#include <Elementary.h>

#include "eail_box.h"
#include "eail_priv.h"
#include "eail_utils.h"
#include "eail_factory.h"

/**
 * @brief EailBox type definition
 */
G_DEFINE_TYPE(EailBox, eail_box, EAIL_TYPE_WIDGET);

/**
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Handler for realized event, used to notify about box content
 * changes
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info (reference toobject added to box)
 */
void
eail_box_children_add_event(void *data,
                            Evas_Object *obj,
                            void *event_info)
{
   AtkObject *atk_parent = NULL;

   atk_parent = ATK_OBJECT(data);
   if (!atk_parent) return;

   eail_emit_children_changed_obj(TRUE, atk_parent, NULL);
}

/**
 * @brief Handler for realized event, used to notify about box content
 * changes
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info (reference toobject added to box)
 */
void
eail_box_children_rm_event(void *data,
                            Evas_Object *obj,
                            void *event_info)
{
   AtkObject *atk_parent = NULL;

   atk_parent = ATK_OBJECT(data);
   if (!atk_parent) return;

   eail_emit_children_changed_obj(FALSE, atk_parent, NULL);
}

/**
 * @brief EailBox initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_box_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_box_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_FILLER;
   g_return_if_fail(EAIL_IS_WIDGET(obj));

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget)
     {
        ERR("No evas object inside EailWidget was found");
        return;
   }

   evas_object_smart_callback_add(nested_widget, "child,added",
                                  eail_box_children_add_event, obj);
   evas_object_smart_callback_add(nested_widget, "child,removed",
                                  eail_box_children_rm_event, obj);
}

/**
 * @brief Gets widget's children
 *
 * @param widget EailWidget instance
 * @return Eina_List containing widget's children
 */
static Eina_List *
eail_box_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *obj = eail_widget_get_widget(widget);

   if (obj)
     list = elm_box_children_get(obj);

   return list;
}

/**
 * @brief EailBox instance initializer
 *
 * @param box EailBox instance
 */
static void
eail_box_init(EailBox *box)
{
}

/**
 * @brief EailBox class initializer
 *
 * @param klass EailBoxClass instance
 */
static void
eail_box_class_init(EailBoxClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_box_get_widget_children;

   class->initialize = eail_box_initialize;
}
