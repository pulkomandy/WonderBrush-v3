/*
 * Copyright 2009, Stephan Aßmus <superstippi@gmx.de>.
 * All rights reserved.
 */
#include "ObjectTreeView.h"

#include <new>
#include <stdio.h>

#include <Bitmap.h>
#include <Window.h>

#include "AutoDeleter.h"
#include "Column.h"
#include "ColumnTreeViewColors.h"
#include "CommandStack.h"
#include "Document.h"
#include "Object.h"
#include "RenameObjectCommand.h"
#include "TextViewPopup.h"

using std::nothrow;


ObjectColumnTreeItem::ObjectColumnTreeItem(float height, Object* object)
	: EasyColumnTreeItem(height)
	, object(object)
{
}


ObjectColumnTreeItem::~ObjectColumnTreeItem()
{
}


void
ObjectColumnTreeItem::Update()
{
	BBitmap icon(BRect(0, 0, 15, 15), 0, B_RGBA32);
	if (object->GetIcon(&icon))
		SetContent(1, &icon);
	SetContent(0, object->Name());
}


// #pragma mark - ObjectTreeView


enum {
	MSG_RENAME_OBJECT			= 'rnoj',
	MSG_RENAME_SELECTED_ITEM	= 'rnit',
	MSG_DRAG_SORT_OBJECTS		= 'drgo'
};


// constructor
ObjectTreeView::ObjectTreeView(BRect frame, Document* document,
		Selection* selection)
	:
	ColumnTreeView(frame),
	fDocument(document),
	fSelection(selection),
	fLayerObserver(this)
{
}

#ifdef __HAIKU__

// constructor
ObjectTreeView::ObjectTreeView(Document* document, Selection* selection)
	:
	ColumnTreeView(),
	fDocument(document),
	fSelection(selection),
	fLayerObserver(this)
{
}

#endif // __HAIKU__

// destructor
ObjectTreeView::~ObjectTreeView()
{
}

// AttachedToWindow
void
ObjectTreeView::AttachedToWindow()
{
	ColumnTreeView::AttachedToWindow();
	Window()->AddShortcut('e', B_COMMAND_KEY,
		new BMessage(MSG_RENAME_SELECTED_ITEM), this);

	fSelection->AddListener(this);

	_RecursiveAddItems(fDocument->RootLayer(), NULL);
}

// DetachedFromWindow
void
ObjectTreeView::DetachedFromWindow()
{
	fSelection->RemoveListener(this);
	Window()->RemoveShortcut('e', B_COMMAND_KEY);
	ColumnTreeView::DetachedFromWindow();
}

// MouseDown
void
ObjectTreeView::MouseDown(BPoint where)
{
	MakeFocus(true);
	ColumnTreeView::MouseDown(where);
}

// KeyDown
void
ObjectTreeView::KeyDown(const char* bytes, int32 numBytes)
{
	switch (bytes[0]) {
		// TODO: Some re-configurable global short cut handling...
		case 'e':
		case 'E':
			_HandleRenameSelectedItem();
			break;
		case B_FUNCTION_KEY:
			if (BMessage* message = Window()->CurrentMessage()) {
				int32 key;
				if (message->FindInt32("key", &key) == B_OK) {
					switch (key) {
						case B_F2_KEY:
							_HandleRenameSelectedItem();
							break;
						default:
							break;
					}
				}
			}
			break;

		default:
			ColumnTreeView::KeyDown(bytes, numBytes);
			break;
	}
}

// MessageReceived
void
ObjectTreeView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_RENAME_SELECTED_ITEM:
			_HandleRenameSelectedItem();
			break;
		case MSG_RENAME_OBJECT:
			_HandleRenameObject(message);
			break;

		case LayerObserver::MSG_OBJECT_ADDED:
		case LayerObserver::MSG_OBJECT_REMOVED:
		case LayerObserver::MSG_OBJECT_CHANGED:
			if (fDocument->WriteLock()) {
				Layer* layer;
				Object* object;
				int32 index;
				if (message->FindPointer("layer", (void**)&layer) == B_OK
					&& message->FindPointer("object", (void**)&object) == B_OK
					&& message->FindInt32("index", &index) == B_OK) {
					if (!fDocument->HasLayer(layer)) {
						fDocument->WriteUnlock();
						break;
					}
					if (message->what == LayerObserver::MSG_OBJECT_ADDED)
						_ObjectAdded(layer, object, index);
					else if (message->what == LayerObserver::MSG_OBJECT_REMOVED)
						_ObjectRemoved(layer, object, index);
					else
						_ObjectChanged(layer, object, index);
				}
				fDocument->WriteUnlock();
			}
			break;
		case LayerObserver::MSG_AREA_INVALIDATED:
			// not interested
			break;

		default:
			ColumnTreeView::MessageReceived(message);
	}
}

