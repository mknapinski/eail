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
 * @file eail_separator.c
 * @brief EailSeparator implementation
 */

#include <Elementary.h>
#include "eail_separator.h"

/**
 * @brief EailSeparator type definition
 */
G_DEFINE_TYPE(EailSeparator, eail_separator, EAIL_TYPE_WIDGET);

/**
 * @brief Gets the state set of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param accessible AtkObject instance
 * @return AtkStateSet representing the state set of the accessible
 */
static AtkStateSet*
eail_separator_ref_state_set(AtkObject *accessible)
{
   AtkStateSet *state_set;
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(accessible));
   if (!widget) return NULL;

   state_set = ATK_OBJECT_CLASS(eail_separator_parent_class)->ref_state_set(accessible);

   if (elm_separator_horizontal_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_HORIZONTAL);
   else
     atk_state_set_add_state(state_set, ATK_STATE_VERTICAL);

   return state_set;
}

/**
 * @brief EailSeparator initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_separator_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_separator_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_SEPARATOR;
}

/**
 * @brief EailSeparator interface initializer
 *
 * @param separator EailSeparator instance
 */
static void
eail_separator_init(EailSeparator *separator)
{
}

/**
 * @brief EailSeparator class initializer
 *
 * @param klass EailSeparatorClass instance
 */
static void
eail_separator_class_init(EailSeparatorClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   atk_class->initialize = eail_separator_initialize;
   atk_class->ref_state_set = eail_separator_ref_state_set;
}
