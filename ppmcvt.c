#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pbm.h"
#include <unistd.h>


PPMImage * new_ppmimage( unsigned int width, unsigned int height, unsigned int max)
{
  PPMImage *ppmobj = (PPMImage *)malloc(sizeof(PPMImage));
  ppmobj->width = width;
  ppmobj->height = height;
  ppmobj->max = max;

  for(int i=0; i<3; i++)
  {
  ppmobj->pixmap[i] = (unsigned int **)malloc(height * sizeof(unsigned int *));
  for(int j=0; j<height; j++)
  {
    ppmobj->pixmap[i][j] = (unsigned int *)malloc(width * sizeof(unsigned int *));
  }
  }
  return ppmobj;
}

PGMImage * new_pgmimage( unsigned int width, unsigned int height, unsigned int max)
{
  PGMImage *pgmobj = (PGMImage *)malloc(sizeof(PGMImage));
  pgmobj->width = width;
  pgmobj->height = height;
  pgmobj->max = max;

  pgmobj->pixmap = (unsigned int **)malloc(height * sizeof(unsigned int *));
  for(int j=0; j<height; j++)
  {
    pgmobj->pixmap[j] = (unsigned int *)malloc(width * sizeof(unsigned int *));
  }
  return pgmobj;
}

PBMImage * new_pbmimage( unsigned int width, unsigned int height)
{
  PBMImage *pbmobj = (PBMImage *)malloc(sizeof(PBMImage));
  pbmobj->width = width;
  pbmobj->height = height;

  pbmobj->pixmap = (unsigned int **)malloc(height * sizeof(unsigned int *));
  for(int j=0; j<height; j++)
  {
    pbmobj->pixmap[j] = (unsigned int *)malloc(width * sizeof(unsigned int *));
  }
  return pbmobj;
}

void del_ppmimage(PPMImage * delppm)
{
  for(int k=0; k<3; k++)
  {
    for(int h=0; h< delppm->height; h++)
    {
      free(delppm->pixmap[k][h]);
    }
    free(delppm->pixmap[k]);
  }
  free(delppm);
}

void del_pbmimage(PBMImage * delpbm)
{
    for(int h=0; h< delpbm->height; h++)
    {
      free(delpbm->pixmap[h]);
    }
    free(delpbm->pixmap);
  free(delpbm);
}

void del_pgmimage(PGMImage * delpgm)
{
    for(int h=0; h< delpgm->height; h++)
    {
      free(delpgm->pixmap[h]);
    }
    free(delpgm->pixmap);
  free(delpgm);
}

//////////////////////////////////////////////////////////////////////////////////////////////

//Function to convert image file to a sepia file.
void apply_sepia(const char* inputfname, const char* outputfname)
{
  PPMImage * ppmimg;
  ppmimg = read_ppmfile(inputfname);
  for(int h = 0; h < ppmimg->height; h++)
  {
    for(int w = 0; w < ppmimg->width; w++)
    {
      int newR, newG, newB;
      unsigned int oldR = ppmimg->pixmap[0][h][w];
      unsigned int oldG = ppmimg->pixmap[1][h][w];
      unsigned int oldB = ppmimg->pixmap[2][h][w];

      newR = 0.393*(oldR) + 0.769*(oldG) + 0.189*(oldB);
      newG = 0.349*(oldR) + 0.686*(oldG) + 0.168*(oldB);
      newB = 0.272*(oldR) + 0.534*(oldG) + 0.131*(oldB);

      ppmimg->pixmap[0][h][w] = newR < ppmimg->max? newR:ppmimg->max;
      ppmimg->pixmap[1][h][w] = newG < ppmimg->max? newG:ppmimg->max;
      ppmimg->pixmap[2][h][w] = newB < ppmimg->max? newB:ppmimg->max;
      // ppmimg->pixmap[2][h][w] = 0.272*(** oldR) + 0.534*(** oldG) + 0.131*(** oldB);
      // unsigned int** newB = 0.272*ppmimg->pixmap[0][h][w] + 0.534*ppmimg->pixmap[1][h][w] + 0.131*ppmimg->pixmap[2][h][w];
    }
  }

  write_ppmfile(ppmimg, outputfname);
  del_ppmimage(ppmimg);
}


