//
//  GetMetadataForFile.c
//  FITSImporter
//
//  Created by Cédric Foellmi on 19/07/09.
//
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h> 
#include "qfits.h"

/* ------------------------------------------------------------------------------------------------------

getCleanedHeaderValue: read FITS header and return striped CFStringRef value.

------------------------------------------------------------------------------------------------------ */
CFStringRef concatWithT(CFStringRef str1, CFStringRef str2) {
	CFIndex length = CFStringGetLength(str1) + CFStringGetLength(str2) + 1;
	CFMutableStringRef cfmvalue = CFStringCreateMutable(kCFAllocatorDefault, length);
	CFStringAppend(cfmvalue, str1);
	CFStringAppend(cfmvalue, CFSTR("T"));
	CFStringAppend(cfmvalue, str2);
	return cfmvalue;
}


/* ------------------------------------------------------------------------------------------------------

getCleanedHeaderValue: read FITS header and return striped CFStringRef value.

------------------------------------------------------------------------------------------------------ */
CFStringRef getCleanedHeaderValue(const char* filename, char* keyword) {
	char *headerValue = NULL;
	headerValue = qfits_query_hdr((const char*)filename, keyword);
//	printf("For keyword %s, headerValue is: %s\n", keyword, headerValue);
	
	if (headerValue != NULL) {
		CFStringRef cfvalue = CFStringCreateWithCString(kCFAllocatorDefault, headerValue, kCFStringEncodingUTF8);
		CFIndex length = CFStringGetLength(cfvalue);
		CFMutableStringRef cfmvalue = CFStringCreateMutable(kCFAllocatorDefault, length);
		CFStringAppend(cfmvalue, cfvalue);		
		CFStringTrim(cfmvalue, CFSTR("'"));
		CFStringTrimWhitespace(cfmvalue);
		return cfmvalue;
	} else {
		return NULL;
	}
}

