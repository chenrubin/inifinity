#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

#define NUM_V_FUNC_PT 2
#define NUM_V_FUNC_MINI 3
#define NUM_V_FUNC_TAXI 2
#define NUM_V_FUNC_STAXI 2
#define NUM_V_FUNC_PUBCON 2

typedef void (*vptr_func)();

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
	int numSeats;
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

void PublicTransport_ctor(struct PublicTransport *pubTran);
void PublicTransport_dtor(struct PublicTransport *pubTran);
void PublicTransport_cctor(struct PublicTransport *pubTran,
						   struct PublicTransport *res);
void display_Pt(struct PublicTransport *pubTran);
void display_staxi(struct SpecialTaxi *stx);
void display_taxi(struct Taxi *taxi);
void display_pc(struct PublicConvoy *pc);
static void print_count_Pt();
int get_ID(struct PublicTransport *pubTran);
void Minibus_ctor(struct Minibus *mini);
void Minibus_dtor(struct Minibus *mini);
void Minibus_cctor(struct Minibus *mini, struct Minibus *mini_res);
void Display_Mini(struct Minibus *mini);
void Wash_Mini(struct Minibus *mini, int minutes);

void print_info_mini(struct Minibus *m);
struct PublicTransport print_info_int(int i);
void print_info_ptRef(struct PublicTransport *pt);
void taxi_display(struct Taxi s);

/* Minibus virtuals*/
typedef void (*func2)(struct Minibus *, int); /* wash*/


static int s_count = 0;
static void (*pt_vtable[NUM_V_FUNC_PT])() = {NULL}; /* moran amra*/
static void (*mini_vtable[NUM_V_FUNC_MINI])() = {NULL}; /* shaddad amar*/
static void (*taxi_vtable[NUM_V_FUNC_TAXI])() = {NULL}; /* jesus*/
static void (*specialtaxi_vtable[NUM_V_FUNC_STAXI])() = {NULL}; /* jesus*/
static void (*publicconvoy_vtable[NUM_V_FUNC_PUBCON])() = {NULL}; /* jesus*/

/* PublicTransport Ctor	*/
void PublicTransport_ctor(struct PublicTransport *pubTran)
{
	pubTran->vptr = pt_vtable;
	pt_vtable[0] = PublicTransport_dtor;
	pt_vtable[1] = display_Pt;
	++s_count;
	pubTran->m_license_plate = s_count;
	printf("PublicTransport::Ctor()%d\n", pubTran->m_license_plate);
}

/* PublicTransport dtor */
void PublicTransport_dtor(struct PublicTransport *pubTran)
{
	--s_count;
	printf("PublicTransport::Dtor()%d\n", pubTran->m_license_plate);
}