// InitiateDrag
bool
ObjectTreeView::InitiateDrag(BPoint point, int32 index, bool wasSelected,
	BMessage* _message)
{
	try {
		BMessage dragMessage(MSG_DRAG_SORT_OBJECTS);
		float totalHeight = -1.0f;
		float maxHeight = 100.0f;
		bool fadeOutAtBottom = false;
		int32 count = CountSelectedItems();
		int32 addedItems = 0;
		for (int32 i = 0; i < count; i++) {
			ObjectColumnTreeItem* item = dynamic_cast<ObjectColumnTreeItem*>(
				ItemAt(CurrentSelection(i)));
			if (item == NULL || item->object == NULL)
				continue;
			if (dragMessage.AddPointer("object", item->object) != B_OK)
				return false;
			addedItems++;
			if (!fadeOutAtBottom) {
				totalHeight += item->Height() + 1.0;
				fadeOutAtBottom = totalHeight > maxHeight;
			}
		}
		if (addedItems == 0)
			return false;

		BRect bitmapBounds(0, 0, Bounds().Width(), totalHeight);
		BBitmap* dragBitmap = new BBitmap(bitmapBounds, B_BITMAP_ACCEPTS_VIEWS,
			B_RGBA32);
		ObjectDeleter<BBitmap> bitmapDeleter(dragBitmap);
		BView* view = new BView(bitmapBounds, "offscreen", 0, 0);
		dragBitmap->AddChild(view);
		if (!dragBitmap->Lock())
			return false;

		// configure view to match ourself
		view->SetDrawingMode(DrawingMode());
		view->SetLowColor(LowColor());
		view->SetHighColor(HighColor());
		BFont font;
		GetFont(&font);
		view->SetFont(&font);

		float previousItemBottom = -1.0;
		for (int32 i = 0; i < count; i++) {
			ColumnTreeItem* item = ItemAt(CurrentSelection(i));
			if (item == NULL)
				continue;

			BRect itemRect(bitmapBounds);
			itemRect.top = previousItemBottom + 1.0;
			itemRect.bottom = itemRect.top + item->Height();

			for (int32 c = 0; c < CountColumns(); c++) {
				Column* column = _VisibleColumnAt(c);
				BRect columnRect = _VisibleColumnFrame(column);
				columnRect.top = itemRect.top;
				columnRect.bottom = itemRect.bottom;
				item->Draw(view, column, columnRect, columnRect,
					0, &Colors()->item_colors);
			}

			previousItemBottom = itemRect.bottom;
			if (previousItemBottom > totalHeight)
				break;
		}
		view->SetHighColor(tint_color(view->LowColor(), B_DARKEN_2_TINT));
		view->StrokeRect(view->Bounds());
		view->Sync();
		dragBitmap->Unlock();

		// Mess with the alpha channel
		uint8* row = (uint8*)dragBitmap->Bits();
		int32 bpr = dragBitmap->BytesPerRow();
		int32 width = bitmapBounds.IntegerWidth() + 1;
		int32 height = bitmapBounds.IntegerHeight() + 1;
		for (int32 y = 0; y < height; y++) {
			uint8* p = row;
			uint8 alpha = 110;
			if (fadeOutAtBottom && y >= height - 40)
				alpha = alpha * (height - y) / 40;
			for (int32 x = 0; x < width; x++) {
				p[3] = (p[3] * alpha) >> 8;
				p += 4;
			}
			row += bpr;
		}

		bitmapDeleter.Detach();

		DragMessage(&dragMessage, dragBitmap, B_OP_ALPHA, BPoint(point.x,
			point.y - ItemFrame(CurrentSelection(0)).top));

		if (_message)
			*_message = dragMessage;

		return true;

	} catch (...) {
	}
	return false;
}

// SelectionChanged
void
ObjectTreeView::SelectionChanged()
{
	// Sync the selections
	int32 count = CountSelectedItems();
	if (count == 0) {
		fSelection->DeselectAll(this);
		return;
	}
	bool extend = false;
	for (int32 i = 0; i < count; i++) {
		ObjectColumnTreeItem* item = dynamic_cast<ObjectColumnTreeItem*>(
			ItemAt(CurrentSelection(i)));
		if (item == NULL)
			continue;
		fSelection->Select(Selectable(item->object), this, extend);
		extend = true;
	}
}

// #pragma mark -

// ObjectSelected
void
ObjectTreeView::ObjectSelected(const Selectable& object,
	const Selection::Controller* controller)
{
	if (controller == this) {
		// ignore changes triggered by ourself
		return;
	}

	SelectItem(dynamic_cast<Object*>(object.Get()));
}

// ObjectDeselected
void
ObjectTreeView::ObjectDeselected(const Selectable& object,
	const Selection::Controller* controller)
{
	if (controller == this) {
		// ignore changes triggered by ourself
		return;
	}

//printf("ObjectTreeView::ObjectDeselected(%p)\n", object.Get());
	// TODO...
}

// #pragma mark -

