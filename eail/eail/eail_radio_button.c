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
 * @file eail_radio_button.c
 * @brief EailRadioButton implementation
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_radio_button.h"
#include "eail_priv.h"

static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief Definition of EailRadioButton type
 */
G_DEFINE_TYPE_WITH_CODE(EailRadioButton,
                        eail_radio_button,
                        EAIL_TYPE_BUTTON,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/**
 * @brief Callback for 'on_change' event
 *
 * Called to notify AtkObject about state change.
 *
 * @param data user data passed to callback
 * @param obj source object
 * @param event_info event info
 */
static void
_on_change(void *data, Evas_Object *obj, void *event_info)
{
   EailRadioButton *button = EAIL_RADIO_BUTTON(data);
   button->state = elm_radio_state_value_get(obj) ? EINA_TRUE : EINA_FALSE;

   atk_object_notify_state_change(ATK_OBJECT(button), ATK_STATE_CHECKED,
                                  button->state == EINA_TRUE ? TRUE : FALSE);
}

/**
 * @brief Gets a reference to the state set of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * Implementation of ref_state_set from AtkObject interface.
 *
 * @param obj AtkObject instance
 *
 * @returns AtkStateSet representing the state set of the accessible
 */
static AtkStateSet*
eail_radio_button_ref_state_set(AtkObject *obj)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   AtkStateSet *state_set =
      ATK_OBJECT_CLASS(eail_radio_button_parent_class)->ref_state_set(obj);
   if (widget==NULL)
     {
        atk_state_set_add_state(state_set, ATK_STATE_DEFUNCT);
        return state_set;
     }

   /* get group value*/
   int selected_value  = elm_radio_value_get(widget);
   int widget_value = elm_radio_state_value_get(widget);

   if (selected_value == widget_value)
     {
        atk_state_set_add_state(state_set, ATK_STATE_CHECKED);
     }

   return state_set;
}

/**
 * @brief Initialize EailRadionButton as AtkObject
 *
 * @param obj AtkObject instance
 * @param data user data passed to initialization
 */
static void
eail_radio_button_initialize(AtkObject *obj, gpointer data)
{
    ATK_OBJECT_CLASS(eail_radio_button_parent_class)->initialize(obj, data);
    Evas_Object *widget = (Evas_Object *) data;
    EailRadioButton *radio = EAIL_RADIO_BUTTON(obj);
    int value = elm_radio_state_value_get(widget);
    int selected = elm_radio_value_get(widget);
    if (selected == value)
      {
        radio->state = EINA_TRUE;
      }
    evas_object_smart_callback_add(widget, "changed", _on_change, obj);

    obj->role = ATK_ROLE_RADIO_BUTTON;
}

/**
 * @brief Initialize EailRadioButton object
 *
 * @param radio_button EailRadioButton instance
 */
static void
eail_radio_button_init(EailRadioButton *radio_button)
{
   radio_button->click_description = NULL;
   radio_button->state = EINA_FALSE;
}

/**
 * @brief EailRadioButton finalize function
 *
 * @param object GObject instance
 */
static void
eail_radio_button_finalize(GObject *object)
{
   EailRadioButton *button = EAIL_RADIO_BUTTON(object);
   if (button->click_description) free(button->click_description);
   G_OBJECT_CLASS(eail_radio_button_parent_class)->finalize(object);
}

/**
 * @brief Object class initialization
 *
 * @param klass EailRadioButtonClass instance
 */
static void
eail_radio_button_class_init(EailRadioButtonClass *klass)
{
    AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    class->initialize = eail_radio_button_initialize;
    class->ref_state_set = eail_radio_button_ref_state_set;
    gobject_class->finalize = eail_radio_button_finalize;
}

/**
 * @brief Gets the number of accessible actions available on the object
 *
 * If there are more than one, the first one is considered
 * the "default" action of the object.
 *
 * Implementation of get_n_actions from AtkAction interface.
 *
 * @param action AtkAction instance
 *
 * @returns integer representing the number of available actions
 */
static int
eail_radio_button_n_action_get(AtkAction *action)
{
   return 1;
}

/**
 * @brief Gets the name of the specified action of the object
 *
 * Implementation of get_action_name from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i index of action
 *
 * @returns string representing the name of the specified action
 */
static const char*
eail_radio_button_action_name_get(AtkAction *action, int i)
{
   const char *action_name;
   switch (i)
     {
      case 0:
         /*click - This is called whenever the user changes the state of one of*/
         /*the radio objects within the group of radio objects that work together. */
         action_name = "click";
         break;
      default:
         action_name = NULL;
         break;
     }
   return action_name;
}

/**
 * @brief Sets a description of the specified action of the object
 *
 * Implementation of set_description from AtkAction.
 *
 * @param action AtkAction instance
 * @param i action index
 * @param description action's description to set
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_radio_button_description_set(AtkAction *action, gint i, const char *description)
{
   if (i != 0) return FALSE;

   EailRadioButton *button = EAIL_RADIO_BUTTON(action);
   if (button->click_description) free(button->click_description);
   button->click_description = g_strdup(description);
   return TRUE;
}

/**
 * @brief Gets the description of the specified action of the object
 *
 * Implementation of get_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i index of action
 *
 * @returns string representing the description of the specified action
 */
static const char*
eail_radio_button_description_get(AtkAction *action, gint i)
{
   if (i != 0) return NULL;

   EailRadioButton *button = EAIL_RADIO_BUTTON(action);
   return button->click_description;
}

/**
 * @brief Performs the specified action on the object
 *
 * Implementation of do_action from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_radio_button_do_action(AtkAction *action, int i)
{
   Evas_Object *obj = eail_widget_get_widget(EAIL_WIDGET(action));
   EailRadioButton *button = EAIL_RADIO_BUTTON(action);
   if (button->state == EINA_TRUE) return FALSE;
   if ((elm_object_disabled_get(obj)) || (!evas_object_visible_get(obj)))
     return FALSE;
   const char *action_name = atk_action_get_name(action, i);
   if (action_name == NULL) return FALSE;
   int state_value = elm_radio_state_value_get(obj);
   elm_radio_value_set(obj, state_value);
   button->state = EINA_TRUE;

   if (!g_strcmp0(action_name, "click"))
        evas_object_smart_callback_call(obj, "changed", NULL);

   return TRUE;
}

/**
 * @brief AtkAction interface initialization
 *
 * @param iface AtkActionIface instance
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action = eail_radio_button_do_action;
   iface->get_n_actions = eail_radio_button_n_action_get;
   iface->get_description = eail_radio_button_description_get;
   iface->get_name = eail_radio_button_action_name_get;
   iface->set_description = eail_radio_button_description_set;
}
