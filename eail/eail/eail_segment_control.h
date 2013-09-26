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

#ifndef EAIL_SEGMENT_CONTROL_H
#define EAIL_SEGMENT_CONTROL_H

/**
 * @file eail_segment_control.h
 *
 * @brief Header for EailSegmentControl implementation
 */

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailSegmentControl class
 */
#define EAIL_TYPE_SEGMENT_CONTROL               (eail_segment_control_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailSegmentControl
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_SEGMENT_CONTROL(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                                EAIL_TYPE_SEGMENT_CONTROL, \
                                                EailSegmentControl))
/**
 * @brief Macro upcasts a subclass (klass) to the EailSegmentControl class
 *
 * @param klass subclass object
 */
#define EAIL_SEGMENT_CONTROL_CLASS(klass)       (G_TYPE_SEGMENT_CONTROL_CLASS_CAST((klass), \
                                                EAIL_TYPE_SECTOR_CONTROL, \
                                                EailSegmentControlClass))
/**
 * @brief Tests whether object (obj) is an instance of EailSegmentControl class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_SEGMENT_CONTROL(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                                EAIL_TYPE_SEGMENT_CONTROL))
/**
 * @brief Tests whether given klass is a subclass of EailSegmentControl
 *
 * @param klass klass object
 */
#define EAIL_IS_SEGMENT_CONTROL_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                                EAIL_TYPE_SEGMENT_CONTROL))
/**
 * @brief Gets EailSegmentControl class structure from an obj (class instance)
 *
 * @param obj object instance to get EailSegmentControl class from
 */
#define EAIL_SEGMENT_CONTROL_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                                EAIL_TYPE_SEGMENT_CONTROL, \
                                                EailSegmentControlClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailSegmentControl*/
typedef struct _EailSegmentControl EailSegmentControl;
/** @brief Definition of object class for Atk EailSegmentControl*/
typedef struct _EailSegmentControlClass EailSegmentControlClass;

/** @brief Definition of object structure for Atk EailSegmentControl*/
struct _EailSegmentControl
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailSegmentControl*/
struct _EailSegmentControlClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailSegmentControl GType
 *
 * @returns GType for EailSegmentControl implementation*/
GType eail_segment_control_get_type(void);

#ifdef __cplusplus
}
#endif

#endif