//Function to convert the ppm file to pbm file
void ppmtopbm(const char* inputfname, const char* outputfname)
{
  PPMImage * ppmimg;
  ppmimg = read_ppmfile(inputfname);
  PBMImage * pbmimg = new_pbmimage(ppmimg->width, ppmimg->height);

  for(int h = 0; h < ppmimg->height; h++)
  {
    for(int w = 0; w < ppmimg->width; w++)
    {
      unsigned int pR = ppmimg->pixmap[0][h][w];
      unsigned int pG = ppmimg->pixmap[1][h][w];
      unsigned int pB = ppmimg->pixmap[2][h][w];

      unsigned int pixel = (pR + pG + pB)/3.0;

      if(pixel < (ppmimg->max/2.0))
      {
      pbmimg->pixmap[h][w] = 1;
      }
      else
      {
      pbmimg->pixmap[h][w] = 0;
      }
    }
  }

  write_pbmfile(pbmimg, outputfname);

  del_ppmimage(ppmimg);
  del_pbmimage(pbmimg);
}

//Function to convert the ppm file to pgm file
void ppmtopgm(const char* inputfname, const char* outputfname, int pgmmax)
{
  PPMImage * ppmimg;
  ppmimg = read_ppmfile(inputfname);
  PGMImage * pgmimg = new_pgmimage(ppmimg->width, ppmimg->height, ppmimg->max);

  for(int h = 0; h < ppmimg->height; h++)
  {
    for(int w = 0; w < ppmimg->width; w++)
    {
      unsigned int pR = ppmimg->pixmap[0][h][w];
      unsigned int pG = ppmimg->pixmap[1][h][w];
      unsigned int pB = ppmimg->pixmap[2][h][w];

      pgmimg->pixmap[h][w] = ((pR + pG + pB)/ppmimg->max)*pgmmax;
    }
  }

  write_pgmfile(pgmimg, outputfname);

  del_ppmimage(ppmimg);
  del_pgmimage(pgmimg);

}

//Function to isolate a channel.
void isolateChannel(const char* inputfname, const char* outputfname, int channel)
{
  PPMImage * ppmimg;
  ppmimg = read_ppmfile(inputfname);

  for(int h = 0; h < ppmimg->height; h++)
  {
    for(int w = 0; w < ppmimg->width; w++)
    {
      if(channel == 0)
      {
        ppmimg->pixmap[1][h][w] = 0;
        ppmimg->pixmap[2][h][w] = 0;

      }
      else if(channel == 1)
      {
        ppmimg->pixmap[0][h][w] = 0;
        ppmimg->pixmap[2][h][w] = 0;

      }
      else
      {
        ppmimg->pixmap[0][h][w] = 0;
        ppmimg->pixmap[1][h][w] = 0;

      }

      }
    }

  write_ppmfile(ppmimg, outputfname);

  del_ppmimage(ppmimg);
}

//Function to remove a channel.
void removeChannel(const char* inputfname, const char* outputfname, int channel)
{
  PPMImage * ppmimg;
  ppmimg = read_ppmfile(inputfname);

  for(int h = 0; h < ppmimg->height; h++)
  {
    for(int w = 0; w < ppmimg->width; w++)
    {
      if(channel == 0)
      {
        ppmimg->pixmap[0][h][w] = 0;

      }
      else if(channel == 1)
      {
        ppmimg->pixmap[1][h][w] = 0;

      }
      else
      {
        ppmimg->pixmap[2][h][w] = 0;

      }

      }
    }

  write_ppmfile(ppmimg, outputfname);

  del_ppmimage(ppmimg);
}

//Mirroring an image.
void mirrorVertical(const char* inputfname, const char* outputfname)
{
  PPMImage * ppmimg;
  ppmimg = read_ppmfile(inputfname);
  int imgW, imgH, midVal;
  imgW = ppmimg->width, imgH = ppmimg->height;
  midVal = imgW/2;

  PPMImage* new_ppmimg = new_ppmimage(imgW, imgH, ppmimg->max);

  for (int h = 0; h < imgH; ++h)
  {
    for (int w = 0; w < imgW; ++w)
    {
      if(w <= midVal)
      {
        new_ppmimg->pixmap[0][h][imgW - w] = ppmimg->pixmap[0][h][w];
        new_ppmimg->pixmap[1][h][imgW - w] = ppmimg->pixmap[1][h][w];
        new_ppmimg->pixmap[2][h][imgW - w] = ppmimg->pixmap[2][h][w];
      }
      else
      {
        new_ppmimg->pixmap[0][h][imgW - w] = ppmimg->pixmap[0][h][w];
        new_ppmimg->pixmap[1][h][imgW - w] = ppmimg->pixmap[1][h][w];
        new_ppmimg->pixmap[2][h][imgW - w] = ppmimg->pixmap[2][h][w];

      }
    }
  }
  write_ppmfile(new_ppmimg, outputfname);

  del_ppmimage(ppmimg);
  del_ppmimage(new_ppmimg);

}

