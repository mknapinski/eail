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

#ifndef EAIL_GRID_H
#define EAIL_GRID_H

/**
 * @file eail_grid.h
 *
 * @brief Header for EailGrid implementation
 */

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailGrid class
 */
#define EAIL_TYPE_GRID              (eail_grid_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailGrid
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_GRID(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                     EAIL_TYPE_GRID, EailGrid))
/**
 * @brief Macro upcasts a subclass (klass) to the EailGrid class
 *
 * @param klass subclass object
 */
#define EAIL_GRID_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                     EAIL_TYPE_GRID, EailGridClass))
/**
 * @brief Tests whether object (obj) is an instance of EailGrid class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_GRID(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                     EAIL_TYPE_GRID))
/**
 * @brief Tests whether given klass is a subclass of EailGrid
 *
 * @param klass klass object
 */
#define EAIL_IS_GRID_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                     EAIL_TYPE_GRID))
/**
 * @brief Gets EailGrid class structure from an obj (class instance)
 *
 * @param obj object instance to get EailGrid class from
 */
#define EAIL_GRID_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                     EAIL_TYPE_GRID, EailGridClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailGrid*/
typedef struct _EailGrid      EailGrid;
/** @brief Definition of class structure for Atk EailGrid*/
typedef struct _EailGridClass EailGridClass;

/** @brief Definition of object structure for Atk EailGrid*/
struct _EailGrid
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailGrid*/
struct _EailGridClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailGrid GType
 * @returns GType for EailGrid implementation*/
GType eail_grid_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
