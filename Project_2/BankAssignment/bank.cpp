//	bank.org	2013 apr 15	last modified 2014 feb 21
//
//	Shows organization of instructor's bank.cpp.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <string.h>

using namespace std;

	struct cust {
		int cusn;	// customer number	
		int entr;	// when entered
		int jtim;	// time needed by clerk to do customer's job
		cust *link;
	} *custh;		// pointer to head node
	struct evnt {
		int time;	// cusn enters, or clrk done	
		int what;	// 1: customer enters; 2: clerk finishes job
		int whom;	// if 1: cusn; if 2: clrk
		int data;	// if 1: jtim; if 2: cusn
		evnt *link;
	} *evnth;		// pointer to head node
	struct idlq {
		int clrk;	// which clerk
		int time;	// when began idle
		idlq *link;
	} *idlqh;		// pointer to head node
 	struct watq {
		int cusn;	// customer number
		int entr;	// when entered	
		int jtim;	// time needed by clerk to do customer's job
		watq *link;
	} *watqh,*watqt;	// pointers to head node, tail node

	int const infinity=2147483647;			// 2**31 - 1
	int *idle,maxwait,minwait,now,totwait;

int
main( int argc,char **argv )
{
	int newerr( size_t );
	void readjobs( char *,int & ),simul( int,int,int,int ),
		stats( int,int );

	cust *p,*q;
	int i,m,m0,m1,n,v=0;

	if ( (argc < 4) || (5 < argc) ) {
		cout << "** use is >bank [-v] nclerk0 nclerk1 file **\n";
		exit( 1 );
	}
	//set_new_handler( newerr );
	if ( strcmp( *++argv,"-v" ) == 0 ) {
		v++;
	} else {
		--argv;
	}
	m0 = atoi( *++argv );
	m1 = atoi( *++argv );
	if ( (m0 < 1) || (m1 < m0) ) {
		cout << "** bad nclerk0 and/or nclerk1 **\n";
		exit( 1 );
	}
	idle = new int [m1];
	readjobs( *++argv,n );
	cout.setf( ios::fixed,ios::floatfield );
	cout.setf( ios::showpoint );
	for ( m = m0;  m <= m1;  m++ ) {
		cout << setw( 3 ) << m << " clerk" << (m < 2 ? "" : "s")
			<< " --\n";
		for ( i = 0;  i < 2;  i++ ) {
			simul( i,v,n,m );
			cout << (i == 0 ? "FCFS:" : " SJF:") << " finish at "
				<< now << '\n';
			stats( n,m );
		}
		cout << '\n';
	}
	for ( p = custh;  p != (cust *)0; ) {
		q = p->link;
		delete p;
		p = q;
	}
	delete [] idle;
	return( 0 );
}
void
addevnt( evnt *pevnt )
{
	evnt *p,*q;
        if(evnth == (evnt *)0)
        {
            evnth = pevnt;
            pevnt->link = (evnt *)0;
        }
        else
        {
            p = (evnt *)0;
            q = evnth;
            while(q != (evnt *)0)
            {
                if(q->time <= pevnt->time)
                {
                    p = q;
                    q = q->link;
                }
                else
                {
                    pevnt->link = q;
                    if(p == (evnt *)0)
                    {
                        evnth = pevnt;
                    }
                    else
                    {
                        p->link = pevnt;
                    }
                    break;
                }
            }
            if(q == (evnt *)0)
            {
                p->link = pevnt;
                pevnt->link = (evnt *)0;
            }
        }
}			
void
getcust( int clrk )
{
	void addevnt( evnt * );

	evnt *pevnt;
	watq *q;
	int wait;
        pevnt = new evnt;
        pevnt->time = now + watqh->jtim;
        pevnt->what = 2;
        pevnt->whom = clrk;
        pevnt->data = watqh->cusn;
        addevnt(pevnt);
        wait = now - watqh->entr;
        if(wait < minwait)
        {
            minwait = wait;
        }
        totwait += (unsigned int) wait;
        if(maxwait < wait)
        {
            maxwait = wait;
        }
        q = watqh;
        watqh = watqh->link;
        delete q;
        if(watqh == (watq *)0)
        {
            watqt = (watq *)0;
        }
}
void
init( int n,int m )
{
	cust *pcust;
	evnt *pevnt,*q;
	idlq *pidlq,*r;
	int i;
        for(i = 0; i < m;)
        {
            idle[i] = 0;
            pidlq = new idlq;
            pidlq->clrk = ++i;
            pidlq->time = 0;
            if(i == 1)
            {
                idlqh = pidlq;
            }
            else
            {
                r->link = pidlq;
            }
            r = pidlq;
        }
        r->link = (idlq *)0;
        watqh = watqt = (watq *)0;
        totwait = 0;
        now = maxwait = 0;
        minwait = infinity;
        pcust = custh;
        for(int i = 0; i < n;)
        {
            pevnt = new evnt;
            pevnt->time = pcust->entr;
            pevnt->what = 1;
            pevnt->whom = ++i;
            pevnt->data = pcust->jtim;
            if(i == 1)
            {
                evnth = pevnt;
            }
            else
            {
                q->link = pevnt;
            }
            q = pevnt;
            pcust = pcust->link;
        }
        q->link = (evnt *)0;
}
int
newerr( size_t x )
{
	cout << "** new has failed **\n";
	exit( 1 );
}
void
readjobs( char *fname,int & n )
{
	ifstream file;
	struct {
		unsigned short int entr;
		unsigned short int jtim;
	} rec;
	cust *pcust,*q;
        file.open(fname, ios::in | ios::binary);
        if(file.fail())
        {
            cout << "** readjobs: can't open\"" << fname << "\" **\n";
            exit(1);
        }
        else
        {
            cout << "\t\t\tbank: data file is \"" << fname << "\"\n";
            n = 0;
        }
        while(file.read((char *) & rec, sizeof(rec)))
        {
            n++;
            pcust = new cust;
            pcust->entr = rec.entr;
            pcust->jtim = rec.jtim;
            if(n == 1)
            {
                custh = pcust;
            }
            else
            {
                q->link = pcust;
            }
            q = pcust;
        }
        file.close();
        q->link = (cust *)0;
}
void
simul( int mode,int v,int n,int m )
{
	void getcust( int ),init( int,int ),showevnt( void ),toclrk( int,int ),
		toidlq( int ),towatq( int,int,int );

	evnt *q;
	int clrk,data,jtim,what,whom;
        init(n, m);
        while(evnth != (evnt *)0)
        {
            now = evnth->time;
            what = evnth->what;
            whom = evnth->whom;
            data = evnth->data;
            q = evnth;
            evnth = evnth->link;
            delete q;
            if(v)
            {
                cout << setw(8) << now << ":";
            }
            if(what == 1)
            {
                if(v)
                {
                    cout << " customer" << setw(5) << whom << " enters; ";
                }
                if(idlqh == (idlq *)0)
                {
                    if(v)
                    {
                        cout << "to wait queue\n";
                    }
                    towatq(mode, whom, data);
                }
                else
                {
                    if(v)
                    {
                        cout << "to clerk" << setw(4) << idlqh->clrk << '\n';
                    }
                    toclrk(whom, data);
                }
            }
            else
            {
                if(v)
                {
                    cout << "    clerk" << setw(5) << whom << " is done with customer" << setw(5) << data << "; ";
                }
                if(watqh == (watq *)0)
                {
                    if(v)
                    {
                        cout << "to idle queue\n";
                    }
                    toidlq(whom);
                }
                else
                {
                    if(v)
                    {
                        cout << "gets customer " << setw(5) << watqh->cusn << '\n';
                    }
                    getcust(whom);
                }
            }
        }
}
        
        
void
stats( int n, int m )
{
	idlq *p;
	unsigned int totidle;
	unsigned short i,maxidle,minidle;

	while ( idlqh != (idlq *)0 ) {
		p = idlqh->link;
		idle[(idlqh->clrk)-1] += now-(idlqh->time);
		delete idlqh;
		idlqh = p;
	}
	for ( minidle = infinity, maxidle = totidle = i = 0;  i < m;  i++ ) {
		if ( idle[i] < minidle ) {
			minidle = idle[i];
		}
		totidle += (unsigned int)idle[i];
		if ( maxidle < idle[i] ) {
			maxidle = idle[i];
		}
	}
	cout << "minwait =" << setw( 6 ) << minwait
		<< "  meanwait =" << setw( 12 ) << setprecision( 4 )
		<< (double)totwait/(double)n
		<< "  maxwait =" << setw( 6 ) << maxwait
		<< "\nminidle =" << setw( 6 ) << minidle
		<< "  meanidle =" << setw( 12 ) << setprecision( 4 )
		<< (double)totidle/(double)m
		<< "  maxidle =" << setw( 6 ) << maxidle << '\n';
}

