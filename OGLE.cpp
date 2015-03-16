#include "stdafx.h"
#include "OGLEPlugin.h"
#include "ogle.h"
#include "Ptr/Ptr.in"

#include <string>

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 34962
#endif

#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 34963
#endif

#ifndef GL_READ_ONLY
#define GL_READ_ONLY 35000
#endif

#ifndef GL_WRITE_ONLY
#define GL_WRITE_ONLY 35001
#endif

/////////////////////////////////////////////
// Static OGLE Variables
/////////////////////////////////////////////

int OGLE::vertexCount = 0;
int OGLE::normalCount = 0;
int OGLE::groupCount = 0;

float OGLE::scale = 1.0;
bool OGLE::logFunctions = 0;
bool OGLE::captureNormals = 0;

char *OGLE::polyTypes[] = {
	"TRIANGLES",
	"TRIANGLE_STRIP",
	"TRIANGLE_FAN",
	"QUADS",
	"QUAD_STRIP",
	"POLYGON"
};
int OGLE::nPolyTypes = 6;
map<const char*, bool, OGLE::ltstr> OGLE::polyTypesEnabled;

FILE *OGLE::LOG = fopen("ogle.log", "w");


/////////////////////////////////////////////
// OGLE Initialization
/////////////////////////////////////////////

OGLE::OGLE(InterceptPluginCallbacks *_callBacks, const GLCoreDriver *_GLV) :
currSet(0),
currNormal(0),
currTexture(0),
callBacks(_callBacks),
GLV(_GLV),
buffers(2048)
{
	currSet = 0;
	currNormal = 0;
	currTexture = 0;
}

void OGLE::startRecording(string _objFileName) {
	OGLE::init();
	objFileName = _objFileName;
	objFile = fopen(objFileName.c_str(), "w");
}

void OGLE::stopRecording() {
	objFile = 0;
	objFileName = "";
}

void OGLE::init() {
	vertexCount = 0;
	normalCount = 0;
	groupCount = 0;
}


/////////////////////////////////////////////////////////////////////////////////
// OGLE's OpenGLFunctions
/////////////////////////////////////////////////////////////////////////////////

void OGLE::glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, 
							GLenum type, const GLvoid *indices) {

  checkBuffers();

  indices = getBufferedIndices(indices);

  if(!indices) {
	  fprintf(OGLE::LOG, "\tOGLE::glDrawRangeElements: indices is null\n");	

	  return;
  }

  newSet(mode);

  for(int i = 0; i < count; i++) {
	GLint index = derefIndexArray(type, indices, i);

	if(index >= start && index <= end) {
		glArrayElement(index);
	}
  }

  addSet(currSet);
  currSet = 0;

}


void OGLE::glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    checkBuffers();

	indices = getBufferedIndices(indices);

	if(!indices) {
		fprintf(OGLE::LOG, "\tOGLE::glDrawElements: indices is null\n");	
		return;
	}

	newSet(mode);

	int index;
	for(int i = 0; i < count; i ++) {
		index = derefIndexArray(type, indices, i);
		glArrayElement(index);
	}

	addSet(currSet);
	currSet = 0;
}


void  OGLE::glDrawArrays (GLenum mode, GLint first, GLsizei count) {

  checkBuffers();

  newSet(mode);

  for(int i = 0; i < count; i ++) {
	GLint index = i + first;
	glArrayElement(index);
  }

  addSet(currSet);
  currSet = 0;
}

void OGLE::glBegin(GLenum mode) {
    checkBuffers();

	newSet(mode);
}

void OGLE::glEnd() {
  addSet(currSet);
  currSet = 0;
}

void  OGLE::glVertexfv(GLfloat *V, GLsizei n) {

	if(currSet) {
		currSet->addElement(new OGLE::Element(new OGLE::Vertex(V, n), currTexture, 
			(captureNormals ? currNormal : 0))
			);
	}
}

void  OGLE::glNormalfv(GLfloat *V, GLsizei n) {
	if(captureNormals) {
		currNormal = new OGLE::Vertex(V, n);
		currNormal->w = 0;
	}
}

