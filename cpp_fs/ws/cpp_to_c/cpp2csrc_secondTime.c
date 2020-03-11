#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

#define NUM_PUBTRAN_FUNCS 2
#define NUM_MINIBUS_FUNCS 3
#define NUM_TAXI_FUNCS 2
#define NUM_STAXI_FUNCS 2
#define NUM_PublicConvoy_FUNCS 2

#define max_func(x, y) (x) > (y) ? (x) : (y)

static int s_count = 0;

typedef void (*vptr_func)();

static vptr_func pubtran_vtable[NUM_PUBTRAN_FUNCS];
static vptr_func mini_vtable[NUM_MINIBUS_FUNCS];
static vptr_func taxi_vtable[NUM_TAXI_FUNCS];
static vptr_func staxi_vtable[NUM_STAXI_FUNCS];
static vptr_func PublicConvoy_vtable[NUM_PublicConvoy_FUNCS];

enum
{
    DTOR = 0,
    DISPLAY = 1,
    WASH = 2
};

struct PublicTransport
{
    vptr_func *vptr;
    int m_license_plate;
};

struct Minibus
{
    struct PublicTransport pT;
    int m_numSeats;
};

struct Taxi
{
    struct PublicTransport pT;
};

struct SpecialTaxi
{
    struct Taxi tx;
};

struct PublicConvoy
{
    struct PublicTransport pT;
    struct PublicTransport *m_pt1;
    struct PublicTransport *m_pt2;
    struct Minibus m_m;
    struct Taxi m_t;
};

/* PublicTransport */ 
void PublicTransport_ctor(struct PublicTransport *pt);
void PublicTransport_dtor(struct PublicTransport *pt);
void PublicTransport_cctor(struct PublicTransport *this,
                            struct PublicTransport *other);
void Pt_display();
void UpdatePtVtable();
static void pb_print_count();
int Pt_get_Id(struct PublicTransport *pT);
/**/

/* Minibus */
void Minibus_ctor(struct Minibus *mini);
void Minibus_dtor(struct Minibus *mini);
void Minibus_cctor(struct Minibus *this,
                   struct Minibus *other);
void Mini_display();
void Mini_wash(struct Minibus *mini, int minutes);
void UpdateMiniVtable();
void print_info_mini(struct Minibus *m);
/**/

/* Taxi */
void Taxi_ctor(struct Taxi *tx);
void Taxi_dtor(struct Taxi *tx);
void Taxi_cctor(struct Taxi *this, struct Taxi *other);
void Taxi_display(struct Taxi *tx);
void UpdateTaxiVtable();
/**/

/* Special Taxi */
void UpdateSpecialTaxiVtable();
void SpecialTaxi_ctor(struct SpecialTaxi *stx);
void SpecialTaxi_cctor(struct SpecialTaxi *this, struct SpecialTaxi *other);
void SpecialTaxi_dtor(struct SpecialTaxi *stx);
void SpecialTaxi_display(struct SpecialTaxi *stx);

/* Public convoy */
void UpdatePublicConvoyVtable();
/**/

/* general */
void print_info_i(struct PublicTransport *pT, int i);
void print_info_pt(struct PublicTransport *a);
/**/

/* public transport methods */
void PublicTransport_ctor(struct PublicTransport *pt)
{
    ++s_count;
    pt->m_license_plate = s_count;
    pt->vptr = pubtran_vtable;
    UpdatePtVtable();

    printf("PublicTransport::Ctor()%d\n", pt->m_license_plate);
}

void PublicTransport_dtor(struct PublicTransport *pt)
{
    --s_count;
    printf("PublicTransport::Dtor()%d\n", pt->m_license_plate);
}

void PublicTransport_cctor(struct PublicTransport *this,
                            struct PublicTransport *other)
{
    (void)other;
    ++s_count;
    this->m_license_plate = s_count;
 /*   UpdatePtVtable();*/
    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void Pt_display(struct PublicTransport *pT)
{
    printf("PublicTransport::display(): %d\n", pT->m_license_plate);
}

int Pt_get_Id(struct PublicTransport *pT)
{
    return pT->m_license_plate;
}

/**/
static void pb_print_count()
{
    printf("s_count: %d\n", s_count);
}

/*minibus methods*/
void Minibus_ctor(struct Minibus *mini)
{
    PublicTransport_ctor(&mini->pT);
    UpdateMiniVtable();
    mini->pT.vptr = mini_vtable;
    mini->m_numSeats = 20;

    printf("Minibus::Ctor()\n");
}

void Minibus_dtor(struct Minibus *mini)
{
    printf("Minibus::Dtor()\n");
    mini->pT.vptr = pubtran_vtable;
    PublicTransport_dtor(&mini->pT);
}

void Minibus_cctor(struct Minibus *this,
                   struct Minibus *other)
{
    PublicTransport_cctor(&this->pT, &other->pT);
    this->m_numSeats = other->m_numSeats;

