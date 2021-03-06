QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WonderBrush
TEMPLATE = app

include (src_common.pro)


INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += agg/font_freetype
INCLUDEPATH += agg/include
INCLUDEPATH += cimg
INCLUDEPATH += icon/include

#INCLUDEPATH += model
#INCLUDEPATH += model/fills
#INCLUDEPATH += model/objects
#INCLUDEPATH += model/property
#INCLUDEPATH += model/property/specific_properties
#INCLUDEPATH += model/snapshots
#INCLUDEPATH += render
#INCLUDEPATH += support

QMAKE_CXXFLAGS += -iquote $$PWD/edits
QMAKE_CXXFLAGS += -iquote $$PWD/edits/base
QMAKE_CXXFLAGS += -iquote $$PWD/gui
QMAKE_CXXFLAGS += -iquote $$PWD/gui/colorpicker
QMAKE_CXXFLAGS += -iquote $$PWD/gui/colorpicker/qt
QMAKE_CXXFLAGS += -iquote $$PWD/gui/icons
QMAKE_CXXFLAGS += -iquote $$PWD/gui/misc
QMAKE_CXXFLAGS += -iquote $$PWD/gui/misc/qt
QMAKE_CXXFLAGS += -iquote $$PWD/gui/qt
QMAKE_CXXFLAGS += -iquote $$PWD/gui/scrollview
QMAKE_CXXFLAGS += -iquote $$PWD/gui/scrollview/qt
QMAKE_CXXFLAGS += -iquote $$PWD/gui/stateview
QMAKE_CXXFLAGS += -iquote $$PWD/gui/tools
QMAKE_CXXFLAGS += -iquote $$PWD/gui/tools/qt
QMAKE_CXXFLAGS += -iquote $$PWD/model
QMAKE_CXXFLAGS += -iquote $$PWD/model/document
QMAKE_CXXFLAGS += -iquote $$PWD/model/fills
QMAKE_CXXFLAGS += -iquote $$PWD/model/objects
QMAKE_CXXFLAGS += -iquote $$PWD/model/property
QMAKE_CXXFLAGS += -iquote $$PWD/model/property/specific_properties
QMAKE_CXXFLAGS += -iquote $$PWD/model/snapshots
QMAKE_CXXFLAGS += -iquote $$PWD/model/text
QMAKE_CXXFLAGS += -iquote $$PWD/render
QMAKE_CXXFLAGS += -iquote $$PWD/render/text
QMAKE_CXXFLAGS += -iquote $$PWD/support
QMAKE_CXXFLAGS += -iquote $$PWD/tools
QMAKE_CXXFLAGS += -iquote $$PWD/tools/brush
QMAKE_CXXFLAGS += -iquote $$PWD/tools/text
QMAKE_CXXFLAGS += -iquote $$PWD/tools/text/qt
QMAKE_CXXFLAGS += -iquote $$PWD/tools/transform
QMAKE_CXXFLAGS += -iquote $$PWD/tools/transform/qt

LIBS += -Lagg -lagg -Lgui/colorpicker -lcolorpicker \
	-Lgui/scrollview -lscrollview -Licon -licon -ldl -lfreetype

# Weirdly we need to explicitly add libX11, since otherwise the linker complains
# about symbol XGetWindowAttributes not being defined.
LIBS += -lX11

TARGETDEPS += agg/libagg.a gui/colorpicker/libcolorpicker.a icon/libicon.a \
	gui/scrollview/libscrollview.a

