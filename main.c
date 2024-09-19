#include <stdio.h>
#include <stdlib.h>

typedef struct arvore
{
    int num;
    struct arvore *esq;
    struct arvore *dir;
    struct arvore *pai;
    int fb;
}arvore;

typedef struct avl
{
    arvore *raiz;
    int altura;
}avl;

avl *cria_arvore()
{
    avl *raiz = (avl *)malloc(sizeof(avl));
    raiz->altura = 0;
    raiz->raiz = NULL;
    return raiz;
}

arvore *cria_no(int num)
{
    arvore *novo = (arvore *)malloc(sizeof(arvore));
    novo->dir = NULL;
    novo->esq = NULL;
    novo->pai = NULL;
    novo->fb = 0;
    novo->num = num;
    return novo;
}

arvore *rot_esq(arvore *des)
{
    arvore *filho = des->dir;
    arvore *neto = filho->esq;  
    filho->esq = des;
    des->dir = neto;
    des->fb = 0;
    filho->fb = 0;
    return filho;
}
arvore *rot_dir(arvore *des){
    arvore *filho = des->esq;
    arvore *neto = filho->dir;
    filho->dir = des;
    des->esq = neto;
    des->fb = 0;
    filho->fb = 0;
    return filho;
}
arvore *rot_esqdir(arvore *des){
    arvore *filho = des->esq;
    arvore *neto = filho->dir;
    arvore *nova_raiz = NULL;
    des->esq = rot_esq(filho);
    nova_raiz = rot_dir(des);
    if(neto->fb == 0)
    {
        des->fb = 0;
        filho->fb = 0;
    }
    else if(neto->fb == -1)
    {
        filho->fb = 1;
        des->fb = 0;
        neto->fb = 0;
    }
    else if(neto->fb == 1)
    {
        des->fb = -1;
        filho->fb = 0;
        neto->fb = 0;
    }
    return nova_raiz;
}
arvore *rot_diresq(arvore *des){
    arvore *filho = des->dir;
    arvore *neto = filho->esq;
    arvore *nova_raiz = NULL;
    des->dir = rot_dir(filho);
    nova_raiz = rot_esq(des);
    if(neto->fb == 0)
    {
        des->fb = 0;
        filho->fb = 0;
    }
    else if(neto->fb == -1)
    {
        filho->fb = 1;
        des->fb = 0;
        neto->fb = 0;
    }
    else if(neto->fb == 1)
    {
        des->fb = -1;
        filho->fb = 0;
        neto->fb = 0;
    }
    return nova_raiz;
}

arvore *liga_no(avl *a,int num,arvore **pai)
{
    arvore *aux = a->raiz;
    arvore *cand = a->raiz;
    arvore *novo = cria_no(num);
    *pai = NULL;
    while(aux != NULL)
    {
        if(aux->fb != 0)
        {
            cand = aux;
        }
        *pai = aux;
        if(num < aux->num)
        {
            aux = aux->esq;
        } 
        else
        {
            aux = aux->dir;
        }
    }    
    if(num < (*pai)->num)
    {
        (*pai)->esq = novo;
    }
    else
    {
        (*pai)->dir = novo;
    }
    return cand;
}

void recalculo_fb(arvore *cand,int num)
{
    while(cand->num != num)
    {
        if(num < cand->num)
        {
            cand->fb++;
            cand = cand->esq;
        }        
        else
        {
            cand->fb--;
            cand = cand->dir;
        }
    }
}

arvore *rotacao_geral(arvore *des)
{
    arvore *nova_raiz = NULL;
    if(des->fb == -2)
    {
        if(des->dir->fb == -1)
        {
            nova_raiz = rot_esq(des);
        }
        else
        {
            nova_raiz = rot_esqdir(des);
        }
    }
    else
    {
        if(des->esq->fb == 1)
        {
            nova_raiz = rot_dir(des);
        }
        else
        {
            nova_raiz = rot_diresq(des);
        }
    }
    return nova_raiz;
}

avl *insercao (avl *a,int chave)
{
    arvore *aux = a->raiz;
    arvore *novo = cria_no(chave);
    arvore *pai = NULL;
    if(a->raiz == NULL)
    {
        a->raiz = novo;
    }
    else
    {
        aux = liga_no(a,chave,&pai);
        recalculo_fb(aux,chave);
        if(aux->fb == -2 || aux->fb == 2)
        {
            if(a->raiz == aux)
            {
                a->raiz = rotacao_geral(aux);
            }
            else if(pai->esq == aux)
            {
                pai->esq = rotacao_geral(aux);
            }
            else
            {
                pai->dir = rotacao_geral(aux);
            }
        }
    }
    return a;
}

void em_ordem(arvore *raiz)
{    
    if(raiz != NULL)
    {
        em_ordem(raiz->esq);
        printf("%i ",raiz->num);
        em_ordem(raiz->dir);
    }
}

int main(void)
{    
    avl *raiz = cria_arvore();
    raiz = insercao(raiz,70);
    raiz = insercao(raiz,55);
    raiz = insercao(raiz,48);
    raiz = insercao(raiz,42);
    raiz = insercao(raiz,36);
    raiz = insercao(raiz,76);
    em_ordem(raiz->raiz);
    printf("\n");
    return 0;
}