#include "stdafx.h"
#include "processimage.h"
#include "xipl.h"
#include <string>

ProcessImage::ProcessImage()
{
	arr=NULL;
	strcpy_s(m_alg,200,"");
}

ProcessImage::~ProcessImage()
{
	if (arr)
	{
		delete [] arr;
	}
}

void ProcessImage::GetOptionsStr(char* str)
{	
	strcpy_s(str,200,"");

	if (featuresMode==PIXELSONLY)
	{
		strcat_s(str,200,"PIXELSONLY.");
	}
	else if (featuresMode==SYMMETRY)
	{
		strcat_s(str,200,"SYMMETRY.");
	}
	
	if (thresholdMode==GRAYSCALE)
	{
		strcat_s(str,200,"GRAYSCALE.");
		if (grayNormMode==NOGRAYNORM)
		{
			strcat_s(str,200,"NOGRAYNORM.");
		} 
		else
		{
			strcat_s(str,200,"HISTOGRAMEQ.");
		}
	}
	else
	{
		strcat_s(str,200,"THRESHOLD.");
	}

	if (edgeRemovalMode==NOEDGEREMOVAL)
	{
		strcat_s(str,200,"NOEDGEREMOVAL");
	} 
	else
	{
		char num[10];
		strcat_s(str,200,"MAXBLACKPIXELS");
		strcat_s(str,200,itoa(maxblackpixelsinmargin,num,10));
	}
}

bool ProcessImage::SetOptionsStr(const char* str)
{
	char optstr[200];
	
	strcpy_s(optstr,200,str);
	char*token;
	char *nexttoken;

	token=strtok_s(optstr,".",&nexttoken);

	while (token!=NULL)
	{
		if (strcmp(token,"PIXELSONLY")==0)
		{
			featuresMode=PIXELSONLY;
			arr=new uint[WIDTH*HEIGHT];
			m_numOfFeatures=WIDTH*HEIGHT;
		}
		else if (strcmp(token,"SYMMETRY")==0)
		{
			featuresMode=SYMMETRY;
			arr=new uint[boxesnumber*maxboxwidth*maxboxheight];
			m_numOfFeatures=boxesnumber*maxboxwidth*maxboxheight;
		}
		else if (strcmp(token,"GRAYSCALE")==0)
		{
			thresholdMode=GRAYSCALE;
		}
		else if (strcmp(token,"THRESHOLD")==0)
		{
			thresholdMode=THRESHOLD;
		}
		else if (strcmp(token,"NOGRAYNORM")==0)
		{
			grayNormMode=NOGRAYNORM;
		}
		else if (strcmp(token,"HISTOGRAMEQ")==0)
		{
			grayNormMode=HISTOGRAMEQ;
		}
		else if (strcmp(token,"NOEDGEREMOVAL")==0)
		{
			edgeRemovalMode=NOEDGEREMOVAL;
		}
		else if (strcmp(token,"MAXBLACKPIXELS0")==0)
		{
			edgeRemovalMode=MAXBLACKPIXELS;
			maxblackpixelsinmargin=0;
		}
		else if (strcmp(token,"MAXBLACKPIXELS1")==0)
		{
			edgeRemovalMode=MAXBLACKPIXELS;
			maxblackpixelsinmargin=1;
		}
		else if (strcmp(token,"MAXBLACKPIXELS2")==0)
		{
			edgeRemovalMode=MAXBLACKPIXELS;
			maxblackpixelsinmargin=2;
		}
		else
			return false;

		token=strtok_s(NULL,".",&nexttoken);
	}

	GetOptionsStr(m_alg);

	return true;
}

