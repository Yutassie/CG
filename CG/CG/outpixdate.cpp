#include "ppmload.h"

void main(void){
	struct ppmimg *image1 = NULL, *image2 = NULL, *image3 = NULL;
	unsigned int ave = 0;
	int sel;
	int j, i;
	image1 = makeimagestruct(image1);
	image2 = makeimagestruct(image2);
	image3 = makeimagestruct(image3);
	loadppmimage("6.ppm", image1);
	cloneppmimage(image1, image2);
	image3 = createppmimage(image3, image1->iwidth, image1->iheight, image1->cmode);
	FILE *fp = NULL;
	fp = fopen("pixmap.txt", "w");
	if (fp == NULL)
		exit(3);
	
	for (j = 0; j < image1->iheight; j++){
		for (i = 0; i < image1->iwidth; i++){
			struct RGBColor trgb = getPnmPixel(image1, i, j);
			if (trgb.R <= 170 && trgb.R >= 0 && trgb.G <= 255 && trgb.G >= 180 && trgb.B <= 170 && trgb.B >= 0)
				;
			else{
				fprintf(fp, "%d,%d,%d,%d,%d\n", i-30, j-30, trgb.R, trgb.G, trgb.B);
			}

		}
	}
	
	fclose(fp);
	//saveppmimage(image3, "mozic_inu8.ppm");
	deleteppmimg(image1);
	deleteppmimg(image2);
	deleteppmimg(image3);
}