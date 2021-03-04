#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//huffman baslangic
 struct dugum{
    char harf;
    int frekans;
    struct dugum *sol,*sag;

};

 struct agac {
    int alan;
    int boyut;
    struct dugum **dugumler;
};

void siralama(struct agac *agac1) // dugumleri siralar
{
    int i,j;

    int maxIndex,maxFrekans;
    struct agac *temp;
    for (i =0;i<agac1->boyut-1;i++)
    {
        maxIndex=i;
        maxFrekans=agac1->dugumler[i]->frekans;

        for (j=i+1;j<agac1->boyut;j++)
        {
            if (agac1->dugumler[j]->frekans>maxFrekans)
            {
                maxFrekans=agac1->dugumler[j]->frekans;
                maxIndex=j;

            }
        }

        if (maxIndex!=i)
        {

            temp=agac1->dugumler[maxIndex];
            agac1->dugumler[maxIndex]=agac1->dugumler[i];
            agac1->dugumler[i]=temp;
        }

    }
}

struct dugum *yeniDugum(char harf, int frekans) // yeni dugum olusturur
{
    struct dugum *temp = malloc(sizeof(struct dugum));
    temp->harf = harf;
    temp->frekans = frekans;
    temp->sol = NULL;
    temp->sag = NULL;
    return temp;
}

struct agac *yeniAgac() // yeni agac olusturur
{
    struct agac *agac1 = malloc(sizeof(struct agac));
    agac1->alan=8;
    agac1->boyut=0;
    agac1->dugumler=malloc(agac1->alan*sizeof(struct dugum ));
    return agac1;
}
void *dugumEkle(struct agac *agac1, struct dugum *temp)  //dugum ekler
{
    if (agac1->boyut+1==agac1->alan)
    {
        agac1->alan*= 2;
        agac1->dugumler=realloc(agac1->dugumler,agac1->alan*sizeof(struct dugum ));
    }

    agac1->dugumler[agac1->boyut]=temp;
    agac1->boyut++;

}


struct dugum *dugumSil(struct agac *agac1)  // agac olusturulurken dusuk frekanslý dugumleri siler
{
    agac1->boyut--;
    return agac1->dugumler[agac1->boyut];
}

void kodBul(struct dugum *temp, char *kod, int index,int *encodedBoyut)  // karakterlerin huffman kodlarý burada bulunur
{


    int fk;
    int *tut=&encodedBoyut;
    int boyut=*encodedBoyut;


    if (temp->sol==NULL&&temp->sag==NULL)
    {
        kod[index] = '\0';
        //kodlari gosteren satir
       // printf("\n->%c karakteri: Kod: %s Frekans: %d",temp->harf,kod,temp->frekans);
        fk=strlen(kod)*temp->frekans;
        boyut=boyut+fk;
        *encodedBoyut=boyut;



    }
    if (temp->sol!=NULL)
    {
        kod[index++] = '0';
        kodBul(temp->sol,kod,index,*tut);
        index--;
    }
    if (temp->sag!=NULL)
    {
        kod[index++] = '1';
        kodBul(temp->sag,kod,index,*tut);
        index--;
    }



}
//huffman son


//lzss baslangic
struct lzssDugum {
    uint8_t jump;
    uint8_t lengtRlet;

};


int lzssBenzerlikBul(int a,int b,char *metin)  // tekrar sayisini bulur
{
    int uzunluk = 0;


    while(metin[a]==metin[b]){

        uzunluk++;
        a++;
        b++;


    }
    if(uzunluk<3) uzunluk=0;

    return uzunluk;
}

