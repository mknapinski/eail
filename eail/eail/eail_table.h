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
#ifndef EAIL_TABLE_H
#define EAIL_TABLE_H

/**
 * @file eail_table.h
 *
 * @brief Header for EailTable implementation
 */

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailTable class
 */
#define EAIL_TYPE_TABLE             (eail_table_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailTable
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_TABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                     EAIL_TYPE_TABLE, EailTable))

/**
 * @brief Macro upcasts a subclass (klass) to the EailTable class
 *
 * @param klass subclass object
 */
#define EAIL_TABLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                     EAIL_TYPE_TABLE, EailTableClass))

/**
 * @brief Tests whether object (obj) is an instance of EailTable class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_TABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                     EAIL_TYPE_TABLE))

/**
 * @brief Tests whether given klass is a subclass of EailTable
 *
 * @param klass klass object
 */
#define EAIL_IS_TABLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                     EAIL_TYPE_TABLE))

/**
 * @brief Gets EailTable class structure from an obj (class instance)
 *
 * @param obj object instance to get EailTable class from
 */
#define EAIL_TABLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                     EAIL_TYPE_TABLE, EailTableClass))
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailTable*/
typedef struct _EailTable      EailTable;
/** @brief Definition of object class for Atk EailTable*/
typedef struct _EailTableClass EailTableClass;

/** @brief Definition of object structure for Atk EailTable*/
struct _EailTable
{
   /** @brief Parent object that functionality is being extended*/
    EailWidget parent;
};

/** @brief Definition of object class for Atk EailTable*/
struct _EailTableClass
{
   /** @brief Parent object that functionality is being extended*/
    EailWidgetClass parent_class;
};

/** @brief Getter for EailTable GType
 *
 * @returns GType for EailTable implementation*/
GType eail_table_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
