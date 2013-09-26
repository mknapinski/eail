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
 * @file eail_item_parent.c
 * @brief Implementation of interface that is used by EailItem implementation
 * to allow special behavior of wide range item-objects
*/

#include "eail_item_parent.h"

/**
 * @brief Gets GType for initialized interface
 *
 * @returns GType for initialized interface
 */
GType
eail_item_parent_get_type(void)
{
   static volatile GType type_id__volatile = 0;

   if (g_once_init_enter(&type_id__volatile))
     {
        GType type_id = g_type_register_static_simple(
                                    G_TYPE_INTERFACE,
                                    "EailItemParent",
                                    sizeof(EailItemParentIface),
                                    NULL,
                                    0,
                                    NULL,
                                    0);

        g_once_init_leave(&type_id__volatile, type_id);
     }

   return type_id__volatile;
}

/**
 * @brief Gets the name of item
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 *
 * @return string representing the name of item or NULL if the name is not available
 */
const gchar *
eail_item_parent_get_item_name(EailItemParent   *parent,
                               EailItem         *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), NULL);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_item_name)
     return iface->get_item_name(parent, item);

   return NULL;
}

/**
 * @brief Gets the role of item
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem object
 *
 * @returns AtkRole representing the role of the specified EailItem
 */
AtkRole
eail_item_parent_get_item_role(EailItemParent   *parent,
                               EailItem         *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), ATK_ROLE_INVALID);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_item_role)
     return iface->get_item_role(parent, item);

   return ATK_OBJECT(item)->role;
}

/**
 * @brief Gets the index of item in parent object
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 *
 * @returns integer representing the index of item in parent object
 */
gint
eail_item_parent_get_item_index_in_parent(EailItemParent    *parent,
                                          EailItem          *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), -1);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_item_index_in_parent)
     return iface->get_item_index_in_parent(parent, item);

   return -1;
}

/**
 * @brief Gets the number of accessible children
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 *
 * @returns integer representing the number of accessible children or -1 if
 * not implemented
 */
gint
eail_item_parent_get_n_children(EailItemParent    *parent,
                                EailItem          *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), -1);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_n_children)
     return iface->get_n_children(parent, item);

   return -1;
}

/**
 * @brief Gets a reference to the specified accessible child of the object.
 *
 * The accessible children are 0-based so the first accessible child
 * is at index 0, the second at index 1 and so on.
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 * @param index gint representing index of child to ref
 *
 * @returns AtkObject representing the specified accessible child
 * or NULL if not implemented
 */
AtkObject *
eail_item_parent_ref_n_child(EailItemParent    *parent,
                             EailItem          *item,
                             gint index)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), NULL);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->ref_n_child)
     return iface->ref_n_child(parent, item, index);

   return NULL;
}

/**
 * @brief Gets a reference to the state set of EailItem
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 * @param state_set current state set
 *
 * @returns AtkStateSet representing the state set of given EailItem
 */
AtkStateSet *
eail_item_parent_ref_item_state_set(EailItemParent  *parent,
                                    EailItem        *item,
                                    AtkStateSet     *state_set)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), NULL);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->ref_item_state_set)
     return iface->ref_item_state_set(parent, item, state_set);

   return state_set;
}

/**
 * @brief Grabs focus of EailItem
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 *
 * @returns TRUE on success, FALSE otherwise
 */
gboolean
eail_item_parent_grab_item_focus(EailItemParent *parent,
                                 EailItem       *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), FALSE);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->grab_item_focus)
     return iface->grab_item_focus(parent, item);

   return FALSE;
}

/**
 * @brief Gets the rectangle which gives the extent of the component
 *
 * Implementation of atk_component_get_extents.
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem object
 * @param [out] x x coordinate
 * @param [out] y y coordinate
 * @param [out] width width of the rectangle
 * @param [out] height height of the rectangle
 * @param coord_type specifies whether the coordinates are relative to the
 * screen or to the components top level window
 */
void
eail_item_parent_get_item_extents(EailItemParent    *parent,
                                  EailItem          *item,
                                  gint                *x,
                                  gint                *y,
                                  gint                *width,
                                  gint                *height,
                                  AtkCoordType         coord_type)
{
   EailItemParentIface *iface;

   g_return_if_fail(EAIL_IS_ITEM_PARENT(parent));

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_item_extents)
     iface->get_item_extents(parent, item, x, y, width, height, coord_type);
}

/**
 * @brief Gets nested Evas_Object of given EailItem
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 *
 * @returns nested Evas_Object representing the given EailItem
 */
Evas_Object * eail_item_parent_get_evas_obj(EailItemParent   *parent,
                                            EailItem         *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), NULL);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_evas_obj)
     return iface->get_evas_obj(parent, item);

   return NULL;
}

/**
 * @brief Gets supported actions
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 *
 * @returns integer representing supported actions
 */
gint
eail_item_parent_get_actions_supported(EailItemParent *parent,
                                       EailItem       *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), FALSE);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_actions_supported)
     return iface->get_actions_supported(parent, item);

   return EAIL_ACTION_SUPPORTED_NONE;
}

/**
 * @brief Checks if content get is supported
 *
 * @param parent object that holds EailItem in its content
 * @param item EailItem instance
 *
 * @returns TRUE if content get is supported, FALSE otherwise. Default
 * implementation returns TRUE (used if no redefinition in EailItemParent
 * interface implementation is defined)
 */
gboolean
eail_item_parent_is_is_content_get_supported(EailItemParent *parent,
                                             EailItem       *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), FALSE);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->is_content_get_supported)
     return iface->is_content_get_supported(parent, item);

   return TRUE;
}