void
toclrk(int cusn,int jtim )
{
	void addevnt( evnt * );

	evnt *p;
	idlq *q;
	int clrk;
        clrk = idlqh->clrk;
        p = new evnt;
        p->time = now + jtim;
        p->what = 2;
        p->whom = clrk;
        p->data = cusn;
        idle[clrk - 1] += now - idlqh->time;
        q = idlqh;
        idlqh = idlqh->link;
        delete q;
        addevnt(p);
        minwait = 0;
}
void
toidlq( int clrk )
{
	idlq *p,*pidlq,*q;
        pidlq = new idlq;
        pidlq->clrk = clrk;
        pidlq->time = now;
        if(idlqh == (idlq *)0)
        {
            pidlq->link = (idlq *)0;
            idlqh = pidlq;
        }
        else
        {
            p = (idlq *)0;
            q = idlqh;
            while(q != (idlq *)0)
            {
                if(idle[clrk - 1] <= idle[q->clrk - 1] + now - q->time)
                {
                    p = q;
                    q = q->link;
                }
                else
                {
                    if(p == (idlq *)0)
                    {
                        idlqh = pidlq;
                        idlqh->link = q;
                    }
                    else
                    {
                        p->link = pidlq;
                        pidlq->link = q;
                    }
                    break;
                }
            }
            if(q == (idlq *)0)
            {
                p->link = pidlq;
                pidlq->link = (idlq *)0;
            }
        }
}
void
towatq( int mode,int cusn,int jtim )
{
	watq *p,*pwatq,*q;
        pwatq = new watq;
        pwatq->cusn = cusn;
        pwatq->entr = now;
        pwatq->jtim = jtim;
        if(watqt == (watq *)0)
        {
            watqh = watqt = pwatq;
            watqt->link = (watq *)0;
        }
        else
        {
            if(mode == 0)
            {
                watqt->link = pwatq;
                watqt = pwatq;
                watqt->link = (watq *)0;
            }
            else
            {
                p = (watq *)0;
                q = watqh;
                while(q != (watq *)0)
                {
                    if(q->jtim <= jtim)
                    {
                        p = q;
                        q = q->link;
                    }
                    else
                    {
                        pwatq->link = q;
                        if(p == (watq *)0)
                        {
                            watqh = pwatq;
                        }
                        else
                        {
                            p->link = pwatq;
                        }
                        break;
                    }
                }
                if(q == (watq *)0)
                {
                    watqt->link = pwatq;
                    pwatq->link = (watq *)0;
                    watqt = pwatq;
                }
            }
        }
}
