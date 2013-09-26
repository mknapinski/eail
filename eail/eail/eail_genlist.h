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
 * @file eail_genlist.h
 *
 * @brief Header for EailGenlist implementation
 */

#ifndef EAIL_GENLIST_H
#define EAIL_GENLIST_H

#include "eail_scrollable_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailGenlist class
 */
#define EAIL_TYPE_GENLIST             (eail_genlist_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailGenlist
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_GENLIST(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                       EAIL_TYPE_GENLIST, EailGenlist))
/**
 * @brief Macro upcasts a subclass (klass) to the EailGenlist class
 *
 * @param klass subclass object
 */
#define EAIL_GENLIST_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                       EAIL_TYPE_GENLIST, EailGenlistClass))
/**
 * @brief Tests whether object (obj) is an instance of EailGenlist class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_GENLIST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                       EAIL_TYPE_GENLIST))
/**
 * @brief Tests whether given klass is a subclass of EailGenlist
 *
 * @param klass klass object
 */
#define EAIL_IS_GENLIST_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                       EAIL_TYPE_GENLIST))
/**
 * @brief Gets EailGenlist class structure from an obj (class instance)
 *
 * @param obj object instance to get EailGenlist class from
 */
#define EAIL_GENLIST_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                       EAIL_TYPE_GENLIST, EailGenlistClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailGenlist*/
typedef struct _EailGenlist      EailGenlist;
/** @brief Definition of class structure for Atk EailGenlist*/
typedef struct _EailGenlistClass EailGenlistClass;

/** @brief Definition of object structure for Atk EailGenlist*/
struct _EailGenlist
{
   /** @brief Parent widget whose functionality is being extended*/
   EailScrollableWidget parent;
};

/** @brief Definition of class structure for Atk EailGenlist*/
struct _EailGenlistClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailScrollableWidgetClass parent_class;
};

/** @brief Getter for EailGenlist GType
 * @returns GType for EailGenlist implementation*/
GType eail_genlist_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
