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
 * @file eail_glview.c
 * @brief EailGlview implementation
 */

#include <Elementary.h>
#include "eail_glview.h"

/**
 * @brief EailGlview type definition
 */
G_DEFINE_TYPE(EailGlview, eail_glview, EAIL_TYPE_WIDGET);

/**
 * @brief EailGlview initializer.
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_glview_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_glview_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_ANIMATION;
}

/**
 * @brief EailGlview interface initializer.
 *
 * @param glview EailGlview instance
 * */
static void
eail_glview_init(EailGlview *glview)
{
}

/**
 * @brief EailGlview class initializer.
 *
 * @param klass EailGlviewClass instance
 */
static void
eail_glview_class_init(EailGlviewClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   atk_class->initialize = eail_glview_initialize;
}