/* PublicTransport cctor */
void PublicTransport_cctor(struct PublicTransport *this,
						   struct PublicTransport *other)
{	
	memcpy(this, other, sizeof(struct PublicTransport));
	++s_count;
	this->m_license_plate = s_count;
	printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

int get_ID(struct PublicTransport *pubTran)
{	
	return pubTran->m_license_plate;
}

static void print_count_Pt()
{
	printf("s_count: %d\n", s_count);
}

void display_Pt(struct PublicTransport *pubTran)
{
	printf("PublicTransport::display(): %d\n", pubTran->m_license_plate);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Minibus Ctor */
void Minibus_ctor(struct Minibus *mini)
{
	mini_vtable[0] = PublicTransport_dtor;
	mini_vtable[1] = Display_Mini;
	mini_vtable[2] = Wash_Mini;
	PublicTransport_ctor(&(mini->pT));
	mini->pT.vptr = mini_vtable;
	mini->numSeats = 20;
	printf("Minibus::Ctor()\n");
}

void Minibus_dtor(struct Minibus *mini)
{
	printf("Minibus::Dtor()\n");
	PublicTransport_dtor(&mini->pT);
}

void Minibus_cctor(struct Minibus *this, struct Minibus *other)
{
	PublicTransport_cctor(&this->pT, &other->pT);
	this->numSeats = other->numSeats;
	printf("Minibus::CCtor()\n");
}

void Display_Mini(struct Minibus *mini)
{
    printf("Minibus::display() ID:%d", get_ID(&mini->pT));
	printf(" num seats:%d\n", mini->numSeats);
}

void Wash_Mini(struct Minibus *mini, int minutes)
{
	printf("Minibus::wash(%d) ID:%d\n", minutes,get_ID(&mini->pT));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Taxi_ctor(struct Taxi *taxi)
{
	taxi_vtable[0] = PublicTransport_dtor;
	taxi_vtable[1] = display_taxi;
	PublicTransport_ctor(&(taxi->pT));
	taxi->pT.vptr = taxi_vtable;
	printf("Taxi::Ctor()\n");
}

void Taxi_dtor(struct Taxi *taxi)
{
	printf("Taxi::Dtor()\n");
	PublicTransport_dtor(&taxi->pT);
}

void Taxi_cctor(struct Taxi *this, struct Taxi *other)
{
	PublicTransport_cctor(&this->pT, &other->pT);
	printf("Taxi::CCtor()\n");
}

void display_taxi(struct Taxi *taxi)
{
    printf("Taxi::display() ID:%d\n", get_ID(&taxi->pT));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void SpecialTaxi_ctor(struct SpecialTaxi *this)
{
	specialtaxi_vtable[0] = PublicTransport_dtor;
	specialtaxi_vtable[1] = display_staxi;
	Taxi_ctor(&this->tx);
	this->tx.pT.vptr = specialtaxi_vtable;
	printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxi_dtor(struct SpecialTaxi *this)
{
	printf("SpecialTaxi::Dtor()\n");
	Taxi_dtor(&this->tx);
}

void SpecialTaxi_cctor(struct SpecialTaxi *this, struct SpecialTaxi *other)
{
	Taxi_cctor(&this->tx, &other->tx);
	printf("SpecialTaxi::CCtor()\n");
}

void display_staxi(struct SpecialTaxi *this)
{
    printf("SpecialTaxi::display() ID:%d", get_ID(&this->tx.pT));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Publicconvoy_ctor(struct PublicConvoy *this)
{
	publicconvoy_vtable[0] = PublicTransport_dtor;
	publicconvoy_vtable[1] = display_pc;
	
	PublicTransport_ctor(&(this->pT));
	this->pT.vptr = publicconvoy_vtable;

	this->m_pt1 = (struct PublicTransport *)malloc(sizeof(struct Minibus));
	Minibus_ctor((struct Minibus *)this->m_pt1);
	this->m_pt2 = (struct PublicTransport *)malloc(sizeof(struct Taxi));
	Taxi_ctor((struct Taxi *)this->m_pt2);

	Minibus_ctor(&this->m_m);
	Taxi_ctor(&this->m_t);
}

void Publicconvoy_dtor(struct PublicConvoy *this)
{
	Taxi_dtor(&this->m_t);
	Minibus_dtor(&this->m_m);
	Taxi_dtor((struct Taxi *)&this->m_pt2);
	Minibus_dtor((struct Minibus *)&this->m_pt1);
	PublicTransport_dtor(&this->pT);
}

void Publicconvoy_cctor(struct PublicConvoy *this, struct PublicConvoy *other_)
{
	PublicTransport_ctor(&this->pT);
	
	this->m_pt1 = (struct PublicTransport *)malloc(sizeof(struct Minibus));
	PublicTransport_cctor(this->m_pt1, other_->m_pt1);

	this->m_pt2 = (struct PublicTransport *)malloc(sizeof(struct Taxi));
	PublicTransport_cctor(this->m_pt2, other_->m_pt2);

	Minibus_cctor(&this->m_m,&other_->m_m);
	Taxi_cctor(&this->m_t,&other_->m_t);
}

void display_pc(struct PublicConvoy *this)
{
	this->m_pt1->vptr[DISPLAY](this->m_pt1);
	this->m_pt2->vptr[DISPLAY](this->m_pt2);
	Display_Mini(&this->m_m);
	display_taxi(&this->m_t);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* print_info funcntions */
void print_info_mini(struct Minibus *m)
{
    Wash_Mini(m, 3);
}

struct PublicTransport print_info_int(int i)
{
    struct Minibus ret;
	struct PublicTransport pt;

	Minibus_ctor(&ret);
	printf("print_info(int i)\n");
	Display_Mini(&ret);
	
	PublicTransport_cctor(&pt, &ret.pT);
	Minibus_dtor(&ret);
	
	return pt;
}

void print_info_ptRef(struct PublicTransport *pt)
{
	display_Pt(pt);
}
/* end of print_info functions */

void taxi_display(struct Taxi s)
{
    display_taxi(&s);
}

/* template functions */
int max_func_int(const int *t1, const int *t2)
{
	return (*t1 > *t2 ? *t1 : *t2);
}
/* end of template functions */

int main()
{	
/*	struct PublicTransport pT;
	struct Minibus m;

	/*public transport array*/
/*	int i = 0;
	struct Minibus *m1 = (struct Minibus *)malloc(sizeof(struct Minibus));
	struct Taxi *t2 = (struct Taxi *)malloc(sizeof(struct Taxi));
	struct Minibus *m3 = (struct Minibus *)malloc(sizeof(struct Minibus));
	struct PublicTransport *array[3];
	/**/

	/*public transport arr2*/
/*	struct Minibus m_arr2;
	struct PublicTransport m_arr2_pt;
	struct Taxi tx_arr2;
	struct PublicTransport tx_arr2_pt;
	struct PublicTransport pt_arr2;
	struct PublicTransport arr2[3];
	/**/
	
	/**/
/*	struct Minibus m2;
	/**/

	/**/
/*	struct Minibus arr3[4];
	struct Taxi *arr4[4] = {NULL};
	/**/

	/**/
/*	int max_int1 = 1;
	int max_int2 = 2;
	float max_int_float = 2.0;
	/**/

	/**/
/*	struct SpecialTaxi stx;
	struct Taxi tx_from_stx;
	/**/

	/**/
	struct PublicConvoy *ts1;
	struct PublicConvoy *ts2;
	/**/

/*	Minibus_ctor(&m);
	print_info_mini(&m);
	pT = print_info_int(3);
	display_Pt(&pT);
	PublicTransport_dtor(&pT);

	Minibus_ctor(m1);
	Taxi_ctor(t2);
	Minibus_ctor(m3);
	array[0] = (struct PublicTransport *)m1;
	array[1] = (struct PublicTransport *)t2; 
	array[2] = (struct PublicTransport *)m3;

	for (i = 0; i < 3; ++i)
	{
		array[i]->vptr[DISPLAY](array[i]);
	}

	Minibus_dtor(m1);
	Taxi_dtor(t2);
	Minibus_dtor(m3);

	free(m1);
	free(t2);
	free(m3);

	Minibus_ctor(&m_arr2);
	PublicTransport_cctor(&m_arr2_pt, (struct PublicTransport *)&m_arr2);
	m_arr2_pt.vptr = pt_vtable;
	Minibus_dtor(&m_arr2);
	arr2[0] = m_arr2_pt;
	Taxi_ctor(&tx_arr2);
	PublicTransport_cctor(&tx_arr2_pt, (struct PublicTransport *)&tx_arr2);
	tx_arr2_pt.vptr = pt_vtable;
	Taxi_dtor(&tx_arr2);
	arr2[1] = tx_arr2_pt; 
	PublicTransport_ctor(&pt_arr2);
	arr2[2] = pt_arr2;

	for (i = 0; i < 3; ++i)
	{
		arr2[i].vptr[DISPLAY](&arr2[i]);
	}
	print_info_ptRef(&arr2[0]);

	print_count_Pt();
	Minibus_ctor(&m2);
	print_count_Pt();

	for (i = 0; i < 4; ++i)
	{
		Minibus_ctor(&arr3[i]);
	}

	for (i = 0; i < 4; ++i)
	{
		arr4[i] = (struct Taxi *)malloc(sizeof(struct Taxi));
		Taxi_ctor(arr4[i]);
	}

	for (i = 3; i >= 0; --i)
	{
		free(arr4[i]);
		Taxi_dtor(arr4[i]);
	}

	printf("%d\n", max_func_int(&max_int1, &max_int2));
	printf("%d\n", max_func_int(&max_int1,(const int *)&max_int_float));

	SpecialTaxi_ctor(&stx);
	Taxi_cctor(&tx_from_stx, (struct Taxi *)&stx);
	taxi_display(tx_from_stx);
	Taxi_dtor(&tx_from_stx);

	SpecialTaxi_dtor(&stx);
	for (i = 3; i >= 0; --i)
	{
		Minibus_dtor(&arr3[i]);
	}

	/* public convoy */
	ts1 = (struct PublicConvoy *)malloc(sizeof(struct PublicConvoy));
	ts2 = (struct PublicConvoy *)malloc(sizeof(struct PublicConvoy));
	Publicconvoy_ctor(ts1);
	Publicconvoy_cctor(ts2, ts1);
	printf("\n!!!!!!!!!!!!!!!!!\n");
	display_pc(ts1);
	display_pc(ts2);
/*	Publicconvoy_dtor(ts1);
	free(ts1);
	display_pc(ts2);
	Publicconvoy_dtor(ts2);
	free(ts2);
	/*end of public convoy*/

/*
	Minibus_dtor(&m2);
	PublicTransport_dtor(&pt_arr2);
	PublicTransport_dtor(&tx_arr2_pt);
	PublicTransport_dtor(&m_arr2_pt);
	Minibus_dtor(&m);
*/
	return 0;
}