void OGLE::glArrayElement(GLint i) {

	if(isElementLocked(i)) {
	  fprintf(OGLE::LOG, "\tLOCKED OUT %d\n", i);
	  return;
	}

	BlobPtr enabled = glState["GL_VERTEX_ARRAY"];

	if(!enabled && enabled->toBool()) return;

	const GLbyte *array = 0, *narray = 0;	
	BlobPtr arrayP, narrayP;

	arrayP = glState["GL_VERTEX_ARRAY_POINTER"];
	if(arrayP) array = getBufferedArray((GLbyte *) arrayP->toVoidP());

	if(!array) {
		fprintf(OGLE::LOG, "O::gAE no VertexPointer\n");
		return;
	}

	GLint dim, j;
	GLenum type;
	GLsizei stride;
	GLfloat V[4];
	BlobPtr dimP, typeP, strideP;
	
	narrayP = glState["GL_NORMAL_ARRAY_POINTER"];
	if(narrayP && captureNormals) {
		if(narray = getBufferedArray((GLbyte *) narrayP->toVoidP())) {
			typeP = glState["GL_NORMAL_ARRAY_TYPE"];
			if((typeP && (type = typeP->toEnum()))) {
				strideP = glState["GL_NORMAL_ARRAY_STRIDE"];
				stride = strideP ? strideP->toSizeI() : 0;

				for(j = 0; j < 3; j++) {
					V[j] = derefVertexArray(narray, 3, type, stride, i, j);
				}
				glNormalfv(V, 3);				
			}
		}
	}

	dimP = glState["GL_VERTEX_ARRAY_SIZE"];	
	if(!(dimP && (dim = dimP->toInt()))) return;

	typeP = glState["GL_VERTEX_ARRAY_TYPE"];
	if(!(typeP && (type = typeP->toEnum()))) return;

	strideP = glState["GL_VERTEX_ARRAY_STRIDE"];
	stride = strideP ? strideP->toSizeI() : 0;



	for(j = 0; j < dim; j++) {
		V[j] = derefVertexArray(array, dim, type, stride, i, j);
	}

	if(currSet) {
		glVertexfv(V, dim);
	}
}


void OGLE::glEnableClientState (GLenum array)
{
  switch(array) {
	  case GL_VERTEX_ARRAY: glState["GL_VERTEX_ARRAY"] = new OGLE::Blob(GL_TRUE); break;
	  case GL_NORMAL_ARRAY: glState["GL_NORMAL_ARRAY"] = new OGLE::Blob(GL_TRUE); break;
  }
}

void OGLE::glDisableClientState (GLenum array)
{
  switch(array) {
	  case GL_VERTEX_ARRAY: 
		  glState["GL_VERTEX_ARRAY"] = 0; 		  
		  glState["GL_NORMAL_ARRAY"] = 0; 		  
		  break;
  }
}


void  OGLE::glLockArraysEXT(GLint first, GLsizei count) {	
  glState["GL_LOCK_ARRAYS_FIRST"] = new OGLE::Blob(first);  
  glState["GL_LOCK_ARRAYS_COUNT"] = new OGLE::Blob(count);  

}

void  OGLE::glUnlockArraysEXT() {
  glState["GL_LOCK_ARRAYS_FIRST"] = 0;
  glState["GL_LOCK_ARRAYS_COUNT"] = 0;
}

void OGLE::glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{	
  glState["GL_VERTEX_ARRAY_SIZE"] = new OGLE::Blob(size);  
  glState["GL_VERTEX_ARRAY_TYPE"] = new OGLE::Blob(type);  
  glState["GL_VERTEX_ARRAY_STRIDE"] = new OGLE::Blob(stride);  
  glState["GL_VERTEX_ARRAY_POINTER"] = new OGLE::Blob(pointer);
  this->glEnableClientState(GL_VERTEX_ARRAY);
}

void OGLE::glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{	
  glState["GL_NORMAL_ARRAY_TYPE"] = new OGLE::Blob(type);  
  glState["GL_NORMAL_ARRAY_STRIDE"] = new OGLE::Blob(stride);  
  glState["GL_NORMAL_ARRAY_POINTER"] = new OGLE::Blob(pointer);
  this->glEnableClientState(GL_NORMAL_ARRAY);
}