/* ------------------------------------------------------------------------------------------------------

SetDateValueFromFITSHeader: read FITS header and set striped string value to attribute name.

------------------------------------------------------------------------------------------------------ */
void SetDateValueFromFITSHeader(const char* filename, 
				CFMutableDictionaryRef attributes, 
				const char* importerAttrName, 
				char* mainKeyword, 
				char* secondaryKeyword1,
				char* secondaryKeyword2)
{
	CFStringRef value1 = getCleanedHeaderValue(filename, mainKeyword);
	CFStringRef value2 = getCleanedHeaderValue(filename, secondaryKeyword1);
	CFStringRef value3 = getCleanedHeaderValue(filename, secondaryKeyword2);
	CFStringRef headerValue = NULL;
	if (value1 != NULL) {
		if ((CFStringGetLength(value1) >= 19) || (CFStringGetLength(value1) == 10)) {
			headerValue = value1;
		} 
	}
	if (value2 != NULL) {
		if (headerValue != NULL) {
			if ((CFStringGetLength(headerValue) < 19) && (CFStringGetLength(value2) >= 19)) {
				headerValue = value2;
			}		
		} else if (CFStringGetLength(value2) >= 10) {
			headerValue = value2;
		}
	}
	if (value3 != NULL) {
		CFRange r = CFStringFind(value3, CFSTR(":"), 0);
		if ((headerValue != NULL) && (CFStringGetLength(headerValue) == 10) && (r.location != kCFNotFound)) {
			headerValue = concatWithT(headerValue, value3);
		}
	}
	
	if (headerValue != NULL) {
		
		CFArrayRef tArray   = CFStringCreateArrayBySeparatingStrings(kCFAllocatorDefault, headerValue, CFSTR("T"));
		CFArrayRef ymdArray = CFStringCreateArrayBySeparatingStrings(kCFAllocatorDefault, CFArrayGetValueAtIndex(tArray, 0), CFSTR("-"));

		CFGregorianDate gregDate;
		gregDate.year   = 0;
		gregDate.month  = 0;
		gregDate.day    = 0.;
		
		if (CFStringGetLength(CFArrayGetValueAtIndex(ymdArray, 0)) > 0) {
			gregDate.year   = CFStringGetIntValue(CFArrayGetValueAtIndex(ymdArray, 0));
		}
		if (CFStringGetLength(CFArrayGetValueAtIndex(ymdArray, 1)) > 0) {
			gregDate.month  = CFStringGetIntValue(CFArrayGetValueAtIndex(ymdArray, 1));
		}
		if (CFStringGetLength(CFArrayGetValueAtIndex(ymdArray, 2)) > 0) {
			gregDate.day    = CFStringGetIntValue(CFArrayGetValueAtIndex(ymdArray, 2));
		}
		gregDate.hour   = 0;
		gregDate.minute = 0;
		gregDate.second = 0.;
		
		CFIndex arraySize = CFArrayGetCount(tArray);
		if (arraySize == 2) {
			CFArrayRef hmsArray = CFStringCreateArrayBySeparatingStrings(kCFAllocatorDefault, CFArrayGetValueAtIndex(tArray, 1), CFSTR(":"));
			if (CFStringGetLength(CFArrayGetValueAtIndex(hmsArray, 0)) > 0) {
				gregDate.hour   = CFStringGetIntValue(CFArrayGetValueAtIndex(hmsArray, 0));
			}
			if (CFStringGetLength(CFArrayGetValueAtIndex(hmsArray, 1)) > 0) {
				gregDate.minute = CFStringGetIntValue(CFArrayGetValueAtIndex(hmsArray, 1));
			}
			if (CFStringGetLength(CFArrayGetValueAtIndex(hmsArray, 2)) > 0) {
				gregDate.second = CFStringGetDoubleValue(CFArrayGetValueAtIndex(hmsArray, 2));			
			}
		}

//		printf("%i %i %i %i %i %f\n\n", gregDate.year, gregDate.month, gregDate.day, gregDate.hour, gregDate.minute, gregDate.second);

		if ((gregDate.year > 0) && (gregDate.month > 0) && (gregDate.day > 0)) {
			CFTimeZoneRef timeZone = CFTimeZoneCreateWithName(kCFAllocatorDefault, CFSTR("UT"), true);
			CFAbsoluteTime absTime = CFGregorianDateGetAbsoluteTime(gregDate, timeZone);
			CFDateRef date = CFDateCreate(kCFAllocatorDefault, absTime);

			CFStringRef cfImporterAttrName = CFStringCreateWithCString(kCFAllocatorDefault, importerAttrName, kCFStringEncodingUTF8);
			CFDictionaryAddValue(attributes, cfImporterAttrName, date);
		}
	}
}


/* ------------------------------------------------------------------------------------------------------

SetStringValueFromFITSHeader: read FITS header and set striped string value to attribute name.

------------------------------------------------------------------------------------------------------ */
void SetStringValueFromFITSHeader(const char* filename, 
				CFMutableDictionaryRef attributes, 
				const char* importerAttrName, 
				char* keyword, 
				char* alternateKeyword)
{
	CFStringRef headerValue = getCleanedHeaderValue(filename, keyword);
	if ((headerValue == NULL) && (alternateKeyword != NULL)) {
		headerValue = getCleanedHeaderValue(filename, alternateKeyword);
	}
	
	if (headerValue != NULL) {		
		CFStringRef cfImporterAttrName = CFStringCreateWithCString(kCFAllocatorDefault, importerAttrName, kCFStringEncodingUTF8);
		CFDictionaryAddValue(attributes, cfImporterAttrName, headerValue);
	}
}

