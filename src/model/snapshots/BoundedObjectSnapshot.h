/*
 * Copyright 2014, Stephan Aßmus <superstippi@gmx.de>.
 * All rights reserved.
 */
#ifndef BOUNDED_OBJECT_SNAPSHOT_H
#define BOUNDED_OBJECT_SNAPSHOT_H

#include "ObjectSnapshot.h"

class BoundedObject;

class BoundedObjectSnapshot : public ObjectSnapshot {
public:
								BoundedObjectSnapshot(
									const BoundedObject* object);
	virtual						~BoundedObjectSnapshot();

	virtual	bool				Sync();

	virtual	void				Layout(LayoutContext& context, uint32 flags);

private:
			float				fOpacity;
			const BoundedObject*	fOriginal;
};

#endif // BOUNDED_OBJECT_SNAPSHOT_H