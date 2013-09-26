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
 * @file eail_frame.c
 * @brief EailFrame implementation
 */

#include <Elementary.h>
#include "eail_frame.h"

/**
 * @brief EailFrame type definition
 */
G_DEFINE_TYPE(EailFrame, eail_frame, EAIL_TYPE_WIDGET);

/**
 * @brief EailFrame initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_frame_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_frame_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_FRAME;
}

/**
 * @brief Gets widget's children
 *
 * @param widget EailWidget instance
 * @return Eina_List representing the list of widget's children
 */
static Eina_List *
eail_frame_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *content;
   Evas_Object *obj;

   obj = eail_widget_get_widget(EAIL_WIDGET(widget));
   if (obj)
     {
        content = elm_object_part_content_get(obj, "default");
        if (content)
          list = eina_list_append(list, content);
     }

   return list;
}

/**
 * @brief Gets obj's name
 *
 * @param obj AtkObject instance
 * @return string representing obj's name
 */
static const gchar*
eail_frame_get_name(AtkObject *obj)
{
   const gchar *name;
   Evas_Object *widget = NULL;

   name = ATK_OBJECT_CLASS(eail_frame_parent_class)->get_name(obj);
   if (NULL != name)
     return name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (widget)
     name = (const gchar*)elm_object_text_get(widget);

   return name;
}

/**
 * @brief EailFrame instance initializer
 *
 * @param frame EailFrame instance
 */
static void
eail_frame_init(EailFrame *frame)
{
}

/**
 * @brief EailFrame class initializer
 *
 * @param klass EailFrameClass instance
 */
static void
eail_frame_class_init(EailFrameClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_frame_get_widget_children;

   class->initialize = eail_frame_initialize;
   class->get_name = eail_frame_get_name;
}
