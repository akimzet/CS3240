//	upfils0.org	2013 apr 24
//
//	Shows oranization of instructor's upfils0.cpp.
//	This could easily be modified to take the names of the data files
//	from the command line.
//

#include <fstream>
#include <iostream>
#include <iomanip>				// setw()
#include <new>
#include <stdlib.h>				// exit()
#include <string.h>

using namespace std;

	struct mdata {
		int acct;
		char name[25];
		float baln;
	};

	struct tdata {
		int acct;
		char name[25];
		float tran;
	};

	struct node {
		node *blink;
		mdata info;
		node *flink;
	};

	node *const nill=(node *)0;		// global const
	node *head,*tail;			// the only global variables

int
main()
{
	int newerr( size_t );
	void makelist( const char * ),showlist( const char * ),
		updatfil( const char * );

	//_set_new_handler( newerr );
	makelist( "masfil" );
	showlist( "\n\nordered master list:\n\n\n" );
	cout << '\n';
	updatfil( "tranfil" );
	showlist( "updated master list:\n\n\n" );
	return( 0 );
}
void
addnode( node *p,mdata & mrec )
{
	void append( mdata & ),insertat( node *,mdata & );
        if(p == nill) append(mrec);
        else insertat(p, mrec);
}
void
append( mdata & mrec )
{
	void newnode( node *&,mdata & );
	node *p;
        newnode(p, mrec);
        p->blink = tail;
        p->flink = nill;
        if(tail == nill) head = p;
        else tail->flink = p;
        tail = p;
}
void
drawline()
{
	int i;
        for(i  = 48; i--;)
        {
            cout << '.';
        }
        cout << '\n';
}
int
found( int acc,node *&p )
{
    p = head;
    while(1)
    {
        if(p == nill) return(0);
		else
		{
			if(p->info.acct < acc) p = p->flink;
			else return(p->info.acct == acc);
		}
    }
}
void
insertat( node *p,mdata & mrec )
{
	void newnode( node *&,mdata & ),prefix( mdata & );
	node *q;
        if(p == head) prefix(mrec);
        else
        {
            newnode(q, mrec);
            q->blink = p->blink;
            q->flink = p;
            p->blink->flink = q;
            p->blink = q;
        }
}
void
makelist( const char *masn )
{
	int found( int,node *& );
	void addnode( node *,mdata & ),drawline( void ),showmrec( mdata & );
	ifstream f;
	mdata mrec;
	node *p;
        f.open(masn, ios::in);
        if(f.fail())
        {
            cout << "** can't open " << masn << " **\n";
            exit(1);
        }
        cout << "original master file:\n\n\n";
        head = tail = nill;
        while(f.read((char *)&mrec, sizeof(mdata)))
        {
            showmrec(mrec);
            if(found(mrec.acct, p))
            {
                cout << "** duplicate account number **\n";
                break;
            }
            addnode(p, mrec);
        }
        f.close();
        drawline();
}
void
newacct( node *p,tdata & trec )
{
	void addnode( node *,mdata & );
	mdata mrec;
        mrec.acct = trec.acct;
        strcpy(mrec.name, trec.name);
        mrec.baln = trec.tran;
        addnode(p, mrec);
}
int
newerr( size_t x )
{
	cout << "** new has failed **\n";
	exit( 1 );
}
void
newnode( node * & p,mdata & mrec )
{
    p = new node;
    p->info = mrec;
}
void
prefix( mdata & mrec )
{
	void newnode( node *&,mdata & );
	node *p;
        newnode(p, mrec);
        p->blink = nill;
        p->flink = head;
        if(head == nill) tail = p;
        else head->blink = p;
        head = p;
}
void
remove( node *p )
{
    if(p == head) head = p->flink;
    else p->blink->flink = p->flink;
    if(p == tail) tail = p->blink;
    else p->flink->blink = p->blink;
    delete p;
}
void
showlist( const char *s )
{
	void drawline( void ),showmrec( mdata & );
	node *p;
        cout << s;
        p = head;
        while(p != nill)
        {
            showmrec(p->info);
            p = p->flink;
        }
        drawline();
}
void
showmrec( mdata & mrec )
{
	cout << setw( 10 ) << (mrec).acct << "  ";
	cout.setf( ios::left,ios::adjustfield );
	cout << setw( 26 ) << (mrec).name;
	cout.setf( ios::right,ios::adjustfield );
	cout.setf( ios::fixed,ios::floatfield );
	cout.setf( ios::showpoint );
	cout.precision( 2 );
	cout << setw( 10 ) << (mrec).baln << '\n';
}
void
showtrec( tdata & trec )
{
	cout << setw( 10 ) << (trec).acct << "  ";
	cout.setf( ios::left,ios::adjustfield );
	cout << setw( 26 ) << (trec).name;
	cout.setf( ios::right,ios::adjustfield );
	cout.setf( ios::fixed,ios::floatfield );
	cout.setf( ios::showpoint );
	cout.precision( 2 );
	cout << setw( 10 ) << (trec).tran << "\n\n";
}	
void
updatfil( const char *trann )
{
	int found( int,node *& );
	void newacct( node *,tdata & ),showlist( const char * ),
		showtrec( tdata & ),updatrec( node *,float & );
	ifstream f;
	node *p;
	tdata trec;
        f.open(trann, ios::in);
        if(f.fail())
        {
            cout << "** can't open " << trann << " **\n";
            exit(1);
        }
        while(f.read((char *)&trec, sizeof(tdata)))
        {
            showtrec(trec);
            if(found(trec.acct, p)) updatrec(p, trec.tran);
            else newacct(p, trec);
            showlist("");
            cout << '\n';
        }
        f.close();
}
void
updatrec( node *p,float & tran )
{
	void remove( node * );
        if((p->info.baln += tran) < 0.01) remove(p);
}
