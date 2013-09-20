check_PROGRAMS = eail_image_tc1 \
		 eail_image_tc2 \
		 eail_image_tc3 \
		 eail_action_slider_tc1 \
		 eail_action_slider_tc2 \
		 eail_check_tc1 \
		 eail_check_tc2 \
		 eail_radio_tc1 \
		 eail_radio_tc2 \
		 eail_menu_tc1 \
		 eail_scroller_tc1 \
		 eail_scroller_tc2 \
		 eail_list_tc1 \
		 eail_list_tc2 \
		 eail_list_tc3 \
		 eail_genlist_tc1 \
		 eail_genlist_tc2 \
		 eail_genlist_tc3 \
		 eail_button_tc1 \
		 eail_button_tc2 \
		 eail_button_tc3 \
		 eail_button_tc4 \
		 eail_tooltip_tc1 \
		 eail_separator_tc1 \
		 eail_progressbar_tc1 \
		 eail_progressbar_tc2 \
		 eail_clock_tc1 \
		 eail_spinner_tc1 \
		 eail_spinner_tc2 \
		 eail_calendar_tc1 \
		 eail_calendar_tc2 \
		 eail_grid_tc1 \
		 eail_route_tc1 \
		 eail_route_tc2 \
		 eail_gengrid_tc1 \
		 eail_gengrid_tc2 \
		 eail_gengrid_tc3 \
		 eail_dayselector_tc1 \
		 eail_fileselector_tc1 \
		 eail_diskselector_tc1 \
		 eail_diskselector_tc2 \
		 eail_diskselector_tc3 \
		 eail_conformant_tc1 \
		 eail_notify_tc1 \
		 eail_popup_tc1 \
		 eail_web_tc1 \
		 eail_win_tc1 \
		 eail_win_tc2 \
		 eail_toolbar_tc1 \
		 eail_index_tc1 \
		 eail_ctxpopup_tc1 \
		 eail_ctxpopup_tc2 \
		 eail_focus_test \
		 eail_multibuttonentry_tc1 \
		 eail_multibuttonentry_tc2 \
		 eail_multibuttonentry_tc3 \
		 eail_multibuttonentry_tc4 \
		 eail_segment_control_tc1 \
		 eail_segment_control_tc2 \
		 eail_file_selector_button_tc1 \
		 eail_file_selector_entry_tc1 \
		 eail_naviframe_tc1 \
		 eail_naviframe_tc2 \
		 eail_naviframe_page_tc1 \
		 eail_colorselector_tc1 \
		 eail_colorselector_tc2 \
		 eail_hover_tc1 \
		 eail_hover_tc2 \
		 eail_panes_tc1 \
		 eail_panes_tc2 \
		 eail_datetime_tc1 \
		 eail_datetime_tc2 \
		 eail_panel_tc1 \
		 eail_panel_tc2 \
		 eail_hoversel_tc1 \
		 eail_hoversel_tc2 \
		 eail_slideshow_tc1 \
		 eail_slideshow_tc2 \
		 eail_slideshow_tc3 \
		 eail_map_tc1 \
		 eail_map_tc2 \
		 eail_map_tc3 \
		 eail_glview_tc1 \
		 eail_flipselector_tc1 \
		 eail_flipselector_tc2 \
		 eail_flipselector_tc3 \
		 eail_mapbuf_tc1 \
		 eail_video_tc1 \
		 eail_video_tc2 \
		 eail_video_tc3 \
		 eail_layout_tc1 \
		 eail_flip_tc1 \
		 eail_flip_tc2 \
		 eail_bubble_tc1 \
		 eail_bubble_tc2 \
		 eail_plug_tc1 \
		 eail_plug_tc2 \
		 eail_label_tc1 \
		 eail_label_tc2 \
		 eail_slider_tc1 \
		 eail_slider_tc2 \
		 eail_prefs_tc1 \
		 eail_background_tc1 \
		 eail_background_tc2 \
		 eail_table_tc1 \
		 eail_thumb_tc1 \
		 eail_thumb_tc2 \
		 eail_thumb_tc3 \
		 eail_icon_tc1 \
		 eail_icon_tc2 \
		 eail_icon_tc3 \
		 eail_inwin_tc1 \
		 eail_entry_tc1 \
		 eail_entry_tc2 \
		 eail_entry_tc3 \
		 eail_photo_tc1 \
		 eail_photo_tc2 \
		 eail_photo_tc3 \
		 eail_photocam_tc1 \
		 eail_photocam_tc2 \
		 eail_photocam_tc3

