//
// Lightpad - DisplayPanel.cpp
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#include "DisplayPanel.h"

#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/dcbuffer.h>
#include <wx/clrpicker.h>

#include "Magick++.h"

#include "Colors.h"
#include "Helpers.h"
#include "Project.h"

wxDEFINE_EVENT(DISPLAY_REFRESH, wxCommandEvent);

// Initialize the file panel and it's elements
DisplayPanel::DisplayPanel(wxPanel *parent)
	: wxPanel(parent, ID_Panel_Display, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	m_timer = new wxTimer(this, ID_DisplayPanel_Timer);

	// Initialize variables
	base_image_path = getResourcePath("launchpad_display/base/base_4096.png");
	button_image_path = getResourcePath("launchpad_display/buttons/buttons_4096.png");
	image_size = MAXIMUM_LAUNCHPAD_IMAGE_SIZE;
	button_size = MAXIMUM_LAUNCHPAD_BUTTON_SIZE;
	button_radius = image_size / 512;
	panel_width = -1;
	panel_height = -1;
	image_xpos = 0;
	image_ypos = 0;
	frame = 0;

	launchpad_base_image = new Magick::Image(base_image_path);
	launchpad_button_image = new Magick::Image(button_image_path);
	bzero(launchpad_button_images, sizeof(launchpad_button_images));
	lp_img = NULL;

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		button_colors[i] = 0;
	}
	paintNow();

	Bind(DISPLAY_REFRESH, &DisplayPanel::RefreshNow, this, ID_TimelinePanel_CellSelect);
}

DisplayPanel::~DisplayPanel() {
	delete m_timer;
	delete launchpad_base_image;
	delete launchpad_button_image;
	for (int i = 0; i < 6; i++) delete launchpad_button_images[i];
}

void DisplayPanel::paintEvent(wxPaintEvent &event) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC canvas(this);
	render(canvas);
}

void DisplayPanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC canvas(this);
	render(canvas);
}

void DisplayPanel::RefreshNow() {
	Refresh();
	Update();
}

void DisplayPanel::RefreshNow(wxCommandEvent &event) {
	RefreshNow();
}

void DisplayPanel::OnSize(wxSizeEvent &event) {
	// Re-render when resizing
	Refresh();
	event.Skip();
}

void DisplayPanel::startstop(wxCommandEvent &event) {
	if (m_timer->IsRunning()) m_timer->Stop();
	else m_timer->Start(1000 / 60);
}

void DisplayPanel::play_next_frame(wxTimerEvent &event) {
	frame += 1;
	std::cout << frame << std::endl;
	Refresh();
}

void DisplayPanel::resize_images(int min_fit_size) {
	int new_image_size = closest_two_power(min_fit_size, MINIMUM_LAUNCHPAD_IMAGE_SIZE,
	                                       MAXIMUM_LAUNCHPAD_IMAGE_SIZE);
	double ratio = MAXIMUM_LAUNCHPAD_IMAGE_SIZE / new_image_size;

	// Load the other resolutions of the image as needed
	if (new_image_size != image_size) {
		// launchpad_base_image->LoadFile(base_image_path, wxBITMAP_TYPE_PNG);
		// launchpad_button_image->LoadFile(button_image_path, wxBITMAP_TYPE_PNG);

		image_size = new_image_size;
		button_size = image_size * ((MAXIMUM_LAUNCHPAD_BUTTON_SIZE * 1.0) / MAXIMUM_LAUNCHPAD_IMAGE_SIZE);
		button_radius = image_size / 512;

		Magick::Image *buttons_image = new Magick::Image(*launchpad_button_image);
		Magick::Geometry size = buttons_image->size();
		size.width(size.width() / ratio);
		size.height(size.height() / ratio);
		buttons_image->scale(size);

		for (int i = 0; i < 6; i++) {
			if (launchpad_button_images[i]) delete launchpad_button_images[i];
			launchpad_button_images[i] = new Magick::Image(*buttons_image);
			launchpad_button_images[i]->crop(Magick::Geometry(button_size, button_size, button_size * i, 0));
		}

		if (lp_img) delete lp_img;
		lp_img = new Magick::Image(*launchpad_base_image);
		size = lp_img->size();
		size.width(size.width() / ratio);
		size.height(size.height() / ratio);
		lp_img->scale(size);

		delete buttons_image;
	}
}

