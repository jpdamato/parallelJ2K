
#include "sft_bitmapLoader.h"
#include "opj_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void iopj_image_destroy(opj_image_t *image) {
	int i;
	if(image) {
		if(image->comps) {
			/* image components */
			for(i = 0; i < image->numcomps; i++) {
				opj_image_comp_t *image_comp = &image->comps[i];
				if(image_comp->data) {
					sft_free(image_comp->data);
				}
			}
			sft_free(image->comps);
		}
		sft_free(image);
	}
}

opj_image_t* iopj_image_create(int numcmpts, opj_image_cmptparm_t *cmptparms, OPJ_COLOR_SPACE clrspc) 
{
	int compno;
	opj_image_t *image = NULL;

	image = (opj_image_t*) sft_calloc(1, sizeof(opj_image_t));
	if(image) 
	{
		image->color_space = clrspc;
		image->numcomps = numcmpts;
		/* allocate memory for the per-component information */
		image->comps = (opj_image_comp_t*)sft_malloc(image->numcomps * sizeof(opj_image_comp_t));
		if(!image->comps) 
		{
			fprintf(stderr,"Unable to allocate memory for image.\n");
			iopj_image_destroy(image);
			return NULL;
		}
		/* create the individual image components */
		for(compno = 0; compno < numcmpts; compno++) 
		{
			opj_image_comp_t *comp = &image->comps[compno];
			comp->dx = cmptparms[compno].dx;
			comp->dy = cmptparms[compno].dy;
			comp->w = cmptparms[compno].w;
			comp->h = cmptparms[compno].h;
			comp->x0 = cmptparms[compno].x0;
			comp->y0 = cmptparms[compno].y0;
			comp->prec = cmptparms[compno].prec;
			comp->bpp = cmptparms[compno].bpp;
			comp->sgnd = cmptparms[compno].sgnd;
			comp->data = (int*) sft_calloc(comp->w * comp->h, sizeof(int));
			if(!comp->data) 
			{
				fprintf(stderr,"Unable to allocate memory for image.\n");
				iopj_image_destroy(image);
				return NULL;
			}
		}
	}

	return image;
}


