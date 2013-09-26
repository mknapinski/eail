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
 * @file eail_factory.h
 *
 * @brief Header for EailFactory implementation
 */

#ifndef EAIL_FACTORY_H
#define EAIL_FACTORY_H

#include <atk/atk.h>

#include <Evas.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for EailFactory cache entry*/
typedef struct _EailFactoryObj EailFactoryObj;

/** @brief Definition of object structure for EailFactory cache entry*/
struct _EailFactoryObj
{
   AtkObject *atk_obj;/**< @brief AtkObject representation*/
   Evas_Object *evas_obj;/**< @brief Evas_Object that needs atk representation*/
   Elm_Object_Item *obj_item;/**< @brief Elm_Object_Item that needs atk representation*/
};

/**
 * @brief Creates an accessible EAIL widget that represents the given Evas_Object*
 */
AtkObject * eail_factory_get_accessible    (Evas_Object *widget);

/**
 * @brief Returns an instance of AtkObject for the given item
 */
AtkObject * eail_factory_get_item_atk_obj  (Elm_Object_Item *item,
                                            AtkRole role,
                                            AtkObject *parent);

/**
 * @brief Creates an EailFactoryObj instance for the given item and adds it to cache
 */
void eail_factory_append_item_to_cache     (AtkObject *atk_obj,
                                            Elm_Object_Item *item);
/**
 * @brief Tries to find an AtkObject representing the given Elm_Object_Item * in cache
 */
EailFactoryObj *eail_factory_find_obj_for_item (const Elm_Object_Item *item);

/**
 * @brief Unregisters a widget and associated AtkObject* representation from
 * factory cache
 */
void eail_factory_unregister_wdgt_from_cache(Evas_Object *widget);

/**
 * @brief Unregisters an item and associated AtkObject* representation from
 * factory cache
 */
void eail_factory_unregister_item_from_cache(Elm_Object_Item *item);
Eina_List * eail_factory_find_objects_with_role(AtkRole role);

#ifdef __cplusplus
}
#endif

#endif
