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
 * @file eail_item.h
 *
 * @brief Header for EailItem implementation
 */

#ifndef EAIL_ITEM_H
#define EAIL_ITEM_H

#include <atk/atk.h>
#include <Elementary.h>
/**
 * @brief Returns a value corresponding to the type of EailItem class
 */
#define EAIL_TYPE_ITEM            (eail_item_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailItem
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                   EAIL_TYPE_ITEM, EailItem))
/**
 * @brief Macro upcasts a subclass (klass) to the EailItem class
 *
 * @param klass subclass object
 */
#define EAIL_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                   EAIL_TYPE_ITEM, EailItemClass))
/**
 * @brief Tests whether object (obj) is an instance of EailItem class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                   EAIL_TYPE_ITEM))
/**
 * @brief Tests whether given klass is a subclass of EailItem
 *
 * @param klass klass object
 */
#define EAIL_IS_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                   EAIL_TYPE_ITEM))
/**
 * @brief Gets EailItem class structure from an obj (class instance)
 *
 * @param obj object instance to get EailItem class from
 */
#define EAIL_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                   EAIL_TYPE_ITEM, EailItemClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailItem*/
typedef struct _EailItem      EailItem;
/** @brief Definition of class structure for Atk EailItem*/
typedef struct _EailItemClass EailItemClass;

/** @brief Definition of object structure for Atk EailItem*/
struct _EailItem
{
   /** @brief Parent object that functionality is being extended*/
   AtkObject parent;

   /** @brief Elementary item that is being represented by EailItem accessible
    * object*/
   Elm_Object_Item *item;
};

/** @brief Definition of class structure for Atk EailItem*/
struct _EailItemClass
{
   /** @brief Parent class whose functionality is being extended*/
   AtkObjectClass parent_class;
};

/** @brief Getter for EailItem GType
 *
 * @returns GType for EailItem implementation*/
GType eail_item_get_type(void);

/** @brief Gets nested Elm_Object_Item * object*/
Elm_Object_Item *eail_item_get_item(EailItem *item);

#ifdef __cplusplus
}
#endif

#endif