SOURCES += \
	WonderBrush.cpp \
        edits/MoveObjectsEdit.cpp \
        edits/base/CompoundEdit.cpp \
        edits/base/EditManager.cpp \
        edits/base/EditStack.cpp \
        edits/base/UndoableEdit.cpp \
        gui/CanvasView.cpp \
	gui/ToolConfigView.cpp \
	gui/misc/NavigatorView.cpp \
	gui/misc/Panel.cpp \
	gui/misc/SwatchGroup.cpp \
	gui/misc/SwatchView.cpp \
	gui/misc/qt/AbstractTreeModel.cpp \
	gui/misc/qt/DualSlider.cpp \
	gui/misc/qt/FontPopup.cpp \
	gui/misc/qt/IconButton.cpp \
	gui/misc/qt/IconOptionsControl.cpp \
	gui/misc/qt/NavigatorViewPlatformDelegate.cpp \
	gui/qt/ObjectTreeView.cpp \
	gui/qt/ResourceTreeView.cpp \
	gui/qt/Window.cpp \
	gui/stateview/StateView.cpp \
	gui/stateview/ViewState.cpp \
	gui/tools/qt/BrushToolConfigView.cpp \
	gui/tools/qt/TextToolConfigView.cpp \
	gui/tools/qt/TransformToolConfigView.cpp \
	model/property/CommonPropertyIDs.cpp \
	model/BaseObject.cpp \
	model/CurrentColor.cpp \
	model/Selectable.cpp \
	model/Selection.cpp \
	model/document/Document.cpp \
	model/fills/Brush.cpp \
	model/fills/Color.cpp \
	model/fills/Gradient.cpp \
	model/fills/Paint.cpp \
	model/fills/StrokeProperties.cpp \
	model/fills/Style.cpp \
	model/objects/BoundedObject.cpp \
	model/objects/BrushStroke.cpp \
	model/objects/Filter.cpp \
	model/objects/Image.cpp \
	model/objects/Layer.cpp \
	model/objects/LayerObserver.cpp \
	model/objects/Object.cpp \
	model/objects/Rect.cpp \
	model/objects/Shape.cpp \
	model/objects/Styleable.cpp \
	model/objects/Text.cpp \
	model/property/Property.cpp \
	model/property/PropertyObject.cpp \
	model/property/PropertyObjectProperty.cpp \
	model/property/specific_properties/ColorProperty.cpp \
	model/property/specific_properties/IconProperty.cpp \
	model/property/specific_properties/Int64Property.cpp \
	model/property/specific_properties/OptionProperty.cpp \
	model/snapshots/BrushStrokeSnapshot.cpp \
	model/snapshots/FilterSnapshot.cpp \
	model/snapshots/ImageSnapshot.cpp \
	model/snapshots/LayerSnapshot.cpp \
	model/snapshots/ObjectSnapshot.cpp \
	model/snapshots/RectSnapshot.cpp \
	model/snapshots/ShapeSnapshot.cpp \
	model/snapshots/StyleableSnapshot.cpp \
	model/snapshots/TextSnapshot.cpp \
	model/text/CharacterStyle.cpp \
	model/text/Font.cpp \
	model/text/StyleRun.cpp \
	model/text/StyleRunList.cpp \
	platform/qt/platform_bitmap_support.cpp \
	platform/qt/platform_support.cpp \
	platform/qt/platform_support_ui.cpp \
	platform/qt/PlatformMessageEvent.cpp \
	platform/qt/PlatformMimeDataManager.cpp \
	platform/qt/PlatformResourceParser.cpp \
	platform/qt/PlatformScrollArea.cpp \
	platform/qt/PlatformSemaphoreManager.cpp \
	platform/qt/PlatformSignalMessageAdapter.cpp \
	platform/qt/PlatformThread.cpp \
	platform/qt/system/BApplication.cpp \
	platform/qt/system/ArchivingManagers.cpp \
	platform/qt/system/BAlignment.cpp \
	platform/qt/system/BAppDefs.cpp \
	platform/qt/system/BArchivable.cpp \
	platform/qt/system/BBitmap.cpp \
	platform/qt/system/BByteOrder.cpp \
	platform/qt/system/BControl.cpp \
	platform/qt/system/BCursor.cpp \
	platform/qt/system/BDataIO.cpp \
	platform/qt/system/BDirectory.cpp \
	platform/qt/system/BEntry.cpp \
	platform/qt/system/BFile.cpp \
	platform/qt/system/BFlattenable.cpp \
	platform/qt/system/BGradient.cpp \
	platform/qt/system/BGraphicsDefs.cpp \
	platform/qt/system/BInterfaceDefs.cpp \
	platform/qt/system/BInvoker.cpp \
	platform/qt/system/BHandler.cpp \
	platform/qt/system/BLayoutUtils.cpp \
	platform/qt/system/BList.cpp \
	platform/qt/system/BLocker.cpp \
	platform/qt/system/BLooper.cpp \
	platform/qt/system/BMessage.cpp \
	platform/qt/system/BMessageAdapter.cpp \
	platform/qt/system/BMessageFilter.cpp \
	platform/qt/system/BMessageRunner.cpp \
	platform/qt/system/BMessageUtils.cpp \
	platform/qt/system/BMessenger.cpp \
	platform/qt/system/BOS.cpp \
	platform/qt/system/BPath.cpp \
	platform/qt/system/BPoint.cpp \
	platform/qt/system/BPointerList.cpp \
	platform/qt/system/BRect.cpp \
	platform/qt/system/BRegion.cpp \
	platform/qt/system/BRegionSupport.cpp \
	platform/qt/system/BResources.cpp \
	platform/qt/system/BShape.cpp \
	platform/qt/system/BSize.cpp \
	platform/qt/system/BScreen.cpp \
	platform/qt/system/BString.cpp \
	platform/qt/system/BTranslationUtils.cpp \
	platform/qt/system/BView.cpp \
	platform/qt/system/BWindow.cpp \
	render/FontCache.cpp \
	render/GaussFilter.cpp \
	render/LayoutContext.cpp \
	render/LayoutState.cpp \
	render/Path.cpp \
	render/RenderBuffer.cpp \
	render/RenderEngine.cpp \
	render/RenderManager.cpp \
	render/RenderThread.cpp \
	render/StackBlurFilter.cpp \
	render/TextLayout.cpp \
	render/TextRenderer.cpp \
	render/VertexSource.cpp \
	render/text/FontRegistry.cpp \
	support/AbstractLOAdapter.cpp \
	support/Debug.cpp \
	support/HashString.cpp \
	support/Listener.cpp \
	support/ListenerAdapter.cpp \
	support/Notifier.cpp \
	support/ObjectTracker.cpp \
	support/Referenceable.cpp \
	support/RWLocker.cpp \
	support/support.cpp \
	support/support_ui.cpp \
	support/Transformable.cpp \
	tools/DragStateViewState.cpp \
	tools/Tool.cpp \
	tools/TransformViewState.cpp \
	tools/brush/BrushTool.cpp \
	tools/brush/BrushToolState.cpp \
	tools/text/TextTool.cpp \
	tools/text/TextToolState.cpp \
	tools/transform/ChannelTransform.cpp \
	tools/transform/TransformableGroup.cpp \
	tools/transform/TransformTool.cpp \
	tools/transform/TransformToolState.cpp