int BlackPixelsAround(IplImage* oldimage, int k, int row, int col, int threshold)
{
	int blackPixelsAround=0;
	int neighbourpos;
	int alignedwidth=oldimage->widthStep;

	if (row>0)
	{
		neighbourpos=k-alignedwidth;
		if(oldimage->imageData[neighbourpos]<threshold)
			blackPixelsAround++;
	}

	if (row<oldimage->height-1)
	{
		neighbourpos=k+alignedwidth;
		if(oldimage->imageData[neighbourpos]<threshold)
			blackPixelsAround++;
	}

	if (col>0)
	{
		neighbourpos=k-1;
		if(oldimage->imageData[neighbourpos]<threshold)
			blackPixelsAround++;
	}

	if (col<oldimage->width-1)
	{
		neighbourpos=k+1;
		if(oldimage->imageData[neighbourpos]<threshold)
			blackPixelsAround++;
	} 

	/// diagonal
	if (row>0 && col>0)
	{
		neighbourpos=k-alignedwidth-1;
		if(oldimage->imageData[neighbourpos]<threshold)
			blackPixelsAround++;
	}

	if (row<oldimage->height-1 && col>0)
	{
		neighbourpos=k+alignedwidth-1;
		if(oldimage->imageData[neighbourpos]<threshold)
			blackPixelsAround++;
	}

	if (row>0 && col<oldimage->width-1)
	{
		neighbourpos=k-alignedwidth+1;
		if(oldimage->imageData[neighbourpos]<threshold)
			blackPixelsAround++;
	}

	if(row<oldimage->height-1 && col<oldimage->width-1)
	{
		neighbourpos=k+alignedwidth+1;
		if(oldimage->imageData[neighbourpos]<threshold)
			blackPixelsAround++;
	} 

	return blackPixelsAround;
}

void ProcessImage::findblackedges(IplImage*im, int maxblacksinblankrow, int maxblacksinblankcol, int &toprow, int &bottomrow, int&leftcol, int &rightcol, int threshold)
{
	int blacksinrows[500];
	int blacksincols[500];

	for (int i=0;i<500;i++)
	{
		blacksinrows[i]=0;
		blacksincols[i]=0;
	}

	int k=0;
	for (int row=0;row<im->height;row++)
	{		
		for (int col=0;col<im->widthStep;col++)
		{
			if(col<im->width)
			{
				uchar p=im->imageData[k];

				if (p<threshold /*&& BlackPixelsAround(im,k,row,col,threshold)>=minblackpixelsaround*/)
				{
					blacksinrows[row]++;
					blacksincols[col]++;
				}
			}

			k++;
		}
	}

	for (toprow=0; toprow<im->height; toprow++)
	{
		if (blacksinrows[toprow]>maxblacksinblankrow)
		{
			break;
		}
	}

	for (bottomrow=im->height-1; bottomrow>=0; bottomrow--)
	{
		if (blacksinrows[bottomrow]>maxblacksinblankrow)
		{
			break;
		}
	}

	for (leftcol=0; leftcol<im->width; leftcol++)
	{
		if (blacksincols[leftcol]>maxblacksinblankcol)
		{
			break;
		}
	}

	for (rightcol=im->width-1; rightcol>=0; rightcol--)
	{
		if (blacksincols[rightcol]>maxblacksinblankcol)
		{
			break;
		}
	}
}


double blackratio(IplImage*im,int l,int t,int r,int b,int threshold)
{
	int blacks=0,whites=0;//,sum=0,num=0;
	for (int row=t;row<=b;row++)
	{		
		for (int col=l;col<=r;col++)
		{
			uchar p;
			iplGetPixel(im,col,row,&p);

//			if (ctype==BW)
			{			
				if (p<threshold)
					blacks++;
				else
					whites++;
			}
/*			else//GRAY
			{
				sum+=255-p;
				num++;
			}*/
		}
	}

//	if (ctype==BW)
	{
		return blacks/(double)(blacks+whites);
	}
/*	else
	{
		return (sum/(double)num)/255;
	}*/
}

double grayavg(IplImage*im,int l,int t,int r,int b,int &pixelsnum)
{
	pixelsnum=0;
	int pixelsum=0;
	for (int row=t;row<=b;row++)
	{		
		for (int col=l;col<=r;col++)
		{
			uchar p;
			iplGetPixel(im,col,row,&p);
			pixelsum+=p;
			pixelsnum++;
		}
	}
	return pixelsum/(double)pixelsnum;
}

