/**

@page cvs Getting Rio with anonymous CVS LAL server

  The Rio package (along all the packages developed at LAL)
 can be retrieved by using the anonymous LAL CVS server. For example, by 
 using a csh UNIX shell :
@verbatim
   csh> setenv CVSROOT :pserver:anonymous@cvsserver.lal.in2p3.fr:/projects/cvs
   csh> cvs login
   password : <empty>
   csh> mkdir Rio
   csh> cd Rio
   csh> cvs co -d <version> -r <version> Rio
@endverbatim
    

*/
