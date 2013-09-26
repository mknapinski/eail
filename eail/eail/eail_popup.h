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

#ifndef EAIL_POPUP_H
#define EAIL_POPUP_H

/**
 * @file eail_popup.h
 *
 * @brief Header for EailPopup implementation
 */

#include "eail_notify.h"
/**
 * @brief Returns a value corresponding to the type of EailPopup class
 */
#define EAIL_TYPE_POPUP             (eail_popup_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailPopup
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_POPUP(obj)             (G_TYPE_POPUP_INSTANCE_CAST((obj), \
                                            EAIL_TYPE_POPUP, \
                                            EailPopup))
/**
 * @brief Macro upcasts a subclass (klass) to the EailPopup class
 *
 * @param klass subclass object
 */
#define EAIL_POPUP_CLASS(klass)     (G_TYPE_POPUP_CLASS_CAST((klass), \
                                            EAIL_TYPE_POPUP, \
                                            EailPopupClass))
/**
 * @brief Tests whether object (obj) is an instance of EailPopup class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_POPUP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                            EAIL_TYPE_POPUP))
/**
 * @brief Tests whether given klass is a subclass of EailPopup
 *
 * @param klass klass object
 */
#define EAIL_IS_POPUP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                            EAIL_TYPE_POPUP))
/**
 * @brief Gets EailPopup class structure from an obj (class instance)
 *
 * @param obj object instance to get EailPopup class from
 */
#define EAIL_POPUP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                            EAIL_TYPE_POPUP, \
                                            EailPopupClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailPopup*/
typedef struct _EailPopup EailPopup;
/** @brief Definition of object class for Atk EailPopup*/
typedef struct _EailPopupClass EailPopupClass;

/** @brief Definition of object structure for Atk EailPopup*/
struct _EailPopup
{
   /** @brief Parent widget whose functionality is being extended*/
   EailNotify parent;
};

/** @brief Definition of object class for Atk EailPopup*/
struct _EailPopupClass
{
   /** @brief Parent object that functionality is being extended*/
   EailNotifyClass parent_class;
};

/** @brief Getter for EailPopup GType
 *
 * @returns GType for EailPopup implementation*/
GType eail_popup_get_type(void);

#ifdef __cplusplus
}
#endif

#endif

