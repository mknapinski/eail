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
 * @file eail_clock.h
 *
 * @brief Header for EailClock implementation
 */

#ifndef EAIL_CLOCK_H
#define EAIL_CLOCK_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailClock class
 */
#define EAIL_TYPE_CLOCK             (eail_clock_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailClock
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_CLOCK(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                     EAIL_TYPE_CLOCK, EailClock))
/**
 * @brief Macro upcasts a subclass (klass) to the EailClock class
 *
 * @param klass subclass object
 */
#define EAIL_CLOCK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                     EAIL_TYPE_CLOCK, EailClockClass))
/**
 * @brief Tests whether object (obj) is an instance of EailClock class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_CLOCK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                     EAIL_TYPE_CLOCK))
/**
 * @brief Tests whether given klass is a subclass of EailClock
 *
 * @param klass klass object
 */
#define EAIL_IS_CLOCK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                     EAIL_TYPE_CLOCK))
/**
 * @brief Gets EailClock class structure from an obj (class instance)
 *
 *  @param obj object instance to get EailClock class from
 */
#define EAIL_CLOCK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                     EAIL_TYPE_CLOCK, EailClockClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailClock*/
typedef struct _EailClock      EailClock;
/** @brief Definition of class structure for Atk EailClock*/
typedef struct _EailClockClass EailClockClass;

/** @brief Definition of object structure for Atk EailClock*/
struct _EailClock
{
   /** @brief Parent widget whose functionality is being extended */
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailClock*/
struct _EailClockClass
{
   /** @brief parent AtkObject that functionality is being extended */
   EailWidgetClass parent_class;
};

/**
 * @brief Getter for EailClock GType
 *
 * @returns GType for EailClock implementation
 * */
GType eail_clock_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
