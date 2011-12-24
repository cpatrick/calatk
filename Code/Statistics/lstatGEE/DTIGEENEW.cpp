# include <cstdio>
# include <cstdlib>
# include <cmath>
# include "pointdef.h"
//# include "math.h"
# include <iostream>
# include <string>
# include <fstream>
//#include <windows.h>
# include "GEEutil_MN.h"
# include "pxanalyse_MN.h"
# include "surallfileM10Y06_MN_JP.h"
# include "geometry.h"

using namespace std;

unsigned int congrval, tausval;

int Ncov, TotalImg, NopointSur,  NOtraG,  Xdimen, Ydimen, Zdimen,  Nsample, NcovVar, DimSPD, NOmaxTIMEs;
int htNoM,  htNoR;
int DimYY,  NcovDim, DimXX;

char *outputDir;

// Ncov= Number of covariates*Number of Y dimention
// TotalImg= Total number of images
// NopointSur= Total number of points on the surface and images
// NOtraG= Total number triangles in the mesh
// Xdimen= dimension of X direction
// Ydimen= dimension of Y direction
// Zdimen= dimension of Z direction
// NOmaxTIMEs=the Maximum number of times points

// htNoM= number of families
// htNoP= number of parameters in Beta
// htNoR= number of constraints

int main(int argc, char *argv[])
{
  int    i, j, k, i1, j1, k1, ii, jj, kk, dimX, dimY, dimZ, dii, djj, dkk, dmm;
  int    num_subjects, num_cov, noRow, num_const;
  int    seed, SEEDY[12], flag;
  int    Waldbeta,   pointINDX;
  int    noiter;
  string tempo;
  FILE * dat2, *dat3;

  printf("Please enter the dimension of response NOW\n");
  scanf("%d", &DimSPD);
  //DimSPD=6;

  seed = 17;
  setSEED(SEEDY, seed);
  init1(SEEDY);

  printf("Please enter No of Subjects NOW\n");
  scanf("%d", &num_subjects);
  //num_subjects=15;
  printf("No of Subjects is: %d\n", num_subjects);

  printf("Please enter the maximum number of time points for all subjects NOW\n");
  scanf("%d", &NOmaxTIMEs);
  printf("The maximum number of time points is: %d\n", NOmaxTIMEs);

  printf("Please enter No of Covariates NOW\n");
  scanf("%d", &num_cov);
  //num_cov=3;
  printf("No of Covariates is: %d\n", num_cov);

  printf("Please enter dimesions (dimx, dimy, dimz) \n");
  scanf("%d %d %d", &dimX, &dimY, &dimZ);
  //dimX=128;
  //dimY=104;
  //dimZ=80;

  printf(" dimesions (dimx, dimy, dimz) is: %d, %d, %d\n", dimX, dimY, dimZ);
  int voxels = dimX * dimY * dimZ;

  int *    allDimTime = ivector_ht(1, num_subjects); /* reading the number of
                                                       time points insides  HT
                                                       */
  int *    CCID = ivector_ht(0, DimSPD * num_cov);   // define column for
                                                     // nonsingular R_1
  float ** cov_beta = matrix_ht(0, num_cov * DimSPD - 1, 0, num_cov * DimSPD - 1);
  float ** ExactTime = matrix_ht(1, num_subjects, 1, NOmaxTIMEs);
  int *    indxMi = ivector_ht(0, num_subjects);  /* reading the number of time
                                                    points insides  HT */
  int *    indxMi2 = ivector_ht(0, num_subjects); /* reading the number of time
                                                    points insides  HT */
  float ** INRR = matrix_ht(0, DimSPD * num_cov, 0, DimSPD * num_cov);
  float ** INRRnew = matrix_ht(0, DimSPD * num_cov, 0, DimSPD * num_cov);
  double **INRRnewD = dmatrix_ht(1, DimSPD * num_cov, 1, DimSPD * num_cov);
  int *    Irank = ivector_ht(0, DimSPD * num_cov);
  float ** LCOVL = matrix_ht(0, DimSPD * num_cov - 1, 0, DimSPD * num_cov - 1); //
                                                                                // linear
                                                                                // constraints
                                                                                // covariance
                                                                                // matrix_ht
  float *  LLBB = vector_ht(0, DimSPD * num_cov - 1);                           //
                                                                                // linear
                                                                                // constraints
  int *    imageMask = ivector_ht(1, dimX * dimY * dimZ);
  float ** midRR = matrix_ht(0, DimSPD * num_cov, 0, DimSPD * num_cov);
  float ** RR = matrix_ht(0, DimSPD * num_cov - 1, 0, DimSPD * num_cov - 1); //
                                                                             // linear
                                                                             // constraints:
                                                                             // Wald
                                                                             // test
  float *  rr00 = vector_ht(0, DimSPD * num_cov);
  float *  simulationBias = vector_ht(0, DimSPD * num_cov - 1);
  float *  simulationRMS = vector_ht(0, DimSPD * num_cov - 1);
  float *  simulationSD = vector_ht(0, DimSPD * num_cov - 1);
  float *  tempLB = vector_ht(0, DimSPD * num_cov);
  double **TXX = dmatrix_ht(1, DimSPD * num_cov, 1, DimSPD * num_cov);

  printf("\n Please enter the file name for the number of time points and all time points\n");
  if ( ( dat3 = fopen("filetime.txt", "r") ) == NULL )
    {
    printf("Cannot open the save file\n");
    exit(1);
    }

  TotalImg = 0;
  for ( dii = 1; dii <= num_subjects; dii++ )
    {
    fscanf(dat3, "%d", &allDimTime[dii]);
    TotalImg += allDimTime[dii];
    printf("%d ", allDimTime[dii]);
    }
  for ( dii = 1; dii <= num_subjects; dii++ )
    {
    for ( djj = 1; djj <= allDimTime[dii]; djj++ )
      {
      fscanf(dat3, "%f", &ExactTime[dii][djj]);
      printf("%f ", ExactTime[dii][djj]);
      }
    }
  printf("\n");
  fclose(dat3);

  indxMi[0] = 0;
  indxMi2[0] = 0;
  indxMi[1] = allDimTime[1];
  indxMi2[1] = (int)( allDimTime[1] * ( allDimTime[1] - 1 ) * 0.5 );
  for ( dii = 2; dii <= num_subjects; dii++ )
    {
    indxMi[dii] = allDimTime[dii];
    indxMi2[dii] = (int)( allDimTime[dii] * ( allDimTime[dii] - 1 ) * 0.5 );
    indxMi[dii] += indxMi[dii - 1];
    indxMi2[dii] += indxMi2[dii - 1];
    printf("%d %d %d\n", dii, indxMi[dii], indxMi2[dii]);
    }

  printf("%d %d \n", TotalImg, num_cov);

  float **x0x = matrix_ht(0, TotalImg - 1, 0, num_cov - 1);

  // enter covariates here
  printf("\n Please enter the name of design matrix_ht\n");
  if ( ( dat2 = fopen("filevector_hts.txt", "r") ) == NULL )
    {
    printf("Cannot open the save file\n");
    exit(1);
    }

  for ( dii = 0; dii < TotalImg; dii++ )
    {
    for ( djj = 0; djj < num_cov; djj++ )
      {
      fscanf(dat2, "%f", &x0x[dii][djj]);
      }
    }
  fclose(dat2);

  float ***mask_tmp = fmatrix3_ht(0, dimZ - 1, 0, dimX - 1, 0, dimY - 1);
  printf("\n Please enter the name of mask file\n");
  cin >> tempo;
  if ( ( dat2 = fopen(tempo.c_str(), "r") ) == NULL )
    {
    printf("Cannot open the save file\n");
    exit(1);
    }
  fclose(dat2);
  readAnalyzeBody_char( (char *)tempo.c_str(), dimZ, dimX, dimY, mask_tmp );
  NopointSur = 0;
  int index = 1;
  for ( k = 0; k < dimZ; k++ )
    {
    for ( i = 0; i < dimX; i++ )
      {
      for ( j = 0; j < dimY; j++ )
        {
        if ( mask_tmp[k][i][j] > 0.1 )
          {
          imageMask[index] = 1;
          NopointSur++;
          }
        else
          {
          imageMask[index] = 0;
          }

        index++;
        }
      }
    }
  printf("total points %d \n", NopointSur);
  printf("total voxels %d \n", voxels);

  printf("\n Enter linear constraints: LL*\beta=R0 \n");
  printf("Enter No of Rows\n");
  scanf("%d", &noRow);
  //noRow=2;
  printf("No of Rows is: %d\n", noRow);

  if ( noRow > DimSPD * num_cov - 1 )
    {
    noRow = DimSPD * num_cov - 1;
    }
  for ( ii = 0; ii <= DimSPD * num_cov - 1; ii++ )
    {
    for ( jj = 0; jj <= DimSPD * num_cov - 1; jj++ )
      {
      RR[ii][jj] = 0.0;
      }
    rr00[ii] = 0.0;
    }

  float *  Epsilon = vector_ht(0, num_subjects);
  float ** ImageData = matrix_ht(1, NopointSur, 1, TotalImg * DimSPD);
  float *  Scorevalue = vector_ht(1, NopointSur);
  float *  SmaxEmp = vector_ht(1, Nresample);
  float *  Spvalue = vector_ht(1, NopointSur);
  float *  Srawpvalue = vector_ht(1, NopointSur);
  float *  tempoScorevalue = vector_ht(0, NopointSur);
  float ***tensors = fmatrix3_ht(0, DimSPD * dimZ - 1, 0, dimX - 1, 0, dimY - 1);
  float ***UUmu = fmatrix3_ht(0, NopointSur, 0, num_subjects, 0, noRow + 1);
  float ***UUsgMu = fmatrix3_ht(0, NopointSur, 0, noRow + 1, 0, noRow + 1);
  float *  Waldtest = vector_ht(1,  NopointSur);
  double * Yresponse = dvector_ht(1, TotalImg * DimSPD);

  printf("TotalImg= %d, dim=[%d %d %d]\n", TotalImg, DimSPD * dimZ, dimX, dimY);

  printf("Enter Linear Matrix LL with NoRow and Ncov; Constraint Vector:\n");
  printf("Enter the columns that makes R_1 nonsingular \n");

  for ( dii = 0; dii < DimSPD * num_cov; dii++ )
    {
    for ( djj = 0; djj < DimSPD * num_cov; djj++ )
      {
      INRR[dii][djj] = 0.0;
      midRR[dii][djj] = 0.0;
      }
    }

  if ( ( dat2 = fopen("linearMAGE.txt", "r") ) == NULL )
    {
    printf("Cannot open the save file\n");
    exit(1);
    }
  printf("%d, %d", noRow, DimSPD * num_cov);
  for ( dii = 0; dii < noRow; dii++ )
    {
    for ( djj = 0; djj < DimSPD * num_cov; djj++ )
      {
      fscanf(dat2, "%f", &RR[dii][djj]);
      printf("%f", RR[dii][djj]);
      }
    }
  for ( dii = 0; dii < noRow; dii++ )
    {
    fscanf(dat2, "%f", &rr00[dii]);
    printf("%f \n", rr00[dii]);
    }

  for ( dii = 0; dii < noRow; dii++ )
    {
    fscanf(dat2, "%d", &CCID[dii]);
    //  printf("\n%d",CCID[dii]);
    CCID[dii] = CCID[dii] - 1;
    midRR[0][CCID[dii]] = 1;
    }
  fclose(dat2);

  for ( dii = 0, djj = noRow; dii < DimSPD * num_cov; dii++ )
    {
    if ( midRR[0][dii] < 1.0 )
      {
      CCID[djj] = dii;
      djj++;
      }
    }
  for ( dii = 0; dii < noRow; dii++ )
    {
    for ( djj = 0; djj < noRow; djj++ )
      {
      INRR[dii][djj] = RR[dii][CCID[djj]];
      }
    }
  for ( dii = 0; dii < noRow; dii++ )
    {
    for ( djj = 0; djj < noRow; djj++ )
      {
      printf("%f", INRR[dii][djj]);
      }
    printf("\n");
    }

  ivYS(INRR, 0, noRow - 1);
  for ( dii = 0; dii < noRow; dii++ )
    {
    for ( djj = 0; djj < DimSPD * num_cov; djj++ )
      {
      midRR[dii][djj] = 0.0;
      for ( dkk = 0; dkk < noRow; dkk++ )
        {
        midRR[dii][djj] -= INRR[dii][dkk] * RR[dkk][djj];
        }
      }
    }
  for ( dii = 0; dii < noRow; dii++ )
    {
    for ( djj = 0; djj < noRow; djj++ )
      {
      midRR[dii][CCID[djj]] = INRR[dii][djj];
      }
    }
  for ( dii = noRow; dii < DimSPD * num_cov; dii++ )
    {
    midRR[dii][CCID[dii]] = 1.0;
    }

  //this generates midRR=[R_1^{-1}, -R_1^{-1}R_2]
  for ( dii = 0; dii < DimSPD * num_cov; dii++ )
    {
    for ( djj = 0; djj < DimSPD * num_cov; djj++ )
      {
      INRR[dii][djj] = midRR[dii][djj];
      }
    }

  for ( dii = 0; dii < DimSPD * num_cov; dii++ )
    {
    for ( djj = 0; djj < DimSPD * num_cov; djj++ )
      {
      INRRnew[dii][djj] = 0.0;
      }
    }
  for ( dii = 0; dii < noRow; dii++ )
    {
    for ( djj = 0; djj < DimSPD * num_cov; djj++ )
      {
      INRRnew[dii][djj] = RR[dii][djj];
      }
    }
  for ( dii = noRow; dii < DimSPD * num_cov; dii++ )
    {
    INRRnew[dii][CCID[dii]] = 1.0;
    }
  for ( dii = 1; dii <= DimSPD * num_cov; dii++ )
    {
    for ( djj = 1; djj <= DimSPD * num_cov; djj++ )
      {
      INRRnewD[dii][djj] = INRRnew[dii - 1][djj - 1];
      }
    }

  float ht = DnewIVrank(INRRnewD, Irank, DimSPD * num_cov);
  for ( dii = 1; dii <= DimSPD * num_cov; dii++ )
    {
    for ( djj = 1; djj <= DimSPD * num_cov; djj++ )
      {
      printf("%lf ", INRRnewD[dii][djj]);
      }
    printf("\n");
    }
  // scanf("%d", dii);

  //get all the tensors into memory
  printf("\n Please enter the txt file containing image names\n");
  char *fileName = new char[200];
  scanf("%s", fileName);

  anaimagedata headerSrc;
  int          swapped;

  // Marc start

  FILE *fp = fopen(fileName, "rt");
  dmm = 0;
  for ( i = 0; i < TotalImg; i++ )
    {
    char *eachTensorFile = new char[100];
    fscanf(fp, "%s", eachTensorFile);
    char *name_input_hdr = new char[100];
    char *name_input_img = new char[100];
    sprintf(name_input_hdr, "%s.hdr", eachTensorFile);
    sprintf(name_input_img, "%s.img", eachTensorFile);
    if ( i == 0 )
      {
      swapped = readanahdr(name_input_hdr, &headerSrc);
      }
    printf("reading %s\n", name_input_img);
    readAnalyzeDTI(name_input_img, dimZ, dimX, dimY, DimSPD, tensors);
    index = 1;
    for ( k = 0; k < dimZ; k++ )
      {
      for ( dii = 0; dii < dimX; dii++ )
        {
        for ( j = 0; j < dimY; j++ )
          {
          if ( mask_tmp[k][dii][j] > 0.1 )
            {
            for ( djj = 0; djj < DimSPD; djj++ )
              {
              ImageData[index][i * DimSPD + djj + 1] = tensors[k * DimSPD + djj][dii][j];
              }
            index++;
            }
          } /* k */
        }
      }
    delete[] name_input_hdr; name_input_hdr = NULL;
    delete[] name_input_img; name_input_img = NULL;
    delete[] eachTensorFile; eachTensorFile = NULL;
    } /* i  */
  fclose(fp);
  free_fmatrix3_ht(tensors, 0, DimSPD * dimZ - 1,  0, dimX - 1, 0, dimY - 1);

  // Marc End

  printf("after inputing the images....................\n");

  int p = num_cov; //4 PARAMETERS?????
  int q = 1;       //variance

  Ncov = DimSPD * num_cov;
  NcovVar = DimSPD * q;
  Nsample = num_subjects;

  int totalobs, obsforonemax, mimax;

  double **designXX = dmatrix_ht(1, TotalImg * DimSPD, 1, Ncov);
  double * betaGEE = dvector_ht(1, DimSPD * num_cov + DimSPD * 2);

  printf("after input the images 1\n");

// creat design matrix_ht here

  for ( dii = 1; dii <= TotalImg * DimSPD; dii++ )
    {
    for ( dkk = 1; dkk <= Ncov; dkk++ )
      {
      designXX[dii][dkk] = 0.0;
      }
    }

  for ( dii = 1; dii <= TotalImg; dii++ )
    {
    for ( dkk = 1; dkk <= DimSPD; dkk++ )
      {
      for ( djj = 1; djj <= num_cov; djj++ )
        {
        designXX[( dii - 1 ) * DimSPD + dkk][( dkk - 1 ) * num_cov + djj] = x0x[dii - 1][djj - 1] * 1.0;
        }
      }
    }

  for ( dii = 1; dii <= Ncov; dii++ )
    {
    for ( djj = 1; djj <= Ncov; djj++ )
      {
      TXX[dii][djj] = 0.0;
      }
    }

  for ( dii = 1; dii <= TotalImg * DimSPD; dii++ )
    {
    for ( djj = 1; djj <= Ncov; djj++ )
      {
      for ( dkk = 1; dkk <= Ncov; dkk++ )
        {
        TXX[djj][dkk] += designXX[dii][djj] * designXX[dii][dkk];
        }
      }
    }

  invv(TXX, Ncov);

  printf("after input the images 2\n");

  totalobs = 0;
  mimax = 0;

  for ( dii = 1; dii <= num_subjects; dii++ )
    {
    if ( mimax <= allDimTime[dii] )
      {
      mimax = allDimTime[dii];
      }
    }
  totalobs = TotalImg * DimSPD;
  printf("%d\n", totalobs);
  obsforonemax = mimax * DimSPD;

  double **BrainTheta = dmatrix_ht(1, NopointSur, 1, Ncov + 2 * DimSPD);
  float ***corrMatrix = fmatrix3_ht(1, Nsample, 1, obsforonemax, 1, obsforonemax);
  double * residualli = dvector_ht(1, totalobs);
  double **sum1 = dmatrix_ht(1, Ncov, 1, Ncov);
  double **varGEE = dmatrix_ht(1, Ncov, 1, Ncov);
  float ***varMatrix = fmatrix3_ht(1, Nsample, 1, obsforonemax, 1, obsforonemax);

  printf("after input the images 3\n");

  string summary = "SummaryResFtest.txt";
  FILE * fp1 = fopen(summary.c_str(), "w");  //*.img

  for ( djj = 1; djj <= NopointSur; djj++ )
    {
    Spvalue[djj] = 0.0;
    Srawpvalue[djj] = 0.0;
    Waldtest[djj] = 0.0;
    }

  pointINDX = 1;
  int    counttemp1 = 0;
  double tempValue;

  char  Variancematrix_ht[200] = "Covariance.img";
  FILE *datVAR = fopen(Variancematrix_ht, "wb+");

  index = 1;
  for ( kk = 0; kk < dimZ; kk++ )
    {
    printf("  %d  \n", kk);
    for ( ii = 0; ii < dimX; ii++ )
      {
      for ( jj = 0; jj < dimY; jj++ )
        {
        if ( imageMask[index] >= 1 )
          {
//					 printf("1. %d %d %d %d\n", kk, ii, jj, index);
          flag = 1;
          //attension: You need to give tensors to Y
          tempValue = 0.0;
          for ( i = 0; i < TotalImg; i++ )
            {
            for ( dii = 1; dii <= DimSPD; dii++ )
              {
              Yresponse[i * DimSPD + dii] = ImageData[pointINDX][i * DimSPD + dii];
//              fprintf(dat1, "%lf\n", Yresponse[i*DimSPD+dii]);
              // Yresponse[i*DimSPD+dii]=gasdev();
              tempValue += Yresponse[i * DimSPD + dii] * Yresponse[i * DimSPD + dii];
              }
            }
          //				    scanf("%d", &dii);
//					printf("2. %lf\n", tempValue);
          if ( fabs(tempValue) <= 0.0 )
            {
            flag = 0;
            }
          if ( flag >= 1 )
            {
//                for(i=0; i<TotalImg; i++)
//                 for(dii=1; dii<=DimSPD; dii++)
//              fprintf(fp1, "%f \n", Yresponse[i*DimSPD+dii]);

            GEEestimatesHongtu(betaGEE,
                               residualli,
                               designXX,
                               TXX,
                               Yresponse,
                               Nsample,
                               Ncov,
                               TotalImg,
                               DimSPD,
                               allDimTime,
                               p,
                               q,
                               varMatrix,
                               ExactTime,
                               indxMi,
                               indxMi2);

//                   scanf("%d", &dii);
//					printf("4. \n");
            CalVarGEEHongtu(designXX, residualli, DimSPD, allDimTime, p, betaGEE,  varMatrix,  varGEE, Nsample, Ncov);
            Waldtest[pointINDX] = WaldHongtu(noRow, betaGEE, varGEE, RR, rr00, Ncov);
            tempValue = ii + 1.0;
            fwrite(&tempValue, sizeof( float ), 1, datVAR);
            tempValue = jj + 1.0;
            fwrite(&tempValue, sizeof( float ), 1, datVAR);
            tempValue = kk + 1.0;
            fwrite(&tempValue, sizeof( float ), 1, datVAR);
            for ( dkk = 1; dkk <= Ncov; dkk++ )
              {
              for ( dii = dkk; dii <= Ncov; dii++ )
                {
                tempValue = (float)varGEE[dkk][dii];
                fwrite(&tempValue, sizeof( float ), 1, datVAR);
                }
              }
//					  scanf("%d", &dii);
            for ( dkk = 1; dkk <= Ncov + 2 * DimSPD; dkk++ )
              {
              BrainTheta[pointINDX][dkk] = betaGEE[dkk];
//						  fprintf(fp1, "beta=%f \n", betaGEE[dkk]);
              }
//					  for(dkk=1; dkk<=Ncov; dkk++)
//						  fprintf(fp1, "var=%lf \n", sqrt(varGEE[dkk][dkk]));

            Scorevalue[pointINDX] = SCOREbetaGEEHongtu(noRow,
                                                       pointINDX,
                                                       Yresponse,
                                                       designXX,
                                                       BrainTheta[pointINDX],
                                                       INRRnewD,
                                                       RR,
                                                       rr00,
                                                       Scorevalue,
                                                       UUmu[pointINDX - 1],
                                                       UUsgMu[pointINDX - 1],
                                                       allDimTime,
                                                       DimSPD,
                                                       varMatrix,
                                                       Nsample,
                                                       Ncov);
            //	  printf("%f %f\n", Waldtest[pointINDX], Scorevalue[pointINDX]);
//					  fprintf(fp1, "%f  %f\n", Waldtest[pointINDX],
// Scorevalue[pointINDX]);
//            scanf("%d", &dii);
//					  fclose(fp1);
            }
          else
            {
            Scorevalue[pointINDX] = 0.0;
            Waldtest[pointINDX] = 0.0;
            for ( i = 0; i < num_subjects; i++ )
              {
              for ( j = 0; j <= noRow; j++ )
                {
                UUmu[pointINDX - 1][i][j] = 0.0; //Feb21
                }
              }
            for ( i = 0; i <= noRow; i++ )
              {
              for ( j = 0; j <= noRow; j++ )
                {
                UUsgMu[pointINDX - 1][i][j] = 0.0;
                if ( i == j )
                  {
                  UUsgMu[pointINDX - 1][i][i] = 1.0; //Feb21
                  }
                }
              }
            } // else
          if ( Scorevalue[pointINDX] == 0.0 )
            {
            counttemp1 += 1;
            }
          pointINDX++;
          }
        index++;
        }
      }
    }
  printf("\n pointINDX=%d, NopointSur=%d  index=%d, counttemp1=%d", pointINDX, NopointSur, index, counttemp1);

  fclose(datVAR);

  noiter = 1;
  do
    {
    SmaxEmp[noiter] = 0.0;
    for ( djj = 0; djj < num_subjects; djj++ )
      {
      if ( s_xuni() > 0.5 )
        {
        Epsilon[djj] = 1.0;
        }
      else
        {
        Epsilon[djj] = -1.0;
        }
      }
    for ( dii = 0; dii < NopointSur; dii++ )
      {
      tempoScorevalue[dii] = resampleMUYS(noRow, num_cov * DimSPD, num_subjects, 0,  Epsilon, UUmu[dii], UUsgMu[dii]);
      if ( tempoScorevalue[dii] >= Scorevalue[dii + 1] )
        {
        Srawpvalue[dii + 1] = Srawpvalue[dii + 1] + 1.0;
        }
      if ( tempoScorevalue[dii] > SmaxEmp[noiter] )
        {
        SmaxEmp[noiter] = tempoScorevalue[dii];
        }
      }
    printf("%d\n", noiter);
    noiter++;
    }
  while ( noiter <= Nresample );

  printf("\nNopointSur=%d", NopointSur);

  for ( dii = 1; dii <= NopointSur; dii++ )
    {
    Srawpvalue[dii] = Srawpvalue[dii] / ( Nresample * 1.0 );
    if ( Srawpvalue[dii] < 3.0 )
      {
      printf("dii=%d  %f\n", dii, Srawpvalue[dii]);
      }
    if ( Srawpvalue[dii] > 0 )
      {
      Srawpvalue[dii] = -log10(Srawpvalue[dii] * 1.0);
      }
    else
      {
      Srawpvalue[dii] = 3.0;
      }
    }

  noiter = Nresample;
  sort2YS(noiter, SmaxEmp);

  fprintf(fp1, "Resampling distributions\n");
  for ( dii = 1; dii <= Nresample; dii++ )
    {
    fprintf(fp1, "%14lf\n", SmaxEmp[dii]);
    }
  fclose(fp1);

  int counttemp = 0;

  for ( dii = 1; dii <= NopointSur; dii++ )
    {
    if ( Srawpvalue[dii] == 3.0 )
      {
      counttemp += 1;
      }
    }
  printf("\n counttemp=%d", counttemp);
  CalPvalueYS(NopointSur, SmaxEmp,  Scorevalue, Spvalue);
  float temphongtu = 0.0;
  char diagnostic[200] = "rawPvalue.img";
  FILE *datSResidual1 = fopen(diagnostic, "wb+");
  printf("\n voxels=%d, NopointSur=%d", voxels, NopointSur);

  for ( dkk = 1, djj = 1; dkk <= voxels; dkk++ )
    {
    if ( imageMask[dkk] >= 1 )
      {
      fwrite(&Srawpvalue[djj], sizeof( float ), 1, datSResidual1);
      djj += 1;
      }
    else
      {
      fwrite(&temphongtu, sizeof( float ), 1, datSResidual1);
      }
    }
  fclose(datSResidual1);

  char diagnostic1[200] = "RSPvalue.img";
  FILE *datSResidual2 = fopen(diagnostic1, "wb+");

  for ( dkk = 1, djj = 1; dkk <= voxels; dkk++ )
    {
    if ( imageMask[dkk] >= 1 )
      {
      fwrite(&Spvalue[djj], sizeof( float ), 1, datSResidual2);
      djj += 1;
      }
    else
      {
      fwrite(&temphongtu, sizeof( float ), 1, datSResidual2);
      }
    }
  fclose(datSResidual2);

  char diagnostic2[200] = "SCOREvalue.img";
  FILE *datSResidual3 = fopen(diagnostic2, "wb+");

  //only output the signficant areas
  for ( dkk = 1, djj = 1; dkk <= voxels; dkk++ )
    {
    if ( imageMask[dkk] >= 1 )
      {
      fwrite(&Scorevalue[djj], sizeof( float ), 1, datSResidual3);
      djj += 1;
      }
    else
      {
      fwrite(&temphongtu, sizeof( float ), 1, datSResidual3);
      }
    }
  fclose(datSResidual3);

  char diagnostic3[200] = "WALDvalue.img";
  FILE *datSResidual4 = fopen(diagnostic3, "wb+");

  //only output the signficant areas
  for ( dkk = 1, djj = 1; dkk <= voxels; dkk++ )
    {
    if ( imageMask[dkk] >= 1 )
      {
      fwrite(&Waldtest[djj], sizeof( float ), 1, datSResidual4);
      djj += 1;
      }
    else
      {
      fwrite(&temphongtu, sizeof( float ), 1, datSResidual4);
      }
    }
  fclose(datSResidual4);

  char tempo1[200];
  string *BetaName;

  BetaName = new string[Ncov + 2 * DimSPD + 1];
  for ( dii = 1; dii <= Ncov + 2 * DimSPD; dii++ )
    {
    djj = sprintf(tempo1, "BETA%03d.img", dii);
    BetaName[dii] = tempo1;
    FILE *datSResidual1 = fopen(BetaName[dii].c_str(), "wb+");
    for ( dkk = 1, djj = 1; dkk <= voxels; dkk++ )
      {
      if ( imageMask[dkk] >= 1 )
        {
        temphongtu = (float)BrainTheta[djj][dii];
        djj += 1;
        }
      else
        {
        temphongtu = 0.0;
        }
      fwrite(&temphongtu, sizeof( float ), 1, datSResidual1);
      }
    fclose(datSResidual1);
    } /* dii */

  //yasheng

  free_ivector_ht(allDimTime, 1, num_subjects);   /* reading the number of time
                                                    points insides  HT */
  free_dvector_ht(betaGEE, 1, DimSPD * num_cov + DimSPD * 2);
  free_dmatrix_ht(BrainTheta, 1, NopointSur, 1, Ncov + 2 * DimSPD);
  free_ivector_ht(CCID, 0, DimSPD * num_cov); //define column for nonsingular
                                              // R_1
  free_fmatrix3_ht(corrMatrix, 1, Nsample, 1, obsforonemax, 1, obsforonemax);
  free_matrix_ht(cov_beta, 0, num_cov * DimSPD - 1, 0, num_cov * DimSPD - 1);
  free_dmatrix_ht(designXX, 1, TotalImg * DimSPD, 1, Ncov);
  free_vector_ht(Epsilon, 0, num_subjects);
  free_matrix_ht(ExactTime, 1, num_subjects, 1, NOmaxTIMEs);
  free_matrix_ht(ImageData, 1, NopointSur, 1,  TotalImg * DimSPD);
  free_ivector_ht(imageMask, 1, dimX * dimY * dimZ);
  free_ivector_ht(indxMi, 0, num_subjects);  /* reading the number of time
                                               points insides  HT */
  free_ivector_ht(indxMi2, 0, num_subjects); /* reading the number of time
                                               points insides  HT */
  free_matrix_ht(INRR, 0, DimSPD * num_cov, 0, DimSPD * num_cov);
  free_matrix_ht(INRRnew, 0, DimSPD * num_cov, 0, DimSPD * num_cov);
  free_dmatrix_ht(INRRnewD, 1, DimSPD * num_cov, 1, DimSPD * num_cov);
  free_ivector_ht(Irank, 0, DimSPD * num_cov);
  free_matrix_ht(LCOVL, 0, DimSPD * num_cov - 1, 0, DimSPD * num_cov - 1);   //
                                                                             // linear
                                                                             // constraints
                                                                             // covariance
                                                                             // matrix_ht

  free_vector_ht(LLBB, 0, DimSPD * num_cov - 1);   // linear constraints
  free_fmatrix3_ht(mask_tmp, 0, dimZ - 1, 0, dimX - 1, 0, dimY - 1);
  free_matrix_ht(midRR, 0, DimSPD * num_cov, 0, DimSPD * num_cov);
  free_dvector_ht(residualli, 1, totalobs);
  free_matrix_ht(RR, 0, DimSPD * num_cov - 1, 0, DimSPD * num_cov - 1); //
                                                                        // linear
                                                                        // constraints:
                                                                        // Wald
                                                                        // test
  free_vector_ht(rr00, 0, DimSPD * num_cov);
  free_vector_ht(Scorevalue, 1, NopointSur);
  free_vector_ht(simulationBias, 0, DimSPD * num_cov - 1);
  free_vector_ht(simulationRMS, 0, DimSPD * num_cov - 1);
  free_vector_ht(simulationSD, 0, DimSPD * num_cov - 1);
  free_vector_ht(SmaxEmp, 1, Nresample);
  free_vector_ht(Spvalue, 1, NopointSur);
  free_vector_ht(Srawpvalue, 1, NopointSur);
  free_dmatrix_ht(sum1, 1, Ncov, 1, Ncov);
  free_vector_ht(tempLB, 0, DimSPD * num_cov);
  free_vector_ht(tempoScorevalue, 0, NopointSur);
  free_dmatrix_ht(TXX, 1, Ncov, 1, Ncov);

  free_fmatrix3_ht(UUmu, 0, NopointSur, 0, num_subjects, 0, noRow + 1);
  free_fmatrix3_ht(UUsgMu, 0, NopointSur, 0, noRow + 1, 0, noRow + 1);
  free_dmatrix_ht(varGEE, 1, Ncov, 1, Ncov);
  free_fmatrix3_ht(varMatrix, 1, Nsample, 1, obsforonemax, 1, obsforonemax);
  free_vector_ht(Waldtest, 1, NopointSur);
  free_matrix_ht(x0x, 0, TotalImg - 1, 0, num_cov - 1);
  free_dvector_ht(Yresponse, 1, TotalImg * DimSPD);

  return 1;
} /* end */
