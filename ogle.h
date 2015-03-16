#ifndef __OGLE_H_
#define __OGLE_H_

#include "mtl/mtl.h"
#include "mtl/matrix.h"


#include <vector>
#include <deque>
#include <map>
#include <string>

#include "Ptr/Interface.h"
#include "Ptr/Ptr.h"

#define OGLE_BIND_BUFFERS_ALL_FRAMES 1
#define OGLE_CAPTURE_BUFFERS_ALL_FRAMES 0

class OGLE : public Interface {

public:

	// Classes for 4x4 Matrices and 4x1 Vectors, 
	// the typical mathematical components of OpenGL calculations

	typedef mtl::matrix < float >::type Transform;
    typedef mtl::dense1D < float > Vector;


	//////////////////////////////////////////////////////////////////////
	// OGLE::Vertex -- Class for OpenGL Vertexinate
	//////////////////////////////////////////////////////////////////////

	class Vertex : public Interface {
		public:
			GLfloat x,y,z,w;

			inline Vertex();
			inline Vertex(Vector v);
			inline Vertex(const GLfloat *v, GLint n);
			inline Vertex(GLfloat _x, GLfloat _y, GLfloat _z = 0, GLfloat _w = 1);

			inline void init(GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0, GLfloat _w = 1);
			inline void fromVector(Vector v);

			inline Vector toVector();

			void printToObjFile(FILE *f, const char *typeStr = "");	
	};
	typedef Ptr<Vertex> VertexPtr;

	//////////////////////////////////////////////////////////////////////
	// OGLE::Element -- Class for an OpenGL 'Element' which can have a 
	// Vertex (location), as well as a Normal and a Texture coordinate
	//////////////////////////////////////////////////////////////////////

	class Element : public Interface {
		public:	
			Element() { v = 0; t = 0; n = 0; }

			Element(VertexPtr _v, VertexPtr _t = 0, VertexPtr _n = 0) {
				v = _v; t = _t; n = _n;
			}

			VertexPtr v,t,n;
	};
	typedef Ptr<Element> ElementPtr;

	typedef std::vector<ElementPtr> ElementVec;
	typedef std::vector<VertexPtr> VertexVec;


	//////////////////////////////////////////////////////////////////////
	// OGLE::ElementSet -- Class for a set of OpenGL vertices
	//////////////////////////////////////////////////////////////////////	

	class ElementSet : public Interface {
		public:
			ElementSet(GLenum _mode, Transform _transform);
			ElementSet(GLenum _mode);

  			// add a Element with just a location Vertex
			void addElement(const GLfloat V[], GLsizei dim);
			void addElement(VertexPtr V);
			void addElement(ElementPtr E);


			void printToObj(FILE *f);

			bool hasTransform;
			Transform transform;	
			//vector<VertexPtr> vertices;

			/* for now we are only doing the locations, not the normals/textures */
			vector<ElementPtr> vertices;
			/**/

			GLenum mode;
	};
	typedef Ptr<ElementSet> ElementSetPtr;

	//////////////////////////////////////////////////////////////////////
	// OGLE::ObjFile -- Class for outputting data to an ObjFile
	//////////////////////////////////////////////////////////////////////	

	class ObjFile : public Interface {
		public:

			struct Vertex {
				public:
					int vid, nid;
					Vertex(int _vid = 0, int _nid = 0) : vid(_vid), nid(_nid) {}
			};

			class Face : public Interface {
				public:
					Face() {}
					inline void addVertex(Vertex v);
					inline void shiftVertices(int n = 1);
					inline void clear();
					void printToObj(FILE *f);

					deque<Vertex>vertices;
			};

			typedef Ptr<Face> FacePtr;

	};

	//////////////////////////////////////////////////////////////////////
	// OGLE::Blob -- Class for polymorphic storage of small primitive types
	//////////////////////////////////////////////////////////////////////	

	class Blob : public Interface {
		private:
			char d[16];

		public:
			Blob(const GLint v) { *((GLint *)d) = v; }
			const GLint toInt() { return *((GLint *)d); } 
			const GLuint toUInt() { return *((GLuint *)d); } 
			const GLsizei toSizeI() { return *((GLsizei *)d); } 

			Blob(const GLboolean v) { *((GLboolean *)d) = v; }
			const GLboolean toBool() { return *((GLboolean *)d); } 

			Blob(const GLfloat v) { *((GLfloat *)d) = v; }
			const GLfloat toFloat() { return *((GLfloat *)d); } 
			
			Blob(const GLenum v) { *((GLenum *)d) = v; }
			const GLenum toEnum() { return *((GLenum *)d); } 
			
			Blob(const GLvoid *v) {*((const GLvoid **)d) = v;}
			const GLvoid * toVoidP() { return *((const GLvoid **)d);}			
	};
			
	typedef Ptr<Blob> BlobPtr;


	//////////////////////////////////////////////////////////////////////
	// OGLE::Buffer -- Class for a buffer
	//////////////////////////////////////////////////////////////////////	

	class Buffer : public Interface {
		public:
			GLvoid *ptr;
			GLsizei size;

			GLvoid *map;
			GLenum mapAccess;

			inline Buffer(const GLvoid *_ptr, GLsizei _size);
			inline ~Buffer();
	};
				