// SelectItem
void
ObjectTreeView::SelectItem(Object* object)
{
	ColumnTreeItem* item = _FindLayerTreeViewItem(object);
	if (item != NULL)
		Select(IndexOf(item));
	else
		DeselectAll();
}

// #pragma mark -

// _HandleRenameSelectedItem
void
ObjectTreeView::_HandleRenameSelectedItem()
{
	_HandleRenameItem(CurrentSelection(0));
}

// _HandleRenameItem
void
ObjectTreeView::_HandleRenameItem(int32 index)
{
	ObjectColumnTreeItem* item = dynamic_cast<ObjectColumnTreeItem*>(
		ItemAt(index));

	if (item == NULL || item->object == NULL)
		return;

	item->object->AddReference();

	BMessage* message = NULL;
	try {
		message = new BMessage(MSG_RENAME_OBJECT);
		message->AddPointer("object", item->object);
		message->AddPointer("item", item);
		message->AddInt32("index", index);

		BRect frame(ItemFrame(index));
		frame.left += IndentationOf(item) + 9.0;
		ConvertToScreen(&frame);

		// Hide the current name in order not to irritate during editing.
		item->SetContent(0, "");

		AutoReadLocker locker(fDocument);

		new TextViewPopup(frame, item->object->Name(), message, this);
	} catch (...) {
		delete message;
		item->object->RemoveReference();
	}
}

// _HandleRenameObject
void
ObjectTreeView::_HandleRenameObject(BMessage* message)
{
	Object* object;
	if (message->FindPointer("object", (void**)&object) != B_OK)
		return;

	ObjectColumnTreeItem* item;
	if (message->FindPointer("item", (void**)&item) != B_OK)
		return;

	const char* name;
	if (message->FindString("text", &name) != B_OK) {
		// Restore name which is still valid, but was hidden
		// in order not to irritate during editing.
		if (HasItem(item)) {
			item->SetContent(0, object->Name());
			InvalidateItem(item);
		}
		object->RemoveReference();
		return;
	}

	AutoWriteLocker locker(fDocument);

	if (strcmp(object->Name(), name) == 0) {
		// Happens when we use the TAB key to navigate without renaming
		// In this case, the item needs to be invalidated too, because
		// the name was hidden.
		if (HasItem(item)) {
			item->SetContent(0, object->Name());
			InvalidateItem(item);
		}
	} else {
		// rename via command
		RenameObjectCommand* command = new (nothrow) RenameObjectCommand(object,
			name);
		fDocument->CommandStack()->Perform(command);
	}
	object->RemoveReference();

	locker.Unlock();

	// Edit the name of the next item too?
	int32 next;
	int32 index;
	if (message->FindInt32("next", &next) != B_OK
		|| message->FindInt32("index", &index) != B_OK)
		return;

	if (index + next >= CountItems())
		return;

	_HandleRenameItem(index + next);
}

// _ObjectAdded
void
ObjectTreeView::_ObjectAdded(Layer* layer, Object* object, int32 index)
{
	if (!layer->HasObject(object))
		return;

	ObjectColumnTreeItem* parentItem = _FindLayerTreeViewItem(layer);

	ObjectColumnTreeItem* item = new ObjectColumnTreeItem(20, object);
	item->Update();

	AddSubItem(parentItem, item, index);
	ExpandItem(item);

	if (Layer* subLayer = dynamic_cast<Layer*>(object))
		_RecursiveAddItems(subLayer, item);
}

// _ObjectRemoved
void
ObjectTreeView::_ObjectRemoved(Layer* layer, Object* object, int32 index)
{
	// TODO ...
}

// _ObjectChanged
void
ObjectTreeView::_ObjectChanged(Layer* layer, Object* object, int32 index)
{
	ObjectColumnTreeItem* item = _FindLayerTreeViewItem(object);
	if (!item)
		return;
	item->Update();
	InvalidateItem(item);
}

// _FindLayerTreeViewItem
ObjectColumnTreeItem*
ObjectTreeView::_FindLayerTreeViewItem(const Object* object)
{
	int32 count = CountItems();
	for (int32 i = 0; i < count; i++) {
		ObjectColumnTreeItem* item = dynamic_cast<ObjectColumnTreeItem*>(
			ItemAt(i));
		if (item && item->object == object)
			return item;
	}
	return NULL;
}

// _RecursiveAddItems
void
ObjectTreeView::_RecursiveAddItems(Layer* layer,
	ObjectColumnTreeItem* layerItem)
{
	layer->AddListener(&fLayerObserver);

	int32 count = layer->CountObjects();
	for (int32 i = 0; i < count; i++) {
		Object* object = layer->ObjectAtFast(i);

		ObjectColumnTreeItem* item = new ObjectColumnTreeItem(20, object);
		item->Update();

		if (layerItem)
			AddSubItem(layerItem, item, i);
		else
			AddItem(item, i);
		ExpandItem(item);

		Layer* subLayer = dynamic_cast<Layer*>(object);
		if (subLayer != NULL)
			_RecursiveAddItems(subLayer, item);
	}
}