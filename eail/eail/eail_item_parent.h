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
 * @file eail_item_parent.h
 *
 * @brief Header for EailItemParent implementation
 */

#ifndef EAIL_ITEM_PARENT_H
#define EAIL_ITEM_PARENT_H

#include "eail_item.h"
/**
 * @brief Returns a value corresponding to the type of EailItemParent interface
 */
#define EAIL_TYPE_ITEM_PARENT           (eail_item_parent_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailItemParent
 * interface
 *
 * @param obj AtkObject instance
 */
#define EAIL_ITEM_PARENT(obj)           (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                         EAIL_TYPE_ITEM_PARENT, EailItemParent))
/**
 * @brief Tests whether object (obj) implements EailItemParent interface
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_ITEM_PARENT(obj)        (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                         EAIL_TYPE_ITEM_PARENT))
/**
 * @brief Gets EailItemParent interface structure from an obj (class instance)
 *
 * @param obj object instance to get EailItemParent interface from
 */
#define EAIL_ITEM_PARENT_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE((obj),\
                                         EAIL_TYPE_ITEM_PARENT, EailItemParentIface))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailItemParent*/
typedef struct _EailItemParent      EailItemParent;
/** @brief Definition of interface structure for Atk EailItemParent*/
typedef struct _EailItemParentIface EailItemParentIface;

/** @brief Definition of interface structure for Atk EailItemParent*/
struct _EailItemParentIface
{
   GTypeInterface parent;/**< @brief Parent interface that is being extended*/

   /**@brief Callback definition for eail_item_parent_get_item_name function*/
   const gchar *(*get_item_name)            (EailItemParent   *parent,
                                            EailItem         *item);

   /**@brief Callback definition for eail_item_parent_get_item_role function*/
   AtkRole      (*get_item_role)            (EailItemParent   *parent,
                                            EailItem         *item);

   /**@brief Callback definition for eail_item_parent_get_index_in_parent function*/
   gint         (*get_item_index_in_parent) (EailItemParent   *parent,
                                            EailItem         *item);

   /**@brief Callback definition for eail_item_parent_get_n_children function*/
   gint         (*get_n_children)           (EailItemParent   *parent,
                                            EailItem         *item);

   /**@brief Callback definition for eail_item_parent_ref_n_child function*/
   AtkObject *  (*ref_n_child)              (EailItemParent   *parent,
                                            EailItem         *item,
                                            gint             index);

   /**@brief Callback definition for eail_item_parent_ref_item_state_set function*/
   AtkStateSet *
                (*ref_item_state_set)       (EailItemParent   *parent,
                                            EailItem         *item,
                                            AtkStateSet      *state_set);

   /* AtkComponent interface */
   /**@brief Callback definition for eail_item_parent_grab_item_focus function*/
   gboolean    (*grab_item_focus)          (EailItemParent   *parent,
                                            EailItem         *item);

   /**@brief Callback definition for eail_item_parent_get_item_extents function*/
   void        (*get_item_extents)         (EailItemParent   *parent,
                                            EailItem         *item,
                                            gint             *x,
                                            gint             *y,
                                            gint             *width,
                                            gint             *height,
                                            AtkCoordType      coord_type);

   /**@brief Callback definition for eail_item_parent_get_evas_obj function*/
   Evas_Object * (*get_evas_obj)           (EailItemParent   *parent,
                                            EailItem         *item);

   /**@brief Callback definition for eail_item_parent_get_actions_supported
    * function*/
   gboolean    (*get_actions_supported)       (EailItemParent   *parent,
                                               EailItem         *item);

   /**@brief Callback definition for eail_item_parent_is_content_get_supported
    * function*/
   gboolean    (*is_content_get_supported) (EailItemParent   *parent,
                                            EailItem         *item);
};

/** @brief Enum that is used for representing supported actions by item*/
enum EailActionSupported
{
    EAIL_ACTION_SUPPORTED_NONE = 1 << 0,
    EAIL_ACTION_SUPPORTED_CLICK = 1 << 1,
    EAIL_ACTION_SUPPORTED_PRESS = 1 << 2,
    EAIL_ACTION_SUPPORTED_RELEASE = 1 << 3,
    EAIL_ACTION_SUPPORTED_EXPAND = 1 << 4,
    EAIL_ACTION_SUPPORTED_SHRINK = 1 << 5
};


GType eail_item_parent_get_type(void);
const gchar * eail_item_parent_get_item_name(EailItemParent *parent,
                                             EailItem *item);
AtkRole eail_item_parent_get_item_role(EailItemParent *parent,
                                       EailItem *item);
gint eail_item_parent_get_n_children(EailItemParent *parent,
                                     EailItem *item);
AtkObject * eail_item_parent_ref_n_child(EailItemParent *parent,
                                         EailItem *item,
                                         gint index);
gint eail_item_parent_get_item_index_in_parent(EailItemParent *parent,
                                               EailItem *item);
AtkStateSet * eail_item_parent_ref_item_state_set(EailItemParent *parent,
                                                  EailItem *item,
                                                  AtkStateSet *state_set);
gboolean eail_item_parent_grab_item_focus(EailItemParent *parent,
                                          EailItem *item);
void eail_item_parent_get_item_extents(EailItemParent *parent,
                                       EailItem *item,
                                       gint *x,
                                       gint *y,
                                       gint *width,
                                       gint *height,
                                       AtkCoordType coord_type);
gboolean eail_item_parent_get_actions_supported(EailItemParent *parent,
                                                EailItem *item);
gboolean eail_item_parent_is_is_content_get_supported(EailItemParent *parent,
                                                      EailItem *item);
Evas_Object * eail_item_parent_get_evas_obj(EailItemParent *parent,
                                            EailItem *item);

#ifdef __cplusplus
}
#endif

#endif
