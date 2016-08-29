#!/bin/sh

#  install.sh
#  FITSImporter
#
#  Created by Cédric Foellmi on 29/08/16.
#  Copyright (c) 2016 onekiloparsec. All rights reserved.

#!/bin/sh

GENERATOR_NAME="FITSImporter.mdimporter"
DOWNLOAD_URL="https://github.com/onekiloparsec/FITSImporter/releases/download/1.2.0/${GENERATOR_NAME}.zip"
SYSTEM_IMPORTER_DIR="/Library/Spotlight"
LOCAL_IMPORTER_DIR="${HOME}/Library/Spotlight"
ZIP_FILE_PATH="${SYSTEM_IMPORTER_DIR}/${GENERATOR_NAME}.zip"

echo "\n *** Installing ${GENERATOR_NAME} into ${SYSTEM_IMPORTER_DIR}"

if [ -e "${LOCAL_IMPORTER_DIR}/${GENERATOR_NAME}" ]; then
    echo "\n >>> An old importer is located in ${LOCAL_IMPORTER_DIR}"
    echo " >>> You should remove it first, to avoid conflicts, and relaunch the same command."
    echo " >>> Here is the command to issue:\n$ rm -rf ${LOCAL_IMPORTER_DIR}/${GENERATOR_NAME}\n"
    exit 0
fi

sudo mkdir -p "${SYSTEM_IMPORTER_DIR}"
# curl -kL $DOWNLOAD_URL | /usr/bin/bsdtar -x -v -z -C "${SYSTEM_IMPORTER_DIR}"
echo "\n *** Downloading FITSImporter from https://github.com/onekiloparsec/FITSImporter..."
sudo curl -kL -# $DOWNLOAD_URL -o ${ZIP_FILE_PATH}
echo "\n *** FITSImporter successfully downloaded. Unzipping..."

sudo unzip -o -q ${ZIP_FILE_PATH} -d ${SYSTEM_IMPORTER_DIR}
if [ -s ${SYSTEM_IMPORTER_DIR}/${GENERATOR_NAME} ]
then
  echo " *** FITSImporter successfully unzipped. "
  sudo rm -f "${SYSTEM_IMPORTER_DIR}/${GENERATOR_NAME}.zip" >& /dev/null
else
  echo " *** Couldn't unzip the file: ${ZIP_FILE_PATH} ???"
  echo " *** Try restarting the script. Or send a mail to cedric@onekilopars.ec\n\n"
  exit 1
fi

echo "\n *** Importing the new Spotlight importer ..."
/usr/bin/mdimport -r /Library/Spotlight/FITSImporter.mdimporter

echo "\n *** FITSImporter successfuly installed! Enjoy. All inquiry to be sent to cedric@onekilopars.ec"
echo " *** More FITS utilities as well as awesome apps for astronomers: http://onekilopars.ec\n\n"
