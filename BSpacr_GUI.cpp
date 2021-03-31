#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include "BWidgets/Window.hpp"
#include "BWidgets/Label.hpp"
#include "BWidgets/DialValue.hpp"
#include "Definitions.hpp"
#include "Ports.hpp"

#define BG_FILE "surface.png"

class BSpacr_GUI : public BWidgets::Window
{
public:
	BSpacr_GUI (const char *bundle_path, PuglNativeView parentWindow);
	void portEvent (uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer);
	virtual void onConfigureRequest (BEvents::ExposeEvent* event) override;
	void setTheme ();
	static void valueChangedCallback (BEvents::Event* event);

	LV2UI_Write_Function write_function;
	LV2UI_Controller controller;

protected:
	cairo_surface_t* bgImageSurface;
	BStyles::Fill widgetBg;
	std::string pluginPath;
	Widget bg;
	BWidgets::Label label;
	BWidgets::DialValue dial;

	BColors::ColorSet fgColors = {{{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {0.1, 0.1, 0.1, 1.0}, {0.0, 0.0, 0.0, 0.0}}};
	BColors::ColorSet txColors = {{{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {0.1, 0.1, 0.1, 1.0}, {0.0, 0.0, 0.0, 0.0}}};
	BColors::ColorSet bgColors = {{{0.15, 0.15, 0.15, 1.0}, {0.3, 0.3, 0.3, 1.0}, {0.05, 0.05, 0.05, 1.0}, {0.0, 0.0, 0.0, 1.0}}};
	BStyles::Font ctLabelFont = BStyles::Font ("Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL, 10.0,
						   BStyles::TEXT_ALIGN_CENTER, BStyles::TEXT_VALIGN_MIDDLE);
	BStyles::StyleSet defaultStyles = {"default", {{"background", STYLEPTR (&BStyles::noFill)},
					  {"border", STYLEPTR (&BStyles::noBorder)}}};
	BStyles::StyleSet labelStyles = {"labels", {{"background", STYLEPTR (&BStyles::noFill)},
					{"border", STYLEPTR (&BStyles::noBorder)},
					{"textcolors", STYLEPTR (&fgColors)},
					{"font", STYLEPTR (&ctLabelFont)}}};
	BStyles::Theme theme = BStyles::Theme
	({
		defaultStyles,
		//{"widget", 		{{"background", STYLEPTR (&BStyles::blackFill)},
		//			 {"border", STYLEPTR (&BStyles::noBorder)}}},
		{"label",	 	{{"uses", STYLEPTR (&labelStyles)}}},
		{"dial", 		{{"uses", STYLEPTR (&defaultStyles)},
 					 {"fgcolors", STYLEPTR (&fgColors)},
 					 {"bgcolors", STYLEPTR (&bgColors)},
 					 {"textcolors", STYLEPTR (&fgColors)},
 					 {"font", STYLEPTR (&ctLabelFont)}}}
	});
};

BSpacr_GUI::BSpacr_GUI (const char *bundle_path, PuglNativeView parentWindow) :
	BWidgets::Window (480, 80, "BAmp", parentWindow, true, PUGL_MODULE, 0),
	write_function (NULL),
	controller (NULL),
	bgImageSurface (nullptr),
	widgetBg (BStyles::noFill),
	pluginPath (bundle_path ? std::string (bundle_path) + ((strlen (bundle_path) > 0) && (bundle_path[strlen (bundle_path) - 1] != '/') ? "/" : "") : std::string ("")),
	bg (0, 0, 480, 80, "widget"),
	label (400, 56, 40, 12, "label", "dry/wet"),
	dial (402, 12, 36, 44, "dial", 1.0, 0.0, 1.0, 0.0, "%1.2f")
{
	bgImageSurface = cairo_image_surface_create_from_png ((pluginPath + BG_FILE).c_str());
	widgetBg.loadFillFromCairoSurface (bgImageSurface);
	bg.setBackground (widgetBg);
	dial.setCallbackFunction (BEvents::VALUE_CHANGED_EVENT, valueChangedCallback);
	dial.setHardChangeable (false);
	bg.add (label);
	bg.add (dial);
	setTheme();
	add (bg);
}

void BSpacr_GUI::portEvent (uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if ((port_index == BSPACR_MIX) && (format == 0))
	{
		float* pval = (float*) buffer;
		dial.setValue (*pval);
	}
}

void BSpacr_GUI::setTheme ()
{
	label.applyTheme (theme);
	dial.applyTheme (theme);
}

void BSpacr_GUI::onConfigureRequest (BEvents::ExposeEvent* event)
{
	Window::onConfigureRequest (event);

	double sz = (getWidth() / 480 > getHeight() / 80 ? getHeight() / 80 : getWidth() / 480);
	cairo_surface_t* surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 480 * sz, 80 * sz);
	cairo_t* cr = cairo_create (surface);
	cairo_scale (cr, sz, sz);
	cairo_set_source_surface(cr, bgImageSurface, 0, 0);
	cairo_paint(cr);
	widgetBg.loadFillFromCairoSurface(surface);
	cairo_destroy (cr);
	cairo_surface_destroy (surface);
	bg.setBackground (widgetBg);
	bg.resize (480 * sz, 80 * sz);
	ctLabelFont.setFontSize (10 * sz);
	label.moveTo (400 * sz, 56 * sz);
	label.resize (40 * sz, 12 * sz);
	dial.moveTo (402 * sz, 12 * sz);
	dial.resize (36 * sz, 44 * sz);
	setTheme();
}

void BSpacr_GUI::valueChangedCallback(BEvents::Event* event)
{
	if (!event) return;
	BWidgets::ValueWidget* widget = (BWidgets::ValueWidget*) event->getWidget ();
	if (!widget) return;
	float value = widget->getValue();
	BSpacr_GUI* ui = (BSpacr_GUI*) widget->getMainWindow();
	if (!ui) return;
	ui->write_function(ui->controller, BSPACR_MIX, sizeof(float), 0, &value);
}

static LV2UI_Handle instantiate (const LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path,
						  LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget,
						  const LV2_Feature *const *features)
{
	PuglNativeView parentWindow = 0;
	LV2UI_Resize* resize = NULL;

	if (strcmp(plugin_uri, BSPACR_URI) != 0)
	{
		std::cerr << "BSpacr_GUI: This GUI does not support plugin with URI " << plugin_uri << std::endl;
		return NULL;
	}

	for (int i = 0; features[i]; ++i)
	{
		if (!strcmp(features[i]->URI, LV2_UI__parent)) parentWindow = (PuglNativeView) features[i]->data;
		else if (!strcmp(features[i]->URI, LV2_UI__resize)) resize = (LV2UI_Resize*)features[i]->data;
	}
	if (parentWindow == 0) std::cerr << "BSpacr_GUI: No parent window.\n";

	BSpacr_GUI* ui = new BSpacr_GUI (bundle_path, parentWindow);

	if (ui)
	{
		ui->controller = controller;
		ui->write_function = write_function;
		if (resize) resize->ui_resize(resize->handle, 480, 80 );

		PuglNativeView nativeWindow = puglGetNativeWindow (ui->getPuglView ());
		*widget = (LV2UI_Widget) nativeWindow;
	}
	else std::cerr << "BSpacr_GUI: Couldn't instantiate.\n";
	return (LV2UI_Handle) ui;
}

static void cleanup(LV2UI_Handle ui)
{
	BSpacr_GUI* pluginGui = (BSpacr_GUI*) ui;
	if (pluginGui) delete pluginGui;
}

static void portEvent(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	BSpacr_GUI* pluginGui = (BSpacr_GUI*) ui;
	if (pluginGui) pluginGui->portEvent(port_index, buffer_size, format, buffer);
}

static int callIdle(LV2UI_Handle ui)
{
	BSpacr_GUI* pluginGui = (BSpacr_GUI*) ui;
	if (pluginGui) pluginGui->handleEvents ();
	return 0;
}

static int callResize (LV2UI_Handle ui, int width, int height)
{
	BSpacr_GUI* self = (BSpacr_GUI*) ui;
	if (!self) return 0;
	BEvents::ExposeEvent* ev = new BEvents::ExposeEvent (self, self, BEvents::CONFIGURE_REQUEST_EVENT, self->getPosition().x, self->getPosition().y, width, height);
	self->addEventToQueue (ev);
	return 0;
}

static const LV2UI_Idle_Interface idle = {callIdle};
static const LV2UI_Resize resize = {nullptr, callResize} ;

static const void* extensionData(const char* uri)
{
	if (!strcmp(uri, LV2_UI__idleInterface)) return &idle;
	else if(!strcmp(uri, LV2_UI__resize)) return &resize;
	else return NULL;
}


static const LV2UI_Descriptor guiDescriptor =
{
		BSPACR_GUI_URI,
		instantiate,
		cleanup,
		portEvent,
		extensionData
};

// LV2 Symbol Export
LV2_SYMBOL_EXPORT const LV2UI_Descriptor *lv2ui_descriptor(uint32_t index)
{
	switch (index)
	{
		case 0: return &guiDescriptor;
		default:return NULL;
	}
}