TESTS = $(check_PROGRAMS)

if MAKE_TESTS
noinst_LTLIBRARIES = libeail_test_utils.la
endif

test_cflags = $(TEST_DEPS_CFLAGS)
test_libs = $(TEST_DEPS_LIBS) \
	    $(COVERAGE_LIBS) \
	    libeail_test_utils.la

eail_action_slider_tc1_SOURCES = eail_action_slider_tc1.c
eail_action_slider_tc1_CFLAGS = $(test_cflags)
eail_action_slider_tc1_LDADD = $(test_libs)

eail_action_slider_tc2_SOURCES = eail_action_slider_tc2.c
eail_action_slider_tc2_CFLAGS = $(test_cflags)
eail_action_slider_tc2_LDADD = $(test_libs)

eail_image_tc1_SOURCES = eail_image_tc1.c
eail_image_tc1_CFLAGS = $(test_cflags)
eail_image_tc1_LDADD = $(test_libs)

eail_image_tc2_SOURCES = eail_image_tc2.c
eail_image_tc2_CFLAGS = $(test_cflags)
eail_image_tc2_LDADD = $(test_libs)

eail_image_tc3_SOURCES = eail_image_tc3.c
eail_image_tc3_CFLAGS = $(test_cflags)
eail_image_tc3_LDADD = $(test_libs)

eail_check_tc1_SOURCES = eail_check_tc1.c
eail_check_tc1_CFLAGS = $(test_cflags)
eail_check_tc1_LDADD = $(test_libs)

eail_check_tc2_SOURCES = eail_check_tc2.c
eail_check_tc2_CFLAGS = $(test_cflags)
eail_check_tc2_LDADD = $(test_libs)

eail_radio_tc1_SOURCES = eail_radio_tc1.c
eail_radio_tc1_CFLAGS = $(test_cflags)
eail_radio_tc1_LDADD = $(test_libs)

eail_radio_tc2_SOURCES = eail_radio_tc2.c
eail_radio_tc2_CFLAGS = $(test_cflags)
eail_radio_tc2_LDADD = $(test_libs)

eail_menu_tc1_SOURCES = eail_menu_tc1.c
eail_menu_tc1_CFLAGS = $(test_cflags)
eail_menu_tc1_LDADD = $(test_libs)

eail_scroller_tc1_SOURCES = eail_scroller_tc1.c
eail_scroller_tc1_CFLAGS = $(test_cflags)
eail_scroller_tc1_LDADD = $(test_libs)

eail_scroller_tc2_SOURCES = eail_scroller_tc2.c
eail_scroller_tc2_CFLAGS = $(test_cflags)
eail_scroller_tc2_LDADD = $(test_libs)

eail_list_tc1_SOURCES = eail_list_tc1.c
eail_list_tc1_CFLAGS = $(test_cflags)
eail_list_tc1_LDADD = $(test_libs)

eail_list_tc2_SOURCES = eail_list_tc2.c
eail_list_tc2_CFLAGS = $(test_cflags)
eail_list_tc2_LDADD = $(test_libs)

eail_list_tc3_SOURCES = eail_list_tc3.c
eail_list_tc3_CFLAGS = $(test_cflags)
eail_list_tc3_LDADD = $(test_libs)

eail_genlist_tc1_SOURCES = eail_genlist_tc1.c
eail_genlist_tc1_CFLAGS = $(test_cflags)
eail_genlist_tc1_LDADD = $(test_libs)

eail_genlist_tc2_SOURCES = eail_genlist_tc2.c
eail_genlist_tc2_CFLAGS = $(test_cflags)
eail_genlist_tc2_LDADD = $(test_libs)

eail_genlist_tc3_SOURCES = eail_genlist_tc3.c
eail_genlist_tc3_CFLAGS = $(test_cflags)
eail_genlist_tc3_LDADD = $(test_libs)

eail_button_tc1_SOURCES = eail_button_tc1.c
eail_button_tc1_CFLAGS = $(test_cflags)
eail_button_tc1_LDADD = $(test_libs)