void OGLE::glBindBuffer(GLenum target, GLuint buffer) {

	switch(target) {
		case GL_ARRAY_BUFFER: 
			glState["GL_ARRAY_BUFFER_INDEX"] = new Blob(buffer); break;
		case GL_ELEMENT_ARRAY_BUFFER: 
			glState["GL_ELEMENT_ARRAY_BUFFER_INDEX"] = new Blob(buffer); break;
	}
}

void OGLE::glBufferData(GLenum target, GLsizei size, const GLvoid *data, GLenum usage) {
	GLuint index = getBufferIndex(target);

	if(index) {
		BufferPtr buff = new Buffer(data, size);
		buffers[index] = buff;
	}
}

void OGLE::glBufferSubData(GLenum target, GLint offset, GLsizei size, const GLvoid *data) {
	GLuint index = getBufferIndex(target);

	if(index) {
		BufferPtr buff = buffers[index];
		if(buff && offset < buff->size) {
			GLbyte *p = (GLbyte *) buff->ptr;
			if(offset + size > buff->size) {
				size -= offset + size - buff->size;
			}
			if(data) {
				memcpy(p + offset, data, size);
			}
		}
	}
}

void OGLE::glMapBuffer(GLenum target, GLenum access) {
	glState["GL_MAPPED_BUFFER_TARGET"] = new Blob(target);

	if(GLuint index = getBufferIndex(target)) {
		BufferPtr buff = buffers[index];

		if(buff) {
			buff->mapAccess = access;
		}
	}
}

void OGLE::glMapBufferPost(GLvoid *retValue) {
	BlobPtr target = glState["GL_MAPPED_BUFFER_TARGET"];

	if(target) {
		if(GLuint index = getBufferIndex(target->toEnum())) {
			BufferPtr buff = buffers[index];

			if(buff) {
				buff->map = retValue;
			}
		}
	}

	glState["GL_MAPPED_BUFFER_TARGET"] = 0;
}

