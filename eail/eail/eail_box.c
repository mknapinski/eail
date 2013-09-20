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

/**
 * @brief EailBox type definition
 */
G_DEFINE_TYPE(EailBox, eail_box, EAIL_TYPE_WIDGET);

/**
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailBox initializer
 *
 * @param obj an AtkObject
 * @param data initialization data
 */
static void
eail_box_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_box_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_FILLER;
}

/**
 * @brief Gets widget children
 *
 * @param widget an EailWidget
 * @return widget children list
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
 * @param box an EailBox
 */
static void
eail_box_init(EailBox *box)
{
}

/**
 * @brief EailBox class initializer
 *
 * @param klass an EailBoxClass
 */
static void
eail_box_class_init(EailBoxClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_box_get_widget_children;

   class->initialize = eail_box_initialize;
}
