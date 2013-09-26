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
 * @file eail_slider.h
 *
 * @brief Header for EailSlider implementation
 */

#ifndef EAIL_SLIDER_H
#define EAIL_SLIDER_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailSlider class
 */
#define EAIL_TYPE_SLIDER              (eail_slider_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailSlider
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_SLIDER(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                       EAIL_TYPE_SLIDER, EailSlider))
/**
 * @brief Macro upcasts a subclass (klass) to the EailSlider class
 *
 * @param klass subclass object
 */
#define EAIL_SLIDER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                       EAIL_TYPE_SLIDER, EailSliderClass))
/**
 * @brief Tests whether object (obj) is an instance of EailSlider class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_SLIDER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                       EAIL_TYPE_SLIDER))
/**
 * @brief Tests whether given klass is a subclass of EailSlider
 *
 * @param klass klass object
 */
#define EAIL_IS_SLIDER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                       EAIL_TYPE_SLIDER))
/**
 * @brief Gets EailSlider class structure from an obj (class instance)
 *
 * @param obj object instance to get EailSlider class from
 */
#define EAIL_SLIDER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                       EAIL_TYPE_SLIDER, EailSliderClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailSlider*/
typedef struct _EailSlider      EailSlider;
/** @brief Definition of object class for Atk EailSlider*/
typedef struct _EailSliderClass EailSliderClass;

/** @brief Definition of object structure for Atk EailSlider*/
struct _EailSlider
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailSlider*/
struct _EailSliderClass
{
   /** @brief Parent object that functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailSlider GType
 *
 * @returns GType for EailSlider implementation*/
GType eail_slider_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