//Thumbnail an image.
void thumbnail(const char* inputfname, const char* outputfname, int scale)
{
  PPMImage * ppmimg;
  ppmimg = read_ppmfile(inputfname);
  int imgW, imgH, midVal;
  imgW = ppmimg->width/scale, imgH = ppmimg->height/scale;

  PPMImage* new_ppmimg = new_ppmimage(imgW, imgH, ppmimg->max);

  for (int h = 0; h < imgH; ++h)
  {
    for (int w = 0; w < imgW; ++w)
    {
        new_ppmimg->pixmap[0][h][w] = ppmimg->pixmap[0][h*scale][w*scale];
        new_ppmimg->pixmap[1][h][w] = ppmimg->pixmap[1][h*scale][w*scale];
        new_ppmimg->pixmap[2][h][w] = ppmimg->pixmap[2][h*scale][w*scale];
    }
  }
  write_ppmfile(new_ppmimg, outputfname);

  del_ppmimage(ppmimg);
  del_ppmimage(new_ppmimg);

}

//Tiling a Thumbnail an image.
void tileThumbnail(const char* inputfname, const char* outputfname, int scale)
{
  PPMImage * ppmimg;
  ppmimg = read_ppmfile(inputfname);
  int imgW, imgH, midVal;
  PPMImage* thumbppmimg = new_ppmimage(ppmimg->width, ppmimg->height, ppmimg->max);

  imgW = ppmimg->width/scale, imgH = ppmimg->height/scale;

  PPMImage* new_ppmimg = new_ppmimage(imgW, imgH, ppmimg->max);

  for (int h = 0; h < imgH; ++h)
  {
    for (int w = 0; w < imgW; ++w)
    {
        new_ppmimg->pixmap[0][h][w] = ppmimg->pixmap[0][h*scale][w*scale];
        new_ppmimg->pixmap[1][h][w] = ppmimg->pixmap[1][h*scale][w*scale];
        new_ppmimg->pixmap[2][h][w] = ppmimg->pixmap[2][h*scale][w*scale];
    }
  }

  for(int h_h=0; h_h< scale; h_h++)
  {
    for(int w_w=0; w_w< scale; w_w++)
    {
      for (int h = 0; h < imgH; ++h)
      {
        for (int w = 0; w < imgW; ++w)
        {
            thumbppmimg->pixmap[0][(h_h*imgH) + h][(w_w*imgW) + w] = new_ppmimg->pixmap[0][h][w];
            thumbppmimg->pixmap[1][(h_h*imgH) + h][(w_w*imgW) + w] = new_ppmimg->pixmap[1][h][w];
            thumbppmimg->pixmap[2][(h_h*imgH) + h][(w_w*imgW) + w] = new_ppmimg->pixmap[2][h][w];
        }
      }
    }
  }

  write_ppmfile(thumbppmimg, outputfname);

  del_ppmimage(ppmimg);
  del_ppmimage(new_ppmimg);
  del_ppmimage(thumbppmimg);
}


