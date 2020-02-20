#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

struct PublicTransport
{
	void **arr;
	int m_license_plate;
};

struct Minibus
{
	void **arr;
	struct PublicTransport pT;
	int numSeats;
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
void display_Mini(struct Minibus *mini);
void Wash_mini(struct Minibus *mini, int minutes);

void print_info_mini(struct Minibus *m);
struct PublicTransport print_info_int(int i);


/* PublicTransport virtuals*/
typedef void (*func1)(); /* display */
typedef void (*func3)(struct PublicTransport *); /* dtor*/

/* Minibus virtuals*/
typedef void (*func2)(struct Minibus *, int); /* wash*/


static int s_count = 0;
static void **pt_vtable[2];
static void **mini_vtable[3];


/* PublicTransport Ctor	*/
void PublicTransport_ctor(struct PublicTransport *pubTran)
{
	++(s_count);
	pubTran->m_license_plate = 0;
	pubTran->m_license_plate += s_count;
	printf("PublicTransport::Ctor() %d\n", pubTran->m_license_plate);
}

/* PublicTransport dtor */
void PublicTransport_dtor(struct PublicTransport *pubTran)
{
	--(s_count);
	printf("PublicTransport::Dtor() %d\n", pubTran->m_license_plate);
}

/* PublicTransport cctor */
void PublicTransport_cctor(struct PublicTransport *pubTran,
						   struct PublicTransport *res)
{	
	++s_count;
	pubTran->m_license_plate = s_count;
	memcpy(res, pubTran, sizeof(struct PublicTransport));
	printf("PublicTransport::CCtor() %d\n", pubTran->m_license_plate);
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
	
	PublicTransport_ctor(&(mini->pT));
	mini->numSeats = 20;
	printf("Minibus::Ctor()\n");
}

void Minibus_dtor(struct Minibus *mini)
{
	PublicTransport_dtor(&mini->pT);
	printf("Minibus::Dtor()\n");
}

void Minibus_cctor(struct Minibus *mini, struct Minibus *mini_res)
{
	PublicTransport_cctor(&mini_res->pT, &mini->pT);
	mini_res->numSeats = mini->numSeats;
}

void display_mini(struct Minibus *mini)
{
    printf("Minibus::display() ID:%d", get_ID(&mini->pT));
	printf(" num seats:%d\n", mini->numSeats);
}

void Wash_mini(struct Minibus *mini, int minutes)
{
	printf("Minibus::wash(%d)ID:%d\n", minutes,get_ID(&mini->pT));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* print_info functions */
void print_info_mini(struct Minibus *m)
{
    Wash_mini(m, 3);
}

struct PublicTransport print_info_int(int i)
{
    struct Minibus ret;
	struct PublicTransport pt;

	Minibus_ctor(&ret);
	printf("print_info(int i)\n");
	display_mini(&ret);
	
	PublicTransport_cctor(&pt, &ret.pT);
	Minibus_dtor(&ret);
	
	return pt;
}
/*
struct Taxi
{
	struct PublicTransport pT;
};
*/
int main()
{	
	struct PublicTransport pT;
	struct Minibus m;
	Minibus_ctor(&m);
	print_info_mini(&m);
	pT = print_info_int(3);
	display_Pt(&pT);
	PublicTransport_dtor(&pT);

	return 0;
}
