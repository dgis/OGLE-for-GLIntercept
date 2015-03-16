This is an adaptation of the plugin OGLE for the GLIntercept version 1.3.2
It run very well under Windows 7.
I found the source code licenced under GPL version 2.0, here: http://web.archive.org/web/20100516151113/http://ogle.eyebeamresearch.org/download


CHANGES

Version 0.3c, March 16, 2015

- Update the MTL2 library from version 2.1.2-20 to 2.1.2-22 (http://www.osl.iu.edu/research/mtl/)
- Compile with Visual Studio 2012 for GLIntercept version 1.3.2 with plugin API version 102.


Version 0.3b, March 31, 2005

- Added CaptureTextureCoords option to enable capture of vertices' texture coordinates (UV).


Version 0.2b, January 30, 2005

- Added FlipPolygonStrips option to correct the flipping of every-other polygon draw with *_STRIP OpenGL primitives.


BUILD

To build the OGLE.dll library with Visual Studio 2012, you must put this folder in the GLIntercept source tree as the following:

GLIntercept
├───3rdParty
├───Bin
├───Docs
├───Include
└───Src
	├───Common
	├───GLIConfig
	├───Intermediate
	├───MainLib
	├───Plugins
	│	└───OGLE <------------
	│		├───config
	│		├───docs
	│		├───mtl
	│		└───Ptr
	└───WorkSpaces