int main(int argc, char** argv)
{
  extern char *optarg;
	extern int optind;
  // const char* inputfileName = "clock.ppm";
  // const char* outputfileName = "clock_out.pgm";
  int c, err = 0;
  char* option;
  char* outputfName = NULL;
  char* inputfName = NULL;
  char* gname = NULL;
  char* iname = NULL;
  char* rname = NULL;
  char* tname = NULL;
  char* nname = NULL;
  int intoptarg;
  char* remaining;
  char* colorname;
  int oFlag = 0;
  int chCount = 0;


  while ((c = getopt(argc, argv, "bg:i:r:smt:n:o:")) != -1)
    switch (c)
    {
    case 'b':
      // if(argc > 6)
      // {
      //   printf("Error: Multiple transformations specified\n");
      //   exit(-1);
      // }
      option = "PBM";
      inputfName = argv[argc-1];
      // printf("%d\n",optind);
      chCount++;

      break;
    case 'g':
      if(intoptarg >= 65536)
      {
        printf("Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", optarg);
        exit(-1);
      }
      option = "PGM";
      inputfName = argv[argc-1];
      intoptarg = strtol(optarg, &remaining, 10);
      chCount++;

      break;
    case 'i':
      option = "isolate";
      inputfName = argv[argc-1];
      colorname = optarg;
      if(strcmp(colorname, "red") == 0)
      {
      intoptarg = 0;
      }
      else if(strcmp(colorname, "green") == 0)
      {
      intoptarg = 1;
      }
      else if(strcmp(colorname, "blue") == 0)
      {
      intoptarg = 2;
      }
      else
      {
        printf("Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", colorname);
        exit(-1);
      }
      chCount++;

      break;
    case 'r':
      option = "remove";
      inputfName = argv[argc-1];
      colorname = optarg;
      if(strcmp(colorname, "red") == 0)
      {
      intoptarg = 0;
      }
      else if(strcmp(colorname, "green") == 0)
      {
      intoptarg = 1;
      }
      else if(strcmp(colorname, "blue") == 0)
      {
      intoptarg = 2;
      }
      else
      {
        printf("Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", colorname);
        exit(-1);
      }
      chCount++;

      break;
    case 's':
      option = "sepia";
      inputfName = argv[argc-1];
      chCount++;

      break;
    case 'm':
      option = "mirror";
      inputfName = argv[argc-1];
      chCount++;

      break;
    case 't':
      option = "thumbnail";
      inputfName = argv[argc-1];
      intoptarg = strtol(optarg, &remaining, 10);
      if(intoptarg > 8 || intoptarg < 1)
      {
        printf("Error: Invalid scale factor: %s; must be 1-8\n", optarg);
        exit(-1);
      }
      chCount++;

      break;
    case 'n':
      option = "tile";
      inputfName = argv[argc-1];
      intoptarg = strtol(optarg, &remaining, 10);
      if(intoptarg > 8 || intoptarg < 1)
      {
        printf("Error: Invalid scale factor: %s; must be 1-8\n", optarg);
        exit(-1);
      }
      chCount++;

      break;
    case 'o':
      if(argc <= 4)
      {
        option = "PBM";
        inputfName = argv[argc-1];
        outputfName = optarg;
      }
      else
      {
        outputfName = optarg;
        if(outputfName == NULL)
        {
          printf("Error: No output file specified\n");
          exit(-1);
        }
      }
      oFlag = 1;

      break;
    case '?':
      printf("Usage: ppmcvt [-bgirsmtno] [FILE]\n");
      exit(-1);

      break;
    default: //case for b
      printf("Usage: ppmcvt [-bgirsmtno] [FILE]\n");
      exit(-1);

      break;
    }

  if(chCount > 1)
  {
    printf("Error: Multiple transformations specified\n");
    exit(-1);
  }
  else if(oFlag == 0)
  {
    printf("Error: No output file specified\n");
    exit(-1);

  }
  else if(argc <= optind)
  {
    printf("Error: No input file specified\n");
    exit(-1);
  }

  if(strcmp(option, "PBM") == 0)
  {
    ppmtopbm(inputfName, outputfName);
  }
  else if(strcmp(option, "PGM") == 0)
  {
    ppmtopgm(inputfName, outputfName, intoptarg);
  }
  else if(strcmp(option, "isolate") == 0)
  {
    isolateChannel(inputfName, outputfName, intoptarg);
  }
  else if(strcmp(option, "remove") == 0)
  {
    removeChannel(inputfName, outputfName, intoptarg);
  }
  else if(strcmp(option, "sepia") == 0)
  {
    apply_sepia(inputfName, outputfName);
  }
  else if(strcmp(option, "mirror") == 0)
  {
    mirrorVertical(inputfName, outputfName);
  }
  else if(strcmp(option, "thumbnail") == 0)
  {
    thumbnail(inputfName, outputfName, intoptarg);
  }
  else if(strcmp(option, "tile") == 0)
  {
    tileThumbnail(inputfName, outputfName, intoptarg);
  }

exit(1);
}
