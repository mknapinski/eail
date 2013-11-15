/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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
 * @file eail-explorer.c
 *
 * @brief EAIL Explorer implementation
 */

#define _GNU_SOURCE

#include <Elementary.h>
#include <atk/atk.h>
#include <dlfcn.h>
#include <signal.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <execinfo.h>
#include <Ecore.h>
#include <Ecore_Con.h>
#include "eail-explorer-ui.h"
#include "eail-browser.h"
#include "eail-explorer-tr.h"

#define ENV_EAIL_SERVER	 "EAIL_SERVER"

#define DEFAULT_OPACITY 255

static int _elm_init_count;

void _show_interfaces(Evas_Object *toolbar, AtkObject *obj);

Evas_Object *win = NULL;
Evas_Object *tb = NULL;
Evas_Object *genlist = NULL;
Evas_Object *layout = NULL;
Evas_Object *actual_layout = NULL;
Evas_Object *layout_objects = NULL;
Evas_Object *layout_actions = NULL;
Evas_Object *layout_component = NULL;
Evas_Object *layout_selection = NULL;
Evas_Object *layout_table = NULL;
Evas_Object *layout_image = NULL;
Evas_Object *layout_value = NULL;
Evas_Object *layout_text = NULL;
Evas_Object *layout_edit_text = NULL;
AtkObject *root = NULL;
AtkObject *atk_obj = NULL;
AtkObject *focused = NULL;
guint children_change_listener;
guint focus_tracker_id;

/**
 * @brief switches to Action interface view
 *
 * @param data passed to callback
 * @param obj toolbar object
 * @param event_info additional event info
 */
void _actions_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_actions)
	{
		layout_actions = prepare_layout_action(layout);
	}
	actual_layout = layout_actions;
	print_actions(layout_actions, focused, layout);
}

/**
 * @brief switches to Component interface view
 *
 * @param data Data passed to callback
 * @param obj Toolbar object
 * @param event_info Additional event info
 */
void _component_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_component)
	{
		layout_component = prepare_layout_component(layout);
	}
	actual_layout = layout_component;
	print_component(layout_component, focused, layout);
}

/**
 * @brief switches to Editable Text interface view
 *
 * @param data Data passed to callback
 * @param obj Toolbar object
 * @param event_info Additional event info
 */
void _edit_text_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_edit_text)
	{
		layout_edit_text = prepare_layout_edit_text(layout);
	}
	actual_layout = layout_edit_text;
	print_edit_text(layout_edit_text, focused, layout);
}

/**
 * @brief switches to Object interface view
 *
 * @param data passed to callback
 * @param obj toolbar object
 * @param event_info additional event info
 */
void _objects_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_objects)
	{
		layout_objects = prepare_layout_object(layout);
	}
	actual_layout = layout_objects;
	print_object(layout_objects, focused, layout);
}

/**
 * @brief switches to Image interface view
 *
 * @param data passed to callback
 * @param obj toolbar object
 * @param event_info additional event info
 */
void _image_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_image)
	{
		layout_image = prepare_layout_image(layout);
	}
	actual_layout = layout_image;
	print_image(layout_image, focused, tb, layout);
}

/**
 * @brief switches to Table interface view
 *
 * @param data passed to callback
 * @param obj toolbar object
 * @param event_info additional event info
 */
void _table_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_table)
	{
		layout_table = prepare_layout_table(layout);
	}
	actual_layout = layout_table;
	print_table(layout_table, focused, tb, layout);
}

/**
 * @brief switches to Selection interface view
 *
 * @param data passed to callback
 * @param obj toolbar object
 * @param event_info additional event info
 */
void _selection_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_selection)
	{
		layout_selection = prepare_layout_selection(layout);
	}
	actual_layout = layout_selection;
	print_selection(layout_selection, focused, layout);
}

/**
 * @brief switches to Value interface view
 *
 * @param data passed to callback
 * @param obj toolbar object
 * @param event_info additional event info
 */
void _value_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_value)
	{
		layout_value = prepare_layout_value(layout);
	}
	actual_layout = layout_value;
	print_value(layout_value, focused, tb, layout);
}

/**
 * @brief switches to Text interface view
 *
 * @param data passed to callback
 * @param obj toolbar object
 * @param event_info additional event info
 */
