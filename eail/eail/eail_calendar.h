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
 * @file eail_calendar.h
 *
 * @brief Header for EailCalendar implementation
 */

#ifndef EAIL_CALENDAR_H
#define EAIL_CALENDAR_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailCalendar class
 */
#define EAIL_TYPE_CALENDAR              (eail_calendar_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailCalendar
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_CALENDAR(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                         EAIL_TYPE_CALENDAR, EailCalendar))
/**
 * @brief Macro upcasts a subclass (klass) to the EailCalendar class
 *
 * @param klass subclass object
 */
#define EAIL_CALENDAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                         EAIL_TYPE_CALENDAR, EailCalendarClass))
/**
 * @brief Tests whether object (obj) is an instance of EailCalendar class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_CALENDAR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                         EAIL_TYPE_CALENDAR))
/**
 * @brief Tests whether given klass is a subclass of EailCalendar
 *
 * @param klass klass object
 */
#define EAIL_IS_CALENDAR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                         EAIL_TYPE_CALENDAR))
/**
 * @brief Gets EailCalendar class structure from an obj (class instance)
 *
 * @param obj object instance to get EailCalendar class from
 */
#define EAIL_CALENDAR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                         EAIL_TYPE_CALENDAR, EailCalendarClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailCalendar*/
typedef struct _EailCalendar      EailCalendar;
/** @brief Definition of class structure for Atk EailCalendar*/
typedef struct _EailCalendarClass EailCalendarClass;

/** @brief Definition of object structure for Atk EailCalendar*/
struct _EailCalendar
{
   EailWidget parent; /*!< @brief Parent widget whose functionality is being extended*/
};

/** @brief Definition of class structure for Atk EailCalendar*/
struct _EailCalendarClass
{
   /** @brief parent class whose functionality is being extended */
   EailWidgetClass parent_class;
};

/**
 * @brief Getter for EailCalendar GType
 *
 * @returns GType for EailCalendar implementation
 * */
GType eail_calendar_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
