//
// Lightpad - Colors.h
// ©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha, Origami1105, WhoovesPON3.
// Additional support from LaunchpadFun (http://www.launchpadfun.com/en/).
// https://www.nightwave.co/lightpad
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

const int COLORCOUNT = 128;
extern wxColor velocitycolors[COLORCOUNT];

unsigned char get_closest_velocity(wxColor c);
char get_color_velocity(wxColor c);

class ColorConverter { // All converters take values from 0.0 to 1.0
	public:
		// Color conversion helpers
		static double Hue2RGB(double p, double q, double t);
		static double XYZ2H(double q);
		static double LAB_compare_RGB(int red1, int grn1, int blu1, int red2, int grn2, int blu2);
		static double LAB_compare_RGB(wxColor clr1, wxColor clr2);

		// Base converters
		static void RGB2HSL(double red, double grn, double blu, double *hue, double *sat,
		                    double *lum); 						// XXX Convert to int (0-255)
		static void HSL2RGB(double hue, double sat, double lum, double *red, double *grn,
		                    double *blu); 						// XXX Convert to int (0-255)
		static void RGB2HSV(double red, double grn, double blu, double *hue, double *sat,
		                    double *vel); 						// XXX Convert to int (0-255)
		static void HSV2RGB(double hue, double sat, double vel, double *red, double *grn,
		                    double *blu); 						// XXX Convert to int (0-255)
		static void RGB2CMYK(double red, double grn, double blu, double *cyan, double *mgnta, double *ylw,
		                     double *blk); 		// XXX Convert to int (0-255)
		static void CMYK2RGB(double cyan, double mgnta, double ylw, double blk, double *red, double *grn,
		                     double *blu); 		// XXX Convert to int (0-255)
		static void RGB2YIQ(double red, double grn, double blu, double *ylum, double *iphs,
		                    double *quad); 						// XXX Convert to int (0-255)
		static void YIQ2RGB(double ylum, double iphs, double quad, double *red, double *grn,
		                    double *blu); 						// XXX Convert to int (0-255)
		static void RGB2XYZ(int red, int grn, int blu, double *xrsp, double *ylum, double *zblu);
		static void XYZ2RGB(double xrsp, double ylum, double zblu, int *red, int *grn, int *blu);
		static void XYZ2LAB(double xrsp, double ylum, double zblu, double *lum, double *apt, double *bpt);
		static void LAB2XYZ(double lum, double apt, double bpt, double *xrsp, double *ylum, double *zblu);

		// Two-step converters
		static void RGB2LAB(int red, int grn, int blu, double *lum, double *apt, double *bpt);
		static void LAB2RGB(double lum, double apt, double bpt, int *red, int *grn, int *blu);
		static void HSL2HSV(double hue, double sat, double lum, double *_hue, double *_sat,
		                    double *vel); 						// XXX Convert to int (0-255)
		static void HSV2HSL(double hue, double sat, double vel, double *_hue, double *_sat,
		                    double *lum); 						// XXX Convert to int (0-255)
		static void HSL2CMYK(double hue, double sat, double lum, double *cyan, double *mgnta, double *ylw,
		                     double *blk); 		// XXX Convert to int (0-255)
		static void CMYK2HSL(double cyan, double mgnta, double ylw, double blk, double *hue, double *sat,
		                     double *lum); 		// XXX Convert to int (0-255)
		static void HSV2CMYK(double hue, double sat, double vel, double *cyan, double *mgnta, double *ylw,
		                     double *blk); 		// XXX Convert to int (0-255)
		static void CMYK2HSV(double cyan, double mgnta, double ylw, double blk, double *hue, double *sat,
		                     double *vel); 		// XXX Convert to int (0-255)
		static void HSL2YIQ(double hue, double sat, double lum, double *ylum, double *iphs,
		                    double *quad); 						// XXX Convert to int (0-255)
		static void YIQ2HSL(double ylum, double iphs, double quad, double *hue, double *sat,
		                    double *lum); 						// XXX Convert to int (0-255)
		static void HSV2YIQ(double hue, double sat, double vel, double *ylum, double *iphs,
		                    double *quad); 						// XXX Convert to int (0-255)
		static void YIQ2HSV(double ylum, double iphs, double quad, double *hue, double *sat,
		                    double *vel); 						// XXX Convert to int (0-255)
		static void CMYK2YIQ(double cyan, double mgnta, double ylw, double blk, double *ylum, double *iphs,
		                     double *quad); 		// XXX Convert to int (0-255)
		static void YIQ2CMYK(double ylum, double iphs, double quad, double *cyan, double *mgnta,
		                     double *ylw, double *blk); 	// XXX Convert to int (0-255)
};