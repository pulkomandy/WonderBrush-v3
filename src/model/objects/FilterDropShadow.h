/*
 * Copyright 2013, Stephan Aßmus <superstippi@gmx.de>.
 * All rights reserved.
 */

#ifndef FILTER_DROP_SHADOW_H
#define FILTER_DROP_SHADOW_H

#include "ColorProvider.h"
#include "Object.h"

class FilterDropShadow : public Object {
public:
								FilterDropShadow();
								FilterDropShadow(const FilterDropShadow& other,
									CloneContext& context);
								FilterDropShadow(float radius);
	virtual						~FilterDropShadow();

	// BaseObject interface
	virtual	BaseObject*			Clone(CloneContext& context) const;
	virtual	const char*			DefaultName() const;
	virtual	void				AddProperties(PropertyObject* object,
									uint32 flags = 0) const;
	virtual	bool				SetToPropertyObject(
									const PropertyObject* object,
									uint32 flags = 0);

	// Object interface
	virtual	ObjectSnapshot*		Snapshot() const;

	virtual	bool				IsRegularTransformable() const;

	virtual	void				ExtendDirtyArea(BRect& area) const;

	// FilterDropShadow
			void				SetFilterRadius(float filterRadius);
	inline	float				FilterRadius() const
									{ return fFilterRadius; }

			void				SetOffsetX(float offsetX);
	inline	float				OffsetX() const
									{ return fOffsetX; }

			void				SetOffsetY(float offsetY);
	inline	float				OffsetY() const
									{ return fOffsetY; }

			void				SetOpacity(float opacity);
	inline	float				Opacity() const
									{ return fOpacity; }

			void				SetColor(const ColorProviderRef& color);
	inline	const ColorProviderRef&	Color() const
									{ return fColorProvider; }

private:
			void				_SetMember(float& member, float value);

private:
 			float				fFilterRadius;
 			float				fOffsetX;
 			float				fOffsetY;
 			float				fOpacity;
 			ColorProviderRef	fColorProvider;
};

#endif // FILTER_DROP_SHADOW_H