IplImage* ProcessImage::image2array(IplImage*image, uint* arr,int threshold,const char*alg)
{
	if(m_alg!=NULL && alg!=NULL &&_stricmp(alg,m_alg)!=0)
	{
		printf("i2a error\n");
		exit(1);
	}
	
	IplImage* origin0image;
	if (image->origin==1)
	{
		origin0image=(IplImage*)xiplCreate(image->width,image->height);
		for (int row=0;row<image->height;row++)
		{		
			for (int col=0;col<image->width;col++)
			{
				uchar p;
				iplGetPixel(image,col,row,&p);
				iplPutPixel(origin0image,col,/*image->height-1-*/row,&p);
			}
		}
	}
	else
	{
		origin0image=iplCloneImage(image);
	}

	IplImage*nomarginsimage;
	int toprow,bottomrow,leftcol,rightcol;
	if (edgeRemovalMode==MAXBLACKPIXELS)
	{
		findblackedges(origin0image,maxblackpixelsinmargin,maxblackpixelsinmargin,toprow,bottomrow,leftcol,rightcol,threshold);

		if (rightcol>=0 && rightcol<origin0image->width &&
			leftcol>=0 && leftcol<origin0image->width &&
			toprow>=0 && toprow<origin0image->height &&
			bottomrow>=0 && bottomrow<origin0image->height &&
			rightcol>leftcol+WIDTH/3 && bottomrow>toprow+HEIGHT/3)
		{
			nomarginsimage=(IplImage*)xiplCreate(rightcol-leftcol+1,bottomrow-toprow+1);
			for (int row=toprow;row<=bottomrow;row++)
			{		
				for (int col=leftcol;col<=rightcol;col++)
				{
					uchar p;
					iplGetPixel(origin0image,col,row,&p);
					iplPutPixel(nomarginsimage,col-leftcol,row-toprow,&p);
				}
			}
		}
		else
		{
			nomarginsimage=iplCloneImage(origin0image);
		}
	}
	else
	{
		nomarginsimage=iplCloneImage(origin0image);
	}

	IplImage*resizedimage=(IplImage*)xiplCreate(WIDTH,HEIGHT);

	iplResizeFit(nomarginsimage,resizedimage,IPL_INTER_NN);	

	if (thresholdMode==THRESHOLD)
		iplThreshold(resizedimage,resizedimage,threshold);

	int k=0;
	uchar mingray=255,maxgray=0;

	if (thresholdMode==GRAYSCALE && grayNormMode==HISTOGRAMEQ)
	{
		int range=256;
		IplLUT lut={range+1,NULL,NULL,NULL,IPL_LUT_LOOKUP};
		lut.key=new int[range+1];
		for (int level=0; level<=range;level++)
		{
			lut.key[level]=level;
		}
		lut.value=new int[range];
		IplLUT* plut=&lut;
		iplComputeHisto(resizedimage,&plut);
		iplHistoEqualize(resizedimage,resizedimage,&plut);
		delete []lut.key;
		delete []lut.value;
	}

	if (featuresMode==PIXELSONLY)
	{
		k=0;
		for(int i=0;i<resizedimage->height;i++)
		{
			for(int j=0;j<resizedimage->widthStep;j++)
			{
				if (j<resizedimage->width)
				{
					uchar uc=(uchar)(resizedimage->imageData[k]);
					uint ui=(uint)uc;
					arr[k]=ui;
					k++;
				}
			}
		}
	}
	else if (featuresMode==SYMMETRY)
	{
		for (int i=0; i<maxboxwidth; i++)
		for (int j=0; j<maxboxheight; j++)
		{
			arr[(i*maxboxheight+j)*boxesnumber+0]=(uint)(100*blackratio(resizedimage,0,0,0+i,0+j,threshold));
			arr[(i*maxboxheight+j)*boxesnumber+1]=(uint)(100*blackratio(resizedimage,15-i,0,15,0+j,threshold));
			arr[(i*maxboxheight+j)*boxesnumber+2]=(uint)(100*blackratio(resizedimage,15-i,31-j,15,31,threshold));
			arr[(i*maxboxheight+j)*boxesnumber+3]=(uint)(100*blackratio(resizedimage,0,31-j,0+i,31,threshold));		
			arr[(i*maxboxheight+j)*boxesnumber+4]=(uint)(100*blackratio(resizedimage,0,15-j,0+i,16+j,threshold));
		}
	}

	xiplFree((long)origin0image);
	xiplFree((long)nomarginsimage);
	return resizedimage;
}
