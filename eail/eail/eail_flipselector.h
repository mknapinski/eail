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
 * @file eail_flipselector.h
 *
 * @brief Header for EailFlipselector implementation
 */

#ifndef EAIL_FLIPSELECTOR_H
#define EAIL_FLIPSELECTOR_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailFlipselector class
 */
#define EAIL_TYPE_FLIPSELECTOR              (eail_flipselector_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailFlipselector
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_FLIPSELECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                             EAIL_TYPE_FLIPSELECTOR, EailFlipselector))
/**
 * @brief Macro upcasts a subclass (klass) to the EailFlipselector class
 *
 * @param klass subclass object
 */
#define EAIL_FLIPSELECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                             EAIL_TYPE_FLIPSELECTOR, EailFlipselectorClass));
/**
 * @brief Tests whether object (obj) is an instance of EailFlipselector class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_FLIPSELECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                             EAIL_TYPE_FLIPSELECTOR))
/**
 * @brief Tests whether given klass is a subclass of EailFlipselector
 *
 * @param klass klass object
 */
#define EAIL_IS_FLIPSELECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                             EAIL_TYPE_FLIPSELECTOR))
/**
 * @brief Gets EailFlipselector class structure from an obj (class instance)
 *
 * @param obj object instance to get EailFlipselector class from
 */
#define EAIL_FLIPSELECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                             EAIL_TYPE_FLIPSELECTOR, EailFlipselectorClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailFlipselector*/
typedef struct _EailFlipselector      EailFlipselector;
/** @brief Definition of class structure for Atk EailFlipselector*/
typedef struct _EailFlipselectorClass EailFlipselectorClass;

/** @brief Definition of object structure for Atk EailFlipselector*/
struct _EailFlipselector
{
   EailWidget parent;/**< @brief widget that functionality is being extended*/

   char *next_description;/**< @brief 'next' action description string */
   char *prev_description;/**< @brief 'prev' action description string */
};

/** @brief Definition of class structure for Atk EailFlipselector*/
struct _EailFlipselectorClass
{
   EailWidgetClass parent_class;/**< @brief class that is being extended*/
};

/**
 * @brief Getter for EailFlipselector GType
 * @returns GType for EailFlipselector implementation
 */
GType eail_flipselector_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
