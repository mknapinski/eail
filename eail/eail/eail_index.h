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
 * @file eail_index.h
 *
 * @brief Header for EailIndex implementation
 */

#ifndef EAIL_INDEX_H
#define EAIL_INDEX_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailIndex class
 */
#define EAIL_TYPE_INDEX              (eail_index_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailIndex
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_INDEX(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      EAIL_TYPE_INDEX, EailIndex))
/**
 * @brief Macro upcasts a subclass (klass) to the EailIndex class
 *
 * @param klass subclass object
 */
#define EAIL_INDEX_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                      EAIL_TYPE_INDEX, EailIndexClass))
/**
 * @brief Tests whether object (obj) is an instance of EailIndex class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_INDEX(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      EAIL_TYPE_INDEX))
/**
 * @brief Tests whether given klass is a subclass of EailIndex
 *
 * @param klass klass object
 */
#define EAIL_IS_INDEX_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                      EAIL_TYPE_INDEX))
/**
 * @brief Gets EailIndex class structure from an obj (class instance)
 *
 * @param obj object instance to get EailIndex class from
 */
#define EAIL_INDEX_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                      EAIL_TYPE_INDEX, EailIndexClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailIndex*/
typedef struct _EailIndex      EailIndex;
/** @brief Definition of class structure for Atk EailIndex*/
typedef struct _EailIndexClass EailIndexClass;

/** @brief Definition of object structure for Atk EailIndex*/
struct _EailIndex
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailIndex*/
struct _EailIndexClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailIndex GType
 * @returns GType for EailIndex implementation*/
GType eail_index_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
