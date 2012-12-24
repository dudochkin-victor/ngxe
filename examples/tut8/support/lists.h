#ifndef LISTS_HH
#define LISTS_HH

#include <GL/gl.h>

/*
 *  A simple class to use display lists with.
 *
 *
 *
 */
typedef class GLListsclass{
 public:
  //constructors & destructors
  GLListsclass();
  GLListsclass(int nr);
  ~GLListsclass();
  
  //select list if there is more than 1 else only 0 is valid
  int Select(int nr);

  //general usage
  void Compile() {glNewList(curlist,GL_COMPILE);}
  void Execute() {glCallList(curlist);}
  void CompileExecute() {glNewList(curlist,GL_COMPILE_AND_EXECUTE);}  
  void End() {glEndList();}

 private:
  GLuint lists;
  GLint nrlists;
  GLuint curlist;


}GLListsclass;




#endif
