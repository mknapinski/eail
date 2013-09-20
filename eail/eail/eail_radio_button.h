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

#ifndef EAIL_RADIO_BUTTON_H
#define EAIL_RADIO_BUTTON_H

/**
 * @file eail_radio_button.h
 *
 * @brief Header for EailRadioButton implementation
 */

#include "eail_button.h"
/**
 * @brief Returns a value corresponding to the type of EailRadiobutton class
 */
#define EAIL_TYPE_RADIO_BUTTON              (eail_radio_button_get_type ())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailRadiobutton
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_RADIO_BUTTON(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
                                             EAIL_TYPE_RADIO_BUTTON, \
                                             EailRadioButton))
/**
 * @brief Macro upcasts a subclass (klass) to the EailRadiobutton class
 *
 * @param klass subclass object
 */
#define EAIL_RADIO_BUTTON_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                             EAIL_TYPE_RADIO_BUTTON, \
                                             EailRadioButtonClass))
/**
 * @brief Tests whether object (obj) is an instance of EailRadiobutton class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_RADIO_BUTTON(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj),\
                                             EAIL_TYPE_RADIO_BUTTON))
/**
 * @brief Tests whether given klass is a subclass of EailRadiobutton
 *
 * @param klass klass object
 */
#define EAIL_IS_RADIO_BUTTON_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                             EAIL_TYPE_RADIO_BUTTON))
/**
 * @brief Gets EailRadiobutton class structure from an obj (class instance)
 *
 * @param obj object instance to get EailRadiobutton class from
 */
#define EAIL_RADIO_BUTTON_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                             EAIL_TYPE_RADIO_BUTTON, \
                                             EailRadioButtonClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailRadiobutton*/
typedef struct _EailRadioButton EailRadioButton;
/** @brief Definition of object class for Atk EailRadiobutton*/
typedef struct _EailRadioButtonClass EailRadioButtonClass;

/** @brief Definition of object structure for Atk EailRadiobutton*/
struct _EailRadioButton
{
   /** @brief Parent object that functionality is being extended*/
   EailButton parent;
   char *click_description; /*!< @brief 'click' action description*/
   Eina_Bool state; /*!< @brief state of object (selected/unselected)*/
};

/** @brief Definition of object class for Atk EailRadiobutton*/
struct _EailRadioButtonClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailButtonClass parent_class;
};

/** @brief Getter for EailRadioButton GType
 *
 * @returns GType for EailRadiobutton implementation*/
GType eail_radio_button_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