void OGLE::glUnmapBuffer(GLenum target) {

	if(GLuint index = getBufferIndex(target)) {
		BufferPtr buff = buffers[index];

		if(buff) {
			if(buff->map && buff->mapAccess != 0*GL_WRITE_ONLY) {
				memcpy(buff->ptr, buff->map, buff->size);
			}
			buff->map = 0;
			buff->mapAccess = 0;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
// OGLE utility functions
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// OGLE's Helper Functions
/////////////////////////////////////////////////////////////////////////////////


void OGLE::initFunctions() {

	extensionVBOSupported = false;

	float oglVersion = callBacks->GetGLVersion();
	if(oglVersion >= 1.5f ||
		callBacks->IsGLExtensionSupported("GL_ARB_vertex_buffer_object")) {
		extensionVBOSupported = true;

	    //Attempty to get the core version
		iglGetBufferSubData = (GLvoid (GLAPIENTRY *) (GLenum, GLint, GLsizei, GLvoid *))callBacks->GetGLFunction("glGetBufferSubData");

		if(iglGetBufferSubData == NULL) {
		  //Attempt to get the ARB version instead
		  iglGetBufferSubData = (GLvoid (GLAPIENTRY *) (GLenum, GLint, GLsizei, GLvoid *))callBacks->GetGLFunction("glGetBufferSubDataARB");

		  if(iglGetBufferSubData == NULL) {
			 //Unable to get the entry point, don't check for VBOs
			 extensionVBOSupported = false;
		  }
		}
	}

//	fprintf(OGLE::LOG, "\tVBO: %d, iglMapBuffer: %p, iglUnmapBuffer: %p, iglGetBufferSubData: %p\n", extensionVBOSupported, iglMapBuffer, iglUnmapBuffer, iglGetBufferSubData); fflush(OGLE::LOG);

}





int OGLE::nextVertexID() {
	return ++vertexCount;
}

int OGLE::nextNormalID() {
	return ++normalCount;
}

int OGLE::nextGroupID() {
	return ++groupCount;
}

void OGLE::addSet(ElementSetPtr set) {
	
  if(! set) return;

  set->printToObj(objFile);

  // no need to store the ElementSets
  //  sets.push_back(set);

}

void OGLE::newSet(GLenum mode)
{
	Transform _transform = this->getCurrTransform();
	currSet = new OGLE::ElementSet(mode, _transform);
}


GLfloat OGLE::derefVertexArray(const GLbyte *array, GLint dim, GLenum type, GLsizei stride, GLint vindex, GLint index) {
	GLsizei size = glTypeSize(type);

	const GLbyte *p = array + vindex * (stride ? stride : (dim * size) ) + (index * size);

	GLfloat c;

	switch (type) {
		case GL_SHORT: c = *((GLshort *) p); break;
		case GL_INT: c = *((GLint *) p); break;
		case GL_FLOAT: c = *((GLfloat *) p); break;
		case GL_DOUBLE: c = *((GLdouble *) p); break;
	}
		
	return c;
}


#define GLVoidPDeRef(TYPE, ARRAY, INDEX) (  *((TYPE *)(((char *)ARRAY) + INDEX * sizeof(TYPE)))  )

GLint OGLE::derefIndexArray(GLenum type, const GLvoid *indices, int i) {
	GLint index = 0;

	switch (type) {
		case GL_UNSIGNED_BYTE: index = GLVoidPDeRef(GLubyte, indices, i); break;
		case GL_UNSIGNED_SHORT:  index = GLVoidPDeRef(GLushort, indices, i); break;
		case GL_UNSIGNED_INT:  index = GLVoidPDeRef(GLuint, indices, i);break;
    }

	return index; 

}

bool OGLE::isElementLocked(int i) {
  GLint lock_first;
  GLsizei lock_count;
  BlobPtr lfP, lcP;

  lfP =	glState["GL_LOCK_ARRAYS_FIRST"];
  lock_first = lfP ? lfP->toInt() : -1;

  lcP =	glState["GL_LOCK_ARRAYS_COUNT"];
  lock_count = lcP ? lcP->toSizeI() : 0;

  return (lock_first > -1 && lock_count > 0) && (i < lock_first || i > lock_first + lock_count);
}


GLuint OGLE::getBufferIndex(GLenum target) {
	BlobPtr index;

	switch(target) {
		case GL_ARRAY_BUFFER: 
			index = glState["GL_ARRAY_BUFFER_INDEX"]; break;
		case GL_ELEMENT_ARRAY_BUFFER: 
			index = glState["GL_ELEMENT_ARRAY_BUFFER_INDEX"]; break;
	}

	return index ? index->toUInt() : 0;
}


const GLbyte *OGLE::getBufferedArray(const GLbyte *array) {
	GLuint buffIndex = 0;
	GLuint offset = 0;
	BufferPtr buff;


	if(buffIndex = getBufferIndex(GL_ARRAY_BUFFER)) {
		BufferPtr buff = buffers[buffIndex];
		if(buff && buff->ptr) {
			offset = (GLuint)array;
			array = ((GLbyte *)buff->ptr) + offset;
		}
	}
	return array;
}

const GLvoid *OGLE::getBufferedIndices(const GLvoid *indices) {

	const GLbyte *ptr = (GLbyte *)indices;

	if(GLuint buffIndex = getBufferIndex(GL_ELEMENT_ARRAY_BUFFER)) {
		BufferPtr buff = buffers[buffIndex];
		if(!buff || buff->ptr == 0) {
			return 0;
		}
		if(buff && buff->ptr) {
			GLuint offset = (GLuint)ptr;
			ptr = ((GLbyte *)buff->ptr) + offset;
		}
	}


	return ptr;
}


// Look to see if the program has set up Vertex or Element buffers
// Copy them into buffers we will read from.

void OGLE::checkBuffers() {
	GLenum targets[2] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};

	if(!(extensionVBOSupported && iglGetBufferSubData)) {
		return;
	}

	for(int i = 0; i < 2; i++) {
		int index = getBufferIndex(targets[i]);

		BufferPtr bp = buffers[index];
		if(bp) {
		  iglGetBufferSubData(targets[i], 0, bp->size, bp->ptr);
		}
	}
}


GLsizei OGLE::glTypeSize(GLenum type) {
	GLsizei size;
	switch (type) {
		case GL_SHORT: size = sizeof(GLshort); break;
		case GL_INT: size = sizeof(GLint); break;
		case GL_FLOAT: size = sizeof(GLfloat); break;
		case GL_DOUBLE: size = sizeof(GLdouble); break;
	}	
	return size;
}


OGLE::VertexPtr OGLE::doTransform(VertexPtr vp, Transform T) {
	
	  OGLE::Vector V = vp->toVector();

	  OGLE::Vector R(4);

	  mtl::mult(T,V,R);

	  return new OGLE::Vertex(R);
}
	
OGLE::Transform OGLE::getCurrTransform() {
		int i, j;

		GLfloat mat[16];
		GLV->glGetFloatv(GL_MODELVIEW_MATRIX, mat);

		Transform T(4,4);
		for( i = 0; i < 4; i++) {
			for( j = 0; j < 4; j++) {
				T(j,i) = (float) mat[4*i + j];
			}
		}
		return T;
}

bool OGLE::isIdentityTransform(Transform T) {
	OGLE::Vector V(4,1);
	OGLE::Vector R(4);

	mtl::mult(T,V,R);

	float prod = 1.0;
	for(int t = 0; t < 4; t++) {
		prod *= R[t];
	}

	return prod == 1.0;
}





//////////////////////////////////////////////////////////////////////////////////
// OGLE::Vertex functions
//////////////////////////////////////////////////////////////////////////////////

void OGLE::Vertex::printToObjFile(FILE *f, const char *typeStr) {
	if(!f) return;

	float scale = OGLE::scale;
	fprintf(f, "v%s %e %e %e\n", typeStr, scale * x, scale * y, scale * z);
}


//////////////////////////////////////////////////////////////////////////////////
// OGLE::ElementSet functions
//////////////////////////////////////////////////////////////////////////////////

OGLE::ElementSet::ElementSet(GLenum _mode, Transform _transform) :
	mode(_mode),
	transform(_transform),
	hasTransform(1)
{}

OGLE::ElementSet::ElementSet(GLenum _mode) :
	mode(_mode),
	hasTransform(0)
{}

void OGLE::ElementSet::addElement(const GLfloat V[], GLsizei dim) {
	addElement(new Vertex(V, dim));
}

void OGLE::ElementSet::addElement(VertexPtr V) {	
	addElement(new OGLE::Element(V));
}

void OGLE::ElementSet::addElement(ElementPtr E) {
	if(hasTransform) {
		if(E->v) E->v = OGLE::doTransform(E->v, transform);
		if(E->n) E->n = OGLE::doTransform(E->n, transform);
	}

	vertices.push_back(E);
}

void OGLE::ElementSet::printToObj(FILE *f) {
	if(!f) return;

	if(hasTransform && OGLE::isIdentityTransform(transform)) {
//	  fprintf(OGLE::LOG, "\t\tMODELVIEW WAS IDENTITY\n");
//	  return;
	}

	int i;

	ObjFile::FacePtr face = new ObjFile::Face();

	if(0) {}
	else if(mode == GL_TRIANGLES && polyTypesEnabled["TRIANGLES"]) {
		if(vertices.size() >= 3) fprintf(f, "#TRIANGLES\ng %d\n", OGLE::nextGroupID());
		for(i = 0; i < vertices.size(); i++) {
			VertexPtr v = vertices[i]->v;
			v->printToObjFile(f);

			ObjFile::Vertex ov(OGLE::nextVertexID());			

			VertexPtr n = vertices[i]->n;

			if(n) {
				ov.nid = OGLE::nextNormalID();
				n->printToObjFile(f, "n");
			}

			face->addVertex(ov);

			if(((i + 1) % 3) == 0) {
				face->printToObj(f);
				face->clear();
			}
		}
	}

	else if(mode == GL_TRIANGLE_STRIP && polyTypesEnabled["TRIANGLE_STRIP"]) {
		if(vertices.size() >= 3) fprintf(f, "#TRIANGLE_STRIP\ng %d\n", OGLE::nextGroupID());
		for(i = 0; i < vertices.size(); i++) {
			VertexPtr v = vertices[i]->v;
			v->printToObjFile(f);

			ObjFile::Vertex ov(OGLE::nextVertexID());			

			VertexPtr n = vertices[i]->n;
			if(n) {
				ov.nid = OGLE::nextNormalID();
				n->printToObjFile(f, "n");
			}

			face->addVertex(ov);

			if(i >= 2) {
				face->printToObj(f);
				face->shiftVertices(1);
			}
		}
	}

	else if(mode == GL_TRIANGLE_FAN && polyTypesEnabled["TRIANGLE_FAN"]) {
		if(vertices.size() >= 3) fprintf(f, "#TRIANGLE_FAN\ng %d\n", OGLE::nextGroupID());

		ObjFile::Vertex firstv;

		for(i = 0; i < vertices.size(); i++) {
			VertexPtr v = vertices[i]->v;
			v->printToObjFile(f);

			ObjFile::Vertex ov(OGLE::nextVertexID());			

			VertexPtr n = vertices[i]->n;
			if(n) {
				ov.nid = OGLE::nextNormalID();
				n->printToObjFile(f, "n");
			}

			if(i == 0) {
				firstv = ov;
			}

			face->addVertex(ov);

			if(i >= 2) {
				face->printToObj(f);
				face->clear();
				face->addVertex(firstv);
			}
		}
	}

	else if(mode == GL_QUADS && polyTypesEnabled["QUADS"]) {
		if(vertices.size() >= 4) fprintf(f, "#QUADS\ng %d\n", OGLE::nextGroupID());
		for(i = 0; i < vertices.size(); i++) {
			VertexPtr v = vertices[i]->v;
			v->printToObjFile(f);

			ObjFile::Vertex ov(OGLE::nextVertexID());			

			VertexPtr n = vertices[i]->n;
			if(n) {
				ov.nid = OGLE::nextNormalID();
				n->printToObjFile(f, "n");
			}

			face->addVertex(ov);

			if(((i + 1) % 4) == 0) {
				face->printToObj(f);
				face->clear();
			}
		}
	}

	else if(mode == GL_QUAD_STRIP && polyTypesEnabled["QUAD_STRIP"]) {
		if(vertices.size() >= 4) fprintf(f, "#QUAD_STRIP\ng %d\n", OGLE::nextGroupID());
		for(i = 0; i < vertices.size(); i++) {
			VertexPtr v = vertices[i]->v;
			v->printToObjFile(f);

			ObjFile::Vertex ov(OGLE::nextVertexID());			

			VertexPtr n = vertices[i]->n;
			if(n) {
				ov.nid = OGLE::nextNormalID();
				n->printToObjFile(f, "n");
			}

			face->addVertex(ov);

			if(i >= 3) {
				face->printToObj(f);
				face->shiftVertices(1);
			}
		}
	}

	else if(mode == GL_POLYGON && polyTypesEnabled["POLYGON"]) {
		if(vertices.size() >= 3) fprintf(f, "#POLYGON [%d]\ng %d\n", vertices.size(), OGLE::nextGroupID());
		for(i = 0; i < vertices.size(); i++) {
			VertexPtr v = vertices[i]->v;
			v->printToObjFile(f);

			ObjFile::Vertex ov(OGLE::nextVertexID());			

			VertexPtr n = vertices[i]->n;
			if(n) {
				ov.nid = OGLE::nextNormalID();
				n->printToObjFile(f, "n");
			}

			face->addVertex(ov);
		}
	}

	fflush(f);
}


void OGLE::ObjFile::Face::printToObj(FILE *f) {
	fprintf(f, "f ");
	for(int i = 0; i < vertices.size(); i++) {
		Vertex v = vertices[i];
		fprintf(f, "%d", v.vid);
		if(v.nid) {
			fprintf(f, "//%d", v.nid);
		}
		fprintf(f, " ");
	}
	fprintf(f, "\n");
}