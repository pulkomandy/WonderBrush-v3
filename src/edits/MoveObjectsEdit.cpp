/*
 * Copyright 2010, Stephan Aßmus <superstippi@gmx.de>.
 * All rights reserved.
 */

#include "MoveObjectsEdit.h"

#include <new>

#include <stdio.h>

#include "Layer.h"

// constructor
MoveObjectsEdit::MoveObjectsEdit(Object** objects,
		int32 objectCount, Layer* insertionLayer, int32 insertionIndex,
		Selection* selection)
	: UndoableEdit()
	, fObjects(objects)
	, fObjectCount(objectCount)

	, fOldPositions(new(std::nothrow) PositionInfo[objectCount])

	, fInsertionLayer(insertionLayer)
	, fInsertionIndex(insertionIndex)

	, fSelection(selection)
{
//printf("MoveObjectsEdit::MoveObjectsEdit(%ld, %p, %ld)\n",
//	objectCount, insertionLayer, insertionIndex);

	if (fInsertionLayer != NULL)
		fInsertionLayer->AddReference();

	if (fObjects == NULL || fObjectCount <= 0 || fOldPositions == NULL)
		return;

	// Remember current positions of all objects.
	for (int32 i = 0; i < fObjectCount; i++) {
		// Check if this object is a distant child of any of the other
		// objects that we are moving and remove it from the array in that
		// case.
		bool ignoreObject = false;
		for (int32 j = 0; j < fObjectCount; j++) {
			if (j == i)
				continue;
			Layer* layer = dynamic_cast<Layer*>(fObjects[j]);
			if (layer == NULL)
				continue;
			if (_ObjectIsDistantChildOf(fObjects[i], layer)) {
//printf("ignoring %p\n", fObjects[i]);
				ignoreObject = true;
				break;
			}
		}
		if (ignoreObject) {
			fObjects[i] = NULL;
			continue;
		}

		fObjects[i]->AddReference();

		Layer* parent = fObjects[i]->Parent();
		fOldPositions[i].parent = parent;
		if (parent != NULL)
			fOldPositions[i].index = parent->IndexOf(fObjects[i]);
		else
			fOldPositions[i].index = 0;
		if (parent == fInsertionLayer
			&& fOldPositions[i].index < fInsertionIndex) {
			fInsertionIndex--;
		}
	}
}

// destructor
MoveObjectsEdit::~MoveObjectsEdit()
{
	if (fObjects != NULL) {
		for (int32 i = 0; i < fObjectCount; i++) {
			if (fObjects[i] != NULL)
				fObjects[i]->RemoveReference();
		}
		delete[] fObjects;
	}

	delete[] fOldPositions;

	if (fInsertionLayer != NULL)
		fInsertionLayer->RemoveReference();
}

// InitCheck
status_t
MoveObjectsEdit::InitCheck()
{
	if (fObjects == NULL || fOldPositions == NULL || fObjectCount <= 0
		|| fInsertionLayer == NULL) {
		return B_NO_INIT;
	}

	// Check if object tree changes.
	PositionInfo newPositions[fObjectCount];
	int32 index = fInsertionIndex;
	for (int32 i = 0; i < fObjectCount; i++) {
		newPositions[i].parent = fInsertionLayer;
		newPositions[i].index = index++;
	}
	if (memcmp(newPositions, fOldPositions, sizeof(newPositions)) == 0) {
//printf("MoveObjectsEdit::InitCheck(): no changes!\n");
		return B_BAD_VALUE;
	}

	// Check that we are not trying to add a layer to one of it's sub-layers
	// or to itself.
	for (int32 i = 0; i < fObjectCount; i++) {
		if (fObjects[i] == fInsertionLayer) {
//printf("MoveObjectsEdit::InitCheck(): Cannot add layer to itself!\n");
			return B_BAD_VALUE;
		}
		Layer* layer = dynamic_cast<Layer*>(fObjects[i]);
		if (layer == NULL)
			continue;
		if (_ObjectIsDistantChildOf(fInsertionLayer, layer)) {
//printf("MoveObjectsEdit::InitCheck(): Cannot add layer to one of it's "
//	"sub-layers!\n");
			return B_BAD_VALUE;
		}
	}

	return B_OK;
}

// Perform
status_t
MoveObjectsEdit::Perform(EditContext& context)
{
	fInsertionLayer->SuspendUpdates(true);

	if (fSelection != NULL)
		fSelection->DeselectAll(this);

	for (int32 i = 0; i < fObjectCount; i++) {
		if (fObjects[i] == NULL)
			continue;
		if (fOldPositions[i].parent != NULL)
			fOldPositions[i].parent->RemoveObject(fObjects[i]);
	}
	int32 index = fInsertionIndex;
	for (int32 i = 0; i < fObjectCount; i++) {
		if (fObjects[i] == NULL)
			continue;
		if (!fInsertionLayer->AddObject(fObjects[i], index)) {
			fInsertionLayer->SuspendUpdates(false);
			return B_NO_MEMORY;
		}
		index++;
		if (fSelection != NULL)
			fSelection->Select(Selectable(fObjects[i]), this, true);
	}

	fInsertionLayer->SuspendUpdates(false);

	return B_OK;
}

// Undo
status_t
MoveObjectsEdit::Undo(EditContext& context)
{
	fInsertionLayer->SuspendUpdates(true);

	if (fSelection != NULL)
		fSelection->DeselectAll(this);

	for (int32 i = 0; i < fObjectCount; i++) {
		if (fObjects[i] == NULL)
			continue;
		fInsertionLayer->RemoveObject(fObjects[i]);
	}
	for (int32 i = 0; i < fObjectCount; i++) {
		if (fObjects[i] == NULL)
			continue;
		if (fOldPositions[i].parent != NULL) {
			fOldPositions[i].parent->AddObject(fObjects[i],
				fOldPositions[i].index);
		}
		if (fSelection != NULL)
			fSelection->Select(Selectable(fObjects[i]), this, true);
	}

	fInsertionLayer->SuspendUpdates(false);

	return B_OK;
}

// GetName
void
MoveObjectsEdit::GetName(BString& name)
{
	if (fObjectCount > 1)
		name << "Move objects";
	else
		name << "Move object";
}

// #pragma mark -

// _ObjectIsDistantChildOf
bool
MoveObjectsEdit::_ObjectIsDistantChildOf(const Object* object,
	const Layer* layer) const
{
	int32 count = layer->CountObjects();
	for (int32 i = 0; i < count; i++) {
		const Object* child = layer->ObjectAtFast(i);
		if (object == child)
			return true;
		const Layer* subLayer = dynamic_cast<const Layer*>(child);
		if (subLayer != NULL && _ObjectIsDistantChildOf(object, subLayer))
			return true;

	}
	return false;
}