eail_button_tc2_SOURCES = eail_button_tc2.c
eail_button_tc2_CFLAGS = $(test_cflags)
eail_button_tc2_LDADD = $(test_libs)

eail_button_tc3_SOURCES = eail_button_tc3.c
eail_button_tc3_CFLAGS = $(test_cflags)
eail_button_tc3_LDADD = $(test_libs)

eail_button_tc4_SOURCES = eail_button_tc4.c
eail_button_tc4_CFLAGS = $(test_cflags)
eail_button_tc4_LDADD = $(test_libs)

eail_tooltip_tc1_SOURCES = eail_tooltip_tc1.c
eail_tooltip_tc1_CFLAGS = $(test_cflags)
eail_tooltip_tc1_LDADD = $(test_libs)

eail_clock_tc1_SOURCES = eail_clock_tc1.c
eail_clock_tc1_CFLAGS = $(test_cflags)
eail_clock_tc1_LDADD = $(test_libs)

eail_separator_tc1_SOURCES = eail_separator_tc1.c
eail_separator_tc1_CFLAGS = $(test_cflags)
eail_separator_tc1_LDADD = $(test_libs)

eail_progressbar_tc1_SOURCES = eail_progressbar_tc1.c
eail_progressbar_tc1_CFLAGS = $(test_cflags)
eail_progressbar_tc1_LDADD = $(test_libs)

eail_progressbar_tc2_SOURCES = eail_progressbar_tc2.c
eail_progressbar_tc2_CFLAGS = $(test_cflags)
eail_progressbar_tc2_LDADD = $(test_libs)

eail_spinner_tc1_SOURCES = eail_spinner_tc1.c
eail_spinner_tc1_CFLAGS = $(test_cflags)
eail_spinner_tc1_LDADD = $(test_libs)

eail_spinner_tc2_SOURCES = eail_spinner_tc2.c
eail_spinner_tc2_CFLAGS = $(test_cflags)
eail_spinner_tc2_LDADD = $(test_libs)

eail_calendar_tc1_SOURCES = eail_calendar_tc1.c
eail_calendar_tc1_CFLAGS = $(test_cflags)
eail_calendar_tc1_LDADD = $(test_libs)

eail_calendar_tc2_SOURCES = eail_calendar_tc2.c
eail_calendar_tc2_CFLAGS = $(test_cflags)
eail_calendar_tc2_LDADD = $(test_libs)

eail_grid_tc1_SOURCES = eail_grid_tc1.c
eail_grid_tc1_CFLAGS = $(test_cflags)
eail_grid_tc1_LDADD = $(test_libs)

eail_route_tc1_SOURCES = eail_route_tc1.c
eail_route_tc1_CFLAGS = $(test_cflags)
eail_route_tc1_LDADD = $(test_libs)

eail_route_tc2_SOURCES = eail_route_tc2.c
eail_route_tc2_CFLAGS = $(test_cflags)
eail_route_tc2_LDADD = $(test_libs)

eail_gengrid_tc1_SOURCES = eail_gengrid_tc1.c
eail_gengrid_tc1_CFLAGS = $(test_cflags)
eail_gengrid_tc1_LDADD = $(test_libs)

eail_gengrid_tc2_SOURCES = eail_gengrid_tc2.c
eail_gengrid_tc2_CFLAGS = $(test_cflags)
eail_gengrid_tc2_LDADD = $(test_libs)

eail_gengrid_tc3_SOURCES = eail_gengrid_tc3.c
eail_gengrid_tc3_CFLAGS = $(test_cflags)
eail_gengrid_tc3_LDADD = $(test_libs)

eail_dayselector_tc1_SOURCES = eail_dayselector_tc1.c
eail_dayselector_tc1_CFLAGS = $(test_cflags)
eail_dayselector_tc1_LDADD = $(test_libs)

eail_diskselector_tc1_SOURCES = eail_diskselector_tc1.c
eail_diskselector_tc1_CFLAGS = $(test_cflags)
eail_diskselector_tc1_LDADD = $(test_libs)

eail_diskselector_tc2_SOURCES = eail_diskselector_tc2.c
eail_diskselector_tc2_CFLAGS = $(test_cflags)
eail_diskselector_tc2_LDADD = $(test_libs)