    this->pT.vptr = mini_vtable;
    printf("Minibus::CCtor()\n");
}

void Mini_display(struct Minibus *mini)
{
    printf("Minibus::display() ID:%d" , Pt_get_Id(&mini->pT));
    printf(" num seats:%d\n", mini->m_numSeats);
}

void Mini_wash(struct Minibus *mini, int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n", minutes, Pt_get_Id(&mini->pT));
}
/**/

/* Taxi methods */ 
void Taxi_ctor(struct Taxi *tx)
{
    PublicTransport_ctor(&tx->pT);
    UpdateTaxiVtable();
    tx->pT.vptr = taxi_vtable;

    printf("Taxi::Ctor()\n");
}

void Taxi_dtor(struct Taxi *tx)
{
    printf("Taxi::Dtor()\n");

    tx->pT.vptr = pubtran_vtable;
    PublicTransport_dtor(&tx->pT);
}

void Taxi_cctor(struct Taxi *this, struct Taxi *other)
{
    PublicTransport_cctor(&this->pT, &other->pT);
    this->pT.vptr = taxi_vtable;

    printf("Taxi::CCtor()\n");
}

void Taxi_display(struct Taxi *tx)
{
    printf("Taxi::display() ID:%d\n", Pt_get_Id(&tx->pT));
}
/**/

/* special Taxi methods */ 
void SpecialTaxi_ctor(struct SpecialTaxi *stx)
{
    Taxi_ctor(&stx->tx);
    UpdateSpecialTaxiVtable();
    stx->tx.pT.vptr = staxi_vtable;

    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxi_dtor(struct SpecialTaxi *stx)
{
    printf("SpecialTaxi::Dtor()\n");

    stx->tx.pT.vptr = taxi_vtable;
    Taxi_dtor(&stx->tx);
}

void SpecialTaxi_cctor(struct SpecialTaxi *this, struct SpecialTaxi *other)
{
    Taxi_cctor(&this->tx, &other->tx);
    this->tx.pT.vptr = staxi_vtable;

    printf("SpecialTaxi::CCtor()\n");
}

void SpecialTaxi_display(struct SpecialTaxi *stx)
{
    printf("SpecialTaxi::display() ID:%d\n", Pt_get_Id(&stx->tx.pT));
}
/**/
/* public convoy methods */
void PublicConvoy_ctor(struct PublicConvoy *pc)
{
    UpdatePublicConvoyVtable();
    
    PublicTransport_ctor(&pc->pT);
    pc->pT.vptr = PublicConvoy_vtable;

    pc->m_pt1 = (struct PublicTransport *)malloc(sizeof(struct Minibus));
	Minibus_ctor((struct Minibus *)pc->m_pt1);
	pc->m_pt2 = (struct PublicTransport *)malloc(sizeof(struct Taxi));
	Taxi_ctor((struct Taxi *)pc->m_pt2);
  
    Minibus_ctor(&pc->m_m);
    Taxi_ctor(&pc->m_t);
}

void PublicConvoy_dtor(struct PublicConvoy *pc)
{
    Minibus_dtor((struct Minibus *)pc->m_pt1);
	free(pc->m_pt1);

	Taxi_dtor((struct Taxi *)pc->m_pt2);
	free(pc->m_pt2);
 /*   PublicTransport_dtor(pc->m_pt1);
    free(pc->m_pt1);

    PublicTransport_dtor(pc->m_pt2);
    free(pc->m_pt2);
 */   
    Taxi_dtor(&pc->m_t);
    Minibus_dtor(&pc->m_m);

    pc->pT.vptr = pubtran_vtable;
    PublicTransport_dtor(&pc->pT);
}

void PublicConvoy_cctor(struct PublicConvoy *this, struct PublicConvoy *other)
{
    PublicTransport_cctor(&this->pT, &other->pT);
    this->pT.vptr = PublicConvoy_vtable;

    this->m_pt1 = (struct PublicTransport *)malloc(sizeof(struct Minibus));
    Minibus_cctor((struct Minibus *)this->m_pt1, (struct Minibus *)other->m_pt1);

    this->m_pt2 = (struct PublicTransport *)malloc(sizeof(struct Taxi));
    Taxi_cctor((struct Taxi *)this->m_pt2, (struct Taxi *)other->m_pt2);

    Minibus_cctor(&this->m_m, &other->m_m);
    Taxi_cctor(&this->m_t, &other->m_t);
}

void PublicConvoy_display(struct PublicConvoy *pc)
{
    pc->m_pt1->vptr[DISPLAY](pc->m_pt1);
    pc->m_pt2->vptr[DISPLAY](pc->m_pt2);
    Mini_display(&pc->m_m);
    Taxi_display(&pc->m_t);
}


/**/
static void taxi_display(struct Taxi s)
{
    Taxi_display(&s);
}

void UpdatePtVtable()
{
    pubtran_vtable[0] = PublicTransport_dtor;
    pubtran_vtable[1] = Pt_display;
}

void UpdateMiniVtable()
{
    mini_vtable[0] = Minibus_dtor;
    mini_vtable[1] = Mini_display;
    mini_vtable[2] = Mini_wash;
}

void UpdateTaxiVtable()
{
    taxi_vtable[0] = Taxi_dtor;
    taxi_vtable[1] = Taxi_display;
}

void UpdateSpecialTaxiVtable()
{
    staxi_vtable[0] = SpecialTaxi_dtor;
    staxi_vtable[1] = SpecialTaxi_display;
}

void UpdatePublicConvoyVtable()
{
    PublicConvoy_vtable[0] = PublicConvoy_dtor;
    PublicConvoy_vtable[1] = PublicConvoy_display;
}

void print_info_mini(struct Minibus *m)
{
    Mini_wash(m, 3);
}

void print_info_i(struct PublicTransport *pT, int i)
{
    struct Minibus ret;
    Minibus_ctor(&ret);
    printf("print_info(int i)\n");
    Mini_display(&ret);
    PublicTransport_cctor(pT, (struct PublicTransport *)&ret);
    Minibus_dtor(&ret);
    (void)i;
}

void print_info_pt(struct PublicTransport *a)
{
    Pt_display(a);
}

int main()
{
    struct Minibus m;
    struct PublicTransport pt;

    int i = 0;
    struct Minibus *m1 = (struct Minibus *)malloc(sizeof(struct Minibus));
    struct Taxi *tx = (struct Taxi *)malloc(sizeof(struct Taxi));
    struct Minibus *m3 = (struct Minibus *)malloc(sizeof(struct Minibus));
    struct PublicTransport *array[3];

    struct Minibus mini_arr0;
    struct Taxi taxi_arr1;
    struct PublicTransport pt_arr0;
    struct PublicTransport pt_arr1;
    struct PublicTransport pt_arr2;
    struct PublicTransport arr2[3];

    struct Minibus m2;

    struct Minibus arr3[4];
    struct Taxi *arr4[4];

    struct SpecialTaxi st;
    struct Taxi txForst;

    struct PublicConvoy *ts1;
    struct PublicConvoy *ts2;

    Minibus_ctor(&m);
    print_info_mini(&m);
    print_info_i(&pt, 3);
    Pt_display(&pt);
    PublicTransport_dtor(&pt);

    Minibus_ctor(m1);
    array[0] = (struct PublicTransport *)m1;
    Taxi_ctor(tx);
    array[1] = (struct PublicTransport *)tx;
    Minibus_ctor(m3);
    array[2] = (struct PublicTransport *)m3;

    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr[DISPLAY](array[i]);
    }

    Minibus_dtor(m1);
    Taxi_dtor(tx);
    Minibus_dtor(m3);

    free(m1);
    free(tx);
    free(m3);

    Minibus_ctor(&mini_arr0);
    PublicTransport_cctor(&pt_arr0, (struct PublicTransport *)&mini_arr0);
    Minibus_dtor(&mini_arr0);
    Taxi_ctor(&taxi_arr1);
    PublicTransport_cctor(&pt_arr1, (struct PublicTransport *)&taxi_arr1);
    Taxi_dtor(&taxi_arr1);
    PublicTransport_ctor(&pt_arr2);
    arr2[0] = pt_arr0;
    arr2[1] = pt_arr1;
    arr2[2] = pt_arr2;

    for (i = 0; i < 3; ++i)
    {
        Pt_display(&arr2[i]);
    }

    print_info_pt(&arr2[0]);
    pb_print_count();

    Minibus_ctor(&m2);
    pb_print_count();

    for (i = 0; i < 4; ++i)
    {
        Minibus_ctor(&arr3[i]);
    }

    for (i = 0; i < 4; ++i)
    {
        arr4[i] = (struct Taxi *)malloc(4 * sizeof(struct Taxi));
        Taxi_ctor(arr4[i]);
    }

    for (i = 3; i >= 0; --i)
    {
        Taxi_dtor(arr4[i]);
        free(arr4[i]);
    }
    
    printf("%d\n", max_func(1, 2));
    printf("%d\n", max_func(1, (int)2.0f));

    SpecialTaxi_ctor(&st);
    Taxi_cctor(&txForst, (struct Taxi *)&st);
    taxi_display(txForst);
    Taxi_dtor(&txForst);

    ts1 = (struct PublicConvoy *)malloc(sizeof(struct PublicConvoy));
    PublicConvoy_ctor(ts1);
    ts2 = (struct PublicConvoy *)malloc(sizeof(struct PublicConvoy));
    PublicConvoy_cctor(ts2, ts1);

    PublicConvoy_display(ts1);
    PublicConvoy_display(ts2);

   ts2->pT.vptr[DTOR](ts1);
    free(ts1);
    PublicConvoy_display(ts2);

    ts2->pT.vptr[DTOR](ts2);
    free(ts2);

	SpecialTaxi_dtor(&st);
	for (i = 3; i >= 0; --i)
	{
		Minibus_dtor(&arr3[i]);
	}

	Minibus_dtor(&m2);
	PublicTransport_dtor(&pt_arr2);
	PublicTransport_dtor(&pt_arr1);
	PublicTransport_dtor(&pt_arr0);
	Minibus_dtor(&m);


    return 0;
}