//encode islemi burada yapilir
struct lzssDugum *lzssCompression(char *metin,int boyut,int *dugumAdedi){
    int hafiza=8;
    struct lzssDugum temp;
    int dugumSayisi=-1;
    int searchBuffer,islenmemis=0;
    struct lzssDugum *encoded=malloc(hafiza*sizeof(struct lzssDugum));
    while(islenmemis<boyut){


        int maxEslesme=0;
        int konum=0;

        searchBuffer=islenmemis-255;

        if(searchBuffer<0) searchBuffer=0;


        while(searchBuffer<islenmemis)
        {

            int iTut=islenmemis;
            int uzunluk=lzssBenzerlikBul(searchBuffer,iTut,metin);


           if(uzunluk==0){

           searchBuffer++;

           }

           else if(uzunluk!=0){


                if(uzunluk>maxEslesme)
                {

                    maxEslesme=uzunluk;
                    konum=searchBuffer;

                }

                searchBuffer=searchBuffer+uzunluk;


            }


        }

        if (dugumSayisi + 1 > hafiza)
        {
            hafiza = hafiza*2;
            encoded = realloc(encoded,hafiza * sizeof(struct lzssDugum));
        }

        if(maxEslesme==0){

            temp.jump=0;
            temp.lengtRlet=(int)metin[islenmemis];


        }

        if(maxEslesme!=0){

            temp.jump=islenmemis-konum;
            temp.lengtRlet=maxEslesme;
            islenmemis=islenmemis+maxEslesme;

        }
        dugumSayisi++;
        encoded[dugumSayisi]=temp;
        islenmemis++;
/*  Token icerigini gosteren kod
        printf("\n  -%d.Dugumumuz:",dugumSayisi);

           printf("\n !!imlecin geri gitme miktari: %d",encoded[dugumSayisi].jump);
           if(encoded[dugumSayisi].jump==0){
            printf("\n  !!Dugumde tutulan karakter: %c",encoded[dugumSayisi].lengtRlet);
           }
           else{
            printf("\n    !!Tekrar uzunlugu %d",encoded[dugumSayisi].lengtRlet);
           }
        printf("\n-----------------------------");
  */
    }





    *dugumAdedi=dugumSayisi;
    return encoded;

}

//lzss son

//lz77 baslangic
struct lz77Dugum {
    uint8_t jump;
    uint8_t length;
    char harf;
};

int lz77BenzerlikBul(int a,int b,char *metin) //tekrar sayisini bulur
{
    int uzunluk = 0;



    while(metin[a]==metin[b]){

        uzunluk++;
        a++;
        b++;


    }

    return uzunluk;
}

// encode islemi burada yapilir
struct lz77Dugum *lz77compression(char *metin,int boyut,int *lz77DugumAdedi){
    int hafiza=8;
    struct lz77Dugum temp;
    int lz77DugumSayisi=-1;
    int searchBuffer,islenmemis=0;
    struct lz77Dugum *encoded=malloc(hafiza*sizeof(struct lz77Dugum));
    while(islenmemis<boyut){


        int maxEslesme=0;
        int konum=0;

        searchBuffer=islenmemis-255;

        if(searchBuffer<0) searchBuffer=0;


        while(searchBuffer<islenmemis)
        {

            int iTut=islenmemis;
            int uzunluk=lz77BenzerlikBul(searchBuffer,iTut,metin);


           if(uzunluk==0){

           searchBuffer++;

           }

           else if(uzunluk!=0){


                if(uzunluk>maxEslesme)
                {

                    maxEslesme=uzunluk;
                    konum=searchBuffer;

                }

                searchBuffer=searchBuffer+uzunluk;


            }


        }

         // gerekirse, hafizada yer aç
        if (lz77DugumSayisi + 1 > hafiza)
        {
            hafiza = hafiza*2;
            encoded = realloc(encoded,hafiza * sizeof(struct lz77Dugum));
        }

        if(maxEslesme==0){

            temp.jump=0;
            temp.length=0;
            temp.harf=metin[islenmemis];

        }

        if(maxEslesme!=0){

            temp.jump=islenmemis-konum;
            temp.length=maxEslesme;
            temp.harf=metin[islenmemis+maxEslesme];
            islenmemis=islenmemis+maxEslesme;

        }
        lz77DugumSayisi++;
        encoded[lz77DugumSayisi]=temp;
        islenmemis++;
        /*
        //Token icerigini gosteren kod
        printf("\n***** %d.Tokenimiz:**********************",lz77DugumSayisi);
           printf("\n jump->%d",encoded[lz77DugumSayisi].jump);
         printf(" tekrar->%d",encoded[lz77DugumSayisi].length);
          printf(" harf->%c",encoded[lz77DugumSayisi].harf);
        printf("\n***********************************");
      */
    }



    *lz77DugumAdedi=lz77DugumSayisi;
    return encoded;

}
//lz77 Sonu
char *dosyaOku(FILE *f, int *boyut) //dosya okuma islemini gerceklestirir
{
    char *metin;
    fseek(f,0,SEEK_END); //imleci dosya sonuna koyuyoruz
    *boyut=ftell(f); //imlecin oldugu konumu boyut'a aktardik.(boyutu bulmus olduk)
    metin=malloc(*boyut); //bellek ayirma islemi gerceklestirdik.
    rewind(f);
    fread(metin,1,*boyut,f); // content(tampon bellek)'e metnin tamamini attik
    *(metin+*boyut)='\0'; //metin sonuna sonlandirma karakteri ekledik
    return metin; //metnin tamamini donmus olduk
}

