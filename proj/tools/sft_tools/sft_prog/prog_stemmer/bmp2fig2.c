
/*********************
Converte bitmap tipo windows bmp de 24 bits/pix para
figura crua 132x132 e 12 bits/pix do display nokia 6100
O formato fig nao tem cabecalho e nenhum tipo de compressao.
Cada 3 bytes gera 2 pixels com 4 bits por cor.
Escrito por Marcos A. Stemmer
*****************************/
#include <stdio.h>
#include <string.h>

char * mudaext(char *str)
{
char *p;
for(p=str; *str; str++) if(*str == '.') p=str;
return p;
}

struct st_bmheader {
	unsigned long size;
	unsigned long res;
	unsigned long offset;
	};

struct st_dib {
	unsigned long hsize;
	unsigned long w;
	unsigned long h;
	short planes,bpp;
	};

#define FSAVE
int bmp2fig(char *nomebin, char *nomefig)
{
FILE *fbmp, *ffig;
struct st_bmheader bmh;
struct st_dib dib;
unsigned char rgb[8];
unsigned char frgb[0x8000];
int n=0;
int x, yb;
printf("\nnomebin=\"%s\"\n", nomebin);
printf("\nnomefig=\"%s\"\n", nomefig);
fbmp = fopen(nomebin, "rb");
if(fbmp == NULL) {
	perror(nomebin);
	return 1;
	}
#ifdef FSAVE
ffig = fopen(nomefig, "wb");
if(ffig == NULL) {
	fclose(fbmp);
	perror(nomefig);
	return 2;
	}
#endif
fread(&n, 1,2, fbmp);
if(n != 19778) {
	printf("n = %d\n", n);
	fclose(fbmp);
#ifdef FSAVE
	fclose(ffig);
#endif
	return 3;
	}
n = fread(&bmh, sizeof(bmh), 1, fbmp);
n = fread(&dib, sizeof(dib), 1, fbmp);
printf("Size=%lu w=%lu h=%lu bpp=%d\n", bmh.size, dib.w, dib.h, dib.bpp);
#ifdef FSAVE
fseek(fbmp, bmh.offset, SEEK_SET);
yb = 132*66*3;
do	{
	yb -= 66*3;
	for(x=0; (x < 66*3) && x < 3*dib.w/2; x+=3){
		n = fread(rgb, 1, 6, fbmp);
		if(n != 6) break;
		frgb[yb+x+0] = (rgb[2] & 0xf0) | ((rgb[1]>>4) & 0x0f);
		frgb[yb+x+1] = (rgb[0] & 0xf0) | ((rgb[5]>>4) & 0x0f);
		frgb[yb+x+2] = (rgb[4] & 0xf0) | ((rgb[3]>>4) & 0x0f);
		}
	if(x < 3*dib.w/2) fseek(fbmp, 3*dib.w - 2*x , SEEK_CUR);
	} while(yb > 0 && n==6);
fwrite(frgb, 1, 132*66*3, ffig);
fclose(ffig);
#endif
fclose(fbmp);
return 0;
}

int main(int argc, char **argv)
{
char nomefig[256];
int i;
if(argc < 2) {
	printf("Converte bmp para figura 132x132x12\n%s <figura.bmp>\n",argv[0]);
	return 1;
	}
strncpy(nomefig, argv[1],240);
strcpy(mudaext(nomefig),".fig");
return bmp2fig(argv[1],nomefig);
}
