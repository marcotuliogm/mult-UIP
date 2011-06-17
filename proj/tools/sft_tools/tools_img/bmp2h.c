/**
 * \addtogroup tools
 * @{
 */

/**
 * \brief Conversor BMP para C header RAW R1G1B1R2G2B2 comprimido com RLE
 *
 * Informações sobre o formato BMP obtidas em:
 * http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
 *
 * @author Jimmy Pinto Stelzer<jimmy.stelzer@gmail.com>
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG
//#define VERBOSE

typedef struct {
   unsigned char B;
   unsigned char G;
   unsigned char R;
} COR;

typedef struct {
    unsigned char fator; // RLE 8 bits
    unsigned char R1G1;
    unsigned char B1R2;
    unsigned char G2B2;
} RLE8;

typedef struct {
   unsigned int largura;
   unsigned int altura;
   unsigned short int numeroDeBits;
   unsigned int tamanho;
   unsigned int dados;
} CABECALHO;

int main(int argc, char *argv[]){
    FILE *imagemBMP,*imagemRAW;
    char *arquivoBMP,*arquivoRAW;
    char temp[20];
    int tam;
    unsigned char junk;
    CABECALHO imagemInfo;
    COR cor, cor2;
    RLE8 imagemRLE[132*132*24];
    COR imagemMEM[132*132*24];
    COR ordenado[132][132];
    int numeroMagico=0, i=0, j=0, k=0, l=0, m=0, n=0, o=0, x=0, y=0, qtd=0;
    size_t r;
    if(argc < 3 || argc > 3){
        printf("%s --- ", argv[0]);
        printf("Comversor BMP para o formato C header RAW R1G1B1R2G2B2 comprimido com RLE\n");
        printf("Desenvolvido por Jimmy Pinto Stelzer<jimmy.stelzer@gmail.com>\n\n");
        printf("Uso: %s <pasta> <quantidade>\n", argv[0]);
        return 1;
    }else{
        arquivoBMP = argv[1];
        strcpy(temp, argv[1]);
        arquivoRAW = argv[1];
        tam = atoi(argv[2]);
        //recurso = argv[3];
    }
    for(qtd = 1; qtd<=tam; qtd++){
		numeroMagico=0, i=0, j=0, k=0, l=0, m=0, n=0, o=0, x=0, y=0;
		for(i=0;i<132*132*24;i++){
			imagemRLE[i].fator = 0x00;
			imagemRLE[i].R1G1 = 0x00;
			imagemRLE[i].B1R2 = 0x00;
			imagemRLE[i].G2B2 = 0x00;
		}

		sprintf(arquivoBMP, "%s/%d.bmp", temp, qtd);
		imagemBMP=fopen(arquivoBMP,"rb");
		if(imagemBMP==NULL){
			fprintf(stderr,"Erro! O arquivo %s não pode ser lido.\n",arquivoBMP);
			return 1;
		}
		printf("Processando o arquivo %s.\n", arquivoBMP);
		/* Determinar o formato do arquivo */
		r=fread(&numeroMagico,sizeof(unsigned short),1,imagemBMP);
		if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler o formato do arquivo %s.\n",arquivoBMP); return 1; }
	#ifdef DEBUG
		printf("numeroMagico=0x%x\n", numeroMagico);
	#endif
		if(numeroMagico!=0x4d42){ fprintf(stderr,"Erro! O arquivo %s não é um arquivo BMP válido.\n",arquivoBMP); }

		/* Determinar o tamanho do arquivo */
		r=fread(&imagemInfo.tamanho,4,1,imagemBMP);
		if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler o tamanho do arquivo %s.\n",arquivoBMP); return 1; }
	#ifdef DEBUG
		printf("bmp.tamanho=%d bytes\n", imagemInfo.tamanho);
	#endif
		/* Determinar o incio dos dados */
		fseek(imagemBMP,10,SEEK_SET);
		r=fread(&imagemInfo.dados,4,1,imagemBMP);
		if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler a posição inicial dos dados da imagem no arquivo %s.\n",arquivoBMP); return 1; }

		/* Determinar largura da Imagem */
		fseek(imagemBMP,18,SEEK_SET);
		r=fread(&imagemInfo.largura,4,1,imagemBMP);
		if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler a largura da imagem no arquivo %s.\n",arquivoBMP); return 1; }
	#ifdef DEBUG
		printf("bmp.largura=%d\n", imagemInfo.largura);
	#endif

		/* Determinar altura da Imagem */
		fseek(imagemBMP,22,SEEK_SET);
		r=fread(&imagemInfo.altura,4,1,imagemBMP);
		if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler a altura da imagem no arquivo %s.\n",arquivoBMP); return 1; }
	#ifdef DEBUG
		printf("bmp.altura=%d\n", imagemInfo.altura);
	#endif

		if(imagemInfo.largura > 132 || imagemInfo.altura > 132){
			fprintf(stderr,"Erro! A Imagem (%s) é muito grande para ser usado no KIT. A resolução do Display LCD é 132x132 px.\n Você de usar uma imagem menor ou igual a esse tamanho.\n",arquivoBMP); return 1;
		}

		/* Determinar bits/cor */
		fseek(imagemBMP,28,SEEK_SET);
		r=fread(&imagemInfo.numeroDeBits,2,1,imagemBMP);
		if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler o número de bits por cor da imagem no arquivo %s.\n",arquivoBMP); return 1; }
	#ifdef DEBUG
		printf("bmp.numeroDeBits=%d\n", imagemInfo.numeroDeBits);
	#endif

		/* Abrir o arquivo C header */
		sprintf(arquivoRAW, "%s/%d", temp, qtd);
		//printf("\n\n%s\n", temp);
		imagemRAW=fopen(arquivoRAW,"wb+");
		if(imagemRAW==NULL){
			fprintf(stderr,"Erro! O arquivo %s não pode ser lido.\n",arquivoRAW);
			return 1;
		}