opj_image_t* ibmptoimage(const char *filename, opj_cparameters_t *parameters) 
{
	int subsampling_dx = parameters->subsampling_dx;
	int subsampling_dy = parameters->subsampling_dy;

	int i, numcomps, w, h;
	OPJ_COLOR_SPACE color_space;
	opj_image_cmptparm_t cmptparm[3];	// maximum of 3 components 
	opj_image_t * image = NULL;

	FILE *INP ;

	BITMAPFILEHEADER_t File_h;
	BITMAPINFOHEADER_t Info_h;
	unsigned char *RGB;
	unsigned char *table_R, *table_G, *table_B;
	unsigned int j, PAD = 0;

	int x, y, index;
	int gray_scale = 1;
	int has_color;
	DWORD W, H;

	INP = fopen(filename, "rb");
	if (!INP) 
	{
		fprintf(stderr, "Failed to open %s for reading !!\n", filename);
		return NULL;
	}

	File_h.bfType = getc(INP);
	File_h.bfType = (getc(INP) << 8) + File_h.bfType;

	if (File_h.bfType != 19778) 
	{
		fprintf(stderr,"Error, not a BMP file!\n");
		fclose(INP);
		return NULL;
	}
	// FILE HEADER 
	// ------------- 
	File_h.bfSize = getc(INP);
	File_h.bfSize = (getc(INP) << 8) + File_h.bfSize;
	File_h.bfSize = (getc(INP) << 16) + File_h.bfSize;
	File_h.bfSize = (getc(INP) << 24) + File_h.bfSize;

	File_h.bfReserved1 = getc(INP);
	File_h.bfReserved1 = (getc(INP) << 8) + File_h.bfReserved1;

	File_h.bfReserved2 = getc(INP);
	File_h.bfReserved2 = (getc(INP) << 8) + File_h.bfReserved2;

	File_h.bfOffBits = getc(INP);
	File_h.bfOffBits = (getc(INP) << 8) + File_h.bfOffBits;
	File_h.bfOffBits = (getc(INP) << 16) + File_h.bfOffBits;
	File_h.bfOffBits = (getc(INP) << 24) + File_h.bfOffBits;

	// INFO HEADER 
	// ------------- 

	Info_h.biSize = getc(INP);
	Info_h.biSize = (getc(INP) << 8) + Info_h.biSize;
	Info_h.biSize = (getc(INP) << 16) + Info_h.biSize;
	Info_h.biSize = (getc(INP) << 24) + Info_h.biSize;

	if(Info_h.biSize != 40)
	{
		fprintf(stderr,"Error, unknown BMP header size %ld\n", Info_h.biSize);
		fclose(INP);
		return NULL;
	}
	Info_h.biWidth = getc(INP);
	Info_h.biWidth = (getc(INP) << 8) + Info_h.biWidth;
	Info_h.biWidth = (getc(INP) << 16) + Info_h.biWidth;
	Info_h.biWidth = (getc(INP) << 24) + Info_h.biWidth;
	w = Info_h.biWidth;

	Info_h.biHeight = getc(INP);
	Info_h.biHeight = (getc(INP) << 8) + Info_h.biHeight;
	Info_h.biHeight = (getc(INP) << 16) + Info_h.biHeight;
	Info_h.biHeight = (getc(INP) << 24) + Info_h.biHeight;
	h = Info_h.biHeight;

	Info_h.biPlanes = getc(INP);
	Info_h.biPlanes = (getc(INP) << 8) + Info_h.biPlanes;

	Info_h.biBitCount = getc(INP);
	Info_h.biBitCount = (getc(INP) << 8) + Info_h.biBitCount;

	Info_h.biCompression = getc(INP);
	Info_h.biCompression = (getc(INP) << 8) + Info_h.biCompression;
	Info_h.biCompression = (getc(INP) << 16) + Info_h.biCompression;
	Info_h.biCompression = (getc(INP) << 24) + Info_h.biCompression;

	Info_h.biSizeImage = getc(INP);
	Info_h.biSizeImage = (getc(INP) << 8) + Info_h.biSizeImage;
	Info_h.biSizeImage = (getc(INP) << 16) + Info_h.biSizeImage;
	Info_h.biSizeImage = (getc(INP) << 24) + Info_h.biSizeImage;

	Info_h.biXpelsPerMeter = getc(INP);
	Info_h.biXpelsPerMeter = (getc(INP) << 8) + Info_h.biXpelsPerMeter;
	Info_h.biXpelsPerMeter = (getc(INP) << 16) + Info_h.biXpelsPerMeter;
	Info_h.biXpelsPerMeter = (getc(INP) << 24) + Info_h.biXpelsPerMeter;

	Info_h.biYpelsPerMeter = getc(INP);
	Info_h.biYpelsPerMeter = (getc(INP) << 8) + Info_h.biYpelsPerMeter;
	Info_h.biYpelsPerMeter = (getc(INP) << 16) + Info_h.biYpelsPerMeter;
	Info_h.biYpelsPerMeter = (getc(INP) << 24) + Info_h.biYpelsPerMeter;

	Info_h.biClrUsed = getc(INP);
	Info_h.biClrUsed = (getc(INP) << 8) + Info_h.biClrUsed;
	Info_h.biClrUsed = (getc(INP) << 16) + Info_h.biClrUsed;
	Info_h.biClrUsed = (getc(INP) << 24) + Info_h.biClrUsed;

	Info_h.biClrImportant = getc(INP);
	Info_h.biClrImportant = (getc(INP) << 8) + Info_h.biClrImportant;
	Info_h.biClrImportant = (getc(INP) << 16) + Info_h.biClrImportant;
	Info_h.biClrImportant = (getc(INP) << 24) + Info_h.biClrImportant;

	// Read the data and store them in the OUT file 

	if (Info_h.biBitCount == 24) 
	{
		numcomps = 3;
		color_space = CLRSPC_SRGB;
		// initialize image components 
		memset(&cmptparm[0], 0, 3 * sizeof(opj_image_cmptparm_t));
		for(i = 0; i < numcomps; i++) 
		{
			cmptparm[i].prec = 8;
			cmptparm[i].bpp = 8;
			cmptparm[i].sgnd = 0;
			cmptparm[i].dx = subsampling_dx;
			cmptparm[i].dy = subsampling_dy;
			cmptparm[i].w = w;
			cmptparm[i].h = h;
		}
		// create the image 
		image = iopj_image_create(numcomps, &cmptparm[0], color_space);
		if(!image) 
		{
			fclose(INP);
			return NULL;
		}

		// set image offset and reference grid 
		image->x0 = parameters->image_offset_x0;
		image->y0 = parameters->image_offset_y0;
		image->x1 =	!image->x0 ? (w - 1) * subsampling_dx + 1 : image->x0 + (w - 1) * subsampling_dx + 1;
		image->y1 =	!image->y0 ? (h - 1) * subsampling_dy + 1 : image->y0 + (h - 1) * subsampling_dy + 1;

		// set image data 

		// Place the cursor at the beginning of the image information 
		fseek(INP, 0, SEEK_SET);
		fseek(INP, File_h.bfOffBits, SEEK_SET);

		W = Info_h.biWidth;
		H = Info_h.biHeight;

		// PAD = 4 - (3 * W) % 4; 
		// PAD = (PAD == 4) ? 0 : PAD; 
		PAD = (3 * W) % 4 ? 4 - (3 * W) % 4 : 0;

		RGB = (unsigned char *)malloc((3 * W + PAD) * H * sizeof(unsigned char));

		if ( fread(RGB, sizeof(unsigned char), (3 * W + PAD) * H, INP) != (3 * W + PAD) * H )
		{
			free(RGB);
			iopj_image_destroy(image);
			fprintf(stderr, "\nError: fread return a number of element different from the expected.\n");
			return NULL;
		}

		index = 0;

		for(y = 0; y < (int)H; y++) 
		{
			unsigned char *scanline = RGB + (3 * W + PAD) * (H - 1 - y);
			for(x = 0; x < (int)W; x++) 
			{
				unsigned char *pixel = &scanline[3 * x];
				image->comps[0].data[index] = pixel[2];	// R 
				image->comps[1].data[index] = pixel[1];	// G 
				image->comps[2].data[index] = pixel[0];	// B 
				index++;
			}
		}
		free(RGB);
	}// if (Info_h.biBitCount == 24)  
	else 
		if (Info_h.biBitCount == 8 && Info_h.biCompression == 0)//RGB 
		{
			if(Info_h.biClrUsed == 0) Info_h.biClrUsed = 256;
			else
				if(Info_h.biClrUsed > 256) Info_h.biClrUsed = 256;

			table_R = (unsigned char *) malloc(256 * sizeof(unsigned char));
			table_G = (unsigned char *) malloc(256 * sizeof(unsigned char));
			table_B = (unsigned char *) malloc(256 * sizeof(unsigned char));

			has_color = 0;	
			for (j = 0; j < Info_h.biClrUsed; j++) 
			{
				table_B[j] = (unsigned char)getc(INP);
				table_G[j] = (unsigned char)getc(INP);
				table_R[j] = (unsigned char)getc(INP);
				getc(INP);
				has_color += 
					!(table_R[j] == table_G[j] && table_R[j] == table_B[j]);
			}
			if(has_color) gray_scale = 0;

			// Place the cursor at the beginning of the image information 
			fseek(INP, 0, SEEK_SET);
			fseek(INP, File_h.bfOffBits, SEEK_SET);

			W = Info_h.biWidth;
			H = Info_h.biHeight;
			if (Info_h.biWidth % 2)
				W++;

			numcomps = gray_scale ? 1 : 3;
			color_space = gray_scale ? CLRSPC_GRAY : CLRSPC_SRGB;
			// initialize image components 
			memset(&cmptparm[0], 0, 3 * sizeof(opj_image_cmptparm_t));
			for(i = 0; i < numcomps; i++) 
			{
				cmptparm[i].prec = 8;
				cmptparm[i].bpp = 8;
				cmptparm[i].sgnd = 0;
				cmptparm[i].dx = subsampling_dx;
				cmptparm[i].dy = subsampling_dy;
				cmptparm[i].w = w;
				cmptparm[i].h = h;
			}
			// create the image 
			image = iopj_image_create(numcomps, &cmptparm[0], color_space);
			if(!image) 
			{
				fclose(INP);
				free(table_R); free(table_G); free(table_B);
				return NULL;
			}

			// set image offset and reference grid 
			image->x0 = parameters->image_offset_x0;
			image->y0 = parameters->image_offset_y0;
			image->x1 =	!image->x0 ? (w - 1) * subsampling_dx + 1 : image->x0 + (w - 1) * subsampling_dx + 1;
			image->y1 =	!image->y0 ? (h - 1) * subsampling_dy + 1 : image->y0 + (h - 1) * subsampling_dy + 1;

			// set image data 

			RGB = (unsigned char *) malloc(W * H * sizeof(unsigned char));

			if ( fread(RGB, sizeof(unsigned char), W * H, INP) != W * H )
			{
				free(table_R);
				free(table_G);
				free(table_B);
				free(RGB);
				iopj_image_destroy(image);
				fprintf(stderr, "\nError: fread return a number of element different from the expected.\n");
				return NULL;
			}
			if (gray_scale) 
			{
				index = 0;
				for (j = 0; j < W * H; j++) 
				{
					if ((j % W < W - 1 && Info_h.biWidth % 2) || !(Info_h.biWidth % 2)) 
					{
						image->comps[0].data[index] = 
							table_R[RGB[W * H - ((j) / (W) + 1) * W + (j) % (W)]];
						index++;
					}
				}

			} 
			else 
			{
				index = 0;
				for (j = 0; j < W * H; j++) 
				{
					if ((j % W < W - 1 && Info_h.biWidth % 2) 
						|| !(Info_h.biWidth % 2)) 
					{
						unsigned char pixel_index = 
							RGB[W * H - ((j) / (W) + 1) * W + (j) % (W)];
						image->comps[0].data[index] = table_R[pixel_index];
						image->comps[1].data[index] = table_G[pixel_index];
						image->comps[2].data[index] = table_B[pixel_index];
						index++;
					}
				}
			}
			free(RGB);
			free(table_R);
			free(table_G);
			free(table_B);
		}// RGB8  
		else 
			if (Info_h.biBitCount == 8 && Info_h.biCompression == 1)//RLE8
			{
				unsigned char *pix, *beyond;
				int *gray, *red, *green, *blue;
				unsigned int x, y, max;
				int i, c, c1;
				unsigned char uc;

				if (Info_h.biClrUsed == 0)
					Info_h.biClrUsed = 256;
				else if (Info_h.biClrUsed > 256)
					Info_h.biClrUsed = 256;

				table_R = (unsigned char *) malloc(256 * sizeof(unsigned char));
				table_G = (unsigned char *) malloc(256 * sizeof(unsigned char));
				table_B = (unsigned char *) malloc(256 * sizeof(unsigned char));

				has_color = 0;
				for (j = 0; j < Info_h.biClrUsed; j++)
				{
					table_B[j] = (unsigned char)getc(INP);
					table_G[j] = (unsigned char)getc(INP);
					table_R[j] = (unsigned char)getc(INP);
					getc(INP);
					has_color += !(table_R[j] == table_G[j] && table_R[j] == table_B[j]);
				}

				if (has_color)
					gray_scale = 0;

				numcomps = gray_scale ? 1 : 3;
				color_space = gray_scale ? CLRSPC_GRAY : CLRSPC_SRGB;
				// initialize image components 
				memset(&cmptparm[0], 0, 3 * sizeof(opj_image_cmptparm_t));
				for (i = 0; i < numcomps; i++)
				{
					cmptparm[i].prec = 8;
					cmptparm[i].bpp = 8;
					cmptparm[i].sgnd = 0;
					cmptparm[i].dx = subsampling_dx;
					cmptparm[i].dy = subsampling_dy;
					cmptparm[i].w = w;
					cmptparm[i].h = h;
				}
				// create the image 
				image = iopj_image_create(numcomps, &cmptparm[0], color_space);
				if (!image)
				{
					fclose(INP);
					free(table_R);
					free(table_G);
					free(table_B);
					return NULL;
				}

				// set image offset and reference grid 
				image->x0 = parameters->image_offset_x0;
				image->y0 = parameters->image_offset_y0;
				image->x1 = !image->x0 ? (w - 1) * subsampling_dx + 1 : image->x0 + (w
					- 1) * subsampling_dx + 1;
				image->y1 = !image->y0 ? (h - 1) * subsampling_dy + 1 : image->y0 + (h
					- 1) * subsampling_dy + 1;

				// set image data 

				// Place the cursor at the beginning of the image information 
				fseek(INP, 0, SEEK_SET);
				fseek(INP, File_h.bfOffBits, SEEK_SET);

				W = Info_h.biWidth;
				H = Info_h.biHeight;
				RGB = (unsigned char *) calloc(1, W * H * sizeof(unsigned char));
				beyond = RGB + W * H;
				pix = beyond - W;
				x = y = 0;

				while (y < H)
				{
					c = getc(INP);

					if (c)
					{
						c1 = getc(INP);

						for (i = 0; i < c && x < W && pix < beyond; i++, x++, pix++)
							*pix = (unsigned char)c1;
					}
					else
					{
						c = getc(INP);

						if (c == 0x00) // EOL 
						{
							x = 0;
							++y;
							pix = RGB + x + (H - y - 1) * W;
						}
						else if (c == 0x01) // EOP 
							break;
						else if (c == 0x02) // MOVE by dxdy 
						{
							c = getc(INP);
							x += c;
							c = getc(INP);
							y += c;
							pix = RGB + (H - y - 1) * W + x;
						}
						else // 03 .. 255 
						{
							i = 0;
							for (; i < c && x < W && pix < beyond; i++, x++, pix++)
							{
								c1 = getc(INP);
								*pix = (unsigned char)c1;
							}
							if (c & 1) // skip padding byte 
								getc(INP);
						}
					}
				}// while() 

				if (gray_scale)
				{
					gray = image->comps[0].data;
					pix = RGB;
					max = W * H;

					while (max--)
					{
						uc = *pix++;

						*gray++ = table_R[uc];
					}
				}
				else
				{
					//int *red, *green, *blue;

					red = image->comps[0].data;
					green = image->comps[1].data;
					blue = image->comps[2].data;
					pix = RGB;
					max = W * H;

					while (max--)
					{
						uc = *pix++;

						*red++ = table_R[uc];
						*green++ = table_G[uc];
						*blue++ = table_B[uc];
					}
				}
				free(RGB);
				free(table_R);
				free(table_G);
				free(table_B);
			}// RLE8 
			else 
			{
				fprintf(stderr, 
					"Other system than 24 bits/pixels or 8 bits (no RLE coding) "
					"is not yet implemented [%d]\n", Info_h.biBitCount);
			}
			fclose(INP);
			return image;
}
