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
 * @file eail_action_slider.h
 *
 * @brief Header for EailActionSlider implementation
 */

#ifndef EAIL_ACTION_SLIDER_H
#define EAIL_ACTION_SLIDER_H

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailActionSlider class
 */
#define EAIL_TYPE_ACTION_SLIDER             (eail_action_slider_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailActionSlider
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_ACTION_SLIDER(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                             EAIL_TYPE_ACTION_SLIDER, EailActionSlider))
/**
 * @brief Macro upcasts a subclass (klass) to the EailActionSlider class
 *
 * @param klass subclass object
 */
#define EAIL_ACTION_SLIDER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                             EAIL_TYPE_ACTION_SLIDER, EailActionSliderClass))
/**
 * @brief Tests whether object (obj) is an instance of EailActionSlider class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_ACTION_SLIDER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                             EAIL_TYPE_ACTION_SLIDER))
/**
 * @brief Tests whether given klass is a subclass of EailActionSlider
 *
 * @param klass klass object
 */
#define EAIL_IS_ACTION_SLIDER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                             EAIL_TYPE_ACTION_SLIDER))
/**
 * @brief Gets EailActionSlider class structure from an obj (class instance)
 *
 * @param obj object instance to get EailActionSlider class from
 */
#define EAIL_ACTION_SLIDER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                             EAIL_TYPE_ACTION_SLIDER, EailActionSliderClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailActionSlider*/
typedef struct _EailActionSlider      EailActionSlider;
/** @brief Definition of object class for Atk EailActionSlider*/
typedef struct _EailActionSliderClass EailActionSliderClass;

/** @brief Definition of object structure for Atk EailActionSlider*/
struct _EailActionSlider
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailActionSlider*/
struct _EailActionSliderClass
{
   /** @brief class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailActionSlider GType
 *
 * @returns GType for EailActionSlider implementation*/
GType eail_action_slider_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
