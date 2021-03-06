// Transformable.h

#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include <Rect.h>

#include <agg_trans_perspective.h>

class Transformable : public agg::trans_perspective {
public:
			enum {
				MatrixSize = 9
			};
public:
								Transformable();
								Transformable(const Transformable& other);
	virtual						~Transformable();

			void				StoreTo(double matrix[MatrixSize]) const;
			void				LoadFrom(const double matrix[MatrixSize]);

								// set to or combine with other matrix
			void				SetTransformable(const Transformable& other);
			Transformable&		operator=(const Transformable& other);
			Transformable&		Multiply(const Transformable& other);
			Transformable&		PreMultiply(const Transformable& other);
			Transformable&		MultiplyInverse(const Transformable& other);
			void				Reset();

			void				Invert();

			bool				IsValid() const;
			bool				IsIdentity() const;
			bool				IsTranslationOnly() const;
			bool				IsNotDistorted() const;
			bool				IsPerspective() const;

								// transforms coordiantes
			void				Transform(double* x, double* y) const;
			void				Transform(BPoint* point) const;
			BPoint				Transform(const BPoint& point) const;

			void				InverseTransform(double* x, double* y) const;
			void				InverseTransform(BPoint* point) const;
			BPoint				InverseTransform(const BPoint& point) const;

								// transforms the rectangle "bounds" and
								// returns the *bounding box* of that
			BRect				TransformBounds(BRect bounds) const;

								// some convenience functions
	virtual	void				TranslateBy(BPoint offset);
	virtual	void				RotateBy(BPoint origin, double degrees);
	virtual	void				ScaleBy(BPoint origin, double scaleX,
									double scaleY);

			double				Scale() const;
			void				GetScale(double* scaleX, double* scaleY) const;
			BPoint				Translation() const
									{ return BPoint(tx, ty); }
			bool				GetAffineParameters(double* translationX,
									double* translationY, double* rotation,
									double* scaleX, double* scaleY,
									double* skewX, double* skewY) const;

	virtual	void				TransformationChanged() {}
};

#endif // TRANSFORMABLE_H