/* ------------------------------------------------------------------------------------------------------

SetNumberValueFromHeader: read FITS header and set number value to attribute name.

------------------------------------------------------------------------------------------------------ */
void SetNumberValueFromFITSHeader(const char* filename, 
				CFMutableDictionaryRef attributes, 
				const char* importerAttrName, 
				char* keyword, 
				Boolean isCoordinates)
{
	CFStringRef headerValue = getCleanedHeaderValue(filename, keyword);
	
	if (headerValue != NULL) {
		double number;
		CFRange r = CFStringFind(headerValue, CFSTR(":"), 0);		
		
		if ((isCoordinates == true) && (r.location != kCFNotFound)) {
			double sign = 1.;
			CFArrayRef dmsArray = CFStringCreateArrayBySeparatingStrings(kCFAllocatorDefault, headerValue, CFSTR(":"));

			if (CFArrayGetCount(dmsArray) == 3) {			
				if (CFStringGetDoubleValue(CFArrayGetValueAtIndex(dmsArray, 0)) < 0) {
					sign = -1;
				}
									
				// Work for either DMS or HMS, since the input format is kept. It is not a transformation, just a flattening.
				number = sign * ( abs(CFStringGetDoubleValue(CFArrayGetValueAtIndex(dmsArray, 0))) + 
									  CFStringGetDoubleValue(CFArrayGetValueAtIndex(dmsArray, 1)) / 60 + 
									  CFStringGetDoubleValue(CFArrayGetValueAtIndex(dmsArray, 2)) / 3600 );
			}
		} else {
			number = CFStringGetDoubleValue(headerValue);			
		}

		CFNumberRef value = CFNumberCreate(kCFAllocatorDefault, kCFNumberDoubleType, &number);
		CFStringRef cfImporterAttrName = CFStringCreateWithCString(kCFAllocatorDefault, importerAttrName, kCFStringEncodingUTF8);
		CFDictionaryAddValue(attributes, cfImporterAttrName, value);
	}
}

/* ------------------------------------------------------------------------------------------------------

GetMetadataForFile: Function called for everytime metadata is extracted for a file.

------------------------------------------------------------------------------------------------------ */
Boolean GetMetadataForFile(void* thisInterface,
               CFMutableDictionaryRef attributes,
               CFStringRef contentTypeUTI,
               CFStringRef pathToFile)
{
    /* Pull any available metadata from the file at the specified path */
    /* Return the attribute keys and attribute values in the dict */
    /* Return true if successful, false if there was no data provided */

	// ----- Transforming the CFStringRef into a pure C string.
	CFIndex length = CFStringGetLength(pathToFile);
	char *filename = malloc(length + 1);
	if (filename == NULL) {
		printf("[FITSImporter] Unable to allocate memory for filename.\n");
		return false;
	}
	CFStringGetCString(pathToFile, filename, length + 1, kCFStringEncodingASCII);

	// ----- First screening to check if file exist..
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
		printf("[FITSImporter] File '%s' does not exist. Error %i\n", filename, errno);
        return false;
    }

	// ----- OK we starts to retrieve attributes values and set them.
    Boolean success = false;
	
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_TargetName", "OBJECT", "TARGNAME");
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Instrument", "INSTRUME", NULL);
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Detector", "DETECTOR", NULL);
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Telescope", "TELESCOP", NULL);
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Observatory", "OBSERVAT", NULL);
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Observer", "OBSERVER", NULL);
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_RaDecSystem", "RADECSYS", NULL);
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_LocalSiderealTime", "ST", NULL);
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_UniversalTime", "UTC", "UT");
	SetStringValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_ArchiveFilename", "ARCFILE", "NULL");

	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_RightAscension", "RA", true);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Declination", "DEC", true);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Airmass", "AIRMASS", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Naxis", "NAXIS", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Naxis1", "NAXIS1", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Naxis2", "NAXIS2", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Exptime", "EXPTIME", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Equinox", "EQUINOX", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Epoch", "EPOCH", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_BZero", "BSCALE", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_BScale", "BZERO", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_Bitpix", "BITPIX", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_MJDObs", "MJD-OBS", false);
	SetNumberValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_JulianDate", "JD", false);
	
	SetDateValueFromFITSHeader(filename, attributes, "gov_nasa_gsfc_fits_DateObs", "DATE-OBS", "DATE", "UT");
	
	// ----- STANDARD kMDItemKind;	
	CFStringRef cfvalue = CFStringCreateWithCString(kCFAllocatorDefault, "Flexible Image Transport System", kCFStringEncodingUTF8);
	CFDictionaryAddValue(attributes, CFSTR("kMDItemKind"), cfvalue);

	free(filename);
	success = true;	
    return success;
}

