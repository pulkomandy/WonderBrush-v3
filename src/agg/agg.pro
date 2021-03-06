QT       -= core gui

TARGET = agg
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += include

# suppress -Wunused-parameter, -Wunused-function
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-function

SOURCES += \
    font_freetype/agg_font_freetype.cpp \
    src/agg_arc.cpp \
    src/agg_arrowhead.cpp \
    src/agg_bezier_arc.cpp \
    src/agg_bspline.cpp \
    src/agg_color_rgba.cpp \
    src/agg_curves.cpp \
    src/agg_embedded_raster_fonts.cpp \
    src/agg_gsv_text.cpp \
    src/agg_image_filters.cpp \
    src/agg_line_aa_basics.cpp \
    src/agg_line_profile_aa.cpp \
    src/agg_rounded_rect.cpp \
    src/agg_sqrt_tables.cpp \
    src/agg_trans_affine.cpp \
    src/agg_trans_double_path.cpp \
    src/agg_trans_perspective.cpp \
    src/agg_trans_single_path.cpp \
    src/agg_trans_warp_magnifier.cpp \
    src/agg_vcgen_bspline.cpp \
    src/agg_vcgen_contour.cpp \
    src/agg_vcgen_dash.cpp \
    src/agg_vcgen_markers_term.cpp \
    src/agg_vcgen_smooth_poly1.cpp \
    src/agg_vcgen_stroke.cpp \
    src/agg_vpgen_clip_polygon.cpp \
    src/agg_vpgen_clip_polyline.cpp \
    src/agg_vpgen_segmentator.cpp

HEADERS += \
    include/agg_alpha_mask_u8.h \
    include/agg_arc.h \
    include/agg_array.h \
    include/agg_arrowhead.h \
    include/agg_basics.h \
    include/agg_bezier_arc.h \
    include/agg_bitset_iterator.h \
    include/agg_blur.h \
    include/agg_bounding_rect.h \
    include/agg_bspline.h \
    include/agg_clip_liang_barsky.h \
    include/agg_color_gray.h \
    include/agg_color_rgba.h \
    include/agg_config.h \
    include/agg_conv_adaptor_vcgen.h \
    include/agg_conv_adaptor_vpgen.h \
    include/agg_conv_bspline.h \
    include/agg_conv_clip_polygon.h \
    include/agg_conv_clip_polyline.h \
    include/agg_conv_close_polygon.h \
    include/agg_conv_concat.h \
    include/agg_conv_contour.h \
    include/agg_conv_curve.h \
    include/agg_conv_dash.h \
    include/agg_conv_gpc.h \
    include/agg_conv_marker_adaptor.h \
    include/agg_conv_marker.h \
    include/agg_conv_segmentator.h \
    include/agg_conv_shorten_path.h \
    include/agg_conv_smooth_poly1.h \
    include/agg_conv_stroke.h \
    include/agg_conv_transform.h \
    include/agg_conv_unclose_polygon.h \
    include/agg_curves.h \
    include/agg_dda_line.h \
    include/agg_ellipse_bresenham.h \
    include/agg_ellipse.h \
    include/agg_embedded_raster_fonts.h \
    include/agg_font_cache_manager.h \
    include/agg_gamma_functions.h \
    include/agg_gamma_lut.h \
    include/agg_glyph_raster_bin.h \
    include/agg_gradient_lut.h \
    include/agg_gsv_text.h \
    include/agg_image_accessors.h \
    include/agg_image_filters.h \
    include/agg_line_aa_basics.h \
    include/agg_math.h \
    include/agg_math_stroke.h \
    include/agg_path_length.h \
    include/agg_path_storage.h \
    include/agg_path_storage_integer.h \
    include/agg_pattern_filters_rgba.h \
    include/agg_pixfmt_amask_adaptor.h \
    include/agg_pixfmt_brush.h \
    include/agg_pixfmt_gray.h \
    include/agg_pixfmt_lcd_argb.h \
    include/agg_pixfmt_lcd_bgra.h \
    include/agg_pixfmt_rgba.h \
    include/agg_pixfmt_rgb.h \
    include/agg_pixfmt_rgb_packed.h \
    include/agg_pixfmt_transposer.h \
    include/agg_primary_weights.h \
    include/agg_rasterizer_cells_aa.h \
    include/agg_rasterizer_compound_aa.h \
    include/agg_rasterizer_outline_aa.h \
    include/agg_rasterizer_outline.h \
    include/agg_rasterizer_scanline_aa.h \
    include/agg_rasterizer_sl_clip.h \
    include/agg_renderer_base.h \
    include/agg_renderer_markers.h \
    include/agg_renderer_mclip.h \
    include/agg_renderer_outline_aa.h \
    include/agg_renderer_outline_image.h \
    include/agg_renderer_primitives.h \
    include/agg_renderer_raster_text.h \
    include/agg_renderer_scanline.h \
    include/agg_rendering_buffer_dynarow.h \
    include/agg_rendering_buffer.h \
    include/agg_rounded_rect.h \
    include/agg_scanline_bin.h \
    include/agg_scanline_boolean_algebra.h \
    include/agg_scanline_p.h \
    include/agg_scanline_storage_aa.h \
    include/agg_scanline_storage_bin.h \
    include/agg_scanline_u.h \
    include/agg_shorten_path.h \
    include/agg_simul_eq.h \
    include/agg_span_allocator.h \
    include/agg_span_converter.h \
    include/agg_span_gouraud_gray.h \
    include/agg_span_gouraud.h \
    include/agg_span_gouraud_rgba.h \
    include/agg_span_gradient_alpha.h \
    include/agg_span_gradient_contour.h \
    include/agg_span_gradient.h \
    include/agg_span_gradient_image.h \
    include/agg_span_image_filter_gray.h \
    include/agg_span_image_filter.h \
    include/agg_span_image_filter_rgba.h \
    include/agg_span_image_filter_rgb.h \
    include/agg_span_interpolator_adaptor.h \
    include/agg_span_interpolator_linear.h \
    include/agg_span_interpolator_persp.h \
    include/agg_span_interpolator_trans.h \
    include/agg_span_pattern_gray.h \
    include/agg_span_pattern_rgba.h \
    include/agg_span_pattern_rgb.h \
    include/agg_span_solid.h \
    include/agg_span_subdiv_adaptor.h \
    include/agg_trans_affine.h \
    include/agg_trans_bilinear.h \
    include/agg_trans_double_path.h \
    include/agg_trans_lens.h \
    include/agg_trans_perspective.h \
    include/agg_trans_single_path.h \
    include/agg_trans_viewport.h \
    include/agg_trans_warp_magnifier.h \
    include/agg_vcgen_bspline.h \
    include/agg_vcgen_contour.h \
    include/agg_vcgen_dash.h \
    include/agg_vcgen_markers_term.h \
    include/agg_vcgen_smooth_poly1.h \
    include/agg_vcgen_stroke.h \
    include/agg_vcgen_vertex_sequence.h \
    include/agg_vertex_sequence.h \
    include/agg_vpgen_clip_polygon.h \
    include/agg_vpgen_clip_polyline.h \
    include/agg_vpgen_segmentator.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