void _text_click(void *data, Evas_Object *obj, void *event_info)
{
	if(!focused)
	{
		Elm_Object_Item *selected = elm_toolbar_selected_item_get(obj);
		elm_toolbar_item_selected_set(selected, EINA_FALSE);
		return;
	}
	if(actual_layout)
	{
		elm_object_part_content_unset(layout, "elm.layout");
		evas_object_hide(actual_layout);
	}
	if(!layout_text)
	{
		layout_text = prepare_layout_text(layout);
	}
	actual_layout = layout_text;
	print_text(layout_text, focused, layout);
}

/**
 * @brief highlights the currently focused object
 *
 * The function briefly highlights the currently focused object
 * by decreasing the object's RGB and alpha values.
 *
 * @param target Rectangle object to realize highlight event
 *
 * @return Boolean value, TRUE if alpha parameter is still
 * higher than 25, FALSE otherwise
 */
static Eina_Bool highlight_target(void *target)
{
	Evas_Object *highlight = target;
	int r, g, b, a, op;

	evas_object_color_get(highlight, &r, &g, &b, &a);

	if (a <= 25)
	{
		evas_object_del(highlight);
		return ECORE_CALLBACK_CANCEL;
	}

	op = a - 5;
	r = r - r/10;
	g = g - g/10;
	b = b - b/10;

	evas_object_color_set(highlight, r, g, b, op);

	return ECORE_CALLBACK_RENEW;
}

/**
 * @brief check if given object is Eail Explorer
 *
 * @param obj Atk Object which is checked
 *
 * @return Boolean value, true if given object is Eail Explorer,
 * false otherwise
 */
Eina_Bool is_eail_explorer_win(AtkObject *obj){
	const gchar *name = atk_object_get_name(obj);
	AtkRole role = atk_object_get_role(obj);

	if(ATK_ROLE_WINDOW == role && name && !strcmp(name, EAIL_EXPLORER))
		return EINA_TRUE;
	return EINA_FALSE;
}

/**
 * @brief reset toolbar
 *
 * Function reset toolbar object to inital state
 *
 * @param toolbar object to reset
 */
void reset_toolbar(Evas_Object* toolbar)
{
	Elm_Object_Item *toolbar_it = NULL;

	toolbar_it = elm_toolbar_first_item_get(toolbar);

	while(toolbar_it)
	{
		elm_toolbar_item_selected_set(toolbar_it, EINA_FALSE);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
		toolbar_it = elm_toolbar_item_next_get(toolbar_it);
	}
}

/**
 * @brief highlight and refresh explorer application
 *
 * @param obj focused atk object
 * @param expand EINA_TRUE if browser should be expanded, otherwise EINA_FALSE
 */
void highlight_and_refresh(AtkObject *obj, Eina_Bool expand){
	if(obj)
	{
		if(_is_part_of_eail_explorer(obj))
			return;

		focused = obj;
		reset_toolbar(tb);
		_show_interfaces(tb, focused);
		refresh_layout(tb);
		if(expand)
			expand_browser(obj);
	}
	else
	{
		reset_toolbar(tb);
		evas_object_hide(actual_layout);
		return;
	}

	if(atk_object_get_role(obj) !=  ATK_ROLE_LIST_ITEM && atk_object_get_role(obj) != ATK_ROLE_APPLICATION)
	{
		Eina_List* ee_list = ecore_evas_ecore_evas_list_get();
		Eina_List *l;
		Ecore_Evas *ee;
		Evas* evas = NULL;

		AtkObject *atk_window = obj;
		while(ATK_ROLE_WINDOW != atk_object_get_role(atk_window))
			atk_window = atk_object_get_parent(atk_window);

		int atk_win_x, atk_win_y, atk_win_w, atk_win_h;
		atk_component_get_position(ATK_COMPONENT(atk_window), &atk_win_x, &atk_win_y, ATK_XY_SCREEN);
		atk_component_get_size(ATK_COMPONENT(atk_window), &atk_win_w, &atk_win_h);

		int ee_x, ee_y, ee_w, ee_h;
		EINA_LIST_FOREACH(ee_list, l, ee)
			{
				ecore_evas_geometry_get(ee, &ee_x, &ee_y, &ee_w, &ee_h);
				//TODO: check z dimension, right now MDI z order is not implemented in eail
				if(atk_win_x == ee_x && atk_win_y == ee_y && atk_win_w == ee_w && atk_win_h == ee_h)
					evas = ecore_evas_get(ee);
			}
		eina_list_free(ee_list);

		if(evas){
			Evas_Object *bg = evas_object_rectangle_add(evas);
			int x, y, w, h;
			atk_component_get_position(ATK_COMPONENT(obj), &x, &y, ATK_XY_WINDOW);
			atk_component_get_size(ATK_COMPONENT(obj), &w, &h);

			evas_object_color_set(bg, 120, 120, 120, DEFAULT_OPACITY);
			evas_object_move(bg, x, y);
			evas_object_resize(bg, w, h);
			evas_object_show(bg);
			ecore_animator_add(highlight_target, bg);
		}
	}
}
/**
 * @brief Refreshes view after focusing on tested application
 *
 * @param obj Focused object
 */