eail_diskselector_tc3_SOURCES = eail_diskselector_tc3.c
eail_diskselector_tc3_CFLAGS = $(test_cflags)
eail_diskselector_tc3_LDADD = $(test_libs)

eail_fileselector_tc1_SOURCES = eail_fileselector_tc1.c
eail_fileselector_tc1_CFLAGS = $(test_cflags)
eail_fileselector_tc1_LDADD = $(test_libs)

eail_conformant_tc1_SOURCES = eail_conformant_tc1.c
eail_conformant_tc1_CFLAGS = $(test_cflags)
eail_conformant_tc1_LDADD = $(test_libs)

eail_notify_tc1_SOURCES = eail_notify_tc1.c
eail_notify_tc1_CFLAGS = $(test_cflags)
eail_notify_tc1_LDADD = $(test_libs)

eail_popup_tc1_SOURCES = eail_popup_tc1.c
eail_popup_tc1_CFLAGS = $(test_cflags)
eail_popup_tc1_LDADD = $(test_libs)

eail_toolbar_tc1_SOURCES = eail_toolbar_tc1.c
eail_toolbar_tc1_CFLAGS = $(test_cflags)
eail_toolbar_tc1_LDADD = $(test_libs)

eail_index_tc1_SOURCES = eail_index_tc1.c
eail_index_tc1_CFLAGS = $(test_cflags)
eail_index_tc1_LDADD = $(test_libs)

eail_multibuttonentry_tc1_SOURCES = eail_multibuttonentry_tc1.c
eail_multibuttonentry_tc1_CFLAGS = $(test_cflags)
eail_multibuttonentry_tc1_LDADD = $(test_libs)

eail_multibuttonentry_tc2_SOURCES = eail_multibuttonentry_tc2.c
eail_multibuttonentry_tc2_CFLAGS = $(test_cflags)
eail_multibuttonentry_tc2_LDADD = $(test_libs)

eail_multibuttonentry_tc3_SOURCES = eail_multibuttonentry_tc3.c
eail_multibuttonentry_tc3_CFLAGS = $(test_cflags)
eail_multibuttonentry_tc3_LDADD = $(test_libs)

eail_multibuttonentry_tc4_SOURCES = eail_multibuttonentry_tc4.c
eail_multibuttonentry_tc4_CFLAGS = $(test_cflags)
eail_multibuttonentry_tc4_LDADD = $(test_libs)

eail_ctxpopup_tc1_SOURCES = eail_ctxpopup_tc1.c
eail_ctxpopup_tc1_CFLAGS = $(test_cflags)
eail_ctxpopup_tc1_LDADD = $(test_libs)

eail_ctxpopup_tc2_SOURCES = eail_ctxpopup_tc2.c
eail_ctxpopup_tc2_CFLAGS = $(test_cflags)
eail_ctxpopup_tc2_LDADD = $(test_libs)

eail_focus_test_SOURCES = eail_focus_test.c
eail_focus_test_CFLAGS = $(test_cflags)
eail_focus_test_LDADD = $(test_libs)

eail_segment_control_tc1_SOURCES = eail_segment_control_tc1.c
eail_segment_control_tc1_CFLAGS = $(test_cflags)
eail_segment_control_tc1_LDADD = $(test_libs)

eail_segment_control_tc2_SOURCES = eail_segment_control_tc2.c
eail_segment_control_tc2_CFLAGS = $(test_cflags)
eail_segment_control_tc2_LDADD = $(test_libs)

eail_file_selector_button_tc1_SOURCES = eail_file_selector_button_tc1.c
eail_file_selector_button_tc1_CFLAGS = $(test_cflags)
eail_file_selector_button_tc1_LDADD = $(test_libs)

eail_file_selector_entry_tc1_SOURCES = eail_file_selector_entry_tc1.c
eail_file_selector_entry_tc1_CFLAGS = $(test_cflags)
eail_file_selector_entry_tc1_LDADD = $(test_libs)

eail_naviframe_tc1_SOURCES = eail_naviframe_tc1.c
eail_naviframe_tc1_CFLAGS = $(test_cflags)
eail_naviframe_tc1_LDADD = $(test_libs)

eail_naviframe_tc2_SOURCES = eail_naviframe_tc2.c
eail_naviframe_tc2_CFLAGS = $(test_cflags)
eail_naviframe_tc2_LDADD = $(test_libs)

