//	upfils2.org	2013 may 21
//
//	Shows organization of instructor's "upfils2.cpp".
//	Modified from upfils2.cpp.
//	This reshapes whenever height(tree) becomes
//	greater than original height + delta; or whenever the number of nodes
//	in the tree becomes less than twoin(k)/2, where k is the number of 
//	nodes in the most recently reshaped tree.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <new>
#include <cstdlib>
#include <string.h>

using namespace std;

	struct mrec {
		unsigned int acct;
		char name[25];			// strlen( name ) <= 24
		float baln;
	};

	struct node {
		node *left;
		mrec info;
		node *rite;
	};

	struct trec {
		unsigned int acct;
		char name[25];
		float tran;
	};

	node *const nill=(node *)0;
	const unsigned int arraysize=32;	// fllg of max. no. of nodes in complete tree
	const unsigned int delta=2;
	const unsigned int ssize=32;		// size of stack in reshape
	unsigned int hi,lo,mhz=4200;		// FIND YOUR PROCESSOR SPEED

int
main( int argc,char **argv )
{
	node *lltoct( unsigned int,node * );
	int newerr( size_t );
	unsigned int timeout( void );
	void chop( node * ),fitoll( ifstream &,unsigned int &,node *& ),
		output( const node *,ofstream & ),timein( void ),
		update( node *&,unsigned int &,ifstream &,unsigned int &,unsigned int &,unsigned int & );

	ifstream fp0,fp1;		// masfil, tranfil
	ofstream fp2;			// newmas
	node *tree;
	unsigned int del,ins,maxh,n,t;
	char ch;

	if ( argc != 4 ) {
		cout << "** use is >upfils2 masfil tranfil newmasfil **\n";
		exit( 1 );
	}
	//_set_new_handler( newerr );
	fp0.open( *++argv,ios::in | ios::binary );
	if ( fp0.fail() ) {
		cout << "** can't open \"" << *argv << "\" **\n";
		exit( 1 );
	}
	fp1.open( *++argv,ios::in | ios::binary );
	if ( fp1.fail() ) {
		fp0.close();
		cout << "** can't open \"" << *argv << "\" **\n";
		exit( 1 );
	}
	fp2.open( *++argv,ios::in | ios::binary );
	if ( !fp2.fail() ) {
		cout << "** \"" << *argv << "\" exists **; overwrite? (y/n) ";
		cin.get( ch );
		if ( ch != '\n' ) {
			while ( cin.get() != '\n' ) {
				;
			}
		}
		if ( (ch != 'y') && (ch != 'Y') ) {
			fp0.close();
			fp1.close();
			fp2.close();
			cout << "-- ok: \"" << *argv << "\" is unchanged --\n";
			exit( 0 );
		}
		fp2.close();
	}
	fp2.clear();
	fp2.open( *argv,ios::out | ios::binary );
	if ( fp2.fail() ) {
		fp0.close();
		fp1.close();
		cout << "** can't open \"" << *argv << "\" **\n";
		exit( 1 );
	}
	timein();
	fitoll( fp0,n,tree );
	t = timeout();
	fp0.close();
	cout << "-- read " << n << " recs into linked list --; "
		<< (t+500)/1000 << " msec\n";
	timein();
	tree = lltoct( n,tree );
	t = timeout();
	cout << "-- made complete binary tree --; "
		<< (t+500)/1000 << " msec\n";
	timein();
	update( tree,n,fp1,del,ins,maxh );
	t = timeout();
	fp1.close();
	cout << "-- updated --:\n   " << del << " del; " << ins << " ins; "
		<< del+ins << " changes; max height = " << maxh
		<< "; " << (t+500)/1000 << " msec\n";
	timein();
	output( tree,fp2 );
	t = timeout();
	fp2.close();
	cout << "-- wrote " << n << " recs to file \"" << *argv << "\" --; "
		<< (t+500)/1000 << " msec\n";
	timein();
	chop( tree );
	t = timeout();
	cout << "-- chopped down tree --; " << (t+500)/1000 << " msec\n";
	return( 0 );
}
void
chop( node *p )
{
}
void
fitoll( ifstream & f,unsigned int & n,node * & head )
{
	node *p,*q;
	mrec rec;
        n = 0;
        head = nill;
        while(f.read((char *)&rec, sizeof(mrec)))
        {
            q = new node;
            q->info = rec;
            if(++n == 1) head = q;
            else p->rite = q;
            p = q;
        }
}
void
fix( node *& tree,unsigned int & n,unsigned int & h,unsigned int & maxh0,unsigned int & n0 )
{
	node *reshape( node *,unsigned int & );
	unsigned int fllg( unsigned int );
        tree = reshape(tree, n);
        maxh0 = (h = fllg(n))+delta;
        n0 = (1 << (h - 1));
}
unsigned int
fllg( unsigned int x )
{
	__asm {
		bsr	eax,x
	}
}
void
freeleaf( node *p,node *q )
{
    if(q != nill)
    {
        if(q->left == p) q->left = nill;
        else q->rite = nill;
    }
    delete p;
}
unsigned int
isleaf( node *p )
{
    return((p->left == nill) && (p->rite == nill));
}
node *
lltoct( unsigned int n,node *q )//?
{
	unsigned int fllg( unsigned int ),nu2( unsigned int );

	node *last[arraysize],*p;	// n < 2**arraysize
	unsigned int b,h,i,l;
	char pat[arraysize];		// very short ints: 0 or 1
        if(n < 1) return(nill);
        b = n + 1 - (1 << (h = fllg(n)));
        b += b;
        for(l = 0; l <= h; l++)
        {
            last[l] = nill;
        }
        for(int i = 1; i <= n; i++)
        {
            l = h - nu2(i <= b ? i : i + i - b);
            last[l] = p = q;
            q = q->rite;
            p->left = p->rite = nill;
            pat[l] = (char)0;
            if(l < h)
            {
                if(pat[l + 1] == (char)0)
                {
                    p->left = last[l + 1];
                    pat[l + 1] = (char)1;
                }
            }
            if(l != 0)
            {
                if(last[l - 1])
                {
                    if(last[l - 1]->rite == nill)
                    {
                        last[l - 1]->rite = p;
                        pat[l] = (char)1;
                    }
                }
            }
        }
        return(*last);
}
int
newerr( size_t x )
{
	cout << "** new has failed **\n";
	exit( 1 );
}
unsigned int
nu2( unsigned int x )
{
	__asm {
		bsf	eax,x
	}
}
void
output( const node *p,ofstream & f )
{
    if(p != nill)
    {
        output(p->left, f);
        if(!f.write((char *)&(p->info), sizeof(mrec)))
        {
            cout << "** can't f.write record **\n";
            exit(1);
        }
        output(p->rite, f);
    }
}
void
remove( node *p,node *q )
{
	void freeleaf( node *,node * );
	unsigned int isleaf( node *);

	node *r,*s;
        if(isleaf(p))
        {
            freeleaf(p, q);
            return;
        }
        q = p;
        if(p->rite != nill)
        {
            r = p->rite;
            while(r->left != nill)
            {
                q = r;
                r = r->left;
            }
        }
        else
        {
            r = p->left;
            while(r->rite != nill)
            {
                q = r;
                r = r->rite;
            }
        }
        p->info = r->info;
        remove(r, q);
}	
node *
reshape( node *root,unsigned int & n )
{
	unsigned int fllg( unsigned int ),nu2( unsigned int );

	node *last[arraysize],*p,*q,*stack[ssize],**t;
	unsigned int b,h,i,l;
	char pat[arraysize];		// very short ints: 0 or 1
        if(n < 1) return(nill);
        b = n + 1 - (1 << (h = fllg(n)));
        b += b;
        for(l = 0; l <= h; l++)
        {
            last[l] = nill;
        }
        i = 0;
        *(t = stack) = root;
        while(stack <= t)
        {
            if((p = *t--) != nill)
            {
                if(p->left != nill)
                {
                    if((stack + ssize - 2) <= t)
                    {
                        cout << "** reshape: stack overflow **\n";
                        exit(1);
                    }
                    q = p->left;
                    p->left = nill;
                    *(++t) = p;
                    *(++t) = q;
                }
                else
                {
                    i++;
                    l = h - nu2(i <= b ? i : i + i - b);
                    last[l] = p;
                    q = p->rite;
                    p->left = p->rite = nill;
                    pat[l] = (char)0;
                    if(l < h)
                    {
                        if(pat[l + 1] == (char)0)
                        {
                            p->left = last[l + 1];
                            pat[l + 1] = (char)1;
                        }
                    }
                    if(l != 0)
                    {
                        if(last[l - 1])
                        {
                            if(last[l - 1]->rite == nill)
                            {
                                last[l -1]->rite = p;
                                pat[l] = (char)1;
                            }
                        }
                    }
                    if(q != nill)
                    {
                        if((stack + ssize -1) <= t)
                        {
                            cout << "**reshape: stack overflow **\n";
                            exit(1);
                        }
                        *(++t) = q;
                    }
                }
            }
        }
        return (*last);
}
void
timein()
{
	__asm {
		rdtsc
		mov	hi,edx
		mov	lo,eax
	}
}
unsigned int
timeout()
{
	__asm {
		rdtsc
		sub	eax,lo
		sbb	edx,hi
		div	dword ptr mhz
	}
}
void
update( node *& tree,unsigned int & n,ifstream & f,unsigned int & del,unsigned int & ins,unsigned int & maxh )
{
	unsigned int fllg( unsigned int );
	void remove( node *,node * ),
		fix( node *&,unsigned int &,unsigned int &,unsigned int &,unsigned int & );

	node *p,*q;
	trec rec;
	unsigned int chg,h,l,maxh0,n0;
        chg = del = ins = 0;
        maxh0 = (maxh = h = fllg(n)) + delta;
        n0 = (1 << (h - 1));
        while(f.read((char *)&rec, sizeof(trec)))
        {
            q = nill;
            p = tree;
            l = 0;
            if(p != nill)
            {
                if(rec.acct == p->info.acct)
                {
                    if(p->info.baln += rec.tran < 0.01)
                    {
                        del++;
                        chg++;
                        if((p != tree) || !isleaf(p))
                        {
                            remove(p, q);
                        }
                        else
                        {
                            cout << "** tree is empty **\n";
                            delete p;
                            tree = nill;
                        }
                        if(--n < n0) fix(tree, n ,h, maxh0, n0);
                    }
                    break;
                }
                l++;
                q = p;
                if(rec.acct < p->info.acct) p = p->left;
                else p = p->rite;
            }
            if(p == nill)
            {
                n++;
                ins++;
                chg++;
                if(h < ++l)
                {
                    h = l;
                    if(maxh < h) maxh = h;
                }
                p = new node;
                p->left = p->rite = nill;
                p->info.acct = rec.acct;
                strcpy(p->info.name, rec.name);
                p->info.baln = rec.tran;
                if(rec.acct < q->info.acct) q->left = p;
                else q->rite = p;
            }
            if(maxh0 < h) fix(tree, n, h, maxh0, n0);
        }
}
