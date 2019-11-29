#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

int main_prg(int, char**);

int main(int argc, char** argv){

  struct rusage u;
  struct timeval start, end;
  
  getrusage(RUSAGE_SELF, &u);
  start = u.ru_utime;
  
  main_prg(argc, argv);
  
  getrusage(RUSAGE_SELF, &u );
  end = u.ru_utime;
  
  fprintf(stderr, "%lf\n", (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec)*1e-6);
    
  return(0);
}

int main_prg(int argc, char** argv){
   assert( argc==3 );
   FILE *fp_in = fopen( argv[1], "r" );
   assert( fp_in != NULL );
   FILE *fp_out = fopen( argv[2], "w" );
   assert( fp_out != NULL );

  /** implement here
   *
   * read input values from fp_in
   * write output values to fp_out
   **/
  
  return 0 ;
}

