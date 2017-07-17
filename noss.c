/***********************************************************************
 * Test NOSS algorithm
 *
 * Compile:
 *
 *  make CPPFLAGS=-D__DO_MAIN__ noss
 *
 * Usage (BASH):
 *
 *  [USE_NOSS2=] ./noss lon1 lon2[ lon3[ lon4[ ...]]
 *
 **********************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/***********************************************************************
 * Algorithm as originally written
 * - typo fix near end of first loop
 */
double
noss(double *lon, double *modLon, int nPts) {
double first;
double next;
double boostLon;
int i;

  first     = lon[0];
  modLon[0] = first;
  boostLon  = 0;

  for(i = 1; i < nPts; i++) {

        next = lon[i];

        if(fabs(first - next) > 180) {
             if(first < next) {
                  next -= 360;
                  boostLon = 360;
             }
             else {
                  next += 360;
             }
        }

        first = next;
        modLon[i] = first;
        /* typo? modLon[0] = first; */
  }

  if(boostLon) {

       for(i = 0; i < nPts; i++) {

           modLon[i] += boostLon;
       }
  }
  return boostLon;
}

/***********************************************************************
 * Algorithm re-written as an exercise to simplify and minimize storage
 */
double
noss2(double *lon, double *modLon, int nPts) {
int noBoostLon;
double deltaLon;
int i;

  modLon[0] = lon[0];
  noBoostLon  = 1;

  for(i = 1; i < nPts; i++) {

        deltaLon = modLon[i-1] - lon[i];

        if(deltaLon < -180) {
            modLon[i] = lon[i] - 360;
            noBoostLon = 0;
        }
        else if(deltaLon > 180) {
            modLon[i] = lon[i] + 360;
        }
        else {
            modLon[i] = lon[i];
        }
  }

  if(noBoostLon) return 0.;
  for(i = 0; i < nPts; i++) { modLon[i] += 360.; };
  return 360.;
}


/***********************************************************************
 * Test harness, in the form of main(int argc,char** argv)
 */
int
nossTest(int argc, char** argv, double (*nossCall)(double*,double*,int)) {
double* lon;
double* modLon;
int iArg;


  if (!--argc) return 1;
  ++argv;

  lon = malloc(argc * 2 * sizeof(double));
  modLon = lon + argc;

  for (iArg=0; iArg<argc; ++iArg) {
    if (1 != sscanf(argv[iArg],"%lf",lon+iArg)) {
      fprintf(stderr, "NOSSTEST:  bad argument; [%s] does not start with a valid number string\n", argv[iArg]);
      free(lon);
      return 2;
    }
    modLon[iArg] = lon[iArg];
  }

  fprintf(stdout,"%s%s\ntest_boostLon = %7.2lf\n"
         , nossCall==noss ? "noss" : "noss2"
         , getenv("NOSS_INPLACE") ? "inplace" : ""
         , nossCall(getenv("NOSS_INPLACE") ? modLon : lon
                   ,modLon
                   ,argc
                   )
         );

  for (iArg=0; iArg<argc; ++iArg) {
    fprintf(stdout,"%7.2lf %7.2lf %8.2lf\n", lon[iArg], modLon[iArg], modLon[iArg]-lon[iArg]);
  }

  fprintf(stdout,"\n");

  free(lon);
  return 0;
}


#ifdef __DO_MAIN__
/***********************************************************************
 * MAIN
 */
int
main(int argc, char** argv) {
  return nossTest(argc,argv,getenv("USE_NOSS2") ? noss2 : noss);
}
#endif /* __DO_MAIN__ */