	typedef Ptr<Buffer> BufferPtr;


	////////////////////////////////////////////////////////////////////////
	// OGLE -- the class for actually doing the OpenGLExtraction
	////////////////////////////////////////////////////////////////////////

	struct ltstr
	{
	  bool operator()(const char* s1, const char* s2) const
	  { return strcmp(s1, s2) < 0; }
	};


	OGLE(InterceptPluginCallbacks *_callBacks, const GLCoreDriver *_GLV);

		
	void startRecording(string _objFileName);
	void stopRecording();

	void addSet(ElementSetPtr set);
	void newSet(GLenum mode);

	void glBegin (GLenum mode);
	void glEnd ();

	void glVertexfv(GLfloat *V, GLsizei n);
	void glNormalfv(GLfloat *V, GLsizei n);

	void glArrayElement (GLint i);
	void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	void glDrawArrays (GLenum mode, GLint first, GLsizei count);
	void glEnableClientState (GLenum array);
	void glDisableClientState (GLenum array);
	void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);

	void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
	void glLockArraysEXT(GLint first, GLsizei count);
	void glUnlockArraysEXT();
	void glBindBuffer(GLenum target, GLuint buffer);
	void glBufferData(GLenum target, GLsizei size, const GLvoid *data, GLenum usage);
	void glBufferSubData(GLenum target, GLint offset, GLsizei size, const GLvoid *data);
	void glMapBuffer(GLenum target, GLenum access);
	void glMapBufferPost(GLvoid *retValue);
	void glUnmapBuffer(GLenum target);

	void initFunctions();
	Transform getCurrTransform();
	GLuint getBufferIndex(GLenum target);
	const GLbyte *OGLE::getBufferedArray(const GLbyte *array);
	const GLvoid *getBufferedIndices(const GLvoid *indices);
	void checkBuffers();
	bool isElementLocked(int index);



	InterceptPluginCallbacks *callBacks;
    const GLCoreDriver       *GLV;                  //The core OpenGL driver
	std::map<const char*, BlobPtr, ltstr> glState;
	std::vector<BufferPtr> buffers;

	bool extensionVBOSupported;
	void    (GLAPIENTRY *iglGetBufferSubData) (GLenum, GLint, GLsizei, GLvoid *);

    Ptr<ElementSet> currSet;
    VertexPtr currTexture, currNormal;
	std::vector<ElementSetPtr> sets;

	string objFileName;
	FILE *objFile;



	//////////////////////////////////////////////////////////////////////////////
	// Static methods and variables for the OGLE class

	static int nextVertexID();
	static int nextNormalID();
	static int nextGroupID();
	static void init();

	static GLfloat OGLE::derefVertexArray(const GLbyte *array, GLint dim, GLenum type, GLsizei stride, GLint vindex, GLint index);
	static GLint derefIndexArray(GLenum type, const GLvoid *indices, int i);

	static VertexPtr doTransform(VertexPtr vp, Transform T);
	static bool isIdentityTransform(Transform T);
	static GLsizei glTypeSize(GLenum type);

	static int vertexCount;
	static int normalCount;
	static int groupCount;

	static FILE *LOG;
	static float scale;
	static bool logFunctions;
	static bool captureNormals;
	static char *polyTypes[];
	static int nPolyTypes;
	static map<const char*, bool, ltstr>polyTypesEnabled;
};

typedef Ptr<OGLE> OGLEPtr;





/////////////////////////////////////////////////////////////////////////////////
//
// OGLE::Vertex Short/obvious Methods
//
/////////////////////////////////////////////////////////////////////////////////




OGLE::Vertex::Vertex() {
	init();
}

OGLE::Vertex::Vertex(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) {
	init(x,y,z,w);
}


void OGLE::Vertex::init(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) {
			x = _x; y = _y; z = _z; w = _w;
}

OGLE::Vertex::Vertex(Vector v) {
	init();
	this->fromVector(v);
}

OGLE::Vertex::Vertex(const GLfloat *v, GLint n) {
	  init();
	  Vector V(n);
	  for(int i = 0; i < n; i++) {
		  V[i] = (float)v[i];
	  }
	  this->fromVector(V);
}

void OGLE::Vertex::fromVector(Vector v) {
	switch (v.size()) {
	case 4: w = v[3];
	case 3: z = v[2];
	case 2: y = v[1];
	case 1: x = v[0];				
	}
}


OGLE::Vector OGLE::Vertex::toVector() {
	Vector v(4);
	v[0] = x; v[1] = y; v[2] = z; v[3] = w;
	return v;
}

OGLE::Buffer::Buffer(const GLvoid *_ptr, GLsizei _size) {
	size = _size;
	ptr = malloc(size);

	map = 0;
	mapAccess = 0;

	if(_ptr) {
		memcpy(ptr, _ptr, size);
	}
}


void OGLE::ObjFile::Face::addVertex(Vertex v) { 
	vertices.push_back(v); 
}

void OGLE::ObjFile::Face::shiftVertices(int n) { 
	while(n-- > 0) vertices.pop_front(); 
}

void OGLE::ObjFile::Face::clear() { 
	vertices.clear(); 
}

OGLE::Buffer::~Buffer() {
	if(ptr) free(ptr);
}


#endif // __OGLE_H_

