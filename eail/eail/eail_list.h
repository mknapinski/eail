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
 * @file eail_list.h
 *
 * @brief Header for EailList implementation
 */

#ifndef EAIL_LIST_H
#define EAIL_LIST_H

#include "eail_scrollable_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailList class
 */

#define EAIL_TYPE_LIST            (eail_list_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailList
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_LIST(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                   EAIL_TYPE_LIST, EailList))
/**
 * @brief Macro upcasts a subclass (klass) to the EailList class
 *
 * @param klass subclass object
 */
#define EAIL_LIST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                   EAIL_TYPE_LIST, EailListClass))
/**
 * @brief Tests whether object (obj) is an instance of EailList class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_LIST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                   EAIL_TYPE_LIST))
/**
 * @brief Tests whether given klass is a subclass of EailList
 *
 * @param klass klass object
 */
#define EAIL_IS_LIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                   EAIL_TYPE_LIST))
/**
 * @brief Gets EailList class structure from an obj (class instance)
 *
 * @param obj object instance to get EailList class from
 */
#define EAIL_LIST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                   EAIL_TYPE_LIST, EailListClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailList*/
typedef struct _EailList      EailList;
/** @brief Definition of object class for Atk EailList*/
typedef struct _EailListClass EailListClass;

/** @brief Definition of object structure for Atk EailList*/
struct _EailList
{
   /** @brief Parent widget whose functionality is being extended*/
   EailScrollableWidget parent;
};

/** @brief Definition of object class for Atk EailList*/
struct _EailListClass
{
   /** @brief Parent class whose functionality is being extended*/
  EailScrollableWidgetClass parent_class;
};

/** @brief Getter for EailList GType
 * @returns GType for EailList implementation*/
GType eail_list_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
