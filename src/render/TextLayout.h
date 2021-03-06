/*
 * Copyright 2012 Stephan Aßmus <superstippi@gmx.de>
 * All rights reserved.
 */
#ifndef TEXT_LAYOUT_H
#define TEXT_LAYOUT_H

#include "TextRenderer.h"
#include "Font.h"

class FontCache;

static const unsigned MOVEMENT_CHAR				= 1 << 0;
static const unsigned MOVEMENT_CLUSTER			= 1 << 1;
static const unsigned MOVEMENT_WORD				= 1 << 2;
static const unsigned MOVEMENT_WORD_END			= 1 << 3;
static const unsigned MOVEMENT_WORD_START		= 1 << 4;


static const unsigned ALIGNMENT_LEFT			= 0;
static const unsigned ALIGNMENT_RIGHT			= 1;
static const unsigned ALIGNMENT_CENTER			= 2;

static const unsigned SELECTION_FULL			= 1 << 16;
static const unsigned SELECTION_LINE_DELIMITER	= 1 << 17;
static const unsigned SELECTION_LAST_LINE		= 1 << 20;
static const unsigned TEXT_TRANSPARENT			= 1 << 30;

class TextLayout {
private:
	struct StyleRun {
		int							start;

		Font						font;

		// The following three values override glyph metrics unless 0
		double						ascent;
		double						descent;
		double						width;

		double						glyphSpacing;
		double						fauxWeight;
		double						fauxItalic;

		TextRenderer::Color			fgColor;

		TextRenderer::Color			bgColor;

		bool						strikeOut;
		TextRenderer::Color			strikeOutColor;

		bool						underline;
		unsigned					underlineStyle;
		TextRenderer::Color			underlineColor;
	};

	struct GlyphInfo {
		unsigned					charCode;

		// TODO: This should be referenceable. Then the layout process can
		// happen in the UI thread while the rendering can happen asynchronously.
		// The UI thread may decide to throw away font cache entries, but the
		// refernces of any actually used glyphs would remain valid in each
		// TextLayout.
		const agg::glyph_cache*		glyph;

		double						x;
		double						y;
		double						advanceX;

		double						maxAscend;
		double						maxDescend;

		unsigned					lineIndex;

		StyleRun*					styleRun;
	};

	struct LineInfo {
		unsigned					startOffset;
		double						y;
		double						height;
		double						maxAscent;
		double						maxDescent;
	};

public:
	TextLayout(FontCache* fontCache);
	TextLayout(const TextLayout& layout);
	virtual ~TextLayout();

	TextLayout& operator=(const TextLayout& other);

	void setText(const char* text);
	void setFont(const Font& font);
	void setFirstLineInset(double inset);
	void setLineInset(double inset);
	void setWidth(double width);
	void setAlignment(unsigned alignment);
	void setJustify(bool justify);
	void setGlyphSpacing(double spacing);
	void setLineSpacing(double spacing);
	void setTabs(double* tabs, unsigned count);

	inline const Font& getFont() const
	{
		return fFont;
	}

	inline unsigned getAlignment() const
	{
		return fAlignment;
	}

	inline unsigned getJustify() const
	{
		return fJustify;
	}

	inline double getGlyphSpacing() const
	{
		return fGlyphSpacing;
	}

	void clearStyleRuns();
	bool addStyleRun(int start, const Font& font,
		double metricsAscent, double metricsDescent, double metricsWidth,
		double glyphSpacing, double fauxWeight, double fauxItalic,
		const TextRenderer::Color& fgColor, const TextRenderer::Color& bgColor,
		bool strikeOut, const TextRenderer::Color& strikeOutColor,
		bool underline, unsigned underlineStyle,
		const TextRenderer::Color& underlineColor);

	void layout();

	inline unsigned getGlyphCount() const
	{
		return fGlyphInfoCount;
	}

	inline void getAdvanceX(int index, double* advanceX) {
		if (fGlyphInfoBuffer[index].advanceX > 0.0)
			*advanceX = fGlyphInfoBuffer[index].advanceX;
		else
			*advanceX = 3.0;
	}

	inline bool getInfo(int index, Font& font,
		double& glyphSpacing, double& fauxWeight, double& fauxItalic,
		TextRenderer::Color& fgColor, bool& strikeOut,
		TextRenderer::Color& strikeColor,
		bool& underline, unsigned& underlineStyle,
		TextRenderer::Color& underlineColor) const
	{
		if (index < 0 || index >= (int)fGlyphInfoCount)
			return false;

		StyleRun* style = fGlyphInfoBuffer[index].styleRun;
		if (style != NULL) {
			font = style->font;
			glyphSpacing = style->glyphSpacing;
			fauxWeight = style->fauxWeight;
			fauxItalic = style->fauxItalic;
			fgColor = style->fgColor;
			if (style->strikeOut) {
				strikeOut = true;
				strikeColor = style->strikeOutColor;
			}
			if (style->underline) {
				underline = true;
				underlineStyle = style->underlineStyle;
				underlineColor = style->underlineColor;
			}
		} else {
			font = fFont;
			glyphSpacing = fGlyphSpacing;
			fauxWeight = 0.0;
			fauxItalic = 0.0;
			fgColor.r = 0;
			fgColor.g = 0;
			fgColor.b = 0;
			fgColor.a = (255 << 8) | 255;
			strikeOut = false;
			underline = false;
		}
		return true;
	}

