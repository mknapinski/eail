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
 * @file eail_dayselector.h
 *
 * @brief Header for EailDayselector implementation
 */

#ifndef EAIL_DAYSELECTOR_H
#define EAIL_DAYSELECTOR_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailDayselector class
 */
#define EAIL_TYPE_DAYSELECTOR             (eail_dayselector_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailDayselector
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_DAYSELECTOR(obj)             (G_TYPE_DAYSELECTOR_INSTANCE_CAST((obj), \
                                           EAIL_TYPE_DAYSELECTOR, EailDayselector))
/**
 * @brief Macro upcasts a subclass (klass) to the EailDayselector class
 *
 * @param klass subclass object
 */
#define EAIL_DAYSELECTOR_CLASS(klass)     (G_TYPE_DAYSELECTOR_CLASS_CAST((klass), \
                                           EAIL_TYPE_DAYSELECTOR, EailDayselectorClass))
/**
 * @brief Tests whether object (obj) is an instance of EailDayselector class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_DAYSELECTOR(obj)          (G_TYPE_DAYSELECTOR_INSTANCE_TYPE((obj), \
                                           EAIL_TYPE_DAYSELECTOR))
/**
 * @brief Tests whether given klass is a subclass of EailDayselector
 *
 * @param klass klass object
 */
#define EAIL_IS_DAYSELECTOR_CLASS(klass)  (G_TYPE_DAYSELECTOR_CLASS_TYPE((klass), \
                                           EAIL_TYPE_DAYSELECTOR))
/**
 * @brief Gets EailDayselector class structure from an obj (class instance)
 *
 * @param obj object instance to get EailDayselector class from
 */
#define EAIL_DAYSELECTOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                           EAIL_TYPE_DAYSELECTOR, EailDayselectorClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailDayselector*/
typedef struct _EailDayselector      EailDayselector;
/** @brief Definition of class structure for Atk EailDayselector*/
typedef struct _EailDayselectorClass EailDayselectorClass;

/** @brief Definition of object structure for Atk EailDayselector*/
struct _EailDayselector
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailDayselector*/
struct _EailDayselectorClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailDayselector GType
 * @returns GType for EailDayselector implementation*/
GType eail_dayselector_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
