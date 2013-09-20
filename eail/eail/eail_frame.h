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
 * @file eail_frame.h
 *
 * @brief Header for EailFrame implementation
 */

#ifndef EAIL_FRAME_H
#define EAIL_FRAME_H

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailFrame class
 */
#define EAIL_TYPE_FRAME              (eail_frame_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailFrame
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_FRAME(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      EAIL_TYPE_FRAME, EailFrame))
/**
 * @brief Macro upcasts a subclass (klass) to the EailFrame class
 *
 * @param klass subclass object
 */
#define EAIL_FRAME_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                      EAIL_TYPE_FRAME, EailFrameClass))
/**
 * @brief Tests whether object (obj) is an instance of EailFrame class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_FRAME(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      EAIL_TYPE_FRAME))
/**
 * @brief Tests whether given klass is a subclass of EailFrame
 *
 * @param klass klass object
 */
#define EAIL_IS_FRAME_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                      EAIL_TYPE_FRAME))
/**
 * @brief Gets EailFrame class structure from an obj (class instance)
 *
 * @param obj object instance to get EailFrame class from
 */
#define EAIL_FRAME_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                      EAIL_TYPE_FRAME, EailFrameClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailFrame*/
typedef struct _EailFrame      EailFrame;
/** @brief Definition of class structure for Atk EailFrame*/
typedef struct _EailFrameClass EailFrameClass;

/** @brief Definition of object structure for Atk EailFrame*/
struct _EailFrame
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailFrame*/
struct _EailFrameClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailFrame GType
 * @returns GType for EailFrame implementation*/
GType eail_frame_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