eail_naviframe_page_tc1_SOURCES = eail_naviframe_page_tc1.c
eail_naviframe_page_tc1_CFLAGS = $(test_cflags)
eail_naviframe_page_tc1_LDADD = $(test_libs)

eail_hover_tc1_SOURCES = eail_hover_tc1.c
eail_hover_tc1_CFLAGS = $(test_cflags)
eail_hover_tc1_LDADD = $(test_libs)

eail_hover_tc2_SOURCES = eail_hover_tc2.c
eail_hover_tc2_CFLAGS = $(test_cflags)
eail_hover_tc2_LDADD = $(test_libs)

eail_colorselector_tc1_SOURCES = eail_colorselector_tc1.c
eail_colorselector_tc1_CFLAGS = $(test_cflags)
eail_colorselector_tc1_LDADD = $(test_libs)

eail_colorselector_tc2_SOURCES = eail_colorselector_tc2.c
eail_colorselector_tc2_CFLAGS = $(test_cflags)
eail_colorselector_tc2_LDADD = $(test_libs)

eail_panes_tc1_SOURCES = eail_panes_tc1.c
eail_panes_tc1_CFLAGS = $(test_cflags)
eail_panes_tc1_LDADD = $(test_libs)

eail_panes_tc2_SOURCES = eail_panes_tc2.c
eail_panes_tc2_CFLAGS = $(test_cflags)
eail_panes_tc2_LDADD = $(test_libs)

eail_datetime_tc1_SOURCES = eail_datetime_tc1.c
eail_datetime_tc1_CFLAGS = $(test_cflags)
eail_datetime_tc1_LDADD = $(test_libs)

eail_datetime_tc2_SOURCES = eail_datetime_tc2.c
eail_datetime_tc2_CFLAGS = $(test_cflags)
eail_datetime_tc2_LDADD = $(test_libs)


eail_panel_tc1_SOURCES = eail_panel_tc1.c
eail_panel_tc1_CFLAGS = $(test_cflags)
eail_panel_tc1_LDADD = $(test_libs)

eail_panel_tc2_SOURCES = eail_panel_tc2.c
eail_panel_tc2_CFLAGS = $(test_cflags)
eail_panel_tc2_LDADD = $(test_libs)

eail_web_tc1_SOURCES = eail_web_tc1.c
eail_web_tc1_CFLAGS = $(test_cflags)
eail_web_tc1_LDADD = $(test_libs)

eail_mapbuf_tc1_SOURCES = eail_mapbuf_tc1.c
eail_mapbuf_tc1_CFLAGS = $(test_cflags)
eail_mapbuf_tc1_LDADD = $(test_libs)

eail_hoversel_tc1_SOURCES = eail_hoversel_tc1.c
eail_hoversel_tc1_CFLAGS = $(test_cflags)
eail_hoversel_tc1_LDADD = $(test_libs)

eail_hoversel_tc2_SOURCES = eail_hoversel_tc2.c
eail_hoversel_tc2_CFLAGS = $(test_cflags)
eail_hoversel_tc2_LDADD = $(test_libs)

eail_slideshow_tc1_SOURCES = eail_slideshow_tc1.c
eail_slideshow_tc1_CFLAGS = $(test_cflags)
eail_slideshow_tc1_LDADD = $(test_libs)

eail_slideshow_tc2_SOURCES = eail_slideshow_tc2.c
eail_slideshow_tc2_CFLAGS = $(test_cflags)
eail_slideshow_tc2_LDADD = $(test_libs)

eail_slideshow_tc3_SOURCES = eail_slideshow_tc3.c
eail_slideshow_tc3_CFLAGS = $(test_cflags)
eail_slideshow_tc3_LDADD = $(test_libs)

eail_map_tc1_SOURCES = eail_map_tc1.c
eail_map_tc1_CFLAGS = $(test_cflags)
eail_map_tc1_LDADD = $(test_libs)

eail_map_tc2_SOURCES = eail_map_tc2.c
eail_map_tc2_CFLAGS = $(test_cflags)
eail_map_tc2_LDADD = $(test_libs)

eail_map_tc3_SOURCES = eail_map_tc3.c
eail_map_tc3_CFLAGS = $(test_cflags)
eail_map_tc3_LDADD = $(test_libs)