void focus_announce(AtkObject *obj)
{
	highlight_and_refresh(obj, EINA_TRUE);
}

/**
 * @brief shows i.e. enables or disables the views for interfaces implemented by obj
 *
 * @param toolbar Toolbar object
 * @param obj focused Atk_object
 */
void _show_interfaces(Evas_Object *toolbar, AtkObject *obj)
{
	Elm_Object_Item *toolbar_it;

	if (!ATK_IS_OBJECT(obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, OBJECT);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, OBJECT);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}

	if (!ATK_IS_ACTION (obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, ACTION);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, ACTION);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}

	if (!ATK_IS_COMPONENT (obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, COMPONENT);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, COMPONENT);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}

	if (!ATK_IS_IMAGE (obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, IMAGE);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, IMAGE);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}

	if (!ATK_IS_SELECTION (obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, SELECTION);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, SELECTION);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}

	if (!ATK_IS_TABLE (obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, TABLE);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, TABLE);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}

	if (!ATK_IS_TEXT (obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, TEXT);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, TEXT);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}

	if (!ATK_IS_EDITABLE_TEXT (obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, EDITABLE_TEXT);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, EDITABLE_TEXT);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}

	if (!ATK_IS_VALUE (obj)) {
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, VALUE);
		elm_object_item_disabled_set(toolbar_it, EINA_TRUE);
	}
	else
	{
		toolbar_it = elm_toolbar_item_find_by_label(toolbar, VALUE);
		elm_object_item_disabled_set(toolbar_it, EINA_FALSE);
	}
}

/**
 * @brief callback called when refresh button is clicked
 *
 * @param data UNUSED
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _refresh_bt_cb(void *data, Evas_Object *obj, void *event_info)
{
	elm_object_part_content_unset(layout, "elm.layout");
	evas_object_hide(actual_layout);
	reset_toolbar(tb);
	focused = NULL;
	refresh_browser();
}

/**
 * @brief atk object tree initialization function
 *
 * @param obj AtkObject
 */
static void initialize_atk_objects(AtkObject *obj)
{
	if (!ATK_IS_OBJECT(obj) || is_eail_explorer_win(obj))
		return;
	int i;
	for(i = 0; i < atk_object_get_n_accessible_children(obj); ++i){
		AtkObject *child = atk_object_ref_accessible_child(obj, i);
		if(!ATK_IS_OBJECT(child))
			continue;
		initialize_atk_objects(child);
		g_object_unref(child);
	}
}

/**
 * @brief callback that is used to notify every new window of application
 *
 * @param ihint Structure is used to pass on additional information to callbacks during a signal emission
 * @param n_param_values  Accumulator to collect callback return values in, this is the return value of the current signal emission
 * @param param_values A GValue holding the return value of the signal handler
 * @param data  Callback data that was specified when creating the signal
 *
 * return boolean value whether the signal emission should be aborted
 */
gboolean Children_Changed_Emission_Hook(GSignalInvocationHint *ihint, guint n_param_values, const GValue *param_values, gpointer data){
	GObject *gobject = g_value_get_object (param_values + 0);
	AtkObject *application = ATK_OBJECT(gobject);

	if(ATK_ROLE_APPLICATION == atk_object_get_role(application))
		initialize_atk_objects(application);

	return TRUE;
}