//		fprintf(imagemRAW,"/*\n * Arquivo gerado automaticamente por bmp2c\n - bmp2c foi desenvolvido por Jimmy Pinto Stelzer<jimmy.stelzer@gmail.com>\n* Imagem"
//			" no formato RAW R1G1B1R2G2B2 para usar no Kit de desenvolvimento ARM7 - "
//			"LPC23XX-ARM7 - 2010/02\n * Em conjunto com a biblioteca gráfica ARM-GL\n"
//			" */\n\n"
//			"const unsigned char %s_largura = %d;\n"
//			"const unsigned char %s_altura = %d;\n"
//			"const unsigned char %s[] = {\n"
//			, recurso, imagemInfo.largura, recurso, imagemInfo.altura, recurso);

		/* Posiciona no inicio da Imagem */
		fseek(imagemBMP,imagemInfo.dados,SEEK_SET);
		n = (imagemInfo.largura * 3);
		while(n>0){
			n-=4;
		}
		if(n<0){
			n=-n;
		}
		for(j=0,i=0,m=imagemInfo.largura;i<(imagemInfo.altura*imagemInfo.largura);i++){
			/* Obtem a cor do Pixel em RGB */
			r=fread(&cor.B,sizeof(unsigned char),1,imagemBMP);
			if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler o bit de cor B do arquivo %s.\n",arquivoBMP); return 1; }
			r=fread(&cor.G,sizeof(unsigned char),1,imagemBMP);
			if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler o bit de cor G do arquivo %s.\n",arquivoBMP); return 1; }
			r=fread(&cor.R,sizeof(unsigned char),1,imagemBMP);
			if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler o bit de cor R do arquivo %s.\n",arquivoBMP); return 1; }

			imagemMEM[k].R = cor.R;
			imagemMEM[k].G = cor.G;
			imagemMEM[k].B = cor.B;

			j+=3;
	#ifdef DEBUG
	#ifdef VERBOSE
		printf("[%04x] (%d) RGB arquivo: %02x,%02x,%02x -> RGB memoria: %02x,%02x,%02x\n", j+imagemInfo.dados, i, cor.R, cor.G, cor.B, imagemMEM[k].R, imagemMEM[k].G, imagemMEM[k].B);
	#endif
	#endif
			m--; k++;
			if(m==0){
				m=imagemInfo.largura;
				for(o=0;o<n;o++){
					r=fread(&junk,sizeof(unsigned char),1,imagemBMP);
					if(r<=0){ fprintf(stderr,"Erro! Não foi possível ler o bit de arredondamento do arquivo %s.\n",arquivoBMP); return 1; }
				}
			}
		}
		fclose(imagemBMP);

		/* Expelhar Verticalmente*/
		k=0;
		for(y=imagemInfo.altura-1;y>=0;y--){
			for(x=0;x<imagemInfo.largura;x++){
				ordenado[x][y]=imagemMEM[k++];
			}
		}

		m=0;k=0;j=0;i=0;
		for(y=0;y<imagemInfo.altura;y++){
			for(x=0;x<imagemInfo.largura;x++){


				if(x+1<imagemInfo.largura){
					cor.R = ordenado[x][y].R;
					cor.G = ordenado[x][y].G;
					cor.B = ordenado[x][y].B;
					cor2.R = ordenado[++x][y].R;
					cor2.G = ordenado[x][y].G;
					cor2.B = ordenado[x][y].B;
				}else if(y+1<imagemInfo.altura){
					cor.R = ordenado[x][y].R;
					cor.G = ordenado[x][y].G;
					cor.B = ordenado[x][y].B;
					x=0;
					cor2.R = ordenado[x][++y].R;
					cor2.G = ordenado[x][y].G;
					cor2.B = ordenado[x][y].B;
				}else{
					break;
				}


				/* Encodar RGB em RLE8 R1G1B1R2G2B2 */
				if(imagemRLE[k].fator != 0 && imagemRLE[k].fator < 0xf9 && imagemRLE[k].R1G1 == ((cor.R & 0xf0)|((cor.G & 0xf0) >> 4)) && imagemRLE[k].B1R2 == ((cor.B & 0xf0)|((cor2.R & 0xf0) >> 4)) && imagemRLE[k].G2B2 == ((cor2.G & 0xf0)|((cor2.B & 0xf0) >> 4))){
					imagemRLE[k].fator++;
				}else{
					if(imagemRLE[k].fator != 0){
						k++;
					}
					imagemRLE[k].R1G1 = (cor.R & 0xf0)|((cor.G & 0xf0) >> 4);
					imagemRLE[k].B1R2 = (cor.B & 0xf0)|((cor2.R & 0xf0) >> 4);
					imagemRLE[k].G2B2 = (cor2.G & 0xf0)|((cor2.B & 0xf0) >> 4);
					imagemRLE[k].fator = 1;
				}
	#ifdef DEBUG
	#ifdef VERBOSE
		printf("[%04x] (%d) RGB: %02x,%02x,%02x -> R1G1B1R2G2B2: %02x,%02x,%02x\n", j, i, cor.R, cor.G, cor.B, imagemRLE[k].R1G1, imagemRLE[k].B1R2, imagemRLE[k].G2B2);
	#endif
	#endif
			j+=3;i++;
			}
		}

		/* Gerar Header */
		fputs("    ",imagemRAW);
	#ifdef DEBUG
			printf("\nRAW=%d - RLE=%d - Fator de compresão: %1.4f\n", i, k+1, (float) i/(k+1));
	#endif
		for(i=0,j=0;i<=k;i++){
			fprintf(imagemRAW,"0x%02x,0x%02x,0x%02x,0x%02x,", imagemRLE[i].fator, imagemRLE[i].R1G1, imagemRLE[i].B1R2, imagemRLE[i].G2B2);
			l+=imagemRLE[i].fator;
			j++;
	#ifdef DEBUG
	#ifdef VERBOSE
			printf("0x%02x,0x%02x,0x%02x,0x%02x,", imagemRLE[i].fator, imagemRLE[i].R1G1, imagemRLE[i].B1R2, imagemRLE[i].G2B2);
			if(j>4){ printf("\n"); }
	#endif
	#endif

			if(j>4){ j=0; fputs("\n    ",imagemRAW); }
		}
	#ifdef DEBUG
			printf("\nContagem descompreção RLE=%d\n", l);
	#endif
		//fputs("0xfa\n};\n",imagemRAW);
		fputs("0xfa\n",imagemRAW);
		fclose(imagemRAW);
	}
    return 0;
}

/** @} */
