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
 * @file eail_map.h
 *
 * @brief Header for EailMap implementation
 */
#ifndef EAIL_MAP_H
#define EAIL_MAP_H

#include "eail_image.h"
/**
 * @brief Returns a value corresponding to the type of EailMap class
 */
#define EAIL_TYPE_MAP             (eail_map_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailMap
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_MAP(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                   EAIL_TYPE_MAP, EailMap))
/**
 * @brief Macro upcasts a subclass (klass) to the EailMap class
 *
 * @param klass subclass object
 */
#define EAIL_MAP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                   EAIL_TYPE_MAP, EailMapClass))
/**
 * @brief Tests whether object (obj) is an instance of EailMap class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_MAP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                   EAIL_TYPE_MAP))
/**
 * @brief Tests whether given klass is a subclass of EailMap
 *
 * @param klass klass object
 */
#define EAIL_IS_MAP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                   EAIL_TYPE_MAP))
/**
 * @brief Gets EailMap class structure from an obj (class instance)
 *
 * @param obj object instance to get EailMap class from
 */
#define EAIL_MAP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                   EAIL_TYPE_MAP, EailMapClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailMap*/
typedef struct _EailMap       EailMap;
/** @brief Definition of object class for Atk EailMap*/
typedef struct _EailMapClass  EailMapClass;

/** @brief Definition of object structure for Atk EailMap*/
struct _EailMap
{
   EailImage parent;/**< @brief Parent widget whose functionality is being extended*/
   const char *name;/**< @brief Name string property for map*/
};

/** @brief Definition of object class for Atk EailMap*/
struct _EailMapClass
{
   EailImageClass parent_class;/**< @brief class that is being extended*/
};

/**
 * @brief Getter for EailMap GType
 * @returns GType for EailMap implementation
 */
GType eail_map_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