HEADERS  += \
	WonderBrush.h \
	cimg/CImg.h \
        edits/AddObjectsEdit.h \
        edits/InsertTextEdit.h \
        edits/MoveObjectsEdit.h \
        edits/ObjectAddedEdit.h \
        edits/RemoveTextEdit.h \
        edits/RenameObjectEdit.h \
        edits/SetPropertiesEdit.h \
        edits/SetTextStyleEdit.h \
        edits/SetTextWidthEdit.h \
        edits/TransformObjectEdit.h \
        edits/base/CompoundEdit.h \
        edits/base/EditManager.h \
        edits/base/EditStack.h \
        edits/base/UndoableEdit.h \
        gui/CanvasView.h \
	gui/ToolConfigView.h \
	gui/misc/NavigatorView.h \
	gui/misc/Panel.h \
	gui/misc/SwatchGroup.h \
	gui/misc/SwatchView.h \
	gui/misc/qt/AbstractTreeModel.h \
	gui/misc/qt/DualSlider.h \
	gui/misc/qt/FontPopup.h \
	gui/misc/qt/IconButton.h \
	gui/misc/qt/IconOptionsControl.h \
	gui/misc/qt/ListModel.h \
	gui/misc/qt/NavigatorViewPlatformDelegate.h \
	gui/misc/qt/SwatchGroupPlatformDelegate.h \
	gui/misc/qt/SwatchViewPlatformDelegate.h \
	gui/qt/CanvasViewPlatformDelegate.h \
	gui/qt/ObjectTreeView.h \
	gui/qt/ResourceTreeView.h \
	gui/qt/Window.h \
	gui/icons/BrushIcon.h \
	gui/icons/PathPropertyIcon.h \
	gui/stateview/StateView.h \
	gui/stateview/ViewState.h \
	gui/tools/qt/BrushToolConfigView.h \
	gui/tools/qt/TextToolConfigView.h \
	gui/tools/qt/TransformToolConfigView.h \
	model/BaseObject.h \
	model/CurrentColor.h \
	model/Selectable.h \
	model/Selection.h \
	model/document/Document.h \
	model/document/NotifyingList.h \
	model/fills/BlendingMode.h \
	model/fills/Brush.h \
	model/fills/Color.h \
	model/fills/Gradient.h \
	model/fills/Paint.h \
	model/fills/SetProperty.h \
	model/fills/SharedObjectCache.h \
	model/fills/StrokeProperties.h \
	model/fills/Style.h \
	model/objects/BoundedObject.h \
	model/objects/BrushStroke.h \
	model/objects/Filter.h \
	model/objects/Image.h \
	model/objects/Layer.h \
	model/objects/LayerObserver.h \
	model/objects/Object.h \
	model/objects/Rect.h \
	model/objects/Shape.h \
	model/objects/Styleable.h \
	model/objects/Text.h \
	model/property/CommonPropertyIDs.h \
	model/property/Property.h \
	model/property/PropertyObject.h \
	model/property/PropertyObjectProperty.h \
	model/property/specific_properties/ColorProperty.h \
	model/property/specific_properties/IconProperty.h \
	model/property/specific_properties/Int64Property.h \
	model/property/specific_properties/OptionProperty.h \
	model/snapshots/BrushStrokeSnapshot.h \
	model/snapshots/FilterSnapshot.h \
	model/snapshots/ImageSnapshot.h \
	model/snapshots/LayerSnapshot.h \
	model/snapshots/ObjectSnapshot.h \
	model/snapshots/RectSnapshot.h \
	model/snapshots/ShapeSnapshot.h \
	model/snapshots/StyleableSnapshot.h \
	model/snapshots/TextSnapshot.h \
	model/text/CharacterStyle.h \
	model/text/Font.h \
	model/text/StyleRun.h \
	model/text/StyleRunList.h \
	platform/qt/platform_support_ui.h \
	platform/qt/PlatformMessageEvent.h \
	platform/qt/PlatformMimeDataManager.h \
	platform/qt/PlatformResourceParser.h \
	platform/qt/PlatformScrollArea.h \
	platform/qt/PlatformSemaphoreManager.h \
	platform/qt/PlatformSignalMessageAdapter.h \
	platform/qt/PlatformThread.h \
	platform/qt/PlatformViewMixin.h \
	platform/qt/PlatformWonderBrush.h \
	platform/qt/system/ArchivingManagers.h \
	platform/qt/system/BAlignment.h \
	platform/qt/system/BAppDefs.h \
	platform/qt/system/BApplication.h \
	platform/qt/system/BArchivable.h \
	platform/qt/system/BAutoDeleter.h \
	platform/qt/system/BAutolock.h \
	platform/qt/system/BBeBuild.h \
	platform/qt/system/BBitmap.h \
	platform/qt/system/BByteOrder.h \
	platform/qt/system/Bclipping.h \
	platform/qt/system/BControl.h \
	platform/qt/system/BCursor.h \
	platform/qt/system/BDataIO.h \
	platform/qt/system/BDebug.h \
	platform/qt/system/Bdebugger.h \
	platform/qt/system/BEntry.h \
	platform/qt/system/BDirectory.h \
	platform/qt/system/BErrors.h \
	platform/qt/system/BFile.h \
	platform/qt/system/BFlattenable.h \
	platform/qt/system/BFont.h \
	platform/qt/system/BGradient.h \
	platform/qt/system/BGraphicsDefs.h \
	platform/qt/system/BHandler.h \
	platform/qt/system/Bimage.h \
	platform/qt/system/BInterfaceDefs.h \
	platform/qt/system/BInvoker.h \
	platform/qt/system/BLayoutUtils.h \
	platform/qt/system/BList.h \
	platform/qt/system/BLocker.h \
	platform/qt/system/BLooper.h \
	platform/qt/system/BMessage.h \
	platform/qt/system/BMessageAdapter.h \
	platform/qt/system/BMessageFilter.h \
	platform/qt/system/BMessagePrivate.h \
	platform/qt/system/BMessageRunner.h \
	platform/qt/system/BMessageUtils.h \
	platform/qt/system/BMessenger.h \
	platform/qt/system/BObjectList.h \
	platform/qt/system/BOS.h \
	platform/qt/system/BPath.h \
	platform/qt/system/BPoint.h \
	platform/qt/system/BRect.h \
	platform/qt/system/BRegion.h \
	platform/qt/system/BRegionSupport.h \
	platform/qt/system/BResources.h \
	platform/qt/system/BScreen.h \
	platform/qt/system/BShape.h \
	platform/qt/system/BSize.h \
	platform/qt/system/BStorageDefs.h \
	platform/qt/system/BString.h \
	platform/qt/system/BStringPrivate.h \
	platform/qt/system/BSupportDefs.h \
	platform/qt/system/BTranslationUtils.h \
	platform/qt/system/BTypeConstants.h \
	platform/qt/system/Butf8_functions.h \
	platform/qt/system/BView.h \
	platform/qt/system/BWindow.h \
	platform/qt/system/include/Alignment.h \
	platform/qt/system/include/AppDefs.h \
	platform/qt/system/include/Application.h \
	platform/qt/system/include/Archivable.h \
	platform/qt/system/include/AutoDeleter.h \
	platform/qt/system/include/Autolock.h \
	platform/qt/system/include/BeBuild.h \
	platform/qt/system/include/Bitmap.h \
	platform/qt/system/include/ByteOrder.h \
	platform/qt/system/include/clipping.h \
	platform/qt/system/include/Control.h \
	platform/qt/system/include/Cursor.h \
	platform/qt/system/include/DataIO.h \
	platform/qt/system/include/Debug.h \
	platform/qt/system/include/debugger.h \
	platform/qt/system/include/Directory.h \
	platform/qt/system/include/Entry.h \
	platform/qt/system/include/Errors.h \
	platform/qt/system/include/File.h \
	platform/qt/system/include/Flattenable.h \
	platform/qt/system/include/Font.h \
	platform/qt/system/include/Gradient.h \
	platform/qt/system/include/GraphicsDefs.h \
	platform/qt/system/include/Handler.h \
	platform/qt/system/include/image.h \
	platform/qt/system/include/InterfaceDefs.h \
	platform/qt/system/include/Invoker.h \
	platform/qt/system/include/LayoutUtils.h \
	platform/qt/system/include/List.h \
	platform/qt/system/include/Locker.h \
	platform/qt/system/include/Looper.h \
	platform/qt/system/include/Message.h \
	platform/qt/system/include/MessageAdapter.h \
	platform/qt/system/include/MessageFilter.h \
	platform/qt/system/include/MessagePrivate.h \
	platform/qt/system/include/MessageRunner.h \
	platform/qt/system/include/MessageUtils.h \
	platform/qt/system/include/Messenger.h \
	platform/qt/system/include/ObjectList.h \
	platform/qt/system/include/OS.h \
	platform/qt/system/include/Path.h \
	platform/qt/system/include/Point.h \
	platform/qt/system/include/Rect.h \
	platform/qt/system/include/Region.h \
	platform/qt/system/include/RegionSupport.h \
	platform/qt/system/include/Resources.h \
	platform/qt/system/include/Screen.h \
	platform/qt/system/include/Shape.h \
	platform/qt/system/include/Size.h \
	platform/qt/system/include/StorageDefs.h \
	platform/qt/system/include/String.h \
	platform/qt/system/include/StringPrivate.h \
	platform/qt/system/include/SupportDefs.h \
	platform/qt/system/include/TranslationUtils.h \
	platform/qt/system/include/TypeConstants.h \
	platform/qt/system/include/utf8_functions.h \
	platform/qt/system/include/View.h \
	platform/qt/system/include/Window.h \
	render/FauxWeight.h \
	render/FontCache.h \
	render/GaussFilter.h \
	render/LayoutContext.h \
	render/LayoutState.h \
	render/Path.h \
	render/RenderBuffer.h \
	render/RenderEngine.h \
	render/RenderManager.h \
	render/RenderThread.h \
	render/Scanline.h \
	render/StackBlurFilter.h \
	render/TextLayout.h \
	render/TextRenderer.h \
	render/VertexSource.h \
	render/text/FontRegistry.h \
	support/AbstractLOAdapter.h \
	support/AutoLocker.h \
	support/bitmap_support.h \
	support/BuildSupport.h \
	support/cursors.h \
	support/Debug.h \
	support/DLList.h \
	support/HashString.h \
	support/Listener.h \
	support/ListenerAdapter.h \
	support/Notifier.h \
	support/ObjectCache.h \
	support/ObjectTracker.h \
	support/Referenceable.h \
	support/rgb_hsv.h \
	support/RWLocker.h \
	support/support.h \
	support/support_ui.h \
	support/Transformable.h \
	support/ui_defines.h \
	tools/DragStateViewState.h \
	tools/Tool.h \
	tools/TransformViewState.h \
	tools/brush/BrushTool.h \
	tools/brush/BrushToolState.h \
	tools/text/TextTool.h \
	tools/text/TextToolState.h \
	tools/text/qt/TextToolStatePlatformDelegate.h \
	tools/transform/ChannelTransform.h \
	tools/transform/TransformableGroup.h \
	tools/transform/TransformTool.h \
	tools/transform/TransformToolState.h \
	tools/transform/qt/TransformToolStatePlatformDelegate.h

FORMS += \
	gui/misc/qt/FontPopup.ui \
	gui/qt/Window.ui \
    gui/tools/qt/BrushToolConfigView.ui \
	gui/tools/qt/TextToolConfigView.ui \
	gui/tools/qt/TransformToolConfigView.ui

RESOURCES += \
    wonderbrush2.qrc
