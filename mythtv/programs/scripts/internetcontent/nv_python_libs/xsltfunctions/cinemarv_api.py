# -*- coding: UTF-8 -*-

# ----------------------
# Name: cinemarv_api - XPath and XSLT functions for the CinemaRV.com grabber
# Python Script
# Author:   R.D. Vaughan
# Purpose:  This python script is intended to perform a variety of utility functions
#           for the conversion of data to the MNV standard RSS output format.
#           See this link for the specifications:
#           http://www.mythtv.org/wiki/MythNetvision_Grabber_Script_Format
#
# License:Creative Commons GNU GPL v2
# (http://creativecommons.org/licenses/GPL/2.0/)
#-------------------------------------
__title__ ="cinemarv_api - XPath and XSLT functions for the CinemaRV.com grabber"
__author__="R.D. Vaughan"
__purpose__='''
This python script is intended to perform a variety of utility functions
for the conversion of data to the MNV standard RSS output format.
See this link for the specifications:
http://www.mythtv.org/wiki/MythNetvision_Grabber_Script_Format
'''

__version__="v0.1.0"
# 0.1.0 Initial development


# Specify the class names that have XPath extention functions
__xpathClassList__ = ['xpathFunctions', ]

# Specify the XSLT extention class names. Each class is a stand lone extention function
#__xsltExtentionList__ = ['xsltExtExample', ]
__xsltExtentionList__ = []

import os, sys, re, time, datetime, shutil, urllib.request, urllib.parse, urllib.error, string
from copy import deepcopy
import io

class OutStreamEncoder(object):
    """Wraps a stream with an encoder"""
    def __init__(self, outstream, encoding=None):
        self.out = outstream
        if not encoding:
            self.encoding = sys.getfilesystemencoding()
        else:
            self.encoding = encoding

    def write(self, obj):
        """Wraps the output stream, encoding Unicode strings with the specified encoding"""
        if isinstance(obj, str):
            obj = obj.encode(self.encoding)
        try:
            self.out.buffer.write(obj)
        except OSError:
            pass

    def __getattr__(self, attr):
        """Delegate everything but write to the stream"""
        return getattr(self.out, attr)

if isinstance(sys.stdout, io.TextIOWrapper):
    sys.stdout = OutStreamEncoder(sys.stdout, 'utf8')
    sys.stderr = OutStreamEncoder(sys.stderr, 'utf8')

try:
    from io import StringIO
    from lxml import etree
except Exception as e:
    sys.stderr.write('\n! Error - Importing the "lxml" and "StringIO" python libraries failed on error(%s)\n' % e)
    sys.exit(1)


class xpathFunctions(object):
    """Functions specific extending XPath
    """
    def __init__(self):
        self.functList = ['cinemarvLinkGeneration', 'cinemarvIsCustomHTML', 'cinemarvCheckIfDBItem', ]
        self.TextTail = etree.XPath("string()")
        self.persistence = {}
    # end __init__()

######################################################################################################
#
# Start of XPath extension functions
#
######################################################################################################

    def cinemarvLinkGeneration(self, context, *args):
        '''Generate a link for the CinemaRV.com site. A read of the item's web page is required to
        extract the flash video id.
        Call example: 'mnvXpath:cinemarvLinkGeneration(string(link))'
        return the url link
        '''
        webURL = args[0]
        # If this is for the download then just return what was found for the "link" element
        if 'cinemarvLinkGeneration' in self.persistence:
            if self.persistence['cinemarvLinkGeneration'] is not None:
                returnValue = self.persistence['cinemarvLinkGeneration']
                self.persistence['cinemarvLinkGeneration'] = None
                return returnValue
        else:
            self.persistence['cinemarvLinkGenerationVideoID'] = etree.XPath('//object[@id="flashObj"]//param[@name="flashVars"]/@value', namespaces=common.namespaces)
            self.persistence['cinemarvLinkGenerationParser'] = etree.HTMLParser()

        try:
            webPageElement = etree.parse(webURL, self.persistence['cinemarvLinkGenerationParser'])
        except Exception as errmsg:
            sys.stderr.write('!Warning: The web page URL(%s) could not be read, error(%s)\n' % (webURL, errmsg))
            return webURL
        if webPageElement is None:
            self.persistence['cinemarvLinkGeneration'] = webURL
            return webURL

        tmpVideoID = self.persistence['cinemarvLinkGenerationVideoID'](webPageElement)
        if not len(tmpVideoID):
            self.persistence['cinemarvLinkGeneration'] = webURL
            return webURL
        index = tmpVideoID[0].find('&')
        if index == -1:
            self.persistence['cinemarvLinkGeneration'] = webURL
            return webURL
        videocode = tmpVideoID[0][:index].replace('videoId=', '')
        self.persistence['cinemarvLinkGeneration'] = common.linkWebPage('dummycontext', 'cinemarv')+videocode
        return self.persistence['cinemarvLinkGeneration']
    # end cinemarvLinkGeneration()

    def cinemarvIsCustomHTML(self, context, *args):
        '''Check if the link is for a custom HTML
        Example call: mnvXpath:cinemarvIsCustomHTML(('dummy'))
        return True if the link does not starts with "http://"
        return False if the link starts with "http://"
        '''
        if self.persistence['cinemarvLinkGeneration'] is None:
            return False

        if self.persistence['cinemarvLinkGeneration'].startswith('http://'):
            return False
        else:
            return True
    # end cinemarvIsCustomHTML()

    def cinemarvCheckIfDBItem(self, context, *arg):
        '''Use a unique key value pairing to find out if the 'internetcontentarticles' table already
        has a matching item. This is done to save accessing the Internet when not required.
        Call example: 'mnvXpath:cinemarvCheckIfDBItem(.)'
        return True if a match was found
        return False if a match was not found
        '''
        return common.checkIfDBItem('dummy', {'feedtitle': 'Movie Trailers', 'title': arg[0].replace('Trailer', '').strip(), 'author': arg[1], 'description': arg[2]})
    # end cinemarvCheckIfDBItem()

######################################################################################################
#
# End of XPath extension functions
#
######################################################################################################

######################################################################################################
#
# Start of XSLT extension functions
#
######################################################################################################

######################################################################################################
#
# End of XSLT extension functions
#
######################################################################################################
