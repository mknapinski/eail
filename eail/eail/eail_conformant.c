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
 * @file eail_conformant.c
 * @brief EailConformant implementation
 */

#include <Elementary.h>

#include "eail_conformant.h"

/**
 * @brief EailConformant type definition
 */
G_DEFINE_TYPE(EailConformant, eail_conformant, EAIL_TYPE_LAYOUT);

/**
 * @brief EailConformant initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_conformant_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_conformant_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_FILLER;
}

/**
 * @brief EailConformant instance initializer
 *
 * @param conformant EailConformant instance
 */
static void
eail_conformant_init(EailConformant *conformant)
{
}

/**
 * @brief EailConformant class initializer
 *
 * @param klass EailConformantClass instance to be filled
 */
static void
eail_conformant_class_init(EailConformantClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_conformant_initialize;
}