eail_win_tc1_SOURCES = eail_win_tc1.c
eail_win_tc1_CFLAGS = $(test_cflags)
eail_win_tc1_LDADD = $(test_libs)

eail_win_tc2_SOURCES = eail_win_tc2.c
eail_win_tc2_CFLAGS = $(test_cflags)
eail_win_tc2_LDADD = $(test_libs)

eail_glview_tc1_SOURCES = eail_glview_tc1.c
eail_glview_tc1_CFLAGS = $(test_cflags)
eail_glview_tc1_LDADD = $(test_libs)

eail_flipselector_tc1_SOURCES = eail_flipselector_tc1.c
eail_flipselector_tc1_CFLAGS = $(test_cflags)
eail_flipselector_tc1_LDADD = $(test_libs)

eail_flipselector_tc2_SOURCES = eail_flipselector_tc2.c
eail_flipselector_tc2_CFLAGS = $(test_cflags)
eail_flipselector_tc2_LDADD = $(test_libs)

eail_flipselector_tc3_SOURCES = eail_flipselector_tc3.c
eail_flipselector_tc3_CFLAGS = $(test_cflags)
eail_flipselector_tc3_LDADD = $(test_libs)

eail_video_tc1_SOURCES = eail_video_tc1.c
eail_video_tc1_CFLAGS = $(test_cflags)
eail_video_tc1_LDADD = $(test_libs)

eail_video_tc2_SOURCES = eail_video_tc2.c
eail_video_tc2_CFLAGS = $(test_cflags)
eail_video_tc2_LDADD = $(test_libs)

eail_video_tc3_SOURCES = eail_video_tc3.c
eail_video_tc3_CFLAGS = $(test_cflags)
eail_video_tc3_LDADD = $(test_libs)

eail_layout_tc1_SOURCES = eail_layout_tc1.c
eail_layout_tc1_CFLAGS = $(test_cflags)
eail_layout_tc1_LDADD = $(test_libs)

eail_flip_tc1_SOURCES = eail_flip_tc1.c
eail_flip_tc1_CFLAGS = $(test_cflags)
eail_flip_tc1_LDADD = $(test_libs)

eail_flip_tc2_SOURCES = eail_flip_tc2.c
eail_flip_tc2_CFLAGS = $(test_cflags)
eail_flip_tc2_LDADD = $(test_libs)

eail_bubble_tc1_SOURCES = eail_bubble_tc1.c
eail_bubble_tc1_CFLAGS = $(test_cflags)
eail_bubble_tc1_LDADD = $(test_libs)

eail_bubble_tc2_SOURCES = eail_bubble_tc2.c
eail_bubble_tc2_CFLAGS = $(test_cflags)
eail_bubble_tc2_LDADD = $(test_libs)

eail_plug_tc1_SOURCES = eail_plug_tc1.c
eail_plug_tc1_CFLAGS = $(test_cflags)
eail_plug_tc1_LDADD = $(test_libs)

eail_plug_tc2_SOURCES = eail_plug_tc2.c
eail_plug_tc2_CFLAGS = $(test_cflags)
eail_plug_tc2_LDADD = $(test_libs)

eail_slider_tc1_SOURCES = eail_slider_tc1.c
eail_slider_tc1_CFLAGS = $(test_cflags)
eail_slider_tc1_LDADD = $(test_libs)

eail_slider_tc2_SOURCES = eail_slider_tc2.c
eail_slider_tc2_CFLAGS = $(test_cflags)
eail_slider_tc2_LDADD = $(test_libs)

eail_label_tc1_SOURCES = eail_label_tc1.c
eail_label_tc1_CFLAGS = $(test_cflags)
eail_label_tc1_LDADD = $(test_libs)

eail_label_tc2_SOURCES = eail_label_tc2.c
eail_label_tc2_CFLAGS = $(test_cflags)
eail_label_tc2_LDADD = $(test_libs)

eail_prefs_tc1_SOURCES = eail_prefs_tc1.c
eail_prefs_tc1_CFLAGS = $(test_cflags)
eail_prefs_tc1_LDADD = $(test_libs)

eail_background_tc1_SOURCES = eail_background_tc1.c
eail_background_tc1_CFLAGS = $(test_cflags)
eail_background_tc1_LDADD = $(test_libs)

eail_background_tc2_SOURCES = eail_background_tc2.c
eail_background_tc2_CFLAGS = $(test_cflags)
eail_background_tc2_LDADD = $(test_libs)

