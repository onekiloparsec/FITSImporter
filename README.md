<h3 align="center">
	<img src="http://onekilopars.ec/s/1kpcAstroComponents.png" width="100%" />
</h3>
<p align="center">
<a href="https://github.com/onekiloparsec/SwiftAA">SwiftAA</a> &bull;
<a href="https://github.com/onekiloparsec/QLFits">QLFits</a> &bull;
<b>FITSImporter</b> &bull; 
<a href="https://github.com/onekiloparsec/ObjCFITSIO">ObjCFITSIO</a> 
</p>

FITSImporter
========

**One-line Installer**: Copy-paste the following command into a Terminal window (click [here](https://raw.github.com/onekiloparsec/FITSImporter/master/Scripts/install.sh) to check that it is harmless): 

`curl -fsSLk https://raw.github.com/onekiloparsec/FITSImporter/master/Scripts/install.sh | sh`


Description
-----------

FITSImporter is an OSX Spotlight plugin for FITS (Flexible Image Transport System) files (used by astronomers worldwide to store and share their data).

Below a typical "Get Info" panel applied to a FITS file, with FITSImporter installed.

<img src="Resources/FITSImporter_snap1.png" width=345px>

A typical "search" in the Finder. As you can see, you can search for FITS files using typical astronomical informations (R.A, Dec and so on). 

<img src="Resources/FITSImporter_snap2.png" width=700px>

Be careful, though, the FITSImporter is not responsible for the automatic thumbnailing of the FITS images you can see above (in the CoverFlow of the Finder). That's the role of [QLFits](https://github.com/onekiloparsec/QLFits), the OSX QuickLook plugin for FITS files.


Troubleshooting
-----------

If you have troubles with your FITS files being recognized by Gimp.app or SAO ds9.app, you can unlink these apps to the fits extensions. To unlink, run this command:

lsregister -u /Applications/<Path to your app>

The tool is located here:

/System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/LaunchServices.framework/Versions/A/Support/lsregister


OSX dynamic types
-----------------

For custom or special file extensions, OSX assign dynamic types. FITSImporter contains a list of such types to allow it to recognize them (in addition to the filename extensions). See Info.plist file inside FITSImporter.mdimporter in case of doubt. To check the dynamic type of your FITS file, open a Terminal and type the following command `mdls <path/to/your/file> | grep kMDItemContentType`. You shluld obtain something like `kMDItemContentType             = "dyn.ah62d4rv4ge80q4pysq"`. 

List of known extensions: 

* gov.nasa.gsfc.fits
* gov.nasa.gsfc.fits-header
* com.sao.SAOImageDS9
* dyn.ah62d4rv4ge80q4pysq
* dyn.ah62d4rv4ge80q4py
* dyn.ah62d4rv4ge80q7a
* dyn.ah62d4rv4ge8047a
* dyn.ah62d4rv4ge81a4db
* dyn.ah62d4rv4ge80c6xg
* dyn.ah62d4rv4ge81e5pg
* dyn.ah62d4rv4ge81e65u
* dyn.ah62d4rv4ge81a4k
* dyn.ah62d4rv4ge80w5pgrf4a
* dyn.ah62d4rv4ge81n7xgrf4a
* dyn.ah62d4rv4ge80w5pgrf4hg
* dyn.ah62d4rv4ge81n7xgrf4hg
* dyn.ah62d4rv4ge80262
* dyn.ah62d46dzqm0gw23ssz1gw8brqz6gn25zsvu0e5dfhk2x43xmsu
* dyn.ah62d46dzqm0gw23ssz1gw8brqz6gn25zsvu0e5dfhk2x43xmsv3u


Installation
------------

*[Download the latest binary](http://onekilopars.ec/softwares/FITSImporter.mdimporter.zip)*

Unzip, and put it in `/Library/Spotlight` and then run the following command in your Terminal:

`/usr/bin/mdimport -r /Library/Spotlight/FITSImporter.mdimporter`

If you build from the source, the Xcode scheme post-build action will do it for you.


Author
------

[Cédric Foellmi](https://github.com/onekiloparsec) ([@onekiloparsec](https://twitter.com/onekiloparsec))


LICENSE & NOTES
---------------

FITSImporter is licensed under the MIT license and hosted on GitHub at https://github.com/onekiloparsec/FITSImporter/ Fork the project and feel free to send pull requests with your changes!