	inline void getInfo(int index, const agg::glyph_cache** glyph, double* x,
		double* y, double* height, double* fauxWeight, double* fauxItalic,
		TextRenderer::Color& fgColor, bool& strikeOut,
		TextRenderer::Color& strikeColor, bool& underline,
		unsigned& underlineStyle, TextRenderer::Color& underlineColor) const
	{
		*glyph = fGlyphInfoBuffer[index].glyph;
		*x = fGlyphInfoBuffer[index].x;
		*y = fGlyphInfoBuffer[index].y;
		*height = fGlyphInfoBuffer[index].maxAscend
			+ fGlyphInfoBuffer[index].maxDescend;

		StyleRun* style = fGlyphInfoBuffer[index].styleRun;
		if (style != NULL) {
			*height = style->font.getSize();
			*fauxWeight = style->fauxWeight;
			*fauxItalic = style->fauxItalic;
			fgColor = style->fgColor;
			if (style->strikeOut) {
				strikeOut = true;
				strikeColor = style->strikeOutColor;
			}
			if (style->underline) {
				underline = true;
				underlineStyle = style->underlineStyle;
				underlineColor = style->underlineColor;
			}
			if (style->width > 0.0) {
				// Client provided metrics for this glyph, do not draw the
				// place-holder glyph.
				*glyph = NULL;
			}
		}
	}

	inline void getInfo(unsigned index, unsigned* lineIndex,
		double* x, double* advanceX, double* lineTop, double* lineBottom,
		TextRenderer::Color& bgColor) const
	{
		*lineIndex = fGlyphInfoBuffer[index].lineIndex;

		*x = fGlyphInfoBuffer[index].x;
		if (fGlyphInfoBuffer[index].advanceX > 0.0)
			*advanceX = fGlyphInfoBuffer[index].advanceX;
		else
			*advanceX = 3.0;
		*lineTop = fLineInfoBuffer[*lineIndex].y;
		*lineBottom = *lineTop + fLineInfoBuffer[*lineIndex].height;

		StyleRun* style = fGlyphInfoBuffer[index].styleRun;
		if (style != NULL) {
			bgColor = style->bgColor;
		}
	}

	inline void getGlyphBoundingBox(unsigned index, double* x1, double* y1,
		double* x2, double* y2) const
	{
		double y = fGlyphInfoBuffer[index].y;

		*x1 = fGlyphInfoBuffer[index].x;
		*y1 = y - fGlyphInfoBuffer[index].maxAscend;
		*x2 = *x1 + fGlyphInfoBuffer[index].advanceX;
		*y2 = y + fGlyphInfoBuffer[index].maxDescend;
	}

	inline double getFirstLineInset() const
	{
		return fFirstLineInset;
	}

	inline double getWidth() const
	{
		return fWidth;
	}

	double getActualWidth();
	double getHeight();

	double getScaleX() const;

	int getLineCount();
	int getLineIndex(int textOffset);
	double getLineWidth(int lineIndex);
	void getLineBounds(int lineIndex, double* x1, double* y1,
		double* x2, double* y2);

	int getLineOffsets(int offsets[], unsigned count);

	int getFirstOffsetOnLine(int lineIndex);
	int getLastOffsetOnLine(int lineIndex);

	unsigned getOffset(double x, double y, bool& rightOfCenter);

	void getLineMetrics(int lineIndex, double buffer[]);

	int getPreviousOffset(int offset, unsigned movement);
	int getNextOffset(int offset, unsigned movement);

	void getTextBounds(int textOffset, double& x1, double& y1,
		double& x2, double& y2);

private:
	bool init(const char* text, TextRenderer::FontEngine& fontEngine,
		TextRenderer::FontManager& fontManager, bool hinting, double scaleX,
		unsigned subpixelScale);

	void layout(TextRenderer::FontEngine& fontEngine,
		TextRenderer::FontManager& fontManager,
		bool kerning, double scaleX, unsigned subpixelScale);
	void applyAlignment(const double width);

	void invalidateLayout();
	void validateLayout();

	bool appendGlyph(unsigned charCode, const agg::glyph_cache* glyph,
		StyleRun* styleRun);
	bool appendLine(unsigned startOffset, double y, double lineHeight,
		double maxAscent, double maxDescent);

	static inline bool canEndLine(GlyphInfo* buffer, int offset, int count);

private:
	FontCache*			fFontCache;

	Font				fFont;
	double				fAscent;
	double				fDescent;

	double				fFirstLineInset;
	double				fLineInset;
	double				fWidth;
	unsigned			fAlignment;
	bool				fJustify;

	double				fHeight;

	double				fGlyphSpacing;
	double				fLineSpacing;

	GlyphInfo*			fGlyphInfoBuffer;
	unsigned			fGlyphInfoBufferSize;
	unsigned			fGlyphInfoCount;

	LineInfo*			fLineInfoBuffer;
	unsigned			fLineInfoBufferSize;
	unsigned			fLineInfoCount;

	StyleRun*			fStyleRunBuffer;
	unsigned			fStyleRunBufferSize;
	unsigned			fStyleRunCount;

	double*				fTabBuffer;
	unsigned			fTabCount;

	bool				fSubpixelRendering;
	bool				fKerning;
	bool				fHinting;

	bool				fLayoutPerformed;
};

#endif // TEXT_LAYOUT_H