void DisplayPanel::render_buttons() {
	// Draw the buttons on the screen (and set Launchpad colors)

	launchpad->beginColorUpdate();

	for (int i = 1; i < 99; i++) {
		if (i == 9 || i == 90) continue;
		int btn_x = i % 10;
		int btn_y = 9 - (i / 10);
		int button_style = get_button_style(btn_x, btn_y);
		wxColor bcolor = velocitycolors[button_colors[i]];

		Magick::Image current_button(*launchpad_button_images[button_style]);
		current_button.modulate(180.0, 0.0, 100.0);
		current_button.colorize(50, 50, 50, Magick::ColorRGB(bcolor.Red() / 255.0,
		                        bcolor.Green() / 255.0, bcolor.Blue() / 255.0));
		lp_img->composite(current_button, get_button_position(btn_x)*image_size,
		                  get_button_position(btn_y)*image_size,
		                  Magick::OverCompositeOp);

		launchpad->setColor(i, button_colors[i]);
	}

	launchpad->endColorUpdate();
}

void DisplayPanel::render(wxDC &canvas) {
	int neww, newh, min_fit_size;
	Magick::Geometry size;

	canvas.GetSize(&neww, &newh);
	min_fit_size = std::max(std::min(neww, newh), MINIMUM_LAUNCHPAD_IMAGE_SIZE);

	if (neww != panel_width || newh != panel_height) {
		resize_images(min_fit_size);

		// Reposition as needed
		if ( neww > newh ) {
			image_xpos = (neww - min_fit_size) / 2;
			image_ypos = 0;
		} else {
			image_xpos = 0;
			image_ypos = (newh - min_fit_size) / 2;
		}

		panel_width = neww;
		panel_height = newh;
	}

	render_buttons();

	// Derived from http://stackoverflow.com/questions/28151240/get-rgb-color-with-magick-using-c
	int width = lp_img->columns();
	int height = lp_img->rows();
	Magick::PixelPacket *pixels = lp_img->getPixels(0, 0, width, height);
	Magick::ColorRGB color_sample;
	wxImage *out = new wxImage(width, height);
	out->SetAlpha();

	for (int x = 0; x < width - 1; x++) {
		for (int y = 0; y < height - 1; y++) {
			color_sample = pixels[width * y + x];
			out->SetRGB(x, y, color_sample.red() * 255, color_sample.green() * 255,
			            color_sample.blue() * 255);
			out->SetAlpha(x, y, (1 - color_sample.alpha()) * 255);
		}
	}

	canvas.DrawBitmap(wxBitmap(out->Scale(min_fit_size, min_fit_size)), image_xpos, image_ypos);

	delete out;
}

int DisplayPanel::get_button_style(int btn_x, int btn_y) {
	// Choose button style
	if (btn_x == 0 || btn_x == 9 || btn_y == 0 || btn_y == 9) {
		return 0;
	} else {
		switch (btn_x + (btn_y * 10)) {
			case 44:
				return 2;
				break;
			case 45:
				return 3;
				break;
			case 54:
				return 4;
				break;
			case 55:
				return 5;
				break;
			default:
				return 1;
				break;
		}
	}
}

float DisplayPanel::get_button_position(int digit) {
	float pos = 0.113525390625 + (digit * 0.078125);
	int padding = 0;
	if (digit > 0) padding = 1;
	if (digit == 9) padding = 2;
	return pos + (padding * 0.0078125);
}

void DisplayPanel::colorButton(int button, wxColor color) {
	if (button < 1 || button > 98 || button == 9 || button == 90) return;
	button_colors[button] = ColorConverter::get_closest_velocity(color);
}

void DisplayPanel::colorButton(wxColourPickerEvent &event) {
	colorButton(event.GetInt(), event.GetColour());
}

wxBEGIN_EVENT_TABLE(DisplayPanel, wxPanel)
	EVT_PAINT(DisplayPanel::paintEvent)
	EVT_SIZE(DisplayPanel::OnSize)
	EVT_MENU(ID_Menu_PlayPause, DisplayPanel::startstop)
	EVT_TIMER(ID_DisplayPanel_Timer, DisplayPanel::play_next_frame)
	EVT_COLOURPICKER_CHANGED(ID_TimelinePanel_TimelineGrid, DisplayPanel::colorButton)
	EVT_COMMAND(ID_Panel_Display, DISPLAY_REFRESH, DisplayPanel::RefreshNow)
wxEND_EVENT_TABLE()
