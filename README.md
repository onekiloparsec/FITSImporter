FITSImporter
============

FITSImporter is an OSX Spotlight plugin for FITS (Flexible Image Transport System) files (used by astronomers worldwide to store and share their data).

You can directly [download](http://onekilopars.ec/softwares/FITSImporter.mdimporter.zip) it, from onekilopars.ec website.

Unzip, and put it here:

    ${HOME}/Library/Spotlight
    
and then run the following command in your Terminal.

    /usr/bin/mdimport -r ${HOME}/Library/Spotlight/FITSImporter.mdimporter
    
If you build from the souce, the Xcode scheme post-build action will do it for you.

Once done, you can look for FITS files in your drive using Spotlight.

If you have troubles with your FITS files being recognized by Gimp.app or SAO ds9.app, you can unlink these apps to the fits extensions. To unlink, run this command:

    lsregister -u /Applications/<Path to your app>
    
The tool is located here:

    /System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/LaunchServices.framework/Versions/A/Support/lsregister

<a href="https://flattr.com/submit/auto?user_id=onekiloparsec&url=https%3A%2F%2Fgithub.com%2Fonekiloparsec%2FFITSImporter" target="_blank"><img src="http://api.flattr.com/button/flattr-badge-large.png" alt="Flattr this" title="Flattr this" border="0"></a> 
([@onekiloparsec](http://twitter.com/onekiloparsec)). 

It is released open source under the [GNU General Public Licence](http://en.wikipedia.org/wiki/GNU_General_Public_License).

Below a typical "Get Info" panel applied to a FITS file, with FITSImporter installed.

<img src="Resources/FITSImporter_snap1.png" width=345px>

A typical "search" in the Finder. As you can see, you can search for FITS files using typical astronomical informations (R.A, Dec and so on). 

<img src="Resources/FITSImporter_snap2.png" width=700px>

Be careful, though, the FITSImporter is not responsible for the automatic thumbnailing of the FITS images you can see above (in the CoverFlow of the Finder). That's the role of [QLFits](https://github.com/onekiloparsec/QLFits), the OSX QuickLook plugin for FITS files.