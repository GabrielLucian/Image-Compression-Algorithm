#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include "structs.h"
#include "queue.h"
void advance(Image *image, int x, int y, QuadtreeNode **Pixel,int  index)
{
    int size=sqrt(Pixel[index]->area);
    if(Pixel[index]->top_left==-1)
    {
        for(int i=0;i<size;i++)
            for(int j=0; j<size;j++)
            {
                image->red[x+i][y+j]=Pixel[index]->red;
                image->green[x+i][y+j]=Pixel[index]->green;
                image->blue[x+i][y+j]=Pixel[index]->blue;
            }
    }
    else
    {
        advance(image,x,y,Pixel,Pixel[index]->top_left);
        advance(image,x,y+size/2,Pixel,Pixel[index]->top_right);
        advance(image,x+size/2,y+size/2,Pixel,Pixel[index]->bottom_right);
        advance(image,x+size/2,y,Pixel,Pixel[index]->bottom_left);
    }
}
void decompress(QuadtreeNode **Tree, FILE *output)
{
    int i;
    QuadtreeNode *node=Tree[0];
    int size=sqrt(node->area);
    //printf("%d", size);
    Image *image= malloc(sizeof(Image));
    image->red= malloc(size*sizeof(unsigned char*));
    image->green= malloc(size*sizeof(unsigned char*));
    image->blue= malloc(size*sizeof(unsigned char*));
    for(i=0;i<size;i++)
    {
        image->red[i]= malloc(size*sizeof(unsigned char));
        image->green[i]= malloc(size*sizeof(unsigned char));
        image->blue[i]= malloc(size*sizeof(unsigned char));
    }
    advance(image,0,0,Tree,0);
    fprintf(output,"P6\n%d %d\n255\n", size,size);
    for(i=0;i<size;i++)
        for(int j=0;j<size;j++)
        {
            fprintf(output,"%c%c%c",image->red[i][j],image->green[i][j],image->blue[i][j]);
        }
    for(i=0;i<size;i++)
    {
        free(image->red[i]);
        free(image->green[i]);
        free(image->blue[i]);
    }
    free(image->red);
    free(image->green);
    free(image->blue);
    free(image);
}
Image* split(int size,Image *image, Position poz)
{
    int i,j;
    Image *Corner= malloc(sizeof(Image));
    Corner->red= malloc(size*sizeof(unsigned char*));
    Corner->green= malloc(size*sizeof(unsigned char*));
    Corner->blue= malloc(size*sizeof(unsigned char*));
    for(i=0;i<size;i++)
    {
        Corner->red[i]= malloc(size*sizeof(unsigned char));
        Corner->green[i]= malloc(size*sizeof(unsigned char));
        Corner->blue[i]= malloc(size*sizeof(unsigned char));
    }
    if(poz==TOPLEFT)
    {
        for(i=0;i<size;i++)
            for(j=0;j<size;j++)
            {
                Corner->red[i][j]=image->red[i][j];
                Corner->green[i][j]=image->green[i][j];
                Corner->blue[i][j]=image->blue[i][j];
            }
    }
    if(poz==TOPRIGHT)
    {
        for(i=0;i<size;i++)
            for(j=size;j<2*size;j++)
            {
                Corner->red[i][j-size]=image->red[i][j];
                Corner->green[i][j-size]=image->green[i][j];
                Corner->blue[i][j-size]=image->blue[i][j];
            }
    }
    if(poz==BOTTOMLEFT)
    {
        for(i=size;i<2*size;i++)
            for(j=0;j<size;j++)
            {
                Corner->red[i-size][j]=image->red[i][j];
                Corner->green[i-size][j]=image->green[i][j];
                Corner->blue[i-size][j]=image->blue[i][j];
            }
    }
    if(poz==BOTTOMRIGHT)
    {
        for(i=size;i<2*size;i++)
            for(j=size;j<2*size;j++)
            {
                Corner->red[i-size][j-size]=image->red[i][j];
                Corner->green[i-size][j-size]=image->green[i][j];
                Corner->blue[i-size][j-size]=image->blue[i][j];
            }
    }
    return Corner;
}
QuadtreeNode* medie(Image *image,int size)
{
    int i,j;
    QuadtreeNode *pixel= malloc(sizeof(QuadtreeNode));
    long long red=0,green=0,blue=0;
    for(i=0;i<size;i++)
        for(j=0;j<size;j++)
        {
            red=red+image->red[i][j];
            green=green+image->green[i][j];
            blue=blue+image->blue[i][j];
        }
    red=red/(size*size);
    green=green/(size*size);
    blue=blue/(size*size);
    pixel->area=size*size;
    pixel->red=red;
    pixel->green=green;
    pixel->blue=blue;
    return pixel;
}
int shouldcontinue(Image *image, int size, int factor)
{
    QuadtreeNode *pixelmedie=medie(image,size);
    long long mean=0;
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            mean+=pow((int)(pixelmedie->red)-(int)(image->red[i][j]),2)+pow((int)(pixelmedie->green)-(int)(image->green[i][j]),2)+pow((int)(pixelmedie->blue)-(int)(image->blue[i][j]),2);
    mean=mean/(3*size*size);
    free(pixelmedie);
    if(mean<=factor)
        return 0;
    else
        return 1;
}
void mirror(char type, uint32_t nr, QuadtreeNode **Tree)
{
    int aux,i=0;
    if(type=='h')
        while(i<nr)
        {
            aux=Tree[i]->top_left;
            Tree[i]->top_left=Tree[i]->top_right;
            Tree[i]->top_right=aux;
            aux=Tree[i]->bottom_left;
            Tree[i]->bottom_left=Tree[i]->bottom_right;
            Tree[i]->bottom_right=aux;
            i++;
        }
    if(type=='v')
        while(i<nr)
        {
            aux=Tree[i]->top_left;
            Tree[i]->top_left=Tree[i]->bottom_left;
            Tree[i]->bottom_left=aux;
            aux=Tree[i]->top_right;
            Tree[i]->top_right=Tree[i]->bottom_right;
            Tree[i]->bottom_right=aux;
            i++;
        }
}
void compress(int factor, uint32_t* nr, uint32_t *frunze, FILE *input, QuadtreeNode ***CompressedTree)
{
    int i,j,size,maxval;
    char P[3],c;
    fgets(P,3,input);
    fscanf(input,"%d%d%d%c", &size,&size,&maxval,&c);
    *CompressedTree=malloc(1*sizeof(QuadtreeNode*));
    Image *Corner1,*Corner2,*Corner3,*Corner4;

    Image *image= malloc(sizeof(Image));
    image->red= malloc(size*sizeof(unsigned char*));
    image->green= malloc(size*sizeof(unsigned char*));
    image->blue= malloc(size*sizeof(unsigned char*));
    for(i=0;i<size;i++)
    {
        image->red[i]= malloc(size*sizeof(unsigned char));
        image->green[i]= malloc(size*sizeof(unsigned char));
        image->blue[i]= malloc(size*sizeof(unsigned char));
    }
    for(i=0;i<size;i++)
        for(j=0;j<size;j++)
        {
            fscanf(input,"%c%c%c",&image->red[i][j],&image->green[i][j],&image->blue[i][j]);
        }

    int cursize;
    queue_t *nodes=initQueue();
    queue_t_mat *images=initQueue_mat();
    Image *aux_img;
    QuadtreeNode *aux;
    enqueue_mat(images,image);
    enqueue(nodes,medie(image,size));
    int go;
    j=1;

    while(nodes->len>0 && images->len>0)
    {
        aux_img=dequeue_mat(images);
        aux=dequeue(nodes);
        cursize=sqrt(aux->area);

        *CompressedTree= realloc(*CompressedTree,((*nr)+1)* sizeof(QuadtreeNode*));
        go=shouldcontinue(aux_img,cursize,factor);

        if(cursize>1 && go==1)
        {
            aux->top_left = j++;
            aux->top_right = j++;
            aux->bottom_right = j++;
            aux->bottom_left = j++;

            Corner1 = split(cursize / 2, aux_img, TOPLEFT);
            Corner2 = split(cursize / 2, aux_img, TOPRIGHT);
            Corner3 = split(cursize / 2, aux_img, BOTTOMRIGHT);
            Corner4 = split(cursize / 2, aux_img, BOTTOMLEFT);

            enqueue(nodes, medie(Corner1, cursize / 2));
            enqueue_mat(images, Corner1);
            enqueue(nodes, medie(Corner2, cursize / 2));
            enqueue_mat(images, Corner2);
            enqueue(nodes, medie(Corner3, cursize / 2));
            enqueue_mat(images, Corner3);
            enqueue(nodes, medie(Corner4, cursize / 2));
            enqueue_mat(images, Corner4);
        }
        else
        {
            (*frunze)++;
            aux->top_left=-1;
            aux->top_right=-1;
            aux->bottom_right=-1;
            aux->bottom_left=-1;
        }
        (*CompressedTree)[(*nr)++]=aux;
        for(i=0;i<cursize;i++)
        {
            free(aux_img->red[i]);
            free(aux_img->green[i]);
            free(aux_img->blue[i]);
        }
        free(aux_img->red);
        free(aux_img->green);
        free(aux_img->blue);
        free(aux_img);
    }
    free(nodes);
    free(images);
}

int main(int argc, char *argv[]) {
    int factor=0;
    uint32_t nr=0;
    if(strcmp(argv[1],"-c")==0)
    {
        uint32_t frunze=0;
        QuadtreeNode **CompressedTree=NULL;
        for(int i=0;i< strlen(argv[2]);i++)
            factor=factor*10+argv[2][i]-'0';
        FILE *input=fopen(argv[3], "rb");
        FILE *output= fopen(argv[4],"wb");
        compress(factor,&nr,&frunze, input, &CompressedTree);
        fwrite(&frunze,sizeof(uint32_t),1,output);
        fwrite(&nr,sizeof(uint32_t),1,output);
        for(int i=0;i<nr;i++)
            fwrite(CompressedTree[i],sizeof(QuadtreeNode),1,output);
        for(int i=0;i<nr;i++)
            free(CompressedTree[i]);
        free(CompressedTree);
        fclose(output);
        fclose(input);
    }
    if(strcmp(argv[1], "-d")==0)
    {
        FILE *input=fopen(argv[2], "rb");
        FILE *output= fopen(argv[3],"wb");
        uint32_t frunze;
        fread(&frunze, sizeof(uint32_t),1,input);
        fread(&nr, sizeof(uint32_t),1,input);
        QuadtreeNode **CompressedTree= malloc((nr)* sizeof(QuadtreeNode*));
        for(int i=0;i<nr;i++)
        {
            QuadtreeNode *aux= malloc(sizeof(QuadtreeNode));
            fread(aux, sizeof(QuadtreeNode),1,input);
            CompressedTree[i]=aux;
        }
        decompress(CompressedTree,output);
        for(int i=0;i<nr;i++)
            free(CompressedTree[i]);
        free(CompressedTree);
        fclose(output);
        fclose(input);
    }
    if(strcmp(argv[1],"-m")==0)
    {
        for(int i=0;i< strlen(argv[3]);i++)
            factor=factor*10+argv[3][i]-'0';
        FILE *input=fopen(argv[4], "rb");
        FILE *output= fopen(argv[5],"wb");
        uint32_t frunze=0;
        QuadtreeNode **CompressedTree=NULL;
        compress(factor,&nr,&frunze, input, &CompressedTree);
        mirror(*argv[2],nr,CompressedTree);
        decompress(CompressedTree,output);
        for(int i=0;i<nr;i++)
            free(CompressedTree[i]);
        free(CompressedTree);
        fclose(output);
        fclose(input);
    }
    return 0;
}
