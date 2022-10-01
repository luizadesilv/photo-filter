#include <stdio.h>
#include <stdlib.h>

#define IMAGEM "c:\\EP\\TT1.bmp"

struct BMPpixel
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};
typedef struct BMPpixel pixel;

struct BMPimagem                //detalhes da imagem
{
  unsigned char tipo[2];        //para os caracteres BM
  unsigned long tamByte;        //tamanho total do arquivo em Bytes
  unsigned char reservado[4];   //reservado, sempre 0
  unsigned long inicioImagem;   //quantos bytes at� o in�cio da imagem propriamente dita.
  unsigned long tamCabecalho;   //tamanho do cabe�alho sempre 40d ou 0x28h
  unsigned long largura;        //Largura da imagem em pixels
  unsigned long altura;         //Altura da imagem em pixels
};

struct BMPimagem img;

char cab,dado;

int main()
{
    FILE *arq;
    FILE *arq1;
    FILE *arq2;
    FILE *arq3;
    FILE *arq4;
    FILE *arq5;
    FILE *arq6;
    pixel *dados;
    int i,j,x,y;
    unsigned char limiar;


    if((arq = fopen(IMAGEM,"r+b"))  != NULL)
    {
        printf("\nArquivo%s aberto\n",IMAGEM);

        //fread(Endere�o do destino, tamanho de cada dado, quantidade de dados a ser lida, ponteiro do arquivo);
        fread(img.tipo,sizeof(char),2,arq);                  //BM
        fread(&img.tamByte,sizeof(unsigned long),1,arq);     //tamanho do arquivo em byte
        fread(img.reservado,sizeof(unsigned char),4,arq);    //l� BfReser1 e BfReser2 ,2 bytes cada
        fread(&img.inicioImagem,sizeof(unsigned long),1,arq);//onde come�a a imagem mesmo
        fread(&img.tamCabecalho,sizeof(unsigned long),1,arq);//tamanho do cabe�alho
        fread(&img.largura,sizeof(unsigned long),1,arq);     //largura da imagem
        fread(&img.altura,sizeof(unsigned long),1,arq);      //altura da imagem

        printf("\n\nCaracteristicas da imagem:\n");
        printf("\nTAMANHO DA IMAGEM: %lu bytes",img.tamByte);
        printf("\nTAMANHO DO CABECALHO: %d",img.inicioImagem);
        printf("\nALTURA DA IMAGEM: %d pixels\nLARGURA DA IMAGEM: %d pixels\n",img.altura,img.largura);
        printf("\nCABECALHO:\n");

        //ler o cabe�alho
        char cabec[54];
        fseek(arq,0,SEEK_SET);
        for(i=0;i<54;i++)
        {
            fscanf(arq,"%c",&cab);
            cabec[i]=cab;
            printf("%x ",cabec[i]);
        }

        int tamanhoDados = img.tamByte - img.inicioImagem;   //TAMANHO DA IMAGEM VEZES 3 PQ � RGB
        unsigned char imag[tamanhoDados];

        for(i=54;i<tamanhoDados;i++)
        {
            fscanf(arq,"%c",&dado);
            imag[i]=dado;
          //  printf("%d ",imag[i]);
        }

        arq1 = fopen("C:\\EP\\imgPB.bmp","wb");
        arq2 = fopen("C:\\EP\\imgLIMIAR.bmp","wb");
        arq3 = fopen("C:\\EP\\imgEXTRA.bmp","wb");
        arq4 = fopen("C:\\EP\\imgBORRADO.bmp","wb");
        arq5 = fopen("C:\\EP\\imgBORDA.bmp","wb");
        arq6 = fopen("C:\\EP\\imgENCRIPT.bmp","wb");

        fseek(arq1,0,SEEK_SET);
        fseek(arq2,0,SEEK_SET);
        fseek(arq3,0,SEEK_SET);
        fseek(arq4,0,SEEK_SET);
        fseek(arq5,0,SEEK_SET);
        fseek(arq6,0,SEEK_SET);

        fclose(arq);

        //Ler o cae�alho para os arquivos
        for (i=0;i<54;i++)
        {
            fwrite(&cabec[i],1,1,arq1);
            fwrite(&cabec[i],1,1,arq2);
            fwrite(&cabec[i],1,1,arq3);
            fwrite(&cabec[i],1,1,arq4);
            fwrite(&cabec[i],1,1,arq5);
            fwrite(&cabec[i],1,1,arq6);
        }

        fseek(arq1, img.inicioImagem, SEEK_SET);
        fseek(arq2, img.inicioImagem, SEEK_SET);
        fseek(arq3, img.inicioImagem, SEEK_SET);
        fseek(arq4, img.inicioImagem, SEEK_SET);
        fseek(arq5, img.inicioImagem, SEEK_SET);
        fseek(arq6, img.inicioImagem, SEEK_SET);

        char r[tamanhoDados];

        //MENU LIMIAR
        printf("\n\nDigite um valor de limiar:  \nVALORES ABAIXO DO LIMIAR DEVEM FICAR PRETOS, VALORES ACIMA DEVEM FICAR BRANCOS.\n");
        scanf("%d",&limiar);

        for (i=0;i<tamanhoDados/3;i++)
        {
            unsigned char media = (imag[i*3] + imag[i*3 +1] + imag[i*3 + 2])/3;
            r[i*3] = media > limiar ? 255 : 0;
            r[i*3 + 1] = media > limiar ? 255 : 0;
            r[i*3 + 2] = media > limiar ? 255 : 0;
            //printf("%d ",media);
        }

        fwrite(r,1,tamanhoDados,arq2);
        printf("\n\nFILTRO LIMIAR APLICADO\nARQUIVO DISPONIVEL PARA VISUALIZACAO!");
        fclose(arq2);

        //media RGB
        for (i=0;i<tamanhoDados/3;i++)
        {
            unsigned char media = (imag[i*3] + imag[i*3 +1] + imag[i*3 + 2])/3;
            r[i*3] = media;
            r[i*3 + 1] = media;
            r[i*3 + 2] = media;
            //printf("%d ",media);
        }
        fwrite(r,1,tamanhoDados,arq1);
        printf("\n\nFILTRO PRETO E BRANCO APLICADO\nARQUIVO DISPONIVEL PARA VISUALIZACAO!");

        int matrizF [10] = {-1,-1,-1,-1,8,-1,-1,-1,-1};

        //filtro extra
        for(x=0;x<tamanhoDados/3;x++)
        {
            r[x*3] = (r[x]  *(1)   +
                      r[x+3]*(1)   +
                      r[x+6]*(1)   +

                      r[x+420]*(1) +
                      r[x+423]*(8) +
                      r[x+426]*(1) +

                      r[x+840]*(1) +
                      r[x+843]*(1) +
                      r[x+846]*(1))/9;
        }
        fwrite(r,1,tamanhoDados,arq3);
        printf("\n\nFILTRO AMARELADO APLICADO\nARQUIVO DISPONIVEL PARA VISUALIZACAO!");
        fclose(arq3);

        for (i=0;i<tamanhoDados/3;i++)
        {
            unsigned char media = (imag[i*3] + imag[i*3 +1] + imag[i*3 + 2])/3;
            r[i*3] = media;
            r[i*3 + 1] = media;
            r[i*3 + 2] = media;
            //printf("%d ",media);
        }

        //filtro borrado
        for(x=0;x<tamanhoDados;x++)
        {
            r[x]  =  (r[x]  *(1)  +
                      r[x+3]*(1)  +
                      r[x+6]*(1)  +

                      r[x+9] *(1) +
                      r[x+12]*(8) +
                      r[x+18]*(1) +

                      r[x+21]*(1) +
                      r[x+24]*(1) +
                      r[x+27]*(1))/9;
        }
        fwrite(r,1,tamanhoDados,arq4);
        printf("\n\nFILTRO BORRADO APLICADO\nARQUIVO DISPONIVEL PARA VISUALIZACAO!");
        fclose(arq4);

         for (i=0;i<tamanhoDados/3;i++)
        {
            unsigned char media = (imag[i*3] + imag[i*3 +1] + imag[i*3 + 2])/3;
            r[i*3] = media;
            r[i*3 + 1] = media;
            r[i*3 + 2] = media;
            //printf("%d ",media);
        }

        //filtro bordas
        for(x=0;x<tamanhoDados;x++)
        {
            r[x] =   (r[x]  *(-1)  +
                      r[x+3]*(-1)  +
                      r[x+6]*(-1)  +

                      r[x+9]*(-1)  +
                      r[x+12]*(8)  +
                      r[x+18]*(-1) +

                      r[x+21]*(-1) +
                      r[x+24]*(-1) +
                      r[x+27]*(-1))/9;
        }

        fwrite(r,1,tamanhoDados,arq5);
        printf("\n\nFILTRO BORDAS APLICADO\nARQUIVO DISPONIVEL PARA VISUALIZACAO!");
        fclose(arq5);

        //filtro encripta��o

        for (i=0;i<tamanhoDados/3;i++)
        {
            unsigned char media = (imag[i*3] + imag[i*3 +1] + imag[i*3 + 2])/3;
            r[i*3] = media;
            r[i*3 + 1] = media;
            r[i*3 + 2] = media;
            //printf("%d ",media);
        }

        char senha[22];
        printf("\n\nDigite uma senha de ate 20 caracteres:");

        scanf("%s",&senha);

        int z;
        for(i=0, z=0;i<tamanhoDados;i++, z++)
        {
            if(z==strlen(senha)) z=0;
            r[i] = r[i] ^ senha[z];
        }

        fwrite(r,1,tamanhoDados,arq6);
        printf("\n\nFILTRO ENCRIPTADO APLICADO\nARQUIVO DISPONIVEL PARA VISUALIZACAO!");
        fclose(arq6);
    }
}





