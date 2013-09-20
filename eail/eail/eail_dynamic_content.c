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
 * @file eail_dynamic_content.c
 * @brief Implementation of EailDynamicContent - interface that is being used
 * for handling children hierarchy updates by eail components
*/

#include "eail_dynamic_content.h"

/**
 *
 * @returns GType for initialized interface
 */
GType
eail_dynamic_content_get_type(void)
{
   static volatile GType type_id__volatile = 0;

   if (g_once_init_enter(&type_id__volatile))
     {
        GType type_id = g_type_register_static_simple(
                                    G_TYPE_INTERFACE,
                                    "EailDynamicContent",
                                    sizeof(EailDynamicContentIface),
                                    NULL,
                                    0,
                                    NULL,
                                    0);

        g_once_init_leave(&type_id__volatile, type_id);
     }

   return type_id__volatile;
}

/**
 * @param parent object that suports EailDynamicContent interface
 *
 */
void eail_dynamic_content_update_hierarchy(EailDynamicContent *parent)
{
   EailDynamicContentIface *iface;

   g_return_if_fail(EAIL_IS_DYNAMIC_CONTENT(parent));

   iface = EAIL_DYNAMIC_CONTENT_GET_IFACE(parent);

   if (iface->update_hierarchy)
     iface->update_hierarchy(parent);

}