/**
 * @brief callback for auto-focus on EAIL-Explorer window after application start
 *
 * @param data UNUSED
 * @param obj Window Object
 * @param event_info UNUSED
 */
void _window_create(void *data, Evas_Object *obj, void *event_info)
{
	initialize_atk_objects(atk_get_root());
	children_change_listener = atk_add_global_event_listener(Children_Changed_Emission_Hook, "ATK:AtkObject:children-changed");
	evas_object_smart_callback_del(obj, "focus,in", _window_create);
}

/**
 * @brief callback for close EAIL-Explorer window
 *
 * @param data UNUSED
 * @param obj UNUSED
 * @param event_info UNUSED
 */
void _window_delete(void *data, Evas_Object *obj, void *event_info)
{
	atk_remove_focus_tracker(focus_tracker_id);
	atk_remove_global_event_listener(children_change_listener);
	free_memory();
	free_browser();
}

/**
 * @brief prepares the main application window
 */
static void _prepare_eail_explorer_window()
{
	Evas_Object *refresh_button;

	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, EAIL_EXPLORER);
	elm_win_autodel_set(win, EINA_TRUE);

	layout = elm_layout_add(win);
	elm_layout_file_set(layout, EDJ_PATH_MAIN, "explorer/main");
	elm_win_resize_object_add(win, layout);
	evas_object_show(layout);

	refresh_button = elm_button_add(win);
	elm_object_part_content_set(layout, "elm.refresh.button", refresh_button);
	elm_object_text_set(refresh_button, "Refresh");
	evas_object_smart_callback_add(refresh_button, "clicked", _refresh_bt_cb, NULL);
	evas_object_show(refresh_button);

	tb = elm_toolbar_add(layout);
	elm_object_part_content_set(layout, "elm.toolbar", tb);
	elm_toolbar_select_mode_set(tb, ELM_OBJECT_SELECT_MODE_ALWAYS);

	create_browser(layout);

	elm_toolbar_item_append(tb, NULL, OBJECT, _objects_click, NULL);
	elm_toolbar_item_append(tb, NULL, ACTION, _actions_click, NULL);
	elm_toolbar_item_append(tb, NULL, COMPONENT, _component_click, NULL);
	elm_toolbar_item_append(tb, NULL, IMAGE, _image_click, NULL);
	elm_toolbar_item_append(tb, NULL, SELECTION, _selection_click, NULL);
	elm_toolbar_item_append(tb, NULL, TABLE, _table_click, NULL);
	elm_toolbar_item_append(tb, NULL, TEXT, _text_click, NULL);
	elm_toolbar_item_append(tb, NULL, EDITABLE_TEXT, _edit_text_click, NULL);
	elm_toolbar_item_append(tb, NULL, VALUE, _value_click, NULL);

	reset_toolbar(tb);

	elm_toolbar_menu_parent_set(tb, win);
	evas_object_show(tb);

	evas_object_resize(win, 800, 600);
	elm_win_autodel_set(win, EINA_TRUE);
	evas_object_smart_callback_add(win, "focus,in", _window_create, NULL);
	evas_object_smart_callback_add(win, "delete,request", _window_delete, NULL);
	evas_object_show(win);
}

/**
 * @brief Overloaded elm_init function
 *
 * @param argc argument count
 * @param argv argument array
 *
 * @return The init counter value.
 */
EAPI int
elm_init(int argc, char **argv)
{
	int (*_elm_init)(int, char **) = dlsym(RTLD_NEXT, __func__);

	_elm_init_count++;

	fprintf(stderr, "EAIL Client Hook initialized\n");

	return _elm_init(argc, argv);
}
/**
 * @brief overloaded main loop function
 */
EAPI void
ecore_main_loop_begin(void)
{
	void (*_ecore_main_loop_begin)(void) = dlsym(RTLD_NEXT, __func__);

	if (!_elm_init_count) {
		EINA_LOG_ERR(ERR_FATAL);
		exit(1);
	}

	_prepare_eail_explorer_window();

	focus_tracker_id = atk_add_focus_tracker(focus_announce);
	_ecore_main_loop_begin();

	return;
}
