#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

#define NUM_V_FUNC_PT 2
#define NUM_V_FUNC_MINI 3
#define NUM_V_FUNC_TAXI 2
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

void PublicTransport_ctor(struct PublicTransport *pubTran);
void PublicTransport_dtor(struct PublicTransport *pubTran);
void PublicTransport_cctor(struct PublicTransport *pubTran,
						   struct PublicTransport *res);
void display_Pt(struct PublicTransport *pubTran);						   
int get_ID(struct PublicTransport *pubTran);
void Minibus_ctor(struct Minibus *mini);
void Minibus_dtor(struct Minibus *mini);
void Minibus_cctor(struct Minibus *mini, struct Minibus *mini_res);
void Display_Mini(struct Minibus *mini);
void Wash_Mini(struct Minibus *mini, int minutes);

void print_info_mini(struct Minibus *m);
struct PublicTransport print_info_int(int i);

/* Minibus virtuals*/
typedef void (*func2)(struct Minibus *, int); /* wash*/


static int s_count = 0;
static void (*pt_vtable[NUM_V_FUNC_PT])() = {NULL}; /* moran amra*/
static void (*mini_vtable[NUM_V_FUNC_MINI])() = {NULL}; /* shaddad amar*/
static void (*taxi_vtable[NUM_V_FUNC_TAXI])() = {NULL}; /* jesus*/

/* PublicTransport Ctor	*/
void PublicTransport_ctor(struct PublicTransport *pubTran)
{
	pubTran->vptr = pt_vtable;
	pt_vtable[0] = PublicTransport_dtor;
	pt_vtable[1] = display_Pt;
	++s_count;
	pubTran->m_license_plate = s_count;
	printf("PublicTransport::Ctor() %d\n", pubTran->m_license_plate);
}

/* PublicTransport dtor */
void PublicTransport_dtor(struct PublicTransport *pubTran)
{
	--s_count;
	printf("PublicTransport::Dtor() %d\n", pubTran->m_license_plate);
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
	printf("Minibus::wash(%d)ID:%d\n", minutes,get_ID(&mini->pT));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Taxi_ctor(struct Taxi *taxi)
{
	taxi_vtable[0] = PublicTransport_dtor;
	taxi_vtable[1] = display_Pt;
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
    printf("Taxi::display() ID:%d", get_ID(&taxi->pT));
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

int main()
{	
	struct PublicTransport pT;
	struct Minibus m;

	/*public transport array*/
	int i = 0;
	struct Minibus *m1 = (struct Minibus *)malloc(sizeof(struct Minibus));
	struct Taxi *t2 = (struct Taxi *)malloc(sizeof(struct Taxi));
	struct Minibus *m3 = (struct Minibus *)malloc(sizeof(struct Minibus));
	struct PublicTransport *array[3];
	/**/

	Minibus_ctor(&m);
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

	return 0;
}