int main(void)
{
    //Huffman baslangic
    int sayac=0;
    int metinBoyutu=0;
    char *metin;

    int frekans[255];

    FILE *f;
    char *imlec;
    int i;
     if(f=fopen("source.txt","rb"))
    {
        metin = dosyaOku(f,&metinBoyutu);
        fclose(f);
    }



    struct agac *agac1 =yeniAgac();


    for(i=0;i<255;i++){
        frekans[i]=0;
    }

    for (imlec=&metin[0];*imlec!=0;imlec++)
    {
        frekans[(int)*imlec]++;

    }

    for (i=0;i<255;i++)
    {
        if (frekans[i]>0)
        {
            dugumEkle(agac1,yeniDugum(i,frekans[i]));

        }
    }

    while (agac1->boyut>1)
    {
        siralama(agac1);
        struct dugum *sl = dugumSil(agac1);
        struct dugum *sg = dugumSil(agac1);
        struct dugum *anaDugum =malloc(sizeof(struct dugum));
        anaDugum->sol=sl;
        anaDugum->sag=sg;
        int toplam=sl->frekans+sg->frekans;
        anaDugum->frekans=toplam;
        dugumEkle(agac1,anaDugum);

    }


    char *kodBuf = malloc(256);
    int encodedBoyut=0;

    printf("\n------------------------------------------------");
     printf("\n\n HUFFMAN Inceleme:");
    kodBul(agac1->dugumler[0],kodBuf,0,&encodedBoyut);
    float esasBoyut=(float)metinBoyutu*8;
    float yuzde=(((float)metinBoyutu*8-(float)encodedBoyut)*100.0)/esasBoyut;


    printf("\n \n *Huffman Agaci:");
    printf("\n   ->Txt'nin normal boyutu %d bit",metinBoyutu*8);
    printf("\n   ->Huffman kodlamasi sonrasi boyutu %d bit",encodedBoyut);
    printf("\n   ->Metin %%%.1f oraninda kuculmustur.",yuzde);

    //huffman sonu


    //lzss baslangic
    printf("\n-----------------------------------------------");
    printf("\n\n LZSS inceleme");
    int lzssMetin_boyutu=0, lzssToken_sayisi=0;
    char *lzssKaynak_metin ;
    struct lzssDugum *lzssEncoded_metin;

    if(f=fopen("source.txt","rb"))
    {
        lzssKaynak_metin = dosyaOku(f, &lzssMetin_boyutu);
        fclose(f);
    }



    lzssEncoded_metin = lzssCompression(lzssKaynak_metin, lzssMetin_boyutu, &lzssToken_sayisi); // encoded metinin adresini aliyoruz
    printf("\n -->token adedi %d",lzssToken_sayisi);

    if(f=fopen("lzss.txt", "wb"))
    {
        fwrite(lzssEncoded_metin, sizeof(struct lzssDugum), lzssToken_sayisi, f);
        fclose(f);
    }

    printf("\nLZSS sonucu :");
    printf("\nOrijinal Boyut: %d kb, Kucultulmus Boyutu: %d kb",lzssMetin_boyutu, lzssToken_sayisi * sizeof(struct lzssDugum));
    printf("\n-----------------------------------------------");
    // lzss sonu




    //lz77 baslangic
    printf("\n LZ77 Inceleme");
    int lz77Metin_boyutu=0, lz77Token_sayisi=0;
    char *lz77Kaynak_metin ;
    struct lz77Dugum *lz77Encoded_metin;

    if(f=fopen("source.txt","rb"))
    {
        lz77Kaynak_metin = dosyaOku(f, &lz77Metin_boyutu);
        fclose(f);
    }



    lz77Encoded_metin = lz77compression(lz77Kaynak_metin,lz77Metin_boyutu, &lz77Token_sayisi); // encoded metnin adresini aliyoruz
    printf("\n -->token adedi %d",lz77Token_sayisi);

    if(f=fopen("lz77.txt", "wb"))
    {
        fwrite(lz77Encoded_metin, sizeof(struct lz77Dugum),lz77Token_sayisi, f);
        fclose(f);
    }

    printf("\n\n LZ77 Sonuclari:");
    printf("\nOrijinal Boyut: %d kb, Kucultulmus Boyutu: %d kb",lz77Metin_boyutu,lz77Token_sayisi * sizeof(struct lz77Dugum));
   printf("\n-----------------------------------------------");



    //lz77 sonu




    return 0;
}
