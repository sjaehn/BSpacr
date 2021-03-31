/* VSlider.hpp
 * Copyright (C) 2018, 2019  Sven Jähnichen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BWIDGETS_VSLIDER_HPP_
#define BWIDGETS_VSLIDER_HPP_

#define BWIDGETS_DEFAULT_VSLIDER_WIDTH (BWIDGETS_DEFAULT_VSCALE_WIDTH * 2)
#define BWIDGETS_DEFAULT_VSLIDER_HEIGHT BWIDGETS_DEFAULT_VSCALE_HEIGHT
#define BWIDGETS_DEFAULT_VSLIDER_DEPTH 1.0

#include "Knob.hpp"
#include "VScale.hpp"
#include "Label.hpp"
#include "Focusable.hpp"

namespace BWidgets
{
/**
 * Class BWidgets::VSlider
 *
 * RangeWidget class for a vertical slider.
 * The Widget is clickable by default.
 */
class VSlider : public VScale, public Focusable
{
public:
	VSlider ();
	VSlider (const double  x, const double y, const double width, const double height, const std::string& name,
			 const double value, const double min, const double max, const double step);

	/**
	 * Creates a new (orphan) slider and copies the slider properties from a
	 * source slider.
	 * @param that Source slider
	 */
	VSlider (const VSlider& that);

	/**
	 * Assignment. Copies the slider properties from a source slider and keeps
	 * its name and its position within the widget tree. Emits an expose event
	 * if the widget is visible and a value changed event.
	 * @param that Source slider
	 */
	VSlider& operator= (const VSlider& that);

	/**
	 * Pattern cloning. Creates a new instance of the widget and copies all
	 * its properties.
	 */
	virtual Widget* clone () const override;

	/**
	 * Changes the value of the widget and keeps it within the defined range.
	 * Passes the value to its predefined child widgets.
	 * Emits a value changed event and (if visible) an expose event.
	 * @param val Value
	 */
	virtual void setValue (const double val) override;

	/**
	 * Calls a redraw of the widget and calls postRedisplay () if the the
	 * Widget is visible.
	 * This method should be called if the widgets properties are indirectly
	 * changed.
	 */
	virtual void update () override;

	/**
	 * Scans theme for widget properties and applies these properties.
	 * @param theme Theme to be scanned.
	 * 				styles used are:
	 * 				BWIDGETS_KEYWORD_BORDER
	 * 				BWIDGETS_KEYWORD_BACKGROUND
	 * 				BWIDGETS_KEYWORD_FGCOLORS
	 * 				BWIDGETS_KEYWORD_BGCOLORS
	 * @param name Name of the BStyles::StyleSet within the theme to be
	 * 		  	   applied.
	 */
	virtual void applyTheme (BStyles::Theme& theme) override;
	virtual void applyTheme (BStyles::Theme& theme, const std::string& name) override;

	/**
	 * Predefined empty method to handle a
	 * BEvents::EventType::FOCUS_IN_EVENT.
	 * @param event Focus event
	 */
	virtual void onFocusIn (BEvents::FocusEvent* event) override;

	/**
	 * Predefined empty method to handle a
	 * BEvents::EventType::FOCUS_OUT_EVENT.
	 * @param event Focus event
	 */
	virtual void onFocusOut (BEvents::FocusEvent* event) override;

protected:
	virtual void updateCoords () override;

	Knob knob;
	Label focusLabel;
	double knobRadius;
	BUtilities::Point knobPosition;
};

}

#endif /* BWIDGETS_VSLIDER_HPP_ */