eail_table_tc1_SOURCES = eail_table_tc1.c
eail_table_tc1_CFLAGS = $(test_cflags)
eail_table_tc1_LDADD = $(test_libs)

eail_thumb_tc1_SOURCES = eail_thumb_tc1.c
eail_thumb_tc1_CFLAGS = $(test_cflags)
eail_thumb_tc1_LDADD = $(test_libs)

eail_thumb_tc2_SOURCES = eail_thumb_tc2.c
eail_thumb_tc2_CFLAGS = $(test_cflags)
eail_thumb_tc2_LDADD = $(test_libs)

eail_thumb_tc3_SOURCES = eail_thumb_tc3.c
eail_thumb_tc3_CFLAGS = $(test_cflags)
eail_thumb_tc3_LDADD = $(test_libs)

eail_icon_tc1_SOURCES = eail_icon_tc1.c
eail_icon_tc1_CFLAGS = $(test_cflags)
eail_icon_tc1_LDADD = $(test_libs)

eail_icon_tc2_SOURCES = eail_icon_tc2.c
eail_icon_tc2_CFLAGS = $(test_cflags)
eail_icon_tc2_LDADD = $(test_libs)

eail_icon_tc3_SOURCES = eail_icon_tc3.c
eail_icon_tc3_CFLAGS = $(test_cflags)
eail_icon_tc3_LDADD = $(test_libs)

eail_inwin_tc1_SOURCES = eail_inwin_tc1.c
eail_inwin_tc1_CFLAGS = $(test_cflags)
eail_inwin_tc1_LDADD = $(test_libs)

eail_entry_tc1_SOURCES = eail_entry_tc1.c
eail_entry_tc1_CFLAGS = $(test_cflags)
eail_entry_tc1_LDADD = $(test_libs)

eail_entry_tc2_SOURCES = eail_entry_tc2.c
eail_entry_tc2_CFLAGS = $(test_cflags)
eail_entry_tc2_LDADD = $(test_libs)

eail_entry_tc3_SOURCES = eail_entry_tc3.c
eail_entry_tc3_CFLAGS = $(test_cflags)
eail_entry_tc3_LDADD = $(test_libs)

eail_photo_tc1_SOURCES = eail_photo_tc1.c
eail_photo_tc1_CFLAGS = $(test_cflags)
eail_photo_tc1_LDADD = $(test_libs)

eail_photo_tc2_SOURCES = eail_photo_tc2.c
eail_photo_tc2_CFLAGS = $(test_cflags)
eail_photo_tc2_LDADD = $(test_libs)

eail_photo_tc3_SOURCES = eail_photo_tc3.c
eail_photo_tc3_CFLAGS = $(test_cflags)
eail_photo_tc3_LDADD = $(test_libs)

eail_photocam_tc1_SOURCES = eail_photocam_tc1.c
eail_photocam_tc1_CFLAGS = $(test_cflags)
eail_photocam_tc1_LDADD = $(test_libs)

eail_photocam_tc2_SOURCES = eail_photocam_tc2.c
eail_photocam_tc2_CFLAGS = $(test_cflags)
eail_photocam_tc2_LDADD = $(test_libs)

eail_photocam_tc3_SOURCES = eail_photocam_tc3.c
eail_photocam_tc3_CFLAGS = $(test_cflags)
eail_photocam_tc3_LDADD = $(test_libs)

libeail_test_utils_la_SOURCES = eail_test_utils.c eail_test_utils.h
libeail_test_utils_la_CFLAGS = $(TEST_DEPS_CFLAGS)
libeail_test_utils_la_LIBADD = $(TEST_DEPS_LIBS)

if MAKE_TESTS
all-local: check
endif

if ENABLE_COVERAGE
coverage: check
	$(LCOV) --directory $(srcdir) --capture \
                --output-file $(top_builddir)/eail.info
	$(LCOV) --directory $(srcdir) --output-file $(top_builddir)/eail.info \
                --remove $(top_builddir)/eail.info "*.c" "*.h"
	rm -rf $(top_builddir)/coverage
	$(mkdir_p) $(top_builddir)/coverage
	$(GENHTML) --output-directory $(top_builddir)/coverage \
                $(top_builddir)/eail.info
endif
